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


///////////////////////////////////////////////////////////
// Include files
//
///////////////////////////////////////////////////////////
#include <AME/Mapping/MappingErrors.hpp>
#include <AME/Mapping/MapHeader.hpp>
#include <AME/Graphics/TilesetManager.hpp>
#include <QSize>


namespace ame
{
    ///////////////////////////////////////////////////////////
    // Function type:  Constructor
    // Contributors:   Pokedude
    // Last edit by:   Pokedude
    // Date of edit:   6/11/2016
    //
    ///////////////////////////////////////////////////////////
    MapHeader::MapHeader()
        : /*IUndoable(),*/
          m_Offset(0),
          m_Width(0),
          m_Height(0),
          m_PtrBlocks(0),
          m_PtrBorder(0),
          m_PtrPrimary(0),
          m_PtrSecondary(0)
    {
    }

    ///////////////////////////////////////////////////////////
    // Function type:  Constructor
    // Contributors:   Pokedude
    // Last edit by:   Pokedude
    // Date of edit:   6/11/2016
    //
    ///////////////////////////////////////////////////////////
    MapHeader::MapHeader(const MapHeader &rvalue)
        : /*IUndoable(),*/
          m_Offset(rvalue.m_Offset),
          m_Width(rvalue.m_Width),
          m_Height(rvalue.m_Height),
          m_PtrBlocks(rvalue.m_PtrBlocks),
          m_PtrBorder(rvalue.m_PtrBlocks),
          m_PtrPrimary(rvalue.m_PtrPrimary),
          m_PtrSecondary(rvalue.m_PtrSecondary),
          m_Blocks(rvalue.m_Blocks),
          m_Primary(rvalue.m_Primary),
          m_Secondary(rvalue.m_Secondary)
    {
    }

    ///////////////////////////////////////////////////////////
    // Function type:  Constructor
    // Contributors:   Pokedude
    // Last edit by:   Pokedude
    // Date of edit:   6/11/2016
    //
    ///////////////////////////////////////////////////////////
    MapHeader &MapHeader::operator=(const MapHeader &rvalue)
    {
        m_Blocks = rvalue.m_Blocks;
        m_Width = rvalue.m_Width;
        m_Height = rvalue.m_Height;
        m_PtrBlocks = rvalue.m_PtrBlocks;
        m_PtrBorder = rvalue.m_PtrBlocks;
        m_PtrPrimary = rvalue.m_PtrPrimary;
        m_PtrSecondary = rvalue.m_PtrSecondary;
        m_Primary = rvalue.m_Primary;
        m_Secondary = rvalue.m_Secondary;
        return *this;
    }

    ///////////////////////////////////////////////////////////
    // Function type:  Destructor
    // Contributors:   Pokedude
    // Last edit by:   Pokedude
    // Date of edit:   6/11/2016
    //
    ///////////////////////////////////////////////////////////
    MapHeader::~MapHeader()
    {
        foreach (MapBlock *block, m_Blocks)
            delete block;
    }


    ///////////////////////////////////////////////////////////
    // Function type:  I/O
    // Contributors:   Pokedude
    // Last edit by:   Pokedude
    // Date of edit:   6/11/2016
    //
    ///////////////////////////////////////////////////////////
    bool MapHeader::read(const qboy::Rom &rom, UInt32 offset)
    {
        if (!rom.seek(offset))
            AME_THROW(HDR_ERROR_OFFSET, rom.redirected());


        // Reads the map's dimensions
        m_Width = rom.readWord();
        m_Height = rom.readWord();

        // Reads all the pointers within the structure
        m_PtrBorder = rom.readPointerRef();
        m_PtrBlocks = rom.readPointer();
        m_PtrPrimary = rom.readPointerRef();
        m_PtrSecondary = rom.readPointerRef();

        // Determines whether these pointers are valid
        if (!rom.checkOffset(m_PtrBorder))
            AME_THROW(HDR_ERROR_BORDER, offset + 8);
        if (!rom.checkOffset(m_PtrBlocks))
            AME_THROW(HDR_ERROR_BLOCKS, offset + 12);
        if (!rom.checkOffset(m_PtrPrimary))
            AME_THROW(HDR_ERROR_PRIMARY, offset + 16);
        if (!rom.checkOffset(m_PtrSecondary))
            AME_THROW(HDR_ERROR_SECONDARY, offset + 20);


        // Retrieves the border (different between games!)
        if (rom.info().isFRLG())
        {
            QSize borderSize(rom.readByte(), rom.readByte());
            m_Border.read(rom, m_PtrBorder, borderSize);
        }
        else
        {
            m_Border.read(rom, m_PtrBorder, QSize(2, 2));
        }

        // Retrieves the map block data
        rom.seek(m_PtrBlocks);
        for (unsigned i = 0; i < m_Width * m_Height; i++)
        {
            MapBlock *block = new MapBlock;
            UInt16 data = rom.readHWord();

            block->block = (data & 0x3FF);
            block->permission = (data >> 0xA);
            m_Blocks.push_back(block);
        }

        // Loads the tilesets, if necessary
        if ((m_Primary = TilesetManager::get(m_PtrPrimary)) == NULL)
        {
            m_Primary = new Tileset;
            m_Primary->read(rom, m_PtrPrimary);
        }
        if ((m_Secondary = TilesetManager::get(m_PtrSecondary)) == NULL)
        {
            m_Secondary = new Tileset;
            m_Secondary->read(rom, m_PtrSecondary);
        }


        // Loading successful
        return true;
    }


    ///////////////////////////////////////////////////////////
    // Function type:  Getter
    // Contributors:   Pokedude
    // Last edit by:   Pokedude
    // Date of edit:   6/11/2016
    //
    ///////////////////////////////////////////////////////////
    MapBorder &MapHeader::border()
    {
        return m_Border;
    }

    ///////////////////////////////////////////////////////////
    // Function type:  Getter
    // Contributors:   Pokedude
    // Last edit by:   Pokedude
    // Date of edit:   6/11/2016
    //
    ///////////////////////////////////////////////////////////
    const QList<MapBlock *> &MapHeader::blocks() const
    {
        return m_Blocks;
    }

    ///////////////////////////////////////////////////////////
    // Function type:  Getter
    // Contributors:   Pokedude
    // Last edit by:   Pokedude
    // Date of edit:   6/11/2016
    //
    ///////////////////////////////////////////////////////////
    Tileset *MapHeader::primary() const
    {
        return m_Primary;
    }

    ///////////////////////////////////////////////////////////
    // Function type:  Getter
    // Contributors:   Pokedude
    // Last edit by:   Pokedude
    // Date of edit:   6/11/2016
    //
    ///////////////////////////////////////////////////////////
    Tileset *MapHeader::secondary() const
    {
        return m_Secondary;
    }

    ///////////////////////////////////////////////////////////
    // Function type:  Getter
    // Contributors:   Pokedude
    // Last edit by:   Pokedude
    // Date of edit:   6/11/2016
    //
    ///////////////////////////////////////////////////////////
    const QSize MapHeader::size() const
    {
        return QSize(m_Width, m_Height);
    }
}
