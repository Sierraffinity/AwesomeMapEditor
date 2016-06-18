#ifndef QHEXSPINBOX_H
#define QHEXSPINBOX_H

#include <QSpinBox>

class QHexSpinBox: public QSpinBox
{
    Q_OBJECT

public:
    QHexSpinBox( QWidget * parent = 0) :
        QSpinBox(parent)
    {
    }

    virtual QString textFromValue ( int value ) const
    {
        /* 4 - number of digits, 10 - base of number, '0' - pad character*/
        return QString("%1").arg(value, 2 , 16, QChar('0')).toUpper();
    }
};

#endif // QHEXSPINBOX_H
