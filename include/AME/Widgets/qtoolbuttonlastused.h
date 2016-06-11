#ifndef QTOOLBUTTONLASTUSED_H
#define QTOOLBUTTONLASTUSED_H

#include <QToolButton>
#include <QDebug>

class QToolButtonLastUsed : public QToolButton
{
    Q_OBJECT
public:
    explicit QToolButtonLastUsed(QWidget *parent = 0);
};

#endif // QTOOLBUTTONLASTUSED_H
