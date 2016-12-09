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


#include <QFilterChildrenProxyModel.h>

QFilterChildrenProxyModel::QFilterChildrenProxyModel(QObject *parent) :
    QSortFilterProxyModel(parent)
{

}

bool QFilterChildrenProxyModel::filterAcceptsRow(int source_row, const QModelIndex &source_parent) const
{
    // custom behaviour :
    if(filterRegExp().isEmpty() == false)
    {
        // get source-model index for current row
        QModelIndex source_index = sourceModel()->index(source_row, this->filterKeyColumn(), source_parent) ;
        if(source_index.isValid())
        {
            // if any of children matches the filter, then current index matches the filter as well
            int i, nb = sourceModel()->rowCount(source_index);
            for (i = 0; i < nb; ++i)
            {
                if(filterAcceptsRow(i, source_index))
                {
                    return true;
                }
            }
            // check current index itself
            QString key = sourceModel()->data(source_index, filterRole()).toString();
            QString parentKey = sourceModel()->data(source_parent, filterRole()).toString();
            return key.contains(filterRegExp()) || parentKey.contains(filterRegExp());
        }
    }
    // parent call for initial behaviour
    return QSortFilterProxyModel::filterAcceptsRow(source_row, source_parent) ;
}
