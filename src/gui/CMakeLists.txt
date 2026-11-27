find_package(Qt6 REQUIRED COMPONENTS Widgets)

# Generates the extra code Qt needs for Q_OBJECT classes.
set(CMAKE_AUTOMOC ON)

add_executable(TextEditor
    main.cpp
    MainWindow.h
    MainWindow.cpp
    FindReplace.h
    FindReplace.cpp
)

target_link_libraries(TextEditor PRIVATE Qt6::Widgets)