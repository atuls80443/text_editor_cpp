#pragma once

#include <QMainWindow>
#include <QString>

class QPlainTextEdit;
class QCloseEvent;

// The main application window: text area plus the File menu.
class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);

protected:
    // Called by Qt when the window is about to close; lets us ask about unsaved work.
    void closeEvent(QCloseEvent *event) override;

private slots:
    void newFile();
    void openFile();
    void saveFile();
    void saveFileAs();

private:
    void createActions();                       // Builds the File menu
    bool maybeSave();                           // Asks to save unsaved changes; false = user cancelled
    bool save();                                // Save, or Save As if the file has no name yet
    bool saveAs();                              // Ask for a path, then write
    bool loadFile(const QString &path);         // Read a file into the editor
    bool writeFile(const QString &path);        // Write the editor text to a file
    void setCurrentFile(const QString &path);   // Remember the path and reset the modified flag
    void updateWindowTitle();

    QPlainTextEdit *m_editor;
    QString m_currentFile;                      // Empty means "Untitled"
};