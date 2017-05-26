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
#include <AME/System/Configuration.hpp>
#include <AME/System/Settings.hpp>
#include <AME/Widgets/Rendering/AMEMapView.h>
#include <AME/Widgets/Rendering/AMEEntityView.h>
#include <AME/Widgets/Rendering/AMEBlockView.h>
#include <QImage>


namespace ame
{
    ///////////////////////////////////////////////////////////
    // Local buffers
    //
    ///////////////////////////////////////////////////////////
    UInt8 pixelBuffer[64];
    UInt8 blockBuffer[256];


    ///////////////////////////////////////////////////////////
    // Function type:  Constructor
    // Contributors:   Pokedude
    // Last edit by:   Pokedude
    // Date of edit:   6/17/2016
    //
    ///////////////////////////////////////////////////////////
	AMEMapView::AMEMapView(QWidget *parent)
		: QWidget(parent),
		m_PrimaryForeground(0),
		m_PrimaryBackground(0),
		m_SecondaryForeground(0),
		m_SecondaryBackground(0),
		m_ShowSprites(false),
		m_MovementMode(false),
		m_BlockView(0),
		m_FirstBlock(QPoint(-1, -1)),
		m_LastBlock(QPoint(-1, -1)),
		m_HighlightedBlock(QPoint(-1, -1)),
		m_SelectSize(QSize(1, 1)),
		m_CurrentTool(Cursor::Tool::None),
		m_ShowCursor(false),
		m_ShowGrid(false),
		m_HoveredConnection(0),
		m_IsInit(false),
        m_MovePerm(QImage(":/images/PermGL.png")),
		m_Cursor()
    {
    }

    ///////////////////////////////////////////////////////////
    // Function type:  Destructor
    // Contributors:   Pokedude
    // Last edit by:   Pokedude
    // Date of edit:   6/17/2016
    //
    ///////////////////////////////////////////////////////////
    AMEMapView::~AMEMapView()
    {
        foreach (UInt8 *v, m_BackPixelBuffers)
            delete[] v;
        foreach (UInt8 *v, m_ForePixelBuffers)
            delete[] v;

        delete[] m_PrimaryForeground;
        delete[] m_PrimaryBackground;
        delete[] m_SecondaryForeground;
        delete[] m_SecondaryBackground;
    }


    ///////////////////////////////////////////////////////////
    // Function type:  Helper
    // Contributors:   Pokedude
    // Last edit by:   Pokedude
    // Date of edit:   6/17/2016
    //
    ///////////////////////////////////////////////////////////
    inline void updatePixels(int x, int y, int w, int h, QImage &i, uchar *src)
    {
        if (x >= i.width() || y >= i.height())
            return;
        for (int px = 0; px < w; px++)
            for (int py = 0; py < h; py++)
                i.bits()[x+px+((y+py)*i.width())] = src[px+py*w];
    }

    inline void updatePixels(int x1, int y1, int w1, int h1, QPixmap &p,
                             int x2, int y2, int w2, int h2, QImage &i)
    {
        QPainter painter(&p);
        painter.setCompositionMode(QPainter::CompositionMode_Source);
        painter.drawImage(QRect(x1,y1,w1,h1), i, QRect(x2,y2,w2,h2));
        painter.end();
    }

    ///////////////////////////////////////////////////////////
    // Function type:  Helper
    // Contributors:   Pokedude
    // Last edit by:   Pokedude
    // Date of edit:   6/17/2016
    //
    ///////////////////////////////////////////////////////////
    inline void convertGrayScale(QImage &img)
    {
        for (int i = 0; i < img.height(); i++)
        {
            uchar *scan = img.scanLine(i);
            for (int j = 0; j < img.width(); j++)
            {
                QRgb *rgb = reinterpret_cast<QRgb *>(scan + j*4);
                int gray = qGray(*rgb);
                *rgb = QColor(gray, gray, gray).rgba();
            }
        }
    }

    ///////////////////////////////////////////////////////////
    // Function type:  Helper
    // Contributors:   Pokedude
    // Last edit by:   Pokedude
    // Date of edit:   6/17/2016
    //
    ///////////////////////////////////////////////////////////
    inline void flipVertically(UInt8 *dest, UInt8 *pixels)
    {
        for (int i = 0; i < 8; i++)
            for (int j = 0; j < 8; j++)
                dest[i + j * 8] = pixels[i + (8-1-j) * 8];
    }

    ///////////////////////////////////////////////////////////
    // Function type:  Helper
    // Contributors:   Pokedude
    // Last edit by:   Pokedude
    // Date of edit:   6/17/2016
    //
    ///////////////////////////////////////////////////////////
    inline void flipHorizontally(UInt8 *dest, UInt8 *pixels)
    {
        for (int i = 0; i < 64; i++)
            dest[i] = pixels[i - 2*(i%8) + 8-1];
    }

    ///////////////////////////////////////////////////////////
    // Function type:  Helper
    // Contributors:   Pokedude
    // Last edit by:   Pokedude
    // Date of edit:   6/17/2016
    //
    ///////////////////////////////////////////////////////////
    inline void extractTile(const QByteArray &img, Tile &tile)
    {
        unsigned char buffer[64];
        int x = (tile.tile % 16) * 8;
        int y = (tile.tile / 16) * 8;
        int pos = 0;

        for (int y2 = 0; y2 < 8; y2++)
            for (int x2 = 0; x2 < 8; x2++)
                pixelBuffer[pos++] = (UInt8)((img.at((x+x2) + (y+y2) * 128)) + (tile.palette * 16));

        if (tile.flipX)
        {
            flipHorizontally(buffer, pixelBuffer);
            if (tile.flipY)
                flipVertically(pixelBuffer, buffer);
            else
                memcpy(pixelBuffer, buffer, 64);
        }
        else if (tile.flipY)
        {
            flipVertically(buffer, pixelBuffer);
            memcpy(pixelBuffer, buffer, 64);
        }
    }

    ///////////////////////////////////////////////////////////
    // Function type:  Helper
    // Contributors:   Pokedude
    // Last edit by:   Pokedude
    // Date of edit:   6/17/2016
    //
    ///////////////////////////////////////////////////////////
    inline void extractBlock(UInt8 *pixels, UInt16 block)
    {
        int x = (block % 8) * 16;
        int y = (block / 8) * 16;
        int pos = 0;

        for (int y2 = 0; y2 < 16; y2++)
            for (int x2 = 0; x2 < 16; x2++)
                blockBuffer[pos++] = pixels[(x+x2) + (y+y2) * 128];
    }

    ///////////////////////////////////////////////////////////
    // Function type:  Setter
    // Contributors:   Diegoisawesome
    // Last edit by:   Diegoisawesome
    // Date of edit:   10/24/2016
    //
    ///////////////////////////////////////////////////////////
    void AMEMapView::setCurrentTool(AMEMapView::Tool tool)
    {
        m_CurrentTool = static_cast<Cursor::Tool>(tool);
    }

    ///////////////////////////////////////////////////////////
    // Function type:  Getter
    // Contributors:   Diegoisawesome
    // Last edit by:   Diegoisawesome
    // Date of edit:   10/24/2016
    //
    ///////////////////////////////////////////////////////////
    Cursor::Tool AMEMapView::getCurrentTool(Qt::MouseButtons buttons)
    {
        if (buttons != Qt::NoButton)
        {
            if (m_CurrentTool != Cursor::Tool::None)
                return m_CurrentTool;
            if (buttons & Qt::LeftButton)
                return Cursor::Tool::Draw;
            else if (buttons & Qt::RightButton)
                return Cursor::Tool::Pick;
            else if (buttons & Qt::MiddleButton)
            {
                if (QGuiApplication::keyboardModifiers() & Qt::ControlModifier)
                    return Cursor::Tool::FillAll;
                else
                    return Cursor::Tool::Fill;
            }
        }
        return Cursor::Tool::None;
    }

    ///////////////////////////////////////////////////////////
    // Function type:  Virtual
    // Contributors:   Pokedude
    // Last edit by:   Pokedude
    // Date of edit:   8/25/2016
    //
    ///////////////////////////////////////////////////////////
    void AMEMapView::mousePressEvent(QMouseEvent *event)
    {
		QPoint mapCoords = event->pos();
		mapCoords.rx() /= MAP_BLOCK_SIZE;
		mapCoords.ry() /= MAP_BLOCK_SIZE;
		mapCoords -= (m_MapPositions.at(0) / MAP_BLOCK_SIZE);

		Cursor::Tool tool = getCurrentTool(event->button());

		QRect rect = m_Cursor.mousePressEvent(mapCoords, tool);

		if (!rect.isNull())
		{

			repaint();
		}
		/*
        int mouseX = event->pos().x();
        int mouseY = event->pos().y();
        QPoint mp = m_MapPositions.at(0);
        QSize mz = m_MapSizes.at(0);

        if (mouseX < mp.x()              || mouseY < mp.y()            ||
            mouseX > mp.x() + mz.width() || mouseY > mp.y() + mz.height())
        {
            return;
        }

        m_ValidPress = true;

        mouseX -= mp.x();
        mouseY -= mp.y();

        AMEMapView::Tool currentTool = getCurrentTool(event->buttons());

        QVector<MapBlock> newBlocks = m_SelectedBlocks;
        int selectionWidth = 0;
        int selectionHeight = 0;

        if (!m_MovementMode)
        {
            if (m_BlockView->selectedBlocks().empty())
            {
                selectionWidth = m_SelectSize.width();
                selectionHeight = m_SelectSize.height();
            }
            else
            {
                newBlocks.clear();
                foreach (UInt16 block, m_BlockView->selectedBlocks())
                {
                    MapBlock newBlock;
                    newBlock.block = block;
                    newBlock.permission = -1;
                    newBlocks.append(newBlock);
                }

                //newBlocks = m_BlockView->selectedBlocks();
                selectionWidth = (m_BlockView->selectedBlocks().last() % 8) - (m_BlockView->selectedBlocks().first() % 8) + 1;
                selectionHeight = (m_BlockView->selectedBlocks().last() / 8) - (m_BlockView->selectedBlocks().first() / 8) + 1;
            }
        }
        else
        {
            selectionWidth = 1;
            selectionHeight = 1;
            newBlocks.clear();
            MapBlock newBlock;
            newBlock.block = -1;
            newBlock.permission = m_MPListener->getSelectedIndex();
            newBlocks.append(newBlock);
        }

        int hOffset = (mouseX/16) % selectionWidth;
        int vOffset = (mouseY/16) % selectionHeight;

        MapHeader *header = &m_Maps[0]->header();

        if (currentTool == AMEMapView::Tool::Draw)
        {
            drawOnMousePress(QPoint(mouseX/16, mouseY/16), newBlocks, selectionWidth, selectionHeight);
        }
        else if (currentTool == AMEMapView::Tool::Select)
        {
            m_CursorColor = Qt::GlobalColor::yellow;

            // Determines the moused-over block number
            m_FirstBlock = QPoint(mouseX/16, mouseY/16);
			m_Cursor.beginPick(m_FirstBlock);
            m_LastBlock = m_FirstBlock;
            m_SelectedBlocks.clear();
            m_BlockView->deselectBlocks();
            m_BlockView->repaint();
        }
        else if (currentTool == AMEMapView::Tool::Fill)
        {
            MapBlock oldBlock = *header->getBlock(mouseX/16, mouseY/16);

            QQueue<QPoint> queue;
            QVector<Boolean> done = QVector<Boolean>(header->blocks().length());
            queue.enqueue(QPoint(mouseX/16, mouseY/16));
            while (!queue.isEmpty())
            {
                QPoint cur = queue.dequeue();
                int placeX = (cur.x() % selectionWidth) - hOffset;
                int placeY = (cur.y() % selectionHeight) - vOffset;

                if (placeX < 0)
                    placeX += selectionWidth;
                if (placeY < 0)
                    placeY += selectionHeight;

                if (placeBlock(cur.x(), cur.y(), newBlocks[placeX + (placeY * selectionWidth)]))
                {

                    if (!m_MovementMode)
                    {
                        if (!done[cur.x() + (cur.y() - 1) * header->size().width()] &&
                                header->getBlock(cur.x(), cur.y() - 1) != NULL &&
                                header->getBlock(cur.x(), cur.y() - 1)->block == oldBlock.block)
                        {
                            queue.enqueue(QPoint(cur.x(), cur.y() - 1));
                            done[cur.x() + (cur.y() - 1) * header->size().width()] = true;
                        }

                        if (!done[cur.x() + (cur.y() + 1) * header->size().width()] &&
                                header->getBlock(cur.x(), cur.y() + 1) != NULL &&
                                header->getBlock(cur.x(), cur.y() + 1)->block == oldBlock.block)
                        {
                            queue.enqueue(QPoint(cur.x(), cur.y() + 1));
                            done[cur.x() + (cur.y() + 1) * header->size().width()] = true;
                        }

                        if (!done[cur.x() - 1 + cur.y() * header->size().width()] &&
                                header->getBlock(cur.x() - 1, cur.y()) != NULL &&
                                header->getBlock(cur.x() - 1, cur.y())->block == oldBlock.block)
                        {
                            queue.enqueue(QPoint(cur.x() - 1, cur.y()));
                            done[cur.x() - 1 + cur.y() * header->size().width()] = true;
                        }

                        if (!done[cur.x() + 1 + cur.y() * header->size().width()] &&
                                header->getBlock(cur.x() + 1, cur.y()) != NULL &&
                                header->getBlock(cur.x() + 1, cur.y())->block == oldBlock.block)
                        {
                            queue.enqueue(QPoint(cur.x() + 1, cur.y()));
                            done[cur.x() + 1 + cur.y() * header->size().width()] = true;
                        }
                    }
                    else
                    {
                        if (!done[cur.x() + (cur.y() - 1) * header->size().width()] &&
                                header->getBlock(cur.x(), cur.y() - 1) != NULL &&
                                header->getBlock(cur.x(), cur.y() - 1)->permission == oldBlock.permission)
                        {
                            queue.enqueue(QPoint(cur.x(), cur.y() - 1));
                            done[cur.x() + (cur.y() - 1) * header->size().width()] = true;
                        }

                        if (!done[cur.x() + (cur.y() + 1) * header->size().width()] &&
                                header->getBlock(cur.x(), cur.y() + 1) != NULL &&
                                header->getBlock(cur.x(), cur.y() + 1)->permission == oldBlock.permission)
                        {
                            queue.enqueue(QPoint(cur.x(), cur.y() + 1));
                            done[cur.x() + (cur.y() + 1) * header->size().width()] = true;
                        }

                        if (!done[cur.x() - 1 + cur.y() * header->size().width()] &&
                                header->getBlock(cur.x() - 1, cur.y()) != NULL &&
                                header->getBlock(cur.x() - 1, cur.y())->permission == oldBlock.permission)
                        {
                            queue.enqueue(QPoint(cur.x() - 1, cur.y()));
                            done[cur.x() - 1 + cur.y() * header->size().width()] = true;
                        }

                        if (!done[cur.x() + 1 + cur.y() * header->size().width()] &&
                                header->getBlock(cur.x() + 1, cur.y()) != NULL &&
                                header->getBlock(cur.x() + 1, cur.y())->permission == oldBlock.permission)
                        {
                            queue.enqueue(QPoint(cur.x() + 1, cur.y()));
                            done[cur.x() + 1 + cur.y() * header->size().width()] = true;
                        }
                    }
                }

            }
        }
        else if (currentTool == AMEMapView::Tool::FillAll)
        {
            MapBlock oldBlock = *header->getBlock(mouseX/16, mouseY/16);
            for (int i = 0; i < header->size().height(); i++)
            {
                for (int j = 0; j < header->size().width(); j++)
                {
                    int placeX = (j % selectionWidth) - hOffset;
                    int placeY = (i % selectionHeight) - vOffset;

                    if (placeX < 0)
                        placeX += selectionWidth;
                    if (placeY < 0)
                        placeY += selectionHeight;

                    if (!m_MovementMode)
                    {
                        if (header->getBlock(j, i)->block == oldBlock.block)
                            placeBlock(j, i, newBlocks[placeX + (placeY * selectionWidth)]);
                    }
                    else
                    {
                        if (header->getBlock(j, i)->permission == oldBlock.permission)
                            placeBlock(j, i, newBlocks[placeX + (placeY * selectionWidth)]);
                    }
                }
            }

        }
        repaint();
		*/
    }

    ///////////////////////////////////////////////////////////
    // Function type:  Helper
    // Contributors:   Diegoisawesome
    // Last edit by:   Diegoisawesome
    // Date of edit:   11/9/2016
    //
    ///////////////////////////////////////////////////////////
    void AMEMapView::drawOnMousePress(QPoint pos, QVector<MapBlock> selected,int selectionWidth, int selectionHeight)
    {
        //m_CursorColor = Qt::GlobalColor::red;

        for (int i = 0; i < selectionHeight; i++)
        {
            for (int j = 0; j < selectionWidth; j++)
            {
                if (!m_MovementMode)
                    placeBlock(pos.x() + j, pos.y() + i, selected[j + (i * selectionWidth)]);
                else
                {
                    MapBlock permBlock;
                    permBlock.block = -1;
                    permBlock.permission = m_MPListener->getSelectedIndex();
                    placeBlock(pos.x() + j, pos.y() + i, permBlock);
                }
            }
        }
    }

    ///////////////////////////////////////////////////////////
    // Function type:  Helper
    // Contributors:   Diegoisawesome
    // Last edit by:   Diegoisawesome
    // Date of edit:   10/31/2016
    //
    ///////////////////////////////////////////////////////////
    bool AMEMapView::placeBlock(int x, int y, MapBlock newBlock)
    {
        QSize mapSize = m_Maps[0]->header().size();

        /*if ((x + (y * mapSize.width())) >= (mapSize.width() * mapSize.height()))
            return false;*/

        if (x < 0                || y < 0            ||
            x >= mapSize.width() || y >= mapSize.height())
            return false;

        MapBlock* block = m_Maps[0]->header().getBlock(x, y);
        if (block == NULL)
            return false;

        if (newBlock.block >= 0)
        {
            block->block = newBlock.block;
            x *= 16;
            y *= 16;

            // Sets the block in the actual image (BG & FG)
            int x2, y2;

            x2 = (newBlock.block % 8) * 16;
            y2 = (newBlock.block / 8) * 16;

            updatePixels(x, y, 16, 16, m_MapBackground,
                         x2,y2,16, 16, m_BlockBackground);

            x2 = (newBlock.block % 8) * 16;
            y2 = (newBlock.block / 8) * 16;

            updatePixels(x, y, 16, 16, m_MapForeground,
                         x2,y2,16, 16, m_BlockForeground);
        }

        if (newBlock.permission >= 0)
        {
            block->permission = newBlock.permission;
        }

        return true;
    }

    ///////////////////////////////////////////////////////////
    // Function type:  Virtual
    // Contributors:   Diegoisawesome
    // Last edit by:   Diegoisawesome
    // Date of edit:   10/6/2016
    //
    ///////////////////////////////////////////////////////////
    void AMEMapView::mouseReleaseEvent(QMouseEvent *event)
    {
		QPoint mapCoords = event->pos();
		mapCoords.rx() /= MAP_BLOCK_SIZE;
		mapCoords.ry() /= MAP_BLOCK_SIZE;
		mapCoords -= (m_MapPositions.at(0) / MAP_BLOCK_SIZE);

		QRect rect = m_Cursor.mouseReleaseEvent(mapCoords);

		//if (!rect.isNull())
		//{
			Cursor::Tool tool = m_Cursor.getTool();
			repaint();
		//}
		/*
        m_CursorColor = Qt::GlobalColor::green;

        AMEMapView::Tool currentTool = getCurrentTool(event->button());

        if (currentTool == AMEMapView::Tool::Select && m_ValidPress)
        {
            Map *mm = m_Maps[0];
            //int mapWidth = mm->header().size().width();

            m_SelectSize.setWidth(m_LastBlock.x() - m_FirstBlock.x());
            if (m_SelectSize.width() < 0)
            {
                m_SelectSize.setWidth(-m_SelectSize.width());
                QPoint selectionWidth = QPoint(m_SelectSize.width(), 0);
                m_FirstBlock -= selectionWidth;
                m_LastBlock += selectionWidth;
            }
            m_SelectSize += QSize(1,0);

            m_SelectSize.setHeight(m_LastBlock.y() - m_FirstBlock.y());
            if (m_SelectSize.height() < 0)
            {
                m_SelectSize.setHeight(-m_SelectSize.height());
                QPoint selectionHeight = QPoint(0, m_SelectSize.height());
                m_FirstBlock -= selectionHeight;
                m_LastBlock += selectionHeight;
            }
            m_SelectSize += QSize(0,1);

            m_SelectedBlocks.clear();

            for (int i = 0; i < m_SelectSize.height(); i++)
            {
                for (int j = 0; j < m_SelectSize.width(); j++)
                {
                    MapBlock *oldBlock = mm->header().getBlock(m_FirstBlock.x() + j, m_FirstBlock.y() + i);
                    MapBlock newBlock;
                    newBlock.block = oldBlock->block;
                    newBlock.permission = oldBlock->permission;
                    m_SelectedBlocks.push_back(newBlock);
                }
            }

            if (m_SelectedBlocks.length() == 1)
            {
                m_BlockView->selectBlock(m_SelectedBlocks[0].block);
            }
        }
        repaint();
        m_ValidPress = false;
		*/
    }

    ///////////////////////////////////////////////////////////
    // Function type:  Virtual
    // Contributors:   Diegoisawesome
    // Last edit by:   Diegoisawesome
    // Date of edit:   4/25/2017
    //
    ///////////////////////////////////////////////////////////
    void AMEMapView::mouseMoveEvent(QMouseEvent *event)
    {
		QPoint mapCoords = event->pos();
		mapCoords.rx() /= MAP_BLOCK_SIZE;	// needs to be done this way so rounding doesn't mess us up
		mapCoords.ry() /= MAP_BLOCK_SIZE;
		mapCoords -= (m_MapPositions.at(0) / MAP_BLOCK_SIZE);	// this too

		QRect rect = m_Cursor.mouseMoveEvent(mapCoords);

		if (!rect.isNull())
		{
			if (m_Cursor.getTool() == Draw)
			{
				m_CurrentMap.writeBlocks(rect);
			}
			repaint();
		}
		/*
        int mouseX = event->pos().x();
        int mouseY = event->pos().y();
        bool needsRepaint = false;

        QPoint mp = m_MapPositions.at(0);
        QSize mz = m_MapSizes.at(0);

        int mX = mouseX - mp.x();
        int mY = mouseY - mp.y();

        AMEMapView::Tool currentTool = getCurrentTool(event->buttons());

        if (currentTool == AMEMapView::Tool::Select && m_ValidPress)
        {
            if (mX < 0)
                mX = 0;
            else if (mX >= mz.width())
                mX = mz.width() - 1;

            if (mY < 0)
                mY = 0;
            else if (mY >= mz.height())
                mY = mz.height() - 1;

            if (m_LastBlock != QPoint(mX/16, mY/16))
            {
                needsRepaint = true;
                m_LastBlock = QPoint(mX/16, mY/16);
				m_Cursor.resizeWithAnchor(m_LastBlock, QRect(QPoint(0,0), m_MapSizes.at(0) / 16));
            }
        }
        else
        {
            if (mX < 0           || mY < 0            ||
                mX >= mz.width() || mY >= mz.height())
            {
                for (int i = 1; i < m_Maps.count(); i++)
                {
                    QPoint mpc = m_MapPositions.at(i);
                    QSize mzc = m_MapSizes.at(i);
                    if (mouseX >= mpc.x()              && mouseY >= mpc.y()            &&
                        mouseX <= mpc.x() + mzc.width() && mouseY <= mpc.y() + mzc.height())
                    {
                        if (i != m_HoveredConnection)
                        {
                            m_ShowCursor = false;
							m_Cursor.setVisible(false);
                            m_HoveredConnection = i;
                            repaint();
                        }
                        return;
                    }
                }
                if (m_HoveredConnection != 0)
                {
                    m_HoveredConnection = 0;
                    needsRepaint = true;
                }
                if (m_ShowCursor)
                {
                    m_ShowCursor = false;
					m_Cursor.setVisible(false);
                    needsRepaint = true;
                }
                if (needsRepaint)
                    repaint();
                return;
            }
        }

        if (m_HoveredConnection != 0)
        {
            m_HoveredConnection = 0;
            needsRepaint = true;
        }

        if (m_ShowCursor != true)
        {
            m_ShowCursor = true;
			m_Cursor.setVisible(true);
            needsRepaint = true;
        }


        if (m_HighlightedBlock != QPoint(mX/16, mY/16))
        {

            int selectionWidth = 1;
            int selectionHeight = 1;
            QVector<MapBlock> selected = m_SelectedBlocks;

            if (!m_MovementMode)
            {
                if (m_BlockView->selectedBlocks().empty())
                {
                    selectionWidth = m_SelectSize.width();
                    selectionHeight = m_SelectSize.height();
                }
                else
                {
                    selected.clear();
                    foreach (UInt16 block, m_BlockView->selectedBlocks())
                    {
                        MapBlock newBlock;
                        newBlock.block = block;
                        newBlock.permission = -1;
                        selected.append(newBlock);
                    }
                    selectionWidth = (m_BlockView->selectedBlocks().last() % 8) - (m_BlockView->selectedBlocks().first() % 8) + 1;
                    selectionHeight = (m_BlockView->selectedBlocks().last() / 8) - (m_BlockView->selectedBlocks().first() / 8) + 1;
                }
            }
            else
            {
                selected.clear();
                MapBlock newBlock;
                newBlock.block = -1;
                newBlock.permission = m_MPListener->getSelectedIndex();
                selected.append(newBlock);
            }

            int xBlockOffset = (m_HighlightedBlock.x() + selectionWidth) % selectionWidth;
            int yBlockOffset = (m_HighlightedBlock.y() + selectionHeight) % selectionHeight;
            int xMouseOffset = ((mX/16) - xBlockOffset + selectionWidth) % selectionWidth;
            int yMouseOffset = ((mY/16) - yBlockOffset + selectionHeight) % selectionHeight;
            if (currentTool == AMEMapView::Tool::Draw)
            {
                if( ((m_HighlightedBlock.x() + selectionWidth) / selectionWidth) !=
                    (((mX/16) - xMouseOffset + selectionWidth) / selectionWidth) ||
                    ((m_HighlightedBlock.y() + selectionHeight) / selectionHeight) !=
                    (((mY/16) - yMouseOffset + selectionHeight) / selectionHeight))
                {
                    m_HighlightedBlock = QPoint((mX/16) - xMouseOffset,
                                                (mY/16) - yMouseOffset);
                    drawOnMousePress(m_HighlightedBlock, selected, selectionWidth, selectionHeight);
                    needsRepaint = true;
                }
            }
            else
            {
                m_HighlightedBlock = QPoint(mX/16, mY/16);
				m_Cursor.setPos(m_HighlightedBlock);
                needsRepaint = true;
            }
        }

        if (needsRepaint)
            repaint();
		*/
    }

    ///////////////////////////////////////////////////////////
    // Function type:  Virtual
    // Contributors:   Diegoisawesome
    // Last edit by:   Diegoisawesome
    // Date of edit:   12/7/2016
    //
    ///////////////////////////////////////////////////////////
    void AMEMapView::mouseDoubleClickEvent(QMouseEvent *event)
    {
        if (event->button() == Qt::LeftButton)
        {
            int mouseX = event->pos().x();
            int mouseY = event->pos().y();
            for (int i = 1; i < m_Maps.count(); i++)
            {
                QPoint mp = m_MapPositions.at(i);
                QSize mz = m_MapSizes.at(i);
                if (mouseX >= mp.x()              && mouseY >= mp.y()            &&
                    mouseX <= mp.x() + mz.width() && mouseY <= mp.y() + mz.height())
                {
                    emit loadMapChangeTreeView(m_Maps[i]);
                }
            }

        }
    }

    ///////////////////////////////////////////////////////////
    // Function type:  Virtual
    // Contributors:   Diegoisawesome
    // Last edit by:   Diegoisawesome
    // Date of edit:   12/7/2016
    //
    ///////////////////////////////////////////////////////////
    void AMEMapView::leaveEvent(QEvent *event)
    {
        Q_UNUSED(event);
        m_ShowCursor = false;
		m_Cursor.setVisible(false);
        m_HoveredConnection = 0;

        repaint();
    }

    ///////////////////////////////////////////////////////////
    // Function type:  I/O
    // Contributors:   Pokedude
    // Last edit by:   Pokedude
    // Date of edit:   6/17/2016
    //
    ///////////////////////////////////////////////////////////
    bool AMEMapView::setMap(const qboy::Rom &rom, Map *mainMap)
    {
        // Clears up previous buffers, if any
        foreach (UInt8 *v, m_BackPixelBuffers)
            delete[] v;
        foreach (UInt8 *v, m_ForePixelBuffers)
            delete[] v;

        delete[] m_PrimaryForeground;
        delete[] m_PrimaryBackground;
        delete[] m_SecondaryForeground;
        delete[] m_SecondaryBackground;

        m_Maps.clear();
        m_MapSizes.clear();
        m_MapPositions.clear();
        m_ConnectParts.clear();
        m_MaxRows.clear();
        m_NPCPositions.clear();
        m_NPCSprites.clear();
        m_Palettes.clear();
        m_ConnPalettes.clear();
        m_ConnImages.clear();
        m_BackPixelBuffers.clear();
        m_ForePixelBuffers.clear();
        m_HoveredConnection = 0;


        Q_UNUSED(rom);
        const QSize mainSize = mainMap->header().size();
        const QList<Connection *> connexAll = mainMap->connections().connections();
        //filter out dive/emerge connections
        QList<Connection *> connexOw;
        for (int i = 0; i < connexAll.size(); i++)
        {
            Connection *current = connexAll[i];
            if (current->direction < DIR_Dive)
                connexOw.push_back(current);
        }
        m_Maps.push_back(mainMap);
        m_LayoutView = false;


        // Retrieves every connected map
        for (int i = 0; i < connexOw.size(); i++)
        {
            Connection *current = connexOw[i];
            Map *map = dat_MapBankTable->banks().at(current->bank)->maps().at(current->map);
            m_Maps.push_back(map);
        }

        m_MapSizes.push_back(QSize(mainSize.width()*16, mainSize.height()*16));
        m_MapPositions.push_back(QPoint(0, 0));
        m_WidgetSize = QSize(m_MapSizes.at(0));

		m_Cursor.setBounds(QRect(QPoint(0, 0), mainSize));

        // Calculates the positions for the maps
        int biggestLeftMap = 0; // width of the biggest left-connected map
        int biggestTopMap = 0;  // height of the biggest top-connected map
        const int defaultRowCount = 8;
        for (int i = 0; i < connexOw.size(); i++)
        {
            Connection *current = connexOw[i];
            Map *map = m_Maps[i+1];
            QPoint translation;
            Int32 signedOff = (Int32)(current->offset)*16;

            // Determines the offset by direction
            QSize mapSize = QSize(map->header().size().width()*16, map->header().size().height()*16);
            if (current->direction == DIR_Left)
            {
                int rowCount = defaultRowCount;
                if (map->header().size().width() < rowCount)
                    rowCount = map->header().size().width();

                m_ConnectParts.push_back(QPoint(mapSize.width()-(16*rowCount), 0));
                mapSize.setWidth(16*rowCount);
                translation.setX(-mapSize.width());
                translation.setY(signedOff);
                m_WidgetSize.rwidth() += mapSize.width();
                m_MaxRows.push_back(rowCount);

                if (mapSize.width() > biggestLeftMap)
                    biggestLeftMap = mapSize.width();
            }
            else if (current->direction == DIR_Right)
            {
                int rowCount = defaultRowCount;
                if (map->header().size().width() < rowCount)
                    rowCount = map->header().size().width();

                m_ConnectParts.push_back(QPoint(0, 0));
                mapSize.setWidth(16*rowCount);
                translation.setX(mainSize.width()*16);
                translation.setY(signedOff);
                m_WidgetSize.rwidth() += mapSize.width();
                m_MaxRows.push_back(rowCount);
            }
            else if (current->direction == DIR_Up)
            {
                int rowCount = defaultRowCount;
                if (map->header().size().width() < rowCount)
                    rowCount = map->header().size().width();

                m_ConnectParts.push_back(QPoint(0, mapSize.height()-(16*rowCount)));
                mapSize.setHeight(16*rowCount);
                translation.setX(signedOff);
                translation.setY(-mapSize.height());
                m_WidgetSize.rheight() += mapSize.height();
                m_MaxRows.push_back(rowCount);

                if (mapSize.height() > biggestTopMap)
                    biggestTopMap = mapSize.height();
            }
            else if (current->direction == DIR_Down)
            {
                int rowCount = defaultRowCount;
                if (map->header().size().width() < rowCount)
                    rowCount = map->header().size().width();

                m_ConnectParts.push_back(QPoint(0, 0));
                mapSize.setHeight(16*rowCount);
                translation.setX(signedOff);
                translation.setY(mainSize.height()*16);
                m_WidgetSize.rheight() += mapSize.height();
                m_MaxRows.push_back(rowCount);
            }

            m_MapPositions.push_back(translation);
            m_MapSizes.push_back(mapSize);
        }

        // Adds the biggest left and top offsets to all positions
        // in order to not cause rendering fails on the widget
        for (int i = 0; i < m_MapPositions.size(); i++)
        {
            m_MapPositions[i].rx() += biggestLeftMap;
            m_MapPositions[i].ry() += biggestTopMap;
        }


        // Determines the block count for each game
        int blockCountPrimary;
        int blockCountSecondary;
        if (CONFIG(RomType) == RT_FRLG)
        {
            blockCountPrimary = 0x280;
            blockCountSecondary = 0x180;
        }
        else
        {
            blockCountPrimary = 0x200;
            blockCountSecondary = 0x200;
        }

        m_PrimaryBlockCount = blockCountPrimary;
        m_SecondaryBlockCount = blockCountSecondary;


        // Attempts to load all the blocksets
        QList<UInt8 *> blocksetBack;
        QList<UInt8 *> blocksetFore;
        for (int i = 0; i < m_Maps.size(); i++)
        {
            Tileset *primary = m_Maps[i]->header().primary();
            Tileset *secondary = m_Maps[i]->header().secondary();
            QVector<qboy::Color> palettes;

            // Retrieves the palettes, combines them, removes bg color
            for (int j = 0; j < primary->palettes().size(); j++)
            {
                palettes.append(primary->palettes().at(j)->raw());
                palettes[j * 16].a = 0.0f;
            }
            for (int j = 0; j < secondary->palettes().size(); j++)
            {
                palettes.append(secondary->palettes().at(j)->raw());
                palettes[(primary->palettes().size() * 16) + (j * 16)].a = 0.0f;
            }
            while (palettes.size() < 256) // align pal for OpenGL
            {
                palettes.push_back({ 0.f, 0.f, 0.f, 0.f });
            }


            if (i == 0)
            {
                m_Palettes.clear();
                for (int i = 0; i < palettes.size(); i++)
                {
                    qboy::Color c = palettes[i];
                    QRgb rgb = qRgba(c.r, c.g, c.b, c.a);
                    m_Palettes.push_back(rgb);
                }
            }
            else
            {
                QVector<QRgb> p;
                for (int i = 0; i < palettes.size(); i++)
                {
                    qboy::Color c = palettes[i];
                    QRgb rgb = qRgba(c.r, c.g, c.b, c.a);
                    p.push_back(rgb);
                }
                m_ConnPalettes.push_back(p);
            }


            // Retrieves the raw pixel data of the tilesets
            const QByteArray &priRaw = primary->image()->raw();
            const QByteArray &secRaw = secondary->image()->raw();
            const int secRawMax = (128/8 * secondary->image()->size().height()/8);

            // Creates two buffers for the blockset pixels
            Int32 tilesetHeight1 = blockCountPrimary / 8 * 16;
            Int32 tilesetHeight2 = blockCountSecondary / 8 * 16;
            UInt8 *background1 = new UInt8[128 * tilesetHeight1];
            UInt8 *foreground1 = new UInt8[128 * tilesetHeight1];
            UInt8 *background2 = new UInt8[128 * tilesetHeight2];
            UInt8 *foreground2 = new UInt8[128 * tilesetHeight2];

            // Copy the tileset sizes for our main map
            if (i == 0)
            {
                m_PrimarySetSize = QSize(128, tilesetHeight1);
                m_SecondarySetSize = QSize(128, tilesetHeight2);
            }


            // Parses all the primary block data
            for (int j = 0; j < primary->blocks().size(); j++)
            {
                Block *curBlock = primary->blocks()[j];
                Int32 blockX = (j % 8) * 16;
                Int32 blockY = (j / 8) * 16;

                /* BACKGROUND */
                for (int k = 0; k < 4; k++)
                {
                    Tile tile = curBlock->tiles[k];
                    Int32 subX = ((k % 2) * 8) + blockX;
                    Int32 subY = ((k / 2) * 8) + blockY;

                    if (tile.tile >= blockCountPrimary)
                    {
                        tile.tile -= blockCountPrimary;

                        if (secRawMax > tile.tile)
                            extractTile(secRaw, tile);
                        else
                            memset(pixelBuffer, 0, 64);
                    }
                    else
                    {
                        extractTile(priRaw, tile);
                    }

                    int pos = 0;
                    for (int y = 0; y < 8; y++)
                        for (int x = 0; x < 8; x++)
                            background1[(x+subX) + (y+subY) * 128] = pixelBuffer[pos++];
                }

                /* FOREGROUND */
                for (int k = 0; k < 4; k++)
                {
                    Tile tile = curBlock->tiles[k+4];
                    Int32 subX = ((k % 2) * 8) + blockX;
                    Int32 subY = ((k / 2) * 8) + blockY;;

                    if (tile.tile >= blockCountPrimary)
                    {
                        tile.tile -= blockCountPrimary;

                        if (secRawMax > tile.tile)
                            extractTile(secRaw, tile);
                        else
                            memset(pixelBuffer, 0, 64);
                    }
                    else
                    {
                        extractTile(priRaw, tile);
                    }

                    int pos = 0;
                    for (int y = 0; y < 8; y++)
                        for (int x = 0; x < 8; x++)
                            foreground1[(x+subX) + (y+subY) * 128] = pixelBuffer[pos++];
                }
            }

            // Parses all the secondary block data
            for (int j = 0; j < secondary->blocks().size(); j++)
            {
                Block *curBlock = secondary->blocks()[j];
                Int32 blockX = (j % 8) * 16;
                Int32 blockY = (j / 8) * 16;

                /* BACKGROUND */
                for (int k = 0; k < 4; k++)
                {
                    Tile tile = curBlock->tiles[k];
                    Int32 subX = ((k % 2) * 8) + blockX;
                    Int32 subY = ((k / 2) * 8) + blockY;

                    if (tile.tile >= blockCountPrimary)
                    {
                        tile.tile -= blockCountPrimary;

                        if (secRawMax > tile.tile)
                            extractTile(secRaw, tile);
                        else
                            memset(pixelBuffer, 0, 64);
                    }
                    else
                    {
                        extractTile(priRaw, tile);
                    }

                    int pos = 0;
                    for (int y = 0; y < 8; y++)
                        for (int x = 0; x < 8; x++)
                            background2[(x+subX) + (y+subY) * 128] = pixelBuffer[pos++];
                }

                /* FOREGROUND */
                for (int k = 0; k < 4; k++)
                {
                    Tile tile = curBlock->tiles[k+4];
                    Int32 subX = ((k % 2) * 8) + blockX;
                    Int32 subY = ((k / 2) * 8) + blockY;;

                    if (tile.tile >= blockCountPrimary)
                    {
                        tile.tile -= blockCountPrimary;

                        if (secRawMax > tile.tile)
                            extractTile(secRaw, tile);
                        else
                            memset(pixelBuffer, 0, 64);
                    }
                    else
                    {
                        extractTile(priRaw, tile);
                    }

                    int pos = 0;
                    for (int y = 0; y < 8; y++)
                        for (int x = 0; x < 8; x++)
                            foreground2[(x+subX) + (y+subY) * 128] = pixelBuffer[pos++];
                }
            }


            // Adds the new blocksets
            blocksetBack.push_back(background1);
            blocksetBack.push_back(background2);
            blocksetFore.push_back(foreground1);
            blocksetFore.push_back(foreground2);
        }


        // Creates the image for the main map
        MapHeader &header = m_Maps[0]->header();
        QSize mapSize = header.size();

        // Creates a new pixel buffer for the map
        UInt8 *backMapBuffer = new UInt8[mapSize.width()*16 * mapSize.height()*16];
        UInt8 *foreMapBuffer = new UInt8[mapSize.width()*16 * mapSize.height()*16];
        UInt8 *primaryBg = blocksetBack.at(0);
        UInt8 *secondaryBg = blocksetBack.at(1);
        UInt8 *primaryFg = blocksetFore.at(0);
        UInt8 *secondaryFg = blocksetFore.at(1);

        // Iterates through every map block and writes it to the map buffer
        for (int j = 0; j < header.blocks().size(); j++)
        {
            MapBlock block = *header.blocks().at(j);
            Int32 mapX = (j % mapSize.width()) * 16;
            Int32 mapY = (j / mapSize.width()) * 16;

            if (block.block >= blockCountPrimary)
                extractBlock(secondaryBg, block.block - blockCountPrimary);
            else
                extractBlock(primaryBg, block.block);

            int pos = 0;
            for (int y = 0; y < 16; y++)
                for (int x = 0; x < 16; x++)
                    backMapBuffer[(x+mapX) + (y+mapY) * mapSize.width()*16] = blockBuffer[pos++];

            if (block.block >= blockCountPrimary)
                extractBlock(secondaryFg, block.block - blockCountPrimary);
            else
                extractBlock(primaryFg, block.block);

            pos = 0;
            for (int y = 0; y < 16; y++)
                for (int x = 0; x < 16; x++)
                    foreMapBuffer[(x+mapX) + (y+mapY) * mapSize.width()*16] = blockBuffer[pos++];
        }

        // Appends the pixel buffers
        m_BackPixelBuffers.push_back(backMapBuffer);
        m_ForePixelBuffers.push_back(foreMapBuffer);


        // Renders all the connected maps
        for (int n = 0; n < m_ConnectParts.size(); n++)
        {
            MapHeader &header = m_Maps[n+1]->header();

            // Determines the start block
            const QPoint pos = m_ConnectParts.at(n);
            const QSize mapSize = m_Maps.at(n+1)->header().size();
            const QSize absSize = m_MapSizes.at(n+1);
            int start = (pos.x() / 16) + ((pos.y() / 16) * mapSize.width());

            // Creates the pixel buffers for the connected map
            UInt8 *backMapBuffer = new UInt8[absSize.width()*absSize.height()];
            UInt8 *foreMapBuffer = new UInt8[absSize.width()*absSize.height()];
            UInt8 *primaryBg = blocksetBack.at((n+1)*2);
            UInt8 *secondaryBg = blocksetBack.at((n+1)*2+1);
            UInt8 *primaryFg = blocksetFore.at((n+1)*2);
            UInt8 *secondaryFg = blocksetFore.at((n+1)*2+1);
            DirectionType dir = connexOw.at(n)->direction;
            Int32 rowCount = m_MaxRows.at(n);


            // Iterates through every visible block
            if (dir == DIR_Left || dir == DIR_Right)
            {
                for (int m = 0; m < (absSize.width()/16)*(absSize.height()/16); m++)
                {
                    int processed = (m/rowCount)*mapSize.width();
                    int blockNumber = start + processed + (m%rowCount);

                    MapBlock block = *header.blocks().at(blockNumber);
                    Int32 mapX = (m % (absSize.width()/16)) * 16;
                    Int32 mapY = (m / (absSize.width()/16)) * 16;

                    if (block.block >= blockCountPrimary)
                        extractBlock(secondaryBg, block.block - blockCountPrimary);
                    else
                        extractBlock(primaryBg, block.block);

                    int pos = 0;
                    for (int y = 0; y < 16; y++)
                        for (int x = 0; x < 16; x++)
                            backMapBuffer[(x+mapX) + (y+mapY) * absSize.width()] = blockBuffer[pos++];

                    if (block.block >= blockCountPrimary)
                        extractBlock(secondaryFg, block.block - blockCountPrimary);
                    else
                        extractBlock(primaryFg, block.block);

                    pos = 0;
                    for (int y = 0; y < 16; y++)
                        for (int x = 0; x < 16; x++)
                            foreMapBuffer[(x+mapX) + (y+mapY) * absSize.width()] = blockBuffer[pos++];
                }
            }
            else if (dir == DIR_Down || dir == DIR_Up)
            {
                for (int m = 0; m < (absSize.width()/16)*(absSize.height()/16); m++)
                {
                    int blockNumber = start + m;

                    MapBlock block = *header.blocks().at(blockNumber);
                    Int32 mapX = (m % mapSize.width()) * 16;
                    Int32 mapY = (m / mapSize.width()) * 16;

                    if (block.block >= blockCountPrimary)
                        extractBlock(secondaryBg, block.block - blockCountPrimary);
                    else
                        extractBlock(primaryBg, block.block);

                    int pos = 0;
                    for (int y = 0; y < 16; y++)
                        for (int x = 0; x < 16; x++)
                            backMapBuffer[(x+mapX) + (y+mapY) * mapSize.width()*16] = blockBuffer[pos++];

                    if (block.block >= blockCountPrimary)
                        extractBlock(secondaryFg, block.block - blockCountPrimary);
                    else
                        extractBlock(primaryFg, block.block);

                    pos = 0;
                    for (int y = 0; y < 16; y++)
                        for (int x = 0; x < 16; x++)
                            foreMapBuffer[(x+mapX) + (y+mapY) * mapSize.width()*16] = blockBuffer[pos++];
                }
            }

            // Appends the pixel buffers
            m_BackPixelBuffers.push_back(backMapBuffer);
            m_ForePixelBuffers.push_back(foreMapBuffer);
        }

        // Deletes all unnecessary blocksets
        for (int i = 0; i < blocksetBack.size(); i++)
        {
            if (i == 0)
            {
                m_PrimaryBackground = blocksetBack[0];
                m_PrimaryForeground = blocksetFore[0];
            }
            else if (i == 1)
            {
                m_SecondaryBackground = blocksetBack[1];
                m_SecondaryForeground = blocksetFore[1];
            }
            else
            {
                delete blocksetBack[i];
                delete blocksetFore[i];
            }

        }

        const Int32 tsw = 128;
        Int32 tsh1 = blockCountPrimary / 8 * 16;
        Int32 tsh2 = blockCountSecondary / 8 * 16;
        Int32 lpct = 128 * tsh1;
        Int32 lsct = 128 * tsh2;
        QRgb c = m_Palettes[0];

        m_BlockForeground = QImage(tsw, tsh1 + tsh2, QImage::Format_Indexed8);
        m_BlockBackground = QImage(tsw, tsh1 + tsh2, QImage::Format_Indexed8);

        for (int i = 0; i < lpct; i++)
            m_BlockForeground.bits()[i] = m_PrimaryForeground[i];
        for (int i = 0; i < lsct; i++)
            m_BlockForeground.bits()[lpct+i] = m_SecondaryForeground[i];
        for (int i = 0; i < lpct; i++)
            m_BlockBackground.bits()[i] = m_PrimaryBackground[i];
        for (int i = 0; i < lsct; i++)
            m_BlockBackground.bits()[lpct+i] = m_SecondaryBackground[i];

        m_BlockForeground.setColorTable(m_Palettes);
        m_PalCopy = m_Palettes;
        m_PalCopy[0] = qRgba(qRed(c), qGreen(c), qBlue(c), 255);
        m_BlockBackground.setColorTable(m_PalCopy);


        // Generates all the images
        m_iMapForeground = QImage(m_MapSizes.at(0).width(), m_MapSizes.at(0).height(), QImage::Format_Indexed8);
        m_iMapBackground = QImage(m_MapSizes.at(0).width(), m_MapSizes.at(0).height(), QImage::Format_Indexed8);

        for (int i = 0; i < m_iMapForeground.byteCount(); i++)
            m_iMapForeground.bits()[i] = m_ForePixelBuffers[0][i];
        for (int i = 0; i < m_iMapBackground.byteCount(); i++)
            m_iMapBackground.bits()[i] = m_BackPixelBuffers[0][i];

        m_iMapForeground.setColorTable(m_Palettes);
        m_iMapBackground.setColorTable(m_PalCopy);

        for (int i = 1; i < m_Maps.size(); i++)
        {
            QImage frg(m_MapSizes.at(i).width(), m_MapSizes.at(i).height(), QImage::Format_Indexed8);
            QImage bkg(m_MapSizes.at(i).width(), m_MapSizes.at(i).height(), QImage::Format_Indexed8);
            UInt8 *fgp = m_ForePixelBuffers[i];
            UInt8 *bgp = m_BackPixelBuffers[i];
            c = m_ConnPalettes[i-1][0];

            for (int j = 0; j < frg.byteCount(); j++)
                frg.bits()[j] = fgp[j];
            for (int j = 0; j < bkg.byteCount(); j++)
                bkg.bits()[j] = bgp[j];

            frg.setColorTable(m_ConnPalettes.at(i-1));
            m_ConnPalettes[i-1][0] = qRgba(qRed(c), qGreen(c), qBlue(c), 255);
            bkg.setColorTable(m_ConnPalettes.at(i-1));

            QImage fullimg = bkg.convertToFormat(QImage::Format_ARGB32_Premultiplied);
            QPainter p(&fullimg);
            p.drawImage(0, 0, frg);
            p.end();
            convertGrayScale(fullimg);

            m_ConnImages.push_back(fullimg);
        }


        m_IsInit = true;
        m_MapBackground = QPixmap::fromImage(m_iMapBackground);
        m_MapForeground = QPixmap::fromImage(m_iMapForeground);
        setMinimumSize(m_WidgetSize);
        return true;
    }

    ///////////////////////////////////////////////////////////
    // Function type:  I/O
    // Contributors:   Pokedude
    // Last edit by:   Pokedude
    // Date of edit:   6/17/2016
    //
    ///////////////////////////////////////////////////////////
    bool AMEMapView::setLayout(MapHeader &mainMap)
    {
        const QSize mainSize = mainMap.size();
        m_Maps.push_back(NULL);
        m_MapSizes.push_back(QSize(mainSize.width()*16, mainSize.height()*16));
        m_MapPositions.push_back(QPoint(0, 0));
        m_WidgetSize = QSize(m_MapSizes.at(0));
        m_Header = mainMap;
        m_LayoutView = true;


        // Determines the block count for each game
        int blockCountPrimary;
        int blockCountSecondary;
        if (CONFIG(RomType) == RT_FRLG)
        {
            blockCountPrimary = 0x280;
            blockCountSecondary = 0x180;
        }
        else
        {
            blockCountPrimary = 0x200;
            blockCountSecondary = 0x200;
        }

        m_PrimaryBlockCount = blockCountPrimary;
        m_SecondaryBlockCount = blockCountSecondary;


        // Attempts to load all the blocksets
        QList<UInt8 *> blocksetBack;
        QList<UInt8 *> blocksetFore;
        for (int i = 0; i < 1; i++)
        {
            Tileset *primary = mainMap.primary();
            Tileset *secondary = mainMap.secondary();
            QVector<qboy::Color> palettes;

            // Retrieves the palettes, combines them, removes bg color
            for (int j = 0; j < primary->palettes().size(); j++)
            {
                palettes.append(primary->palettes().at(j)->raw());
                palettes[j * 16].a = 0.0f;
            }
            for (int j = 0; j < secondary->palettes().size(); j++)
            {
                palettes.append(secondary->palettes().at(j)->raw());
                palettes[(primary->palettes().size() * 16) + (j * 16)].a = 0.0f;
            }

            while (palettes.size() < 256) // align pal for OpenGL
            {
                palettes.push_back({ 0.f, 0.f, 0.f, 0.f });
            }


            m_Palettes.clear();
            for (int i = 0; i < palettes.size(); i++)
            {
                qboy::Color c = palettes[i];
                QRgb rgb = qRgba(c.r, c.g, c.b, c.a);
                m_Palettes.push_back(rgb);
            }


            // Retrieves the raw pixel data of the tilesets
            const QByteArray &priRaw = primary->image()->raw();
            const QByteArray &secRaw = secondary->image()->raw();
            const int secRawMax = (128/8 * secondary->image()->size().height()/8);

            // Creates two buffers for the blockset pixels
            Int32 tilesetHeight1 = blockCountPrimary / 8 * 16;
            Int32 tilesetHeight2 = blockCountSecondary / 8 * 16;
            UInt8 *background1 = new UInt8[128 * tilesetHeight1];
            UInt8 *foreground1 = new UInt8[128 * tilesetHeight1];
            UInt8 *background2 = new UInt8[128 * tilesetHeight2];
            UInt8 *foreground2 = new UInt8[128 * tilesetHeight2];

            // Copy the tileset sizes for our main map
            if (i == 0)
            {
                m_PrimarySetSize = QSize(128, tilesetHeight1);
                m_SecondarySetSize = QSize(128, tilesetHeight2);
            }

            // Parses all the primary block data
            for (int j = 0; j < primary->blocks().size(); j++)
            {
                Block *curBlock = primary->blocks()[j];
                Int32 blockX = (j % 8) * 16;
                Int32 blockY = (j / 8) * 16;

                for (int k = 0; k < 4; k++)
                {
                    Tile tile = curBlock->tiles[k];
                    Int32 subX = ((k % 2) * 8) + blockX;
                    Int32 subY = ((k / 2) * 8) + blockY;

                    if (tile.tile >= blockCountPrimary)
                    {
                        tile.tile -= blockCountPrimary;

                        if (secRawMax > tile.tile)
                            extractTile(secRaw, tile);
                        else
                            memset(pixelBuffer, 0, 64);
                    }
                    else
                    {
                        extractTile(priRaw, tile);
                    }

                    int pos = 0;
                    for (int y = 0; y < 8; y++)
                        for (int x = 0; x < 8; x++)
                            background1[(x+subX) + (y+subY) * 128] = pixelBuffer[pos++];
                }

                for (int k = 0; k < 4; k++)
                {
                    Tile tile = curBlock->tiles[k+4];
                    Int32 subX = ((k % 2) * 8) + blockX;
                    Int32 subY = ((k / 2) * 8) + blockY;;

                    if (tile.tile >= blockCountPrimary)
                    {
                        tile.tile -= blockCountPrimary;

                        if (secRawMax > tile.tile)
                            extractTile(secRaw, tile);
                        else
                            memset(pixelBuffer, 0, 64);
                    }
                    else
                    {
                        extractTile(priRaw, tile);
                    }

                    int pos = 0;
                    for (int y = 0; y < 8; y++)
                        for (int x = 0; x < 8; x++)
                            foreground1[(x+subX) + (y+subY) * 128] = pixelBuffer[pos++];
                }
            }

            // Parses all the secondary block data
            for (int j = 0; j < secondary->blocks().size(); j++)
            {
                Block *curBlock = secondary->blocks()[j];
                Int32 blockX = (j % 8) * 16;
                Int32 blockY = (j / 8) * 16;

                for (int k = 0; k < 4; k++)
                {
                    Tile tile = curBlock->tiles[k];
                    Int32 subX = ((k % 2) * 8) + blockX;
                    Int32 subY = ((k / 2) * 8) + blockY;

                    if (tile.tile >= blockCountPrimary)
                    {
                        tile.tile -= blockCountPrimary;

                        if (secRawMax > tile.tile)
                            extractTile(secRaw, tile);
                        else
                            memset(pixelBuffer, 0, 64);
                    }
                    else
                    {
                        extractTile(priRaw, tile);
                    }

                    int pos = 0;
                    for (int y = 0; y < 8; y++)
                        for (int x = 0; x < 8; x++)
                            background2[(x+subX) + (y+subY) * 128] = pixelBuffer[pos++];
                }

                for (int k = 0; k < 4; k++)
                {
                    Tile tile = curBlock->tiles[k+4];
                    Int32 subX = ((k % 2) * 8) + blockX;
                    Int32 subY = ((k / 2) * 8) + blockY;;

                    if (tile.tile >= blockCountPrimary)
                    {
                        tile.tile -= blockCountPrimary;

                        if (secRawMax > tile.tile)
                            extractTile(secRaw, tile);
                        else
                            memset(pixelBuffer, 0, 64);
                    }
                    else
                    {
                        extractTile(priRaw, tile);
                    }

                    int pos = 0;
                    for (int y = 0; y < 8; y++)
                        for (int x = 0; x < 8; x++)
                            foreground2[(x+subX) + (y+subY) * 128] = pixelBuffer[pos++];
                }
            }


            // Adds the new blocksets
            blocksetBack.push_back(background1);
            blocksetBack.push_back(background2);
            blocksetFore.push_back(foreground1);
            blocksetFore.push_back(foreground2);
        }


        // Creates the images for the actual maps
        for (int i = 0; i < m_Maps.size(); i++)
        {
            MapHeader &header = m_Maps[i]->header();
            QSize mapSize = m_MapSizes[i];

            // Creates a new pixel buffer for the map
            UInt8 *backMapBuffer = new UInt8[mapSize.width() * mapSize.height()];
            UInt8 *foreMapBuffer = new UInt8[mapSize.width() * mapSize.height()];
            UInt8 *primaryBg = blocksetBack.at(i*2);
            UInt8 *secondaryBg = blocksetBack.at(i*2+1);
            UInt8 *primaryFg = blocksetFore.at(i*2);
            UInt8 *secondaryFg = blocksetFore.at(i*2+1);

            // Iterates through every map block and writes it to the map buffer
            for (int j = 0; j < header.blocks().size(); j++)
            {
                MapBlock block = *header.blocks().at(j);
                Int32 mapX = (j % mapSize.width()) * 16;
                Int32 mapY = (j / mapSize.width()) * 16;

                if (block.block >= blockCountPrimary)
                    extractBlock(secondaryBg, block.block - blockCountPrimary);
                else
                    extractBlock(primaryBg, block.block);

                int pos = 0;
                for (int y = 0; y < 16; y++)
                    for (int x = 0; x < 16; x++)
                        backMapBuffer[(x+mapX) + (y+mapY) * mapSize.width()*16] = blockBuffer[pos++];

                if (block.block >= blockCountPrimary)
                    extractBlock(secondaryFg, block.block - blockCountPrimary);
                else
                    extractBlock(primaryFg, block.block);

                pos = 0;
                for (int y = 0; y < 16; y++)
                    for (int x = 0; x < 16; x++)
                        foreMapBuffer[(x+mapX) + (y+mapY) * mapSize.width()*16] = blockBuffer[pos++];
            }

            // Appends the pixel buffers
            m_BackPixelBuffers.push_back(backMapBuffer);
            m_ForePixelBuffers.push_back(foreMapBuffer);
        }

        // Deletes all unnecessary blocksets
        for (int i = 0; i < blocksetBack.size(); i++)
        {
            if (i == 0)
            {
                m_PrimaryBackground = blocksetBack[0];
                m_PrimaryForeground = blocksetFore[0];
            }
            else if (i == 1)
            {
                m_SecondaryBackground = blocksetBack[1];
                m_SecondaryForeground = blocksetFore[1];
            }
            else
            {
                delete blocksetBack[i];
                delete blocksetFore[i];
            }

        }


        const Int32 tsw = 128;
        Int32 tsh1 = blockCountPrimary / 8 * 16;
        Int32 tsh2 = blockCountSecondary / 8 * 16;
        Int32 lpct = 128 * tsh1;
        Int32 lsct = 128 * tsh2;
        QRgb c = m_Palettes[0];

        m_BlockForeground = QImage(tsw, tsh1 + tsh2, QImage::Format_Indexed8);
        m_BlockBackground = QImage(tsw, tsh1 + tsh2, QImage::Format_Indexed8);

        for (int i = 0; i < lpct; i++)
            m_BlockForeground.bits()[i] = m_PrimaryForeground[i];
        for (int i = 0; i < lsct; i++)
            m_BlockForeground.bits()[lpct+i] = m_SecondaryForeground[i];
        for (int i = 0; i < lpct; i++)
            m_BlockBackground.bits()[i] = m_PrimaryBackground[i];
        for (int i = 0; i < lsct; i++)
            m_BlockBackground.bits()[lpct+i] = m_SecondaryBackground[i];

        m_BlockForeground.setColorTable(m_Palettes);
        m_PalCopy = m_Palettes;
        m_PalCopy[0] = qRgba(qRed(c), qGreen(c), qBlue(c), 255);
        m_BlockBackground.setColorTable(m_PalCopy);


        // Generates all the images
        m_iMapForeground = QImage(m_MapSizes.at(0).width(), m_MapSizes.at(0).height(), QImage::Format_Indexed8);
        m_iMapBackground = QImage(m_MapSizes.at(0).width(), m_MapSizes.at(0).height(), QImage::Format_Indexed8);

        for (int i = 0; i < m_iMapForeground.byteCount(); i++)
            m_iMapForeground.bits()[i] = m_ForePixelBuffers[0][i];
        for (int i = 0; i < m_iMapBackground.byteCount(); i++)
            m_iMapBackground.bits()[i] = m_BackPixelBuffers[0][i];

        m_iMapForeground.setColorTable(m_Palettes);
        m_iMapBackground.setColorTable(m_PalCopy);
        m_MapForeground = QPixmap::fromImage(m_iMapForeground);
        m_MapBackground = QPixmap::fromImage(m_iMapBackground);

        m_IsInit = true;
        setMinimumSize(m_WidgetSize);
        return true;
    }

    ///////////////////////////////////////////////////////////
    // Function type:  Event
    // Contributors:   Pokedude, Diegoisawesome
    // Last edit by:   Diegoisawesome
    // Date of edit:   11/7/2016
    //
    ///////////////////////////////////////////////////////////
    void AMEMapView::paintEvent(QPaintEvent *event)
    {
        Q_UNUSED(event);

        if (m_IsInit)
        {
			/*QPoint mapCoords = mapFromGlobal(QCursor::pos());	// this is meant to catch scrolls and other jumps
			mapCoords.rx() /= MAP_BLOCK_SIZE;					// but the painter is limited to painting the new area
			mapCoords.ry() /= MAP_BLOCK_SIZE;
			mapCoords -= (m_MapPositions.at(0) / MAP_BLOCK_SIZE);


			QRect result = m_Cursor.mouseMoveEvent(mapCoords);*/


            QPoint pmain = mainPos();
            QPainter painter(this);
            painter.drawPixmap(pmain.x(), pmain.y(), m_MapBackground);

            // Between background and foreground, draw the overworld sprites on the map.
            painter.drawPixmap(pmain.x(), pmain.y(), m_MapForeground);

            if (!m_LayoutView)
            {
                int conncnt = m_MapSizes.size()-1;
                for (int i = 0; i < conncnt; i++)
                {
                    QPoint pconn = m_MapPositions.at(i+1);
                    painter.drawImage(pconn.x(), pconn.y(), m_ConnImages.at(i));
                }
            }

            // Draws the movement data, if requested
            QPoint orig = m_MapPositions.at(0);
            if (m_MovementMode)
            {
                painter.setOpacity(SETTINGS(Translucency) / 100.0f);

                const QSize &ms = m_Maps.at(0)->header().size();
                const QList<MapBlock *> &blocks = m_Maps.at(0)->header().blocks();
                for (int i = 0; i < blocks.size(); i++)
                {
                    int mapX = (i % ms.width()) * 16 + orig.x();
                    int mapY = (i / ms.width()) * 16 + orig.y();
                    int posY = blocks.at(i)->permission * 16;
                    painter.drawImage(QRect(mapX, mapY, 16, 16), m_MovePerm, QRect(0, posY, 16, 16));
                }
                painter.setOpacity(1.0f);
            }
            if (m_ShowGrid)
            {
                const QSize &ms = m_Maps.at(0)->header().size();
                QVector<QLine> lines;
                for (int i = 0; i < ms.width(); i++)
                    lines.append(QLine(orig.x() + (i * 16), orig.y(), orig.x() + (i * 16), orig.y() + ms.height() * 16 - 1));
                for (int i = 0; i < ms.height(); i++)
                    lines.append(QLine(orig.x(), orig.y() + (i * 16), orig.x() + ms.width() * 16 - 1, orig.y() + (i * 16)));
                painter.setPen(Qt::GlobalColor::black);
                painter.drawLines(lines);
            }
            /*if (m_ShowCursor)
            {
                int mapWidth = m_MapSizes.at(0).width() / 16;
                int x = 0, y = 0, sWid = 1, sHei = 1;
                if (!m_MovementMode)
                {
                    if (m_BlockView->selectedBlocks().empty())
                    {
                        if (m_SelectedBlocks.empty())
                        {
                            // Computes the position of the widget
                            int firstX = m_FirstBlock.x();
                            int firstY = m_FirstBlock.y();
                            int lastX = m_LastBlock.x();
                            int lastY = m_LastBlock.y();

                            x = firstX;
                            y = firstY;
                            sWid = lastX - firstX;
                            sHei = lastY - firstY;

                            if (sWid < 0)
                            {
                                x = lastX;
                                sWid = firstX - x;
                            }
                            if (sHei < 0)
                            {
                                y = lastY;
                                sHei = firstY - y;
                            }

                            sWid++;
                            sHei++;
                        }
                        else
                        {
                            x = m_HighlightedBlock.x();
                            y = m_HighlightedBlock.y();
                            sWid = m_SelectSize.width();
                            sHei = m_SelectSize.height();
                        }
                    }
                    else
                    {
                        // Computes the position on widget
                        int firstX = (m_BlockView->selectedBlocks().first() % 8);
                        int firstY = (m_BlockView->selectedBlocks().first() / 8);
                        int lastX = (m_BlockView->selectedBlocks().last() % 8);
                        int lastY = (m_BlockView->selectedBlocks().last() / 8);

                        sWid = lastX - firstX;
                        sHei = lastY - firstY;

                        x = m_HighlightedBlock.x();
                        y = m_HighlightedBlock.y();

                        sWid++;
                        sHei++;
                    }
                }
                else
                {
                    x = m_HighlightedBlock.x();
                    y = m_HighlightedBlock.y();
                }

                if (x < 0)
                {
                    sWid += x;
                    x = 0;
                }
                if (x + sWid >= mapWidth)
                    sWid = mapWidth - x;

                if (y < 0)
                {
                    sHei += y;
                    y = 0;
                }
                if (y + sHei >= (m_MapSizes.at(0).height() / 16))
                    sHei = (m_MapSizes.at(0).height() / 16) - y;

                x *= 16;
                y *= 16;
                sWid *= 16;
                sHei *= 16;

                QPoint mp = m_MapPositions.at(0);
                x += mp.x();
                y += mp.y();

                painter.setPen(m_CursorColor);
                painter.drawRect(x, y, sWid, sHei);
            }*/
			m_Cursor.paintEvent(event, painter, QRect(m_MapPositions.at(0), m_MapSizes.at(0)));
            if (m_HoveredConnection != 0)
            {
                QRect connRect = QRect(m_MapPositions.at(m_HoveredConnection), m_MapSizes.at(m_HoveredConnection) + QSize(-1,-1));
                painter.setOpacity(0.2);
                painter.fillRect(connRect, QColor(255, 0, 255));
                painter.setOpacity(0.4);
                painter.setPen(QColor(255, 0, 255));
                painter.drawRect(connRect);
                painter.setOpacity(1.0);
            }

            painter.end();
        }
    }

    ///////////////////////////////////////////////////////////
    // Function type:  Setter
    // Contributors:   Pokedude
    // Last edit by:   Pokedude
    // Date of edit:   7/6/2016
    //
    ///////////////////////////////////////////////////////////
    void AMEMapView::setMovementMode(bool isInMovementMode)
    {
        m_MovementMode = isInMovementMode;
    }

    ///////////////////////////////////////////////////////////
    // Function type:  Setter
    // Contributors:   Pokedude
    // Last edit by:   Pokedude
    // Date of edit:   7/6/2016
    //
    ///////////////////////////////////////////////////////////
    void AMEMapView::setBlockView(AMEBlockView *view)
    {
        m_BlockView = view;
    }

    ///////////////////////////////////////////////////////////
    // Function type:  Getter
    // Contributors:   Pokedude
    // Last edit by:   Pokedude
    // Date of edit:   7/6/2016
    //
    ///////////////////////////////////////////////////////////
    bool AMEMapView::movementMode() const
    {
        return m_MovementMode;
    }

    ///////////////////////////////////////////////////////////
    // Function type:  Getter
    // Contributors:   Pokedude
    // Last edit by:   Pokedude
    // Date of edit:   6/18/2016
    //
    ///////////////////////////////////////////////////////////
    QPoint AMEMapView::mainPos()
    {
        return m_MapPositions[0];
    }

    ///////////////////////////////////////////////////////////
    // Function type:  Getter
    // Contributors:   Pokedude
    // Last edit by:   Pokedude
    // Date of edit:   6/18/2016
    //
    ///////////////////////////////////////////////////////////
    Map *AMEMapView::mainMap()
    {
        return m_Maps[0];
    }

    ///////////////////////////////////////////////////////////
    // Function type:  Getter
    // Contributors:   Pokedude
    // Last edit by:   Pokedude
    // Date of edit:   6/18/2016
    //
    ///////////////////////////////////////////////////////////
    MapHeader *AMEMapView::layoutHeader()
    {
        return &m_Header;
    }

    ///////////////////////////////////////////////////////////
    // Function type:  Getter
    // Contributors:   Pokedude
    // Last edit by:   Pokedude
    // Date of edit:   6/18/2016
    //
    ///////////////////////////////////////////////////////////
    const QVector<QRgb> &AMEMapView::mainPalettes() const
    {
        return m_Palettes;
    }

    ///////////////////////////////////////////////////////////
    // Function type:  Getter
    // Contributors:   Pokedude
    // Last edit by:   Pokedude
    // Date of edit:   6/18/2016
    //
    ///////////////////////////////////////////////////////////
    const QImage &AMEMapView::blockForeground() const
    {
        return m_BlockForeground;
    }

    ///////////////////////////////////////////////////////////
    // Function type:  Getter
    // Contributors:   Pokedude
    // Last edit by:   Pokedude
    // Date of edit:   6/18/2016
    //
    ///////////////////////////////////////////////////////////
    const QImage &AMEMapView::blockBackground() const
    {
        return m_BlockBackground;
    }

    ///////////////////////////////////////////////////////////
    // Function type:  Getter
    // Contributors:   Pokedude
    // Last edit by:   Pokedude
    // Date of edit:   6/18/2016
    //
    ///////////////////////////////////////////////////////////
    QSize AMEMapView::primarySetSize()
    {
        return m_PrimarySetSize;
    }

    ///////////////////////////////////////////////////////////
    // Function type:  Getter
    // Contributors:   Pokedude
    // Last edit by:   Pokedude
    // Date of edit:   6/18/2016
    //
    ///////////////////////////////////////////////////////////
    QSize AMEMapView::secondarySetSize()
    {
        return m_SecondarySetSize;
    }

    ///////////////////////////////////////////////////////////
    // Function type:  Getter
    // Contributors:   Pokedude
    // Last edit by:   Pokedude
    // Date of edit:   6/18/2016
    //
    ///////////////////////////////////////////////////////////
    Int32 AMEMapView::primaryBlockCount()
    {
        return m_PrimaryBlockCount;
    }

    ///////////////////////////////////////////////////////////
    // Function type:  Getter
    // Contributors:   Pokedude
    // Last edit by:   Pokedude
    // Date of edit:   6/18/2016
    //
    ///////////////////////////////////////////////////////////
    Int32 AMEMapView::secondaryBlockCount()
    {
        return m_SecondaryBlockCount;
    }

    ///////////////////////////////////////////////////////////
    // Function type:  Getter
    // Contributors:   Pokedude
    // Last edit by:   Pokedude
    // Date of edit:   7/5/2016
    //
    ///////////////////////////////////////////////////////////
    MapBorder &AMEMapView::border()
    {
        return m_Header.border();
    }

    ///////////////////////////////////////////////////////////
    // Function type:  Setter
    // Contributors:   Diegoisawesome
    // Last edit by:   Diegoisawesome
    // Date of edit:   4/25/2017
    //
    ///////////////////////////////////////////////////////////
    void AMEMapView::setGridVisible(bool visible)
    {
		if (m_ShowGrid != visible)
		{
			m_ShowGrid = visible;
			repaint();
		}
    }

    ///////////////////////////////////////////////////////////
    // Function type:  Setter
    // Contributors:   Diegoisawesome
    // Last edit by:   Diegoisawesome
    // Date of edit:   11/12/2016
    //
    ///////////////////////////////////////////////////////////
    void AMEMapView::setMPListener(MovePermissionListener *listener)
    {
        m_MPListener = listener;
    }

	///////////////////////////////////////////////////////////
	// Function type:  Setter
	// Contributors:   Diegoisawesome
	// Last edit by:   Diegoisawesome
	// Date of edit:   4/25/2017
	//
	///////////////////////////////////////////////////////////
	void AMEMapView::setBlockManager(MapBlockManager *manager)
	{
		m_BlockManager = manager;
	}
}
