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
// Copyright (C) 2016-2017 Diegoisawesome, Pokedude
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
#include <AME/Widgets/Rendering/Cursor.hpp>
#include <AME/System/Settings.hpp>
#include <QDebug>

namespace ame
{
	Cursor::Cursor(const QPoint& pos, const QRect& bounds, Tool tool, bool visible) :
		m_Rect(pos, QSize(1,1)),
		m_OldRect(pos, QSize(1, 1)),
		m_Bounds(bounds),
		m_Anchor(pos),
		m_Tool(tool),
		m_Visible(visible)
	{

	}

	///////////////////////////////////////////////////////////
	// Function type:  Setter
	// Contributors:   Diegoisawesome
	// Last edit by:   Diegoisawesome
	// Date of edit:   4/21/2017
	//
	///////////////////////////////////////////////////////////
	void Cursor::setTool(Tool tool)
	{
		m_Tool = tool;
	}

	QPoint Cursor::conformToBounds(QPoint pos, const QRect& bounds)
	{
		if (!bounds.contains(pos))
		{
			QPoint bottomRight(bounds.left() + bounds.width(), bounds.top() + bounds.height());

			if (pos.x() < bounds.left())
				pos.setX(bounds.left());
			else if (pos.x() > bottomRight.x())
				pos.setX(bottomRight.x());

			if (pos.y() < bounds.top())
				pos.setY(bounds.top());
			else if (pos.y() > bottomRight.y())
				pos.setY(bottomRight.y());
		}
		return pos;
	}

	///////////////////////////////////////////////////////////
	// Function type:  Setter
	// Contributors:   Diegoisawesome
	// Last edit by:   Diegoisawesome
	// Date of edit:   4/21/2017
	//
	///////////////////////////////////////////////////////////
	void Cursor::setBounds(const QRect& bounds)
	{
		m_Bounds = bounds;
	}

	///////////////////////////////////////////////////////////
	// Function type:  Setter
	// Contributors:   Diegoisawesome
	// Last edit by:   Diegoisawesome
	// Date of edit:   4/21/2017
	//
	///////////////////////////////////////////////////////////
	bool Cursor::setPos(const QPoint& pos)
	{
		if (m_Rect.topLeft() == pos)
			return false;

		if (m_Bounds.contains(pos))
		{
			m_Rect.moveTo(pos);
			setVisible(true);
		}
		else if (m_Visible)
			setVisible(false);
		else
			return false;

		return true;
	}

	///////////////////////////////////////////////////////////
	// Function type:  Setter
	// Contributors:   Diegoisawesome
	// Last edit by:   Diegoisawesome
	// Date of edit:   4/21/2017
	//
	///////////////////////////////////////////////////////////
	void Cursor::setAnchor(const QPoint& pos)
	{
		m_Anchor = pos;
		resizeWithAnchor(pos);
	}

	///////////////////////////////////////////////////////////
	// Function type:  Event
	// Contributors:   Diegoisawesome
	// Last edit by:   Diegoisawesome
	// Date of edit:   4/24/2017
	//
	///////////////////////////////////////////////////////////
	bool Cursor::mousePressEvent(const QPoint& pos, Tool tool)
	{
		if (!m_Bounds.contains(pos))
			return false;

		if (m_Tool == Pick)
		{
			m_Rect = m_OldRect;
			setPos(pos);
		}
		else
			m_OldRect = m_Rect;

		m_Tool = tool;
		
		if (m_Tool == Pick)
			setAnchor(pos);
		return true;
	}

	///////////////////////////////////////////////////////////
	// Function type:  Event
	// Contributors:   Diegoisawesome
	// Last edit by:   Diegoisawesome
	// Date of edit:   4/24/2017
	//
	///////////////////////////////////////////////////////////
	bool Cursor::mouseMoveEvent(const QPoint& pos)
	{
		if (m_Tool == Pick)
			return resizeWithAnchor(pos);
		else
			return setPos(pos);
	}

	///////////////////////////////////////////////////////////
	// Function type:  Event
	// Contributors:   Diegoisawesome
	// Last edit by:   Diegoisawesome
	// Date of edit:   4/24/2017
	//
	///////////////////////////////////////////////////////////
	bool Cursor::mouseReleaseEvent(const QPoint& pos)
	{
		setPos(pos);
		m_Tool = None;
		return true;
	}

	bool Cursor::resizeWithAnchor(const QPoint& pos)
	{
		QRect rect(m_Anchor, pos);

		if (pos.x() < m_Anchor.x())
		{
			rect.setLeft(pos.x());
			rect.setRight(m_Anchor.x());
		}

		if (pos.y() < m_Anchor.y())
		{
			rect.setTop(pos.y());
			rect.setBottom(m_Anchor.y());
		}

		rect &= m_Bounds;

		if (m_Rect == m_Bounds)
			return false;

		m_Rect = rect;
		return true;
	}

	void Cursor::setVisible(bool visible)
	{
		m_Visible = visible;
	}

	///////////////////////////////////////////////////////////
	// Function type:  Helper
	// Contributors:   Diegoisawesome
	// Last edit by:   Diegoisawesome
	// Date of edit:   4/22/2017
	//
	///////////////////////////////////////////////////////////
	QRect Cursor::getAdjustedRect(const QRect& bounds) const
	{
        //const int blockSize = SETTINGS(ScaleFactor) * 16;
		//QRect rect(m_Anchor, m_Position);
		//rect = rect.normalized();
		QRect rect(m_Rect.topLeft() * MAP_BLOCK_SIZE, m_Rect.size() * MAP_BLOCK_SIZE);
		rect -= QMargins(0, 0, 1, 1);
		return rect.adjusted(bounds.x(), bounds.y(), bounds.x(), bounds.y()) & (bounds - QMargins(0, 0, 1, 1));;
	}

	///////////////////////////////////////////////////////////
	// Function type:  Event
	// Contributors:   Diegoisawesome
	// Last edit by:   Diegoisawesome
	// Date of edit:   4/22/2017
	//
	///////////////////////////////////////////////////////////
	void Cursor::paintEvent(QPaintEvent* event, QPainter& painter, const QRect& bounds)
	{
		if (m_Visible)
		{
			QRect rect = getAdjustedRect(bounds);
			//rect &= (bounds - QMargins(0, 0, 1, 1));
			painter.setPen(getToolColor());
			painter.setBrush(Qt::transparent);
			painter.drawRect(rect);
		}
	}

	///////////////////////////////////////////////////////////
	// Function type:  Helper
	// Contributors:   Diegoisawesome
	// Last edit by:   Diegoisawesome
	// Date of edit:   4/21/2017
	//
	///////////////////////////////////////////////////////////
	QColor Cursor::getToolColor() const
	{
		switch (m_Tool)
		{
		case Draw:
			return Qt::GlobalColor::red;
		case Pick:
			return Qt::GlobalColor::yellow;
		default:
			return Qt::GlobalColor::green;
		}
	}
}
