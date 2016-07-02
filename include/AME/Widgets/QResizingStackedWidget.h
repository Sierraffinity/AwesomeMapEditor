#ifndef QRESIZINGSTACKEDWIDGET_H
#define QRESIZINGSTACKEDWIDGET_H

class QResizingStackedWidget : public QStackedWidget
{
public:
    explicit QResizingStackedWidget(QWidget *parent = 0)
    {

    }

    virtual ~QResizingStackedWidget()
    {

    }

    QSize sizeHint() const override
    {
        return currentWidget()->sizeHint();
    }

    QSize minimumSizeHint() const override
    {
        return currentWidget()->minimumSizeHint();
    }
};

#endif // QRESIZINGSTACKEDWIDGET_H
