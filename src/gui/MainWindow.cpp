#include "MainWindow.h"

#include <QFont>
#include <QPlainTextEdit>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
      m_editor(new QPlainTextEdit(this))   // "this" is the parent, so Qt deletes it for us
{
    setWindowTitle("Untitled - Text Editor");
    resize(900, 600);

    // A monospaced font looks like a real editor.
    m_editor->setFont(QFont("Consolas", 11));

    // The text area fills the whole window.
    setCentralWidget(m_editor);
}