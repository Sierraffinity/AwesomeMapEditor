//////////////////////////////////////////////////////////////////////////////////
//
//
//                     d88b         888b           d888  888888888888
//                    d8888b        8888b         d8888  888
//                   d88''88b       888'8b       d8'888  888
//                  d88'  '88b      888 '8b     d8' 888  8888888
//                 d88Y8888Y88b     888  '8b   d8'  888  888
//                d88""""""""88b    888   '8b d8'   888  888
//               d88'        '88b   888    '888'    888  888
//              d88'          '88b  888     '8'     888  888888888888
//
//
// AwesomeMapEditor: A map editor for GBA Pokémon games.
// Copyright (C) 2016 Diegoisawesome, Pokedude
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either version 3
// of the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
//
//////////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////
// Include files
//
///////////////////////////////////////////////////////////
#include <AME/Widgets/Rendering/AMEBorderView.h>
#include <QPainter>


namespace ame
{
    ///////////////////////////////////////////////////////////
    // Function type:  Constructor
    // Contributors:   Pokedude
    // Last edit by:   Pokedude
    // Date of edit:   6/19/2016
    //
    ///////////////////////////////////////////////////////////
    AMEBorderView::AMEBorderView(QWidget *parent)
        : QWidget(parent)
    {
    }

    ///////////////////////////////////////////////////////////
    // Function type:  Destructor
    // Contributors:   Pokedude
    // Last edit by:   Pokedude
    // Date of edit:   6/19/2016
    //
    ///////////////////////////////////////////////////////////
    AMEBorderView::~AMEBorderView()
    {
    }


    ///////////////////////////////////////////////////////////
    // Function type:  Helper
    // Contributors:   Pokedude
    // Last edit by:   Pokedude
    // Date of edit:   6/19/2016
    //
    ///////////////////////////////////////////////////////////
    inline void extractBorderBlock(UInt8 *dest, UInt8 *pixels, UInt16 block)
    {
        int x = (block % 8) * 16;
        int y = (block / 8) * 16;
        int pos = 0;

         for (int y2 = 0; y2 < 16; y2++)
             for (int x2 = 0; x2 < 16; x2++)
                 dest[pos++] = pixels[(x+x2) + (y+y2) * 128];
    }


    ///////////////////////////////////////////////////////////
    // Function type:  Setter
    // Contributors:   Pokedude
    // Last edit by:   Pokedude
    // Date of edit:   6/19/2016
    //
    ///////////////////////////////////////////////////////////
    void AMEBorderView::setMapView(AMEMapView *view, bool layout)
    {
        const QImage &bbg = view->blockBackground();
        const QImage &bfg = view->blockForeground();


        // Fetches the border and the blockset pixel buffers
        MapBorder *pbd;
        if (!layout)
            pbd = &view->mainMap()->header().border();
        else
            pbd = &view->border();

        MapBorder &border = *pbd;
        m_BorderImage = QImage(pbd->width() * 16, pbd->height() * 16, QImage::Format_ARGB32_Premultiplied);

        QPainter painter(&m_BorderImage);
        // Fetches every block and copies it to the pixel buffers
        for (int i = 0; i < border.blocks().size(); i++)
        {
            UInt16 block = border.blocks().at(i)->block;
            Int32 mapX = (i % border.width()) * 16;
            Int32 mapY = (i / border.width()) * 16;
            Int32 bX = (block % 8) * 16;
            Int32 bY = (block / 8) * 16;

            painter.drawImage(QRect(mapX, mapY, 16, 16), bbg, QRect(bX, bY, 16, 16));
            painter.drawImage(QRect(mapX, mapY, 16, 16), bfg, QRect(bX, bY, 16, 16));
        }

        // Sets the minimum size
        painter.end();
        setMinimumSize(border.width()*16, border.height()*16);
    }

    ///////////////////////////////////////////////////////////
    // Function type:  Event
    // Contributors:   Pokedude
    // Last edit by:   Pokedude
    // Date of edit:   6/19/2016
    //
    ///////////////////////////////////////////////////////////
    void AMEBorderView::paintEvent(QPaintEvent *event)
    {
        Q_UNUSED(event);
        QPainter painter(this);
        painter.drawImage(0, 0, m_BorderImage);
        painter.end();
    }
}
