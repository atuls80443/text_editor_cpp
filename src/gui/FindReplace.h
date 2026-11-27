#pragma once

#include <QDialog>
#include <QTextDocument>

class QLineEdit;
class QCheckBox;
class QPushButton;
class QPlainTextEdit;

class FindReplace : public QDialog {
    Q_OBJECT

public:
    explicit FindReplace(QPlainTextEdit *editor, QWidget *parent = nullptr);

private slots:
    void findNext();
    void replace();
    void replaceAll();

private:
    QTextDocument::FindFlags getFindFlags() const;

    QPlainTextEdit *m_editor;
    QLineEdit *m_findEdit;
    QLineEdit *m_replaceEdit;
    QCheckBox *m_caseSensitiveCheck;
    QPushButton *m_findButton;
    QPushButton *m_replaceButton;
    QPushButton *m_replaceAllButton;
    QPushButton *m_closeButton;
};