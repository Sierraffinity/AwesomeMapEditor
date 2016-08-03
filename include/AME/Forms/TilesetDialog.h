#ifndef AME_TILESETDIALOG_H
#define AME_TILESETDIALOG_H

#include <QDialog>

namespace ame {

namespace Ui {
class TilesetDialog;
}

class TilesetDialog : public QDialog
{
    Q_OBJECT

public:
    explicit TilesetDialog(QWidget *parent = 0);
    ~TilesetDialog();

private:
    Ui::TilesetDialog *ui;
};


} // namespace ame
#endif // AME_TILESETDIALOG_H
