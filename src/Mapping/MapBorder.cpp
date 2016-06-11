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
#include <AME/Mapping/MapBorder.hpp>


namespace ame
{
    ///////////////////////////////////////////////////////////
    // Function type:  Constructor
    // Contributors:   Pokedude
    // Last edit by:   Pokedude
    // Date of edit:   6/11/2016
    //
    ///////////////////////////////////////////////////////////
    MapBorder::MapBorder()
        : /*IUndoable(),*/
          m_Width(0),
          m_Height(0)
    {
    }

    ///////////////////////////////////////////////////////////
    // Function type:  Constructor
    // Contributors:   Pokedude
    // Last edit by:   Pokedude
    // Date of edit:   6/11/2016
    //
    ///////////////////////////////////////////////////////////
    MapBorder::MapBorder(const MapBorder &rvalue)
        : /*IUndoable(),*/
          m_Blocks(rvalue.m_Blocks),
          m_Width(rvalue.m_Width),
          m_Height(rvalue.m_Height)
    {
    }

    ///////////////////////////////////////////////////////////
    // Function type:  Constructor
    // Contributors:   Pokedude
    // Last edit by:   Pokedude
    // Date of edit:   6/11/2016
    //
    ///////////////////////////////////////////////////////////
    MapBorder &MapBorder::operator=(const MapBorder &rvalue)
    {
        m_Blocks = rvalue.m_Blocks;
        m_Width = rvalue.m_Width;
        m_Height = rvalue.m_Height;
        return *this;
    }

    ///////////////////////////////////////////////////////////
    // Function type:  Destructor
    // Contributors:   Pokedude
    // Last edit by:   Pokedude
    // Date of edit:   6/11/2016
    //
    ///////////////////////////////////////////////////////////
    MapBorder::~MapBorder()
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
    bool MapBorder::read(const qboy::Rom &rom, UInt32 offset, const QSize &size)
    {
        if (!rom.seek(offset))
            AME_THROW(BRD_ERROR_OFFSET, rom.redirected());

        // Copies the size to the respective members
        m_Width = size.width();
        m_Height = size.height();

        // Reads all blocks
        for (unsigned i = 0; i < m_Width * m_Height; i++)
        {
            MapBlock *block = new MapBlock;
            UInt16 data = rom.readHWord();

            block->block = (data & 0x3FF);
            block->permission = 0; // unused
            m_Blocks.push_back(block);
        }


        // Reading successful
        return true;
    }


    ///////////////////////////////////////////////////////////
    // Function type:  Getter
    // Contributors:   Pokedude
    // Last edit by:   Pokedude
    // Date of edit:   6/11/2016
    //
    ///////////////////////////////////////////////////////////
    const QList<MapBlock *> &MapBorder::blocks() const
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
    UInt32 MapBorder::width() const
    {
        return m_Width;
    }

    ///////////////////////////////////////////////////////////
    // Function type:  Getter
    // Contributors:   Pokedude
    // Last edit by:   Pokedude
    // Date of edit:   6/11/2016
    //
    ///////////////////////////////////////////////////////////
    UInt32 MapBorder::height() const
    {
        return m_Height;
    }
}
