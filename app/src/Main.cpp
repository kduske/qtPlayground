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

#include <QGuiApplication>
#include <QtGlobal>

extern void qt_set_sequence_auto_mnemonic(bool b);

int main(int argc, char *argv[])
{
    // Set up Hi DPI scaling
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    // Workaround bug in Qt's Ctrl+Click = RMB emulation (a macOS feature.)
    // In Qt 5.13.0 / macOS 10.14.6, Ctrl+trackpad click+Drag produces no mouse events at all, but
    // it should produce RMB down/move events.
    // This environment variable disables Qt's emulation so we can implement it ourselves in InputEventRecorder::recordEvent
    qputenv("QT_MAC_DONT_OVERRIDE_CTRL_LMB", "1");

    QGuiApplication app(argc, argv);
    return app.exec();
}
