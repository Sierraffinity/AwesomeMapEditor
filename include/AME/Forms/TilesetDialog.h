#ifndef AME_TILESETDIALOG_H
#define AME_TILESETDIALOG_H

#include <QDialog>
#include <AME/Graphics/TilesetManager.hpp>
#include <AME/Widgets/OpenGL/AMEBlockView.h>

namespace ame {

namespace Ui {
class TilesetDialog;
}

class TilesetDialog : public QDialog
{
    Q_OBJECT

public:

    explicit TilesetDialog(QWidget *parent, AMEMapView *mv, AMEBlockView *bv);
    ~TilesetDialog();

private:

    Ui::TilesetDialog *ui;
    Tileset *m_TilesetPri;
    Tileset *m_TilesetSec;
};


} // namespace ame
#endif // AME_TILESETDIALOG_H
