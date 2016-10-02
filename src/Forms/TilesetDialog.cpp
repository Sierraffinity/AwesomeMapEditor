#include "TilesetDialog.h"
#include "ui_TilesetDialog.h"

namespace ame {

TilesetDialog::TilesetDialog(QWidget *parent, AMEMapView *mv, AMEBlockView *bv) :
    QDialog(parent),
    ui(new Ui::TilesetDialog),
    m_TilesetPri(mv->mainMap()->header().primary()),
    m_TilesetSec(mv->mainMap()->header().secondary())
{

    ui->setupUi(this);
    bv->copyBlocks(ui->openGLWidget);
}

TilesetDialog::~TilesetDialog()
{
    delete ui;
}

} // namespace ame
