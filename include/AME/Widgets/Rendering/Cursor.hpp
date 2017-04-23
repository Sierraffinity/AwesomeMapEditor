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


#ifndef CURSOR_HPP
#define CURSOR_HPP

#include <QPoint>
#include <QSize>
#include <QColor>
#include <QPaintEvent>
#include <QPainter>

namespace ame
{
	class Cursor
	{
		enum Tool {
			None,
			Draw,
			Pick,
			Fill,
			FillAll
		};

	public:
		Cursor(const QPoint& pos = QPoint(), Tool tool = None, bool visible = false);

		void setTool(Tool tool);

		void setPos(const QPoint & pos);

		void beginPick(const QPoint& pos);

		void resizeWithAnchor(const QPoint& pos, const QRect& bounds);

		void setVisible(bool visible);

		QRect getAdjustedRect(const QRect& bounds) const;

		///////////////////////////////////////////////////////////
		/// \brief Paints the cursor.
		///
		///////////////////////////////////////////////////////////
		void paintEvent(QPaintEvent* event, QPainter& painter, const QRect& bounds);

	private:
		QColor getToolColor();

		QRect m_Rect;
		QPoint m_Anchor;
		Tool m_Tool;
		bool m_Visible;
	};
}

#endif // CURSOR_HPP