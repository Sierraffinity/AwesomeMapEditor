#ifndef QRESIZINGSTACKEDWIDGET_H
#define QRESIZINGSTACKEDWIDGET_H

#include <QStackedWidget>

class QResizingStackedWidget : public QStackedWidget
{
public:

    explicit QResizingStackedWidget(QWidget *parent = 0);
    ~QResizingStackedWidget() { }


    QSize sizeHint();
    QSize minimumSizeHint();
};

#endif // QRESIZINGSTACKEDWIDGET_H
