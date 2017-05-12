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
#include <AME/Widgets/Rendering/AMEEntityView.h>
#include <AME/System/Configuration.hpp>
#include <AME/System/Settings.hpp>


namespace ame
{
    ///////////////////////////////////////////////////////////
    // Function type:  Constructor
    // Contributors:   Pokedude, Nekaida
    // Last edit by:   Nekaida
    // Date of edit:   3/18/2017
    //
    ///////////////////////////////////////////////////////////
    AMEEntityView::AMEEntityView(QWidget *parent)
        : QWidget(parent),
          m_MapView(0),
          m_ShowGrid(false),
          m_IsInit(false)
    {
        m_FieldImage = QImage(":/images/Entities_16x16.png");
    }

    ///////////////////////////////////////////////////////////
    // Function type:  Destructor
    // Contributors:   Pokedude
    // Last edit by:   Pokedude
    // Date of edit:   6/19/2016
    //
    ///////////////////////////////////////////////////////////
    AMEEntityView::~AMEEntityView()
    {
    }


    ///////////////////////////////////////////////////////////
    // Function type:  Virtual
    // Contributors:   Pokedude
    // Last edit by:   Pokedude
    // Date of edit:   7/1/2016
    //
    ///////////////////////////////////////////////////////////
    void AMEEntityView::mousePressEvent(QMouseEvent *event)
    {
        emit onMouseClick(event);
    }

    ///////////////////////////////////////////////////////////
    // Function type:  Virtual
    // Contributors:   Diegoisawesome
    // Last edit by:   Diegoisawesome
    // Date of edit:   12/7/2016
    //
    ///////////////////////////////////////////////////////////
    void AMEEntityView::mouseDoubleClickEvent(QMouseEvent *event)
    {
        /*if (event->button() == Qt::LeftButton)
        {
            // TODO: Make this work for event view as well
            int mouseX = event->pos().x();
            int mouseY = event->pos().y();
            for (int i = 1; i < m_MapView->m_Maps.count(); i++)
            {
                QList<QPoint> &mapPos = m_MapView->m_MapPositions;
                QList<QSize> &mapSize = m_MapView->m_MapSizes;
                QPoint mp = mapPos.at(i);
                QSize mz = mapSize.at(i);
                if (mouseX >= mp.x()              && mouseY >= mp.y()            &&
                    mouseX <= mp.x() + mz.width() && mouseY <= mp.y() + mz.height())
                {
                    emit loadMapChangeTreeView(m_MapView->m_Maps[i]);
                }
            }
        }*/
        emit onDoubleClick(event);
    }

    ///////////////////////////////////////////////////////////
    // Function type:  Virtual
    // Contributors:   Pokedude
    // Last edit by:   Pokedude
    // Date of edit:   7/1/2016
    //
    ///////////////////////////////////////////////////////////
    void AMEEntityView::paintEvent(QPaintEvent *event)
    {
        Q_UNUSED(event);
        if (m_MapView && m_Entities)
        {
            // Draws the original map
            QPainter painter(this);
            QPoint mpos = m_MapView->mainPos();

            painter.drawPixmap(mpos.x(), mpos.y(), m_MapView->m_MapBackground);
            /* Do your entity stuff here, Diego <3 */
            painter.drawPixmap(mpos.x(), mpos.y(), m_MapView->m_MapForeground);

            if (!m_MapView->m_LayoutView)
            {
                int conncnt = m_MapView->m_MapSizes.size() - 1;
                for (int i = 0; i < conncnt; i++)
                {
                    QPoint pconn = m_MapView->m_MapPositions.at(i + 1);
                    painter.drawImage(pconn.x(), pconn.y(), m_MapView->m_ConnImages.at(i));
                }
            }

            painter.translate(m_Translation);

            // Draws movement boundaries
            if (m_Selection.type == ET_Npc)
            {
                Npc *npc = static_cast<Npc *>(m_Selection.entity);
                Int32 nX = (npc->positionX * 16) - (npc->moveRadiusX * 16);
                Int32 nY = (npc->positionY * 16) - (npc->moveRadiusY * 16);
                Int32 nW = npc->moveRadiusX * 32 + 15;
                Int32 nH = npc->moveRadiusY * 32 + 15;

                painter.setOpacity(0.2);
                painter.fillRect(nX, nY, nW, nH, QColor(255, 0, 255));
                painter.setOpacity(0.4);
                painter.setPen(QColor(255, 0, 255));
                painter.drawRect(nX, nY, nW, nH);
                painter.setOpacity(1.0);
            }

            QRect nsrc(0, 0, 16, 16);
            QRect wsrc(16, 0, 16, 16);
            QRect tsrc(32, 0, 16, 16);
            QRect ssrc(48, 0, 16, 16);
            painter.setOpacity(0.5);

            // Paints all entities
            foreach (const Npc *npc, m_Entities->npcs())
            {
                if (SETTINGS(ShowSprites))
                {
                    // TODO: Decide what to do with 'invalid' image IDs going forward
                    UInt8 imageID = npc->imageID;
                    if (imageID > CONFIG(OverworldCount))
                        imageID = 0;
                    const QImage &img = m_OW->at(imageID);
                    int x = (npc->positionX * 16) - (img.width() / 2) + 8;
                    int y = (npc->positionY * 16) - img.height() + 16;

                    QRect dst(x, y, img.width(), img.height());
                    QRect src(0, 0, img.width(), img.height());
                    painter.setOpacity(1.0);
                    painter.drawImage(dst, img, src);
                    painter.setOpacity(0.5);
                }
                else
                {
                    QRect dst(npc->positionX*16, npc->positionY*16, 16, 16);
                    painter.drawImage(dst, m_FieldImage, nsrc);
                }
            }
            foreach (const Warp *warp, m_Entities->warps())
            {
                QRect dst(warp->positionX*16, warp->positionY*16, 16, 16);
                painter.drawImage(dst, m_FieldImage, wsrc);
            }
            foreach (const Trigger *trigg, m_Entities->triggers())
            {
                QRect dst(trigg->positionX*16, trigg->positionY*16, 16, 16);
                painter.drawImage(dst, m_FieldImage, tsrc);
            }
            foreach (const Sign *sign, m_Entities->signs())
            {
                QRect dst(sign->positionX*16, sign->positionY*16, 16, 16);
                painter.drawImage(dst, m_FieldImage, ssrc);
            }

            painter.resetTransform();
            painter.setOpacity(1.0);
            QPoint orig = m_MapView->m_MapPositions.at(0);
            if (m_ShowGrid)
            {
                const QSize &ms = m_MapView->m_Maps.at(0)->header().size();
                QVector<QLine> lines;
                for (int i = 0; i < ms.width(); i++)
                    lines.append(QLine(orig.x() + (i * 16), orig.y(), orig.x() + (i * 16), orig.y() + ms.height() * 16 - 1));
                for (int i = 0; i < ms.height(); i++)
                    lines.append(QLine(orig.x(), orig.y() + (i * 16), orig.x() + ms.width() * 16 - 1, orig.y() + (i * 16)));
                painter.setPen(Qt::GlobalColor::black);
                painter.drawLines(lines);
            }

            // Draws the selection square
            if (m_Selection.entity)
            {
                Int32 pX = mpos.x() + m_Selection.position.x();
                Int32 pY = mpos.y() + m_Selection.position.y();
                painter.setPen(QColor(255, 0, 0));
                painter.drawRect(pX, pY, 15, 15);
            }
        }
    }


    ///////////////////////////////////////////////////////////
    // Function type:  Setter
    // Contributors:   Pokedude
    // Last edit by:   Pokedude
    // Date of edit:   6/19/2016
    //
    ///////////////////////////////////////////////////////////
    void AMEEntityView::setEntities(Map *map)
    {
        m_Entities = &map->entities();
    }

    ///////////////////////////////////////////////////////////
    // Function type:  Setter
    // Contributors:   Pokedude
    // Last edit by:   Pokedude
    // Date of edit:   6/19/2016
    //
    ///////////////////////////////////////////////////////////
    void AMEEntityView::setOverworlds(const QList<QImage> *ows)
    {
        m_OW = ows;
    }

    ///////////////////////////////////////////////////////////
    // Function type:  Setter
    // Contributors:   Pokedude
    // Last edit by:   Pokedude
    // Date of edit:   6/19/2016
    //
    ///////////////////////////////////////////////////////////
    void AMEEntityView::setMapView(AMEMapView *view)
    {
        m_MapView = view;
        m_Translation = view->mainPos();
        setMinimumSize(m_MapView->minimumSize());
        m_MapView->resize(m_MapView->minimumSize());
    }

    ///////////////////////////////////////////////////////////
    // Function type:  Setter
    // Contributors:   Pokedude
    // Last edit by:   Pokedude
    // Date of edit:   1/7/2016
    //
    ///////////////////////////////////////////////////////////
    void AMEEntityView::setCurrentEntity(CurrentEntity entity)
    {
        m_Selection = entity;
        update();
    }

    ///////////////////////////////////////////////////////////
    // Function type:  Setter
    // Contributors:   Nekaida
    // Last edit by:   Nekaida
    // Date of edit:   5/11/2017
    //
    ///////////////////////////////////////////////////////////
    void AMEEntityView::setCurrentEntityPosX(int x)
    {
        m_Selection.position.setX(x);
    }

    ///////////////////////////////////////////////////////////
    // Function type:  Setter
    // Contributors:   Nekaida
    // Last edit by:   Nekaida
    // Date of edit:   5/11/2017
    //
    ///////////////////////////////////////////////////////////
    void AMEEntityView::setCurrentEntityPosY(int y)
    {
        m_Selection.position.setY(y);
    }

    ///////////////////////////////////////////////////////////
    // Function type:  Setter
    // Contributors:   Diegoisawesome
    // Last edit by:   Diegoisawesome
    // Date of edit:   12/11/2016
    //
    ///////////////////////////////////////////////////////////
    void AMEEntityView::setGridVisible(bool visible)
    {
        m_ShowGrid = visible;
        repaint();
    }
}
