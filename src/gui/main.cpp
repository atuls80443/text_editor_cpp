#include <QApplication>
#include "MainWindow.h"

int main(int argc, char *argv[]) {
    // Starts Qt and the event loop machinery
    QApplication app(argc, argv);   

    MainWindow window;
    window.show();

    // Runs until the window is closed
    return app.exec();              
}