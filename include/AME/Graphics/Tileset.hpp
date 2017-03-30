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


#ifndef __AME_TILESET_HPP__
#define __AME_TILESET_HPP__


///////////////////////////////////////////////////////////
// Include files
//
///////////////////////////////////////////////////////////
#include <QBoy/Graphics/Image.hpp>
#include <QBoy/Graphics/Palette.hpp>
#include <AME/System/IUndoable.hpp>
#include <AME/System/WriteEntry.hpp>
#include <AME/Graphics/Block.hpp>
#include <AME/Graphics/PropertyTable.hpp>


namespace ame
{
    ///////////////////////////////////////////////////////////
    /// \file    Tileset.hpp
    /// \author  Pokedude
    /// \version 1.0.0.0
    /// \date    6/7/2016
    /// \brief   Defines an image containing several tiles.
    ///
    /// Undo/redo system to be implemented later.
    ///
    ///////////////////////////////////////////////////////////
    class Tileset /*: public IUndoable*/ {
    public:

        ///////////////////////////////////////////////////////////
        /// \brief Default constructor
        ///
        /// Initializes a new instance of ame::Tileset.
        /// Note: Tileset is designed to work with stack
        /// objects, but it is recommended to allocate instances
        /// on the global heap, because of the undo/redo system.
        ///
        ///////////////////////////////////////////////////////////
        Tileset();

        ///////////////////////////////////////////////////////////
        /// \brief Copy constructor
        ///
        /// Copies all members of the given ame::Tileset.
        /// Is only called by template code, not by actual AME code.
        ///
        ///////////////////////////////////////////////////////////
        Tileset(const Tileset &rvalue);

        ///////////////////////////////////////////////////////////
        /// \brief Assignment constructor
        ///
        /// Copies all members of the given ame::Tileset
        /// and stores them in a new class instance.
        ///
        ///////////////////////////////////////////////////////////
        Tileset &operator=(const Tileset &rvalue);

        ///////////////////////////////////////////////////////////
        /// \brief Destructor
        ///
        /// Deletes all the allocated block objects.
        ///
        ///////////////////////////////////////////////////////////
        ~Tileset();


        ///////////////////////////////////////////////////////////
        /// \brief Attempts to read all connections on the map.
        ///
        /// Reads all connections until the specified amount of
        /// connections were read or if an invalid connection
        /// was encountered.
        ///
        /// \param rom Currently opened ROM file
        /// \param offset Offset of the connection table
        /// \returns true if all scripts were read correctly.
        ///
        ///////////////////////////////////////////////////////////
        bool read(const qboy::Rom &rom, UInt32 offset);


        ///////////////////////////////////////////////////////////
        /// \brief Retrieves the offset of this tileset.
        ///
        ///////////////////////////////////////////////////////////
        UInt32 offset() const;

        ///////////////////////////////////////////////////////////
        /// \brief Retrieves the image of this tileset.
        ///
        ///////////////////////////////////////////////////////////
        qboy::Image *image() const;

        ///////////////////////////////////////////////////////////
        /// \brief Retrieves the palettes of this tileset.
        ///
        ///////////////////////////////////////////////////////////
        const QList<qboy::Palette *> &palettes() const;

        ///////////////////////////////////////////////////////////
        /// \brief Retrieves the blocks of this tileset.
        ///
        ///////////////////////////////////////////////////////////
        const QList<Block *> &blocks() const;


    private:

        ///////////////////////////////////////////////////////////
        // Class members
        //
        ///////////////////////////////////////////////////////////
        UInt32 m_Offset;                ///< Offset of the tileset
        UInt8 m_IsCompressed;           ///< Is image compressed?
        UInt8 m_IsSecondary;            ///< Is tileset a main one?
        UInt32 m_PtrImage;              ///< Offset of the image
        UInt32 m_PtrPalette;            ///< Offset of the 6/7 pals
        UInt32 m_PtrBlocks;             ///< Offset of the blocks
        UInt32 m_PtrAnimations;         ///< Offset of the tile anims
        UInt32 m_PtrBehaviour;          ///< Offset of the behaviour bytes
        qboy::Image *m_Image;           ///< Actual image holding 4bpp data
        QList<qboy::Palette *> m_Pals;  ///< Holds all palettes (6/7)
        QList<Block *> m_Blocks;        ///< Holds all blocks, amount depends on type
        PropertyTable *m_Properties;    ///< Holds all block properties
    };
}


#endif // __AME_TILESET_HPP__
