#include <QHexLineEdit.h>
#include <QPainter>
#include <QKeyEvent>
#include <QDebug>

QHexLineEdit::QHexLineEdit(QWidget *parent) :
    QLineEdit(parent)
{
    setStyleSheet("QLineEdit { padding: 0px 0px 0px 13px; }");
}


void QHexLineEdit::paintEvent(QPaintEvent *event)
{
    QLineEdit::paintEvent(event);
    QPainter painter(this);
    painter.setPen(QColor::fromRgb(0x000000));
    painter.drawText(contentsMargins().left() + 3, 0, fontMetrics().width("0x"), height() -1, Qt::AlignVCenter, "0x");
}

void QHexLineEdit::keyPressEvent(QKeyEvent *event)
{
    int key = event->key();
    int mod = event->modifiers();
    bool success = false;
    event->text().toInt(&success, 16);
    if (key == Qt::Key_Backspace || key == Qt::Key_Return || key == Qt::Key_Enter ||
            mod == Qt::ControlModifier)
        success = true;
    if (success == false)
    {
        event->accept();
        return;
    }
    QLineEdit::keyPressEvent(new QKeyEvent(QEvent::KeyPress,
                                           event->key(), event->modifiers(),
                                           event->text().toUpper()));
}
