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
#include <AME/Widgets/Listeners/MovePermissionListener.hpp>


namespace ame
{
    ///////////////////////////////////////////////////////////
    // Function type:  Constructor
    // Contributors:   Pokedude
    // Last edit by:   Pokedude
    // Date of edit:   8/25/2016
    //
    ///////////////////////////////////////////////////////////
    MovePermissionListener::MovePermissionListener(QWidget *parent)
        : QObject(parent)
    {
        m_SelectedIndex.first = -1;
        m_SelectedIndex.second = -1;
    }

    ///////////////////////////////////////////////////////////
    // Function type:  Destructor
    // Contributors:   Pokedude
    // Last edit by:   Pokedude
    // Date of edit:   8/25/2016
    //
    ///////////////////////////////////////////////////////////
    MovePermissionListener::~MovePermissionListener()
    {
    }
    
    ///////////////////////////////////////////////////////////
    // Function type:  Virtual
    // Contributors:   Pokedude
    // Last edit by:   Pokedude
    // Date of edit:   8/25/2016
    //
    ///////////////////////////////////////////////////////////
    bool MovePermissionListener::eventFilter(QObject *o, QEvent *event)
    {
        QLabel *obj = dynamic_cast<QLabel *>(o);
        if (event->type() == QEvent::MouseButtonRelease)
        {
            QMouseEvent *me = reinterpret_cast<QMouseEvent *>(event);
            
            int mouseX = me->pos().x();
            int mouseY = me->pos().y();
            if (mouseX >= obj->width() || mouseY >= obj->height())
            {
                m_SelectedIndex.first = -1;
                m_SelectedIndex.second = -1;
                obj->repaint();
                return QObject::eventFilter(o, event);
            }
            
            // Align the position to a multiple of 16px
            while (mouseX % 16 != 0)
                mouseX--;
            while (mouseY % 16 != 0)
                mouseY--;
            
            // Sets the position
            m_SelectedIndex.first = mouseX;
            m_SelectedIndex.second = mouseY;
            obj->repaint();
            
            // Simulates mouse button press on label
            return QObject::eventFilter(o, event);
        }
        else if (event->type() == QEvent::Paint && m_SelectedIndex.first != -1 && m_SelectedIndex.second != -1)
        {
            bool result = QObject::eventFilter(o, event);
            
            QPixmap pm(":/images/Permissions_16x16.png");
            QPainter painter(&pm);
            painter.setPen(0xFF0000);
            painter.setOpacity(0.8f);
            painter.drawRect(m_SelectedIndex.first, m_SelectedIndex.second, 15, 15);
            painter.end();
            obj->setPixmap(pm);
                        
            return result;
        }
        
        return QObject::eventFilter(o, event);
    }
}
