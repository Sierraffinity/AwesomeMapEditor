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


#ifndef __AME_ITEMTABLE_HPP__
#define __AME_ITEMTABLE_HPP__


///////////////////////////////////////////////////////////
// Include files
//
///////////////////////////////////////////////////////////


namespace ame
{
    ///////////////////////////////////////////////////////////
    /// \file    ItemTable.hpp
    /// \author  Diegoisawesome
    /// \version 1.0.0.0
    /// \date    7/5/2016
    /// \brief   Holds all item names.
    ///
    ///////////////////////////////////////////////////////////
    class ItemTable {
    public:

        ///////////////////////////////////////////////////////////
        /// \brief Default constructor
        ///
        /// Initializes a new instance of ame::ItemTable.
        /// Note: ItemTable is designed to work with stack
        /// objects, but it is recommended to allocate instances
        /// on the global heap, because of the undo/redo system.
        ///
        ///////////////////////////////////////////////////////////
        ItemTable();

        ///////////////////////////////////////////////////////////
        /// \brief Copy constructor
        ///
        /// Copies all members of the given ame::ItemTable.
        /// Is only called by template code, not by actual AME code.
        ///
        ///////////////////////////////////////////////////////////
        ItemTable(const ItemTable &rvalue);

        ///////////////////////////////////////////////////////////
        /// \brief Assignment constructor
        ///
        /// Copies all members of the given ame::ItemTable
        /// and stores them in a new class instance.
        ///
        ///////////////////////////////////////////////////////////
        ItemTable &operator=(const ItemTable &rvalue);

        ///////////////////////////////////////////////////////////
        /// \brief Destructor
        ///
        /// Deletes all the allocated names.
        ///
        ///////////////////////////////////////////////////////////
        ~ItemTable();


        ///////////////////////////////////////////////////////////
        /// \brief Attempts to read the item names.
        ///
        /// \param rom Currently opened ROM file
        /// \returns true if all names were read correctly.
        ///
        ///////////////////////////////////////////////////////////
        bool read(const qboy::Rom &rom);


        ///////////////////////////////////////////////////////////
        /// \brief Retrieves all item names.
        ///
        ///////////////////////////////////////////////////////////
        const QStringList &names() const;

    private:

        ///////////////////////////////////////////////////////////
        // Class members
        //
        ///////////////////////////////////////////////////////////
        QStringList m_Names;    ///< Holds all decoded names
    };
}


#endif // __AME_ITEMTABLE_HPP__
