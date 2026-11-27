#include "FindReplace.h"
#include <QLineEdit>
#include <QCheckBox>
#include <QPushButton>
#include <QPlainTextEdit>
#include <QGridLayout>
#include <QLabel>
#include <QMessageBox>

FindReplace::FindReplace(QPlainTextEdit *editor, QWidget *parent)
    : QDialog(parent), m_editor(editor)
{
    setWindowTitle("Find & Replace");
    setFixedSize(350, 180);

    m_findEdit = new QLineEdit(this);
    m_replaceEdit = new QLineEdit(this);
    m_caseSensitiveCheck = new QCheckBox("Match Case", this);

    m_findButton = new QPushButton("Find Next", this);
    m_replaceButton = new QPushButton("Replace", this);
    m_replaceAllButton = new QPushButton("Replace All", this);
    m_closeButton = new QPushButton("Close", this);

    auto *layout = new QGridLayout(this);
    layout->addWidget(new QLabel("Find:"), 0, 0);
    layout->addWidget(m_findEdit, 0, 1, 1, 2);
    layout->addWidget(new QLabel("Replace:"), 1, 0);
    layout->addWidget(m_replaceEdit, 1, 1, 1, 2);
    
    layout->addWidget(m_caseSensitiveCheck, 2, 1);
    layout->addWidget(m_findButton, 3, 1);
    layout->addWidget(m_replaceButton, 3, 2);
    layout->addWidget(m_replaceAllButton, 4, 1);
    layout->addWidget(m_closeButton, 4, 2);

    connect(m_findButton, &QPushButton::clicked, this, &FindReplace::findNext);
    connect(m_replaceButton, &QPushButton::clicked, this, &FindReplace::replace);
    connect(m_replaceAllButton, &QPushButton::clicked, this, &FindReplace::replaceAll);
    connect(m_closeButton, &QPushButton::clicked, this, &QDialog::close);
}

QTextDocument::FindFlags FindReplace::getFindFlags() const
{
    QTextDocument::FindFlags flags;
    if (m_caseSensitiveCheck->isChecked())
        flags |= QTextDocument::FindCaseSensitively;
    return flags;
}

void FindReplace::findNext()
{
    const QString text = m_findEdit->text();
    if (text.isEmpty()) return;

    // Search forward from current cursor position
    if (!m_editor->find(text, getFindFlags())) {
        // If not found, wrap around to the beginning
        m_editor->moveCursor(QTextCursor::Start);
        if (!m_editor->find(text, getFindFlags())) {
            QMessageBox::information(this, "Find", "Text not found.");
        }
    }
}

void FindReplace::replace()
{
    if (m_editor->textCursor().selectedText().compare(
            m_findEdit->text(), 
            m_caseSensitiveCheck->isChecked() ? Qt::CaseSensitive : Qt::CaseInsensitive) == 0) 
    {
        m_editor->textCursor().insertText(m_replaceEdit->text());
    }
    findNext();
}

void FindReplace::replaceAll()
{
    const QString findText = m_findEdit->text();
    const QString replaceText = m_replaceEdit->text();
    if (findText.isEmpty()) return;

    QTextCursor cursor(m_editor->document());
    cursor.beginEditBlock(); // Groups the undo operation
    cursor.movePosition(QTextCursor::Start);

    int count = 0;
    while (true) {
        cursor = m_editor->document()->find(findText, cursor, getFindFlags());
        if (cursor.isNull()) break;
        cursor.insertText(replaceText);
        count++;
    }
    cursor.endEditBlock();

    QMessageBox::information(this, "Replace All", QString("Replaced %1 occurrences.").arg(count));
}