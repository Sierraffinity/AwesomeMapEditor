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


#ifndef __AME_POKEMONTABLE_HPP__
#define __AME_POKEMONTABLE_HPP__


///////////////////////////////////////////////////////////
// Include files
//
///////////////////////////////////////////////////////////
#include <QBoy/Graphics/Image.hpp>
#include <QBoy/Graphics/Palette.hpp>


namespace ame
{
    ///////////////////////////////////////////////////////////
    /// \file    PokemonTable.hpp
    /// \author  Pokedude
    /// \version 1.0.0.0
    /// \date    6/19/2016
    /// \brief   Holds all pokemon icons and names.
    ///
    ///////////////////////////////////////////////////////////
    class PokemonTable {
    public:

        ///////////////////////////////////////////////////////////
        /// \brief Default constructor
        ///
        /// Initializes a new instance of ame::PokemonTable.
        /// Note: PokemonTable is designed to work with stack
        /// objects, but it is recommended to allocate instances
        /// on the global heap, because of the undo/redo system.
        ///
        ///////////////////////////////////////////////////////////
        PokemonTable();

        ///////////////////////////////////////////////////////////
        /// \brief Copy constructor
        ///
        /// Copies all members of the given ame::PokemonTable.
        /// Is only called by template code, not by actual AME code.
        ///
        ///////////////////////////////////////////////////////////
        PokemonTable(const PokemonTable &rvalue);

        ///////////////////////////////////////////////////////////
        /// \brief Assignment constructor
        ///
        /// Copies all members of the given ame::PokemonTable
        /// and stores them in a new class instance.
        ///
        ///////////////////////////////////////////////////////////
        PokemonTable &operator=(const PokemonTable &rvalue);

        ///////////////////////////////////////////////////////////
        /// \brief Destructor
        ///
        /// Deletes all the allocated block objects.
        ///
        ///////////////////////////////////////////////////////////
        ~PokemonTable();


        ///////////////////////////////////////////////////////////
        /// \brief Attempts to read all required pokemon data.
        ///
        /// For AME, we need the pokémon names, icons and palettes.
        ///
        /// \param rom Currently opened ROM file
        /// \returns true if all things were read correctly.
        ///
        ///////////////////////////////////////////////////////////
        bool read(const qboy::Rom &rom);


        ///////////////////////////////////////////////////////////
        /// \brief Retrieves all Pokémon names.
        ///
        ///////////////////////////////////////////////////////////
        const QStringList &names() const;

        ///////////////////////////////////////////////////////////
        /// \brief Retrieves all Pokémon pictures.
        ///
        ///////////////////////////////////////////////////////////
        const QList<QImage> &images() const;


    private:

        ///////////////////////////////////////////////////////////
        // Class members
        //
        ///////////////////////////////////////////////////////////
        QStringList m_Names;    ///< Holds all decoded names
        QList<QImage> m_Images; ///< Holds all decoded images
    };
}


#endif // __AME_POKEMONTABLE_HPP__
