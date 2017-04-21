#ifndef QHEXSPINBOX_H
#define QHEXSPINBOX_H

#include <QSpinBox>

class QHexSpinBox: public QSpinBox
{
    Q_OBJECT

    Q_PROPERTY(int byteLength READ byteLength WRITE setByteLength)
    Q_PROPERTY(bool upperCase READ upperCase WRITE setUpperCase)

public:
    QHexSpinBox(QWidget * parent = 0) :
        QSpinBox(parent),
        m_byteLength(0),
        m_upperCase(false)
    {
    }

    virtual QString textFromValue (int value) const;
    virtual StepEnabled stepEnabled() const;

    int byteLength() const;
    void setByteLength(int length);

    bool upperCase() const;
    void setUpperCase(bool ucase);

private:
    int m_byteLength;
    bool m_upperCase;

};

#endif // QHEXSPINBOX_H
