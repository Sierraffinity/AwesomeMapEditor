#ifndef QHEXLINEEDIT_H
#define QHEXLINEEDIT_H

#include <QLineEdit>

class QHexLineEdit : public QLineEdit
{
    Q_OBJECT

    //Q_PROPERTY(int byteLength READ byteLength WRITE setByteLength)

public:
    explicit QHexLineEdit(QWidget *parent = 0);

protected:
    void paintEvent(QPaintEvent *event);
    void keyPressEvent(QKeyEvent *event);

private:
    int m_byteLength;

};

#endif // QHEXLINEEDIT_H
