#pragma once

#include <QMainWindow>

class QPlainTextEdit;

// The main application window. Later steps add menus, file handling
// and search to this class.
class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);

private:
    QPlainTextEdit *m_editor;   // Text area; owned by Qt through the parent-child system
};