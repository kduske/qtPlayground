/*
 Copyright 2019 Kristian Duske

 Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated
 documentation files (the "Software"), to deal in the Software without restriction, including without limitation the
 rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit
 persons to whom the Software is furnished to do so, subject to the following conditions:

 The above copyright notice and this permission notice shall be included in all copies or substantial portions of the
 Software.

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE
 WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR
 OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

#include <QAbstractListModel>
#include <QApplication>
#include <QBoxLayout>
#include <QSortFilterProxyModel>
#include <QTableView>

#include <algorithm>
#include <string>
#include <tuple>
#include <vector>

class TableModel : public QAbstractListModel {
private:
    using Row = std::tuple<std::string, std::string>;
    std::vector<Row> m_rows;
public:
    explicit TableModel(QObject* parent = nullptr) :
    QAbstractListModel(parent),
    m_rows({
        { "falloff", "2.0" },
        { "classname", "light" },
        { "light", "300" },
        { "attenuation", "1.0" }
    }) {}

    int columnCount(const QModelIndex& parent) const override {
        return 2;
    }

    int rowCount(const QModelIndex& parent) const override {
        return static_cast<int>(m_rows.size());
    }

    QVariant data(const QModelIndex& index, const int role) const override {
        if (role != Qt::DisplayRole) {
            return QVariant();
        }

        const auto row = index.row();
        if (row < 0 || row >= rowCount(QModelIndex())) {
            return QVariant();
        }

        const auto col = index.column();
        switch (col) {
            case 0:
                return QVariant(QString::fromStdString(std::get<0>(m_rows[static_cast<size_t>(row)])));
            case 1:
                return QVariant(QString::fromStdString(std::get<1>(m_rows[static_cast<size_t>(row)])));
            default:
                return QVariant();
        }
    }

    Qt::ItemFlags flags(const QModelIndex& index) const override {
        return QAbstractListModel::flags(index) | Qt::ItemIsEditable;
    }

    bool setData(const QModelIndex& index, const QVariant& value, const int role) override {
        if (role != Qt::EditRole) {
            return false;
        }

        const auto row = index.row();
        if (row < 0 || row >= rowCount(QModelIndex())) {
            return false;
        }

        const auto strValue = value.toString().toStdString();
        const auto col = index.column();
        switch (col) {
            case 0:
                std::get<0>(m_rows[static_cast<size_t>(row)]) = strValue;
                emit dataChanged(createIndex(row, col), createIndex(row, col));
                return true;
            case 1:
                std::get<1>(m_rows[static_cast<size_t>(row)]) = strValue;
                emit dataChanged(createIndex(row, col), createIndex(row, col));
                return true;
            default:
                return false;
        }

    }
};

int main(int argc, char *argv[])
{
    // Set up Hi DPI scaling
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    // Workaround bug in Qt's Ctrl+Click = RMB emulation (a macOS feature.)
    // In Qt 5.13.0 / macOS 10.14.6, Ctrl+trackpad click+Drag produces no mouse events at all, but
    // it should produce RMB down/move events.
    // This environment variable disables Qt's emulation so we can implement it ourselves in InputEventRecorder::recordEvent
    qputenv("QT_MAC_DONT_OVERRIDE_CTRL_LMB", "1");

    QApplication a(argc, argv);

    auto* container = new QWidget();
    container->setFixedSize(400, 300);

    auto* tableView = new QTableView();
    auto* tableModel = new TableModel(tableView);
    auto* proxyModel = new QSortFilterProxyModel(tableView);
    proxyModel->setSourceModel(tableModel);
    tableView->setModel(proxyModel);

    tableView->setSortingEnabled(true);
    tableView->setEditTriggers(QAbstractItemView::DoubleClicked | QAbstractItemView::AnyKeyPressed);

    auto* layout = new QVBoxLayout(container);
    layout->addWidget(tableView);

    container->show();

    return a.exec();}
