#ifndef QHEXLINEEDIT_H
#define QHEXLINEEDIT_H

#include <QLineEdit>

class QHexLineEdit : public QLineEdit
{
    Q_OBJECT

    /*! Property prefix, sets the prefix.
    */
    //Q_PROPERTY(bool prefix READ prefix WRITE setPrefix)

public:
    explicit QHexLineEdit(QWidget *parent = 0);
    //QString asciiArea;
    //void setPrefix(QString prefix);
protected:
    void paintEvent(QPaintEvent *event);
    void keyPressEvent(QKeyEvent *event);
    
};

#endif // QHEXLINEEDIT_H
