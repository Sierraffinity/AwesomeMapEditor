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
	Cursor::Cursor(const QPoint& pos, Tool tool, bool visible) :
		m_Rect(pos, QSize(1,1)),
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

	///////////////////////////////////////////////////////////
	// Function type:  Setter
	// Contributors:   Diegoisawesome
	// Last edit by:   Diegoisawesome
	// Date of edit:   4/21/2017
	//
	///////////////////////////////////////////////////////////
	void Cursor::setPos(const QPoint& pos)
	{
		m_Rect.moveTo(pos);
	}

	///////////////////////////////////////////////////////////
	// Function type:  Setter
	// Contributors:   Diegoisawesome
	// Last edit by:   Diegoisawesome
	// Date of edit:   4/21/2017
	//
	///////////////////////////////////////////////////////////
	void Cursor::beginPick(const QPoint& pos)
	{
		m_Tool = Pick;
		m_Anchor = pos;
	}

	void Cursor::resizeWithAnchor(const QPoint& pos, const QRect& bounds)
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
		m_Rect = rect & bounds;
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
        const int blockSize = SETTINGS(ScaleFactor) * 16;
		//QRect rect(m_Anchor, m_Position);
		//rect = rect.normalized();
		QRect rect(m_Rect.topLeft() * blockSize, m_Rect.size() * blockSize);
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
	QColor Cursor::getToolColor()
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
