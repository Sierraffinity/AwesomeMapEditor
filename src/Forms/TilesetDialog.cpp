#include "TilesetDialog.h"
#include "ui_TilesetDialog.h"

namespace ame {

TilesetDialog::TilesetDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::TilesetDialog)
{
    ui->setupUi(this);
}

TilesetDialog::~TilesetDialog()
{
    delete ui;
}

} // namespace ame
