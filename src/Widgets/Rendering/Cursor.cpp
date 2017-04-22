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
		m_Position(pos),
		m_Anchor(pos),
		m_Tool(tool),
		m_Visible(visible)
	{

	}

	void Cursor::setTool(Tool tool)
	{
		m_Tool = tool;
	}

	///////////////////////////////////////////////////////////
	// Function type:  Helper
	// Contributors:   Diegoisawesome
	// Last edit by:   Diegoisawesome
	// Date of edit:   4/21/2017
	//
	///////////////////////////////////////////////////////////
	QRect Cursor::getRect()
	{
        const int blockSize = SETTINGS(ScaleFactor) * 16;
		QRect rect(m_Anchor, m_Position);
		rect = rect.normalized();
        rect.setSize(rect.size() * blockSize);
        return rect - QMargins(0, 0, 1, 1);
	}

	///////////////////////////////////////////////////////////
	// Function type:  Event
	// Contributors:   Diegoisawesome
	// Last edit by:   Diegoisawesome
	// Date of edit:   4/21/2017
	//
	///////////////////////////////////////////////////////////
	void Cursor::paintEvent(QPaintEvent* event, QPainter& painter, const QRect& bounds)
	{
		if (m_Visible)
		{
            QRect rect = getRect().adjusted(bounds.x(), bounds.y(), bounds.x(), bounds.y());
			rect &= (bounds - QMargins(0, 0, 1, 1));
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
