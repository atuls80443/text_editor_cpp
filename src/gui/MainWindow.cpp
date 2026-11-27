#include "MainWindow.h"

#include <QAction>
#include <QCloseEvent>
#include <QDir>
#include <QFile>
#include <QFileDialog>
#include <QFileInfo>
#include <QFont>
#include <QKeySequence>
#include <QMenu>
#include <QMenuBar>
#include <QMessageBox>
#include <QPlainTextEdit>
#include <QSaveFile>
#include <QTextDocument>
#include <QLabel>
#include <QStatusBar>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
      m_editor(new QPlainTextEdit(this))
{
    resize(900, 600);
    m_editor->setFont(QFont("Consolas", 11));
    setCentralWidget(m_editor);

    createActions();
    createStatusBar(); //initialize status bar

    // update status bar when cursor moves
    connect(m_editor, &QPlainTextEdit::cursorPositionChanged,
            this, &MainWindow::updateStatusBar);

    // Show a "*" in the title whenever the document has unsaved changes.
    connect(m_editor->document(), &QTextDocument::modificationChanged,
            this, &QWidget::setWindowModified);

    updateWindowTitle();
    updateStatusBar();
}

void MainWindow::createActions()
{
    QMenu *fileMenu = menuBar()->addMenu("&File");
    QMenu *editMenu = menuBar()->addMenu("&Edit");

    // helper for file menu actions
    auto addAction = [this, fileMenu](const QString &text,
                                      const QKeySequence &shortcut,
                                      void (MainWindow::*slot)()) {
        QAction *action = new QAction(text, this);
        action->setShortcut(shortcut);
        connect(action, &QAction::triggered, this, slot);
        fileMenu->addAction(action);
    };

    addAction("&New", QKeySequence::New, &MainWindow::newFile);
    addAction("&Open...", QKeySequence::Open, &MainWindow::openFile);
    fileMenu->addSeparator();
    addAction("&Save", QKeySequence::Save, &MainWindow::saveFile);
    addAction("Save &As...", QKeySequence("Ctrl+Shift+S"), &MainWindow::saveFileAs);
    fileMenu->addSeparator();

    QAction *exitAction = new QAction("E&xit", this);
    exitAction->setShortcut(QKeySequence("Ctrl+Q"));
    connect(exitAction, &QAction::triggered, this, &QWidget::close);
    fileMenu->addAction(exitAction);

    QAction *undoAction = editMenu->addAction("&Undo");
    undoAction->setShortcut(QKeySequence::Undo);
    connect(undoAction, &QAction::triggered, m_editor, &QPlainTextEdit::undo);

     QAction *redoAction = editMenu->addAction("&Redo");
    redoAction->setShortcut(QKeySequence::Redo);
    connect(redoAction, &QAction::triggered, m_editor, &QPlainTextEdit::redo);

    editMenu->addSeparator();

    QAction *cutAction = editMenu->addAction("Cu&t");
    cutAction->setShortcut(QKeySequence::Cut);
    connect(cutAction, &QAction::triggered, m_editor, &QPlainTextEdit::cut);

    QAction *copyAction = editMenu->addAction("&Copy");
    copyAction->setShortcut(QKeySequence::Copy);
    connect(copyAction, &QAction::triggered, m_editor, &QPlainTextEdit::copy);

    QAction *pasteAction = editMenu->addAction("&Paste");
    pasteAction->setShortcut(QKeySequence::Paste);
    connect(pasteAction, &QAction::triggered, m_editor, &QPlainTextEdit::paste);

    editMenu->addSeparator();

    QAction *selectAllAction = editMenu->addAction("Select &All");
    selectAllAction->setShortcut(QKeySequence::SelectAll);
    connect(selectAllAction, &QAction::triggered, m_editor, &QPlainTextEdit::selectAll);
}

void MainWindow::createStatusBar()
{
    // Create a label for line and add it to the right side
    m_statusLabel = new QLabel(this);
    statusBar()->addPermanentWidget(m_statusLabel);
}

void MainWindow::updateStatusBar()
{
    // QTextCursor tracks the current position
    QTextCursor cursor = m_editor->textCursor();
    int line = cursor.blockNumber() + 1;
    int col = cursor.columnNumber() + 1;
    m_statusLabel->setText(QString("Line %1, Col %2").arg(line).arg(col));
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    if (maybeSave())
        event->accept();    // Go ahead and close
    else
        event->ignore();    // User cancelled, keep the window open
}

void MainWindow::newFile()
{
    if (!maybeSave())
        return;
    m_editor->clear();
    setCurrentFile(QString());
}

void MainWindow::openFile()
{
    if (!maybeSave())
        return;

    const QString path = QFileDialog::getOpenFileName(
        this, "Open File", QString(), "Text Files (*.txt);;All Files (*)");
    if (!path.isEmpty())
        loadFile(path);
}

void MainWindow::saveFile()
{
    save();
}

void MainWindow::saveFileAs()
{
    saveAs();
}

bool MainWindow::maybeSave()
{
    if (!m_editor->document()->isModified())
        return true;    

    const QMessageBox::StandardButton answer = QMessageBox::warning(
        this, "Text Editor",
        "The document has unsaved changes.\nDo you want to save them?",
        QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);

    switch (answer) {
    case QMessageBox::Save:
        return save();      // Continue only if the save worked
    case QMessageBox::Cancel:
        return false;
    default:
        return true;        
    }
}

bool MainWindow::save()
{
    return m_currentFile.isEmpty() ? saveAs() : writeFile(m_currentFile);
}

bool MainWindow::saveAs()
{
    const QString path = QFileDialog::getSaveFileName(
        this, "Save File As", m_currentFile, "Text Files (*.txt);;All Files (*)");
    if (path.isEmpty())
        return false;       // User cancelled the dialog
    return writeFile(path);
}

bool MainWindow::loadFile(const QString &path)
{
    QFile file(path);
    // Text mode converts Windows line endings (\r\n) to \n.
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QMessageBox::warning(this, "Open Failed",
            QString("Cannot open %1:\n%2")
                .arg(QDir::toNativeSeparators(path), file.errorString()));
        return false;
    }

    m_editor->setPlainText(QString::fromUtf8(file.readAll()));
    setCurrentFile(path);
    return true;
}

bool MainWindow::writeFile(const QString &path)
{
    // QSaveFile writes to a temporary file first, so a failed save never damages the original.
    QSaveFile file(path);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QMessageBox::warning(this, "Save Failed",
            QString("Cannot write %1:\n%2")
                .arg(QDir::toNativeSeparators(path), file.errorString()));
        return false;
    }

    file.write(m_editor->toPlainText().toUtf8());
    if (!file.commit()) {
        QMessageBox::warning(this, "Save Failed",
            QString("Cannot write %1:\n%2")
                .arg(QDir::toNativeSeparators(path), file.errorString()));
        return false;
    }

    setCurrentFile(path);
    return true;
}

void MainWindow::setCurrentFile(const QString &path)
{
    m_currentFile = path;
    m_editor->document()->setModified(false);
    updateWindowTitle();
}

void MainWindow::updateWindowTitle()
{
    const QString name = m_currentFile.isEmpty()
        ? QString("Untitled")
        : QFileInfo(m_currentFile).fileName();

    setWindowTitle(name + "[*] - Text Editor");     // [*] becomes "*" when modified
    setWindowModified(m_editor->document()->isModified());
}