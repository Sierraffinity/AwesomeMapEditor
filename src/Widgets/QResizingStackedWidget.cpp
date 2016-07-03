#include <AME/Widgets/QResizingStackedWidget.h>


QResizingStackedWidget::QResizingStackedWidget(QWidget *parent)
    : QStackedWidget(parent)
{
}

QSize QResizingStackedWidget::sizeHint()
{
    return currentWidget()->sizeHint();
}

QSize QResizingStackedWidget::minimumSizeHint()
{
    return currentWidget()->minimumSizeHint();
}
