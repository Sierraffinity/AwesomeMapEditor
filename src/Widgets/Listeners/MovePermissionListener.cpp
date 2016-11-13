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
#include <AME/Widgets/Listeners/MovePermissionListener.h>


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
        m_SelectedIndex = 0;
        m_HighlightedBlock = 0;
        m_ShowCursor = false;
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

        if (event->type() == QEvent::MouseButtonPress)
        {
            QMouseEvent *me = reinterpret_cast<QMouseEvent *>(event);

            int mouseX = me->pos().x();
            int mouseY = me->pos().y();
            if (mouseX < 0 || mouseY < 0 ||
                mouseX >= obj->width() || mouseY >= obj->height() ||
                me->buttons() != Qt::MouseButton::LeftButton)
            {
                return QObject::eventFilter(o, event);
            }

            m_ShowCursor = false;
            m_Selecting = true;

            // Align the position to a block
            mouseX /= 16;
            mouseY /= 16;

            // Sets the position
            m_SelectedIndex = mouseX + (mouseY * 4);
            obj->repaint();

            // Simulates mouse button press on label
            return QObject::eventFilter(o, event);
        }
        if (event->type() == QEvent::MouseButtonRelease)
        {
            m_ShowCursor = true;
            m_Selecting = false;
        }
        else if (event->type() == QEvent::MouseMove)
        {
            QMouseEvent *me = reinterpret_cast<QMouseEvent *>(event);

            int mouseX = me->pos().x();
            int mouseY = me->pos().y();

            if (mouseX < 0)
                mouseX = 0;
            if (mouseY < 0)
                mouseY = 0;
            if (mouseX >= obj->width())
                mouseX = obj->width() - 1;
            if (mouseY >= obj->height())
                mouseY = obj->height() - 1;

            if (!m_Selecting)
                m_ShowCursor = true;

            // Align the position to a block
            mouseX /= 16;
            mouseY /= 16;

            // Sets the position
            if (m_HighlightedBlock != mouseX + (mouseY * 4))
            {
                m_HighlightedBlock = mouseX + (mouseY * 4);
                if (m_Selecting)
                    m_SelectedIndex = m_HighlightedBlock;
                obj->repaint();
            }

            // Simulates mouse button press on label
            return QObject::eventFilter(o, event);
        }
        else if (event->type() == QEvent::Leave)
        {
            m_ShowCursor = false;
        }
        else if (event->type() == QEvent::Paint/* && m_SelectedIndex.first != -1 && m_SelectedIndex.second != -1*/)
        {
            bool result = QObject::eventFilter(o, event);

            QPixmap pm(":/images/Permissions_16x16.png");
            QPainter painter(&pm);
            painter.setPen(Qt::GlobalColor::red);
            painter.drawRect((m_SelectedIndex % 4) * 16, (m_SelectedIndex / 4) * 16, 15, 15);
            if (m_ShowCursor)
            {
                painter.setPen(Qt::GlobalColor::green);
                painter.drawRect((m_HighlightedBlock % 4) * 16, (m_HighlightedBlock / 4) * 16, 15, 15);
            }
            painter.end();
            obj->setPixmap(pm);

            return result;
        }
        
        return QObject::eventFilter(o, event);
    }

    UInt16 MovePermissionListener::getSelectedIndex()
    {
        return m_SelectedIndex;
    }
}
