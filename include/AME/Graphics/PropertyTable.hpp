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


#ifndef __AME_PROPERTYTABLE_HPP__
#define __AME_PROPERTYTABLE_HPP__


///////////////////////////////////////////////////////////
// Include files
//
///////////////////////////////////////////////////////////
#include <QBoy/Core/Rom.hpp>
#include <AME/Graphics/Property.hpp>


namespace ame
{
    ///////////////////////////////////////////////////////////
    /// \file    Property.hpp
    /// \author  Pokedude
    /// \version 1.0.0.0
    /// \date    6/20/2016
    /// \brief   Holds all behaviour and background bytes.
    ///
    ///////////////////////////////////////////////////////////
    class PropertyTable /* : public IUndoable */ {
    public:

        ///////////////////////////////////////////////////////////
        /// \brief Default constructor
        ///
        /// Initializes a new instance of ame::PropertyTable.
        /// Note: PropertyTable is designed to work with stack
        /// objects, but it is recommended to allocate instances
        /// on the global heap, because of the undo/redo system.
        ///
        ///////////////////////////////////////////////////////////
        PropertyTable();

        ///////////////////////////////////////////////////////////
        /// \brief Copy constructor
        ///
        /// Copies all members of the given ame::PropertyTable.
        /// Is only called by template code, not by actual AME code.
        ///
        ///////////////////////////////////////////////////////////
        PropertyTable(const PropertyTable &rvalue);

        ///////////////////////////////////////////////////////////
        /// \brief Assignment constructor
        ///
        /// Copies all members of the given ame::PropertyTable
        /// and stores them in a new class instance.
        ///
        ///////////////////////////////////////////////////////////
        PropertyTable &operator=(const PropertyTable &rvalue);

        ///////////////////////////////////////////////////////////
        /// \brief Destructor
        ///
        /// Deletes all the allocated images.
        ///
        ///////////////////////////////////////////////////////////
        ~PropertyTable();


        ///////////////////////////////////////////////////////////
        /// \brief Attempts to read all properties.
        ///
        /// \param rom Currently opened ROM file
        /// \returns true if all properties were read correctly.
        ///
        ///////////////////////////////////////////////////////////
        bool read(const qboy::Rom &rom, UInt32 offset, Int32 blocks);


        ///////////////////////////////////////////////////////////
        /// \brief Retrieves all Pokémon pictures.
        ///
        ///////////////////////////////////////////////////////////
        const QList<Property *> &properties() const;


    private:

        ///////////////////////////////////////////////////////////
        // Class members
        //
        ///////////////////////////////////////////////////////////
        UInt32 m_Offset;
        QList<Property *> m_Properties; ///< Holds all decoded images
    };
}


#endif // __AME_PROPERTYTABLE_HPP__
