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


#include <QHexLineEdit.h>
#include <QPainter>
#include <QKeyEvent>
#include <QDebug>

QHexLineEdit::QHexLineEdit(QWidget *parent) :
    QLineEdit(parent)
{
    //setStyleSheet("QLineEdit { padding: 0px 0px 0px 13px; }");
    QString mask = ">";
    if (m_byteLength > 0)
    {
        for (int _ = 0; _ < m_byteLength; ++_)
        {
            mask.append("HH");
        }
    }
    else
        mask.append("H");
    setInputMask(mask);
}


void QHexLineEdit::paintEvent(QPaintEvent *event)
{
    QLineEdit::paintEvent(event);
    //QPainter painter(this);
    //painter.setPen(QColor::fromRgb(0x000000));
    //painter.drawText(contentsMargins().left() + 3, 0, fontMetrics().width("0x"), height() -1, Qt::AlignVCenter, "0x");
}

void QHexLineEdit::keyPressEvent(QKeyEvent *event)
{
    int key = event->key();
    int mod = event->modifiers();
    bool success = false;
    event->text().toInt(&success, 16);
    if (key == Qt::Key_Backspace || key == Qt::Key_Return || key == Qt::Key_Enter ||
            mod == Qt::ControlModifier)
        success = true;
    if (success == false)
    {
        event->accept();
        return;
    }
    QLineEdit::keyPressEvent(new QKeyEvent(QEvent::KeyPress,
                                           event->key(), event->modifiers(),
                                           event->text().toUpper()));
}
