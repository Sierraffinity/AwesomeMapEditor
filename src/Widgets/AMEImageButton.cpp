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
#include <AME/Widgets/AMEImageButton.h>


namespace ame
{
    ///////////////////////////////////////////////////////////
    // Function type:  Constructor
    // Contributors:   Pokedude
    // Last edit by:   Pokedude
    // Date of edit:   6/18/2016
    //
    ///////////////////////////////////////////////////////////
    AMEImageButton::AMEImageButton(QWidget *parent)
        : QPushButton(parent)
    {
    }

    ///////////////////////////////////////////////////////////
    // Function type:  Destructor
    // Contributors:   Pokedude
    // Last edit by:   Pokedude
    // Date of edit:   6/18/2016
    //
    ///////////////////////////////////////////////////////////
    AMEImageButton::~AMEImageButton()
    {
    }


    ///////////////////////////////////////////////////////////
    // Function type:  Virtual
    // Contributors:   Pokedude
    // Last edit by:   Pokedude
    // Date of edit:   6/18/2016
    //
    ///////////////////////////////////////////////////////////
    void AMEImageButton::showEvent(QShowEvent *event)
    {
        Q_UNUSED(event);
        m_CurrentImage = icon().pixmap(width(), height(), QIcon::Normal, QIcon::On);
    }

    ///////////////////////////////////////////////////////////
    // Function type:  Virtual
    // Contributors:   Pokedude
    // Last edit by:   Pokedude
    // Date of edit:   6/18/2016
    //
    ///////////////////////////////////////////////////////////
    void AMEImageButton::enterEvent(QEvent *event)
    {
        Q_UNUSED(event);
        m_CurrentImage = icon().pixmap(width(), height(), QIcon::Selected, QIcon::On);
    }

    ///////////////////////////////////////////////////////////
    // Function type:  Virtual
    // Contributors:   Pokedude
    // Last edit by:   Pokedude
    // Date of edit:   6/18/2016
    //
    ///////////////////////////////////////////////////////////
    void AMEImageButton::leaveEvent(QEvent *event)
    {
        Q_UNUSED(event);
        m_CurrentImage = icon().pixmap(width(), height(), QIcon::Normal, QIcon::On);
    }

    ///////////////////////////////////////////////////////////
    // Function type:  Virtual
    // Contributors:   Pokedude
    // Last edit by:   Pokedude
    // Date of edit:   6/18/2016
    //
    ///////////////////////////////////////////////////////////
    void AMEImageButton::mousePressEvent(QMouseEvent *event)
    {
        Q_UNUSED(event);
        m_CurrentImage = icon().pixmap(width(), height(), QIcon::Selected, QIcon::On);
    }

    ///////////////////////////////////////////////////////////
    // Function type:  Virtual
    // Contributors:   Pokedude
    // Last edit by:   Pokedude
    // Date of edit:   6/18/2016
    //
    ///////////////////////////////////////////////////////////
    void AMEImageButton::mouseReleaseEvent(QMouseEvent *event)
    {
        Q_UNUSED(event);
        emit clicked();

        m_CurrentImage = icon().pixmap(width(), height(), QIcon::Normal, QIcon::On);
    }

    ///////////////////////////////////////////////////////////
    // Function type:  Virtual
    // Contributors:   Pokedude
    // Last edit by:   Pokedude
    // Date of edit:   6/18/2016
    //
    ///////////////////////////////////////////////////////////
    void AMEImageButton::paintEvent(QPaintEvent *event)
    {
        Q_UNUSED(event);

        // Inits painting
        QPainter painter(this);


        // Checks if button even enabled
        if (isEnabled())
        {
            painter.drawPixmap(0, 0, width(), height(), m_CurrentImage);
        }
        else
        {
            painter.drawPixmap(0, 0, width(), height(), icon().pixmap(width(), height(), QIcon::Disabled, QIcon::On));
        }


        // Done painting
        painter.end();
    }
}
