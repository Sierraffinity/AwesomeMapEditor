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
#include <AME/Widgets/Rendering/AMEBlockView.h>


namespace ame
{

    ///////////////////////////////////////////////////////////
    // Function type:  Constructor
    // Contributors:   Pokedude
    // Last edit by:   Pokedude
    // Date of edit:   6/18/2016
    //
    ///////////////////////////////////////////////////////////
    AMEBlockView::AMEBlockView(QWidget *parent)
        : QWidget(parent),
          m_SelectedBlocks({ 0 }),
          m_FirstBlock(0),
          m_LastBlock(0),
          m_CurrentTool(AMEMapView::Tool::None),
          m_CursorColor(Qt::GlobalColor::red),
          m_ShowHighlight(false),
          m_IsCopy(false),
          m_ShowGrid(false),
          m_IsInit(false)
    {
    }

    ///////////////////////////////////////////////////////////
    // Function type:  Destructor
    // Contributors:   Pokedude
    // Last edit by:   Pokedude
    // Date of edit:   6/18/2016
    //
    ///////////////////////////////////////////////////////////
    AMEBlockView::~AMEBlockView()
    {
    }


    ///////////////////////////////////////////////////////////
    // Function type:  Getter
    // Contributors:   Pokedude, Diegoisawesome
    // Last edit by:   Diegoisawesome
    // Date of edit:   10/4/2016
    //
    ///////////////////////////////////////////////////////////
    QVector<UInt16> AMEBlockView::selectedBlocks()
    {
        return m_SelectedBlocks;
    }

    ///////////////////////////////////////////////////////////
    // Function type:  Helper
    // Contributors:   Diegoisawesome
    // Last edit by:   Diegoisawesome
    // Date of edit:   10/7/2016
    //
    ///////////////////////////////////////////////////////////
    void AMEBlockView::selectBlock(UInt16 newBlock)
    {
        m_FirstBlock = newBlock;
        m_LastBlock = newBlock;
        m_SelectedBlocks = { newBlock };
        static_cast<QScrollArea*>(this->parentWidget()->parentWidget()->parentWidget())->ensureVisible(((newBlock % 8) * 16) + 8, ((newBlock / 8) * 16) + 8, 16, 16);  // TODO: Does not scroll correctly, probably wrong parent
        repaint();
        return;
    }

    ///////////////////////////////////////////////////////////
    // Function type:  Helper
    // Contributors:   Diegoisawesome
    // Last edit by:   Diegoisawesome
    // Date of edit:   10/7/2016
    //
    ///////////////////////////////////////////////////////////
    void AMEBlockView::deselectBlocks()
    {
        m_FirstBlock = -1;
        m_LastBlock = -1;
        m_SelectedBlocks.clear();
        return;
    }

    ///////////////////////////////////////////////////////////
    // Function type:  Setter
    // Contributors:   Pokedude
    // Last edit by:   Pokedude
    // Date of edit:   6/18/2016
    //
    ///////////////////////////////////////////////////////////
    void AMEBlockView::setMapView(AMEMapView *view)
    {
        m_Foreground = view->m_BlockForeground;
        m_Background = view->m_BlockBackground;
        m_IsInit = true;

        QVector<QRgb> pal = m_Background.colorTable();
        pal[0] = qRgb(0, 0, 0);
        m_Background.setColorTable(pal);

        // Sets the minimum size
        setMinimumSize(m_Foreground.size());
    }

    ///////////////////////////////////////////////////////////
    // Function type:  Getter
    // Contributors:   Diegoisawesome
    // Last edit by:   Diegoisawesome
    // Date of edit:   10/4/2016
    //
    ///////////////////////////////////////////////////////////
    AMEMapView::Tool AMEBlockView::getCurrentTool(Qt::MouseButtons buttons)
    {
        if (buttons & Qt::LeftButton)
        {
            if (m_CurrentTool == AMEMapView::Tool::Select)
                return AMEMapView::Tool::Select;
            return AMEMapView::Tool::Draw;
        }
        else if (buttons & Qt::RightButton)
        {
            if (m_CurrentTool == AMEMapView::Tool::Draw)
                return AMEMapView::Tool::Draw;
            return AMEMapView::Tool::Select;
        }
        return AMEMapView::Tool::None;
    }

    ///////////////////////////////////////////////////////////
    // Function type:  Getter
    // Contributors:   Diegoisawesome
    // Last edit by:   Diegoisawesome
    // Date of edit:   10/4/2016
    //
    ///////////////////////////////////////////////////////////
    void AMEBlockView::setCurrentTool(AMEMapView::Tool newTool)
    {
        m_CurrentTool = newTool;
    }

    ///////////////////////////////////////////////////////////
    // Function type:  Setter
    // Contributors:   Diegoisawesome
    // Last edit by:   Diegoisawesome
    // Date of edit:   11/9/2016
    //
    ///////////////////////////////////////////////////////////
    void AMEBlockView::setGridVisible(bool visible)
    {
        m_ShowGrid = visible;
        repaint();
    }

    ///////////////////////////////////////////////////////////
    // Function type:  Virtual
    // Contributors:   Diegoisawesome
    // Last edit by:   Diegoisawesome
    // Date of edit:   10/4/2016
    //
    ///////////////////////////////////////////////////////////
    void AMEBlockView::mousePressEvent(QMouseEvent *event)
    {
        m_ShowHighlight = false;
        AMEMapView::Tool currentTool = getCurrentTool(event->buttons());

        int mouseX = event->pos().x();
        int mouseY = event->pos().y();
        if (mouseX < 0 || mouseY < 0 ||
                mouseX >= width() || mouseY >= height())
        {
            return;
        }

        // Determines the block number
        m_FirstBlock = (mouseX/16) + ((mouseY/16)*8);
        m_LastBlock = m_FirstBlock;

        if (currentTool == AMEMapView::Tool::Select)
            m_CursorColor = Qt::GlobalColor::yellow;

        repaint();
    }

    ///////////////////////////////////////////////////////////
    // Function type:  Virtual
    // Contributors:   Pokedude, Diegoisawesome
    // Last edit by:   Diegoisawesome
    // Date of edit:   10/5/2016
    //
    ///////////////////////////////////////////////////////////
    void AMEBlockView::mouseReleaseEvent(QMouseEvent *event)
    {
        Q_UNUSED(event);
        m_ShowHighlight = true;
        m_CursorColor = Qt::GlobalColor::red;

        // Determines the tile-number
        int selectionWidth = (m_LastBlock % 8) - (m_FirstBlock % 8);
        if (selectionWidth < 0)
        {
            selectionWidth = -selectionWidth;
            m_FirstBlock -= selectionWidth;
            m_LastBlock += selectionWidth;
        }
        selectionWidth++;

        int selectionHeight = (m_LastBlock / 8) - (m_FirstBlock / 8);
        if (selectionHeight < 0)
        {
            selectionHeight = -selectionHeight;
            m_FirstBlock -= selectionHeight * 8;
            m_LastBlock += selectionHeight * 8;
        }
        selectionHeight++;

        m_SelectedBlocks.clear();

        for (int i = 0; i < selectionHeight; i++)
        {
            for (int j = 0; j < selectionWidth; j++)
            {
                m_SelectedBlocks.push_back(m_FirstBlock + j + (i * 8));
            }
        }
        repaint();
    }

    ///////////////////////////////////////////////////////////
    // Function type:  Virtual
    // Contributors:   Diegoisawesome
    // Last edit by:   Diegoisawesome
    // Date of edit:   10/6/2016
    //
    ///////////////////////////////////////////////////////////
    void AMEBlockView::mouseMoveEvent(QMouseEvent *event)
    {
        int mouseX = event->pos().x();
        int mouseY = event->pos().y();
        if (mouseX < 0)
            mouseX = 0;
        else if (mouseY < 0)
            mouseY = 0;
        else if (mouseX >= width())
            mouseX = width() - 1;
        else if (mouseY >= height())
            mouseY = height() - 1;

        AMEMapView::Tool currentTool = getCurrentTool(event->buttons());

        if (currentTool == AMEMapView::Tool::Draw)
        {
            // Determines the block number
            m_FirstBlock = (mouseX/16) + ((mouseY/16)*8);
            m_LastBlock = m_FirstBlock;
        }
        else if (currentTool == AMEMapView::Tool::Select)
        {
            // Determines the block number
            m_LastBlock = (mouseX/16) + ((mouseY/16)*8);
        }

        m_HighlightedBlock = (mouseX/16) + ((mouseY/16)*8);

        repaint();
    }

    ///////////////////////////////////////////////////////////
    // Function type:  Virtual
    // Contributors:   Diegoisawesome
    // Last edit by:   Diegoisawesome
    // Date of edit:   10/6/2016
    //
    ///////////////////////////////////////////////////////////
    void AMEBlockView::enterEvent(QEvent *event)
    {
        Q_UNUSED(event);
        m_ShowHighlight = true;

        repaint();
    }

    ///////////////////////////////////////////////////////////
    // Function type:  Virtual
    // Contributors:   Diegoisawesome
    // Last edit by:   Diegoisawesome
    // Date of edit:   10/6/2016
    //
    ///////////////////////////////////////////////////////////
    void AMEBlockView::leaveEvent(QEvent *event)
    {
        Q_UNUSED(event);
        m_ShowHighlight = false;

        repaint();
    }

    ///////////////////////////////////////////////////////////
    // Function type:  Virtual
    // Contributors:   Diegoisawesome
    // Last edit by:   Diegoisawesome
    // Date of edit:   10/6/2016
    //
    ///////////////////////////////////////////////////////////
    void AMEBlockView::paintEvent(QPaintEvent *event)
    {
        Q_UNUSED(event);
        if (m_IsInit)
        {
            QPainter painter(this);
            painter.drawImage(0, 0, m_Background);
            painter.drawImage(0, 0, m_Foreground);

            if (m_ShowGrid)
            {
                QVector<QLine> lines;
                for (int i = 0; i <= width() / 16; i++)
                    lines.append(QLine(i * 16, 0, i * 16, height() * 16));
                for (int i = 0; i <= height() / 16; i++)
                    lines.append(QLine(0, i * 16, width() * 16, i * 16));
                painter.setPen(Qt::GlobalColor::black);
                painter.drawLines(lines);
            }

            // Draw selection rectangle
            if (m_FirstBlock != -1 && m_LastBlock != -1)
            {
                // Computes the position on widget based on selected blocks
                int x, firstX, y, firstY, lastX, lastY, sWid, sHei;
                firstX = x = m_FirstBlock % 8;
                firstY = y = m_FirstBlock / 8;
                lastX = m_LastBlock % 8;
                lastY = m_LastBlock / 8;
                sWid = lastX - firstX;
                sHei = lastY - firstY;

                if (lastX < firstX)
                {
                    x = lastX;
                    sWid = firstX - x;
                }
                if (lastY < firstY)
                {
                    y = lastY;
                    sHei = firstY - y;
                }

                /*if (m_SelectedBlocks.size() == 1)
                {
                    sWid = 1;
                    sHei = 1;
                }*/

                sWid++;
                sHei++;

                // TODO: Dynamic
                x *= 16;
                y *= 16;
                sWid *= 16;
                sHei *= 16;

                painter.setPen(m_CursorColor);
                painter.drawRect(x, y, sWid, sHei);
            }

            // Shows the selection highlighting
            if (m_ShowHighlight)
            {
                int hiX = (m_HighlightedBlock % 8) * 16;
                int hiY = (m_HighlightedBlock / 8) * 16;

                painter.setPen(QColor(Qt::green));
                painter.drawRect(hiX, hiY, 16, 16);
            }
        }
    }
}
