add_executable(qtPlayground WIN32 MACOSX_BUNDLE)
target_sources(qtPlayground PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}/src/Main.cpp"
)
# target_link_libraries(qtPlayground PRIVATE Qt5::Widgets)
