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
#include <AME/Graphics/GraphicsErrors.hpp>
#include <AME/Graphics/Tileset.hpp>


namespace ame
{
    ///////////////////////////////////////////////////////////
    // Function type:  Constructor
    // Contributers:   Pokedude
    // Last edit by:   Pokedude
    // Date of edit:   6/7/2016
    //
    ///////////////////////////////////////////////////////////
    Tileset::Tileset()
        : /*IUndoable(),*/
          m_Offset(0),
          m_IsCompressed(0),
          m_IsPrimary(0),
          m_PtrImage(0),
          m_PtrPalette(0),
          m_PtrAnimations(0),
          m_PtrBehaviour(0),
          m_Image(new qboy::Image)
    {
    }

    ///////////////////////////////////////////////////////////
    // Function type:  Constructor
    // Contributers:   Pokedude
    // Last edit by:   Pokedude
    // Date of edit:   6/7/2016
    //
    ///////////////////////////////////////////////////////////
    Tileset::Tileset(const Tileset &rvalue)
        : /*IUndoable(),*/
          m_Offset(rvalue.m_Offset),
          m_IsCompressed(rvalue.m_IsCompressed),
          m_IsPrimary(rvalue.m_IsPrimary),
          m_PtrImage(rvalue.m_PtrImage),
          m_PtrPalette(rvalue.m_PtrPalette),
          m_PtrAnimations(rvalue.m_PtrAnimations),
          m_PtrBehaviour(rvalue.m_PtrBehaviour),
          m_Image(rvalue.m_Image),
          m_Pals(rvalue.m_Pals),
          m_Blocks(rvalue.m_Blocks)
    {
    }

    ///////////////////////////////////////////////////////////
    // Function type:  Constructor
    // Contributers:   Pokedude
    // Last edit by:   Pokedude
    // Date of edit:   6/7/2016
    //
    ///////////////////////////////////////////////////////////
    Tileset &Tileset::operator=(const Tileset &rvalue)
    {
        m_Offset = rvalue.m_Offset;
        m_IsCompressed = rvalue.m_IsCompressed;
        m_IsPrimary = rvalue.m_IsPrimary;
        m_PtrImage = rvalue.m_PtrImage;
        m_PtrPalette = rvalue.m_PtrPalette;
        m_PtrAnimations = rvalue.m_PtrAnimations;
        m_PtrBehaviour = rvalue.m_PtrBehaviour;
        m_Image = rvalue.m_Image;
        m_Pals = rvalue.m_Pals;
        m_Blocks = rvalue.m_Blocks;
        return *this;
    }

    ///////////////////////////////////////////////////////////
    // Function type:  Destructor
    // Contributers:   Pokedude
    // Last edit by:   Pokedude
    // Date of edit:   6/7/2016
    //
    ///////////////////////////////////////////////////////////
    Tileset::~Tileset()
    {
        foreach (Block *block, m_Blocks)
            delete block;
        foreach (qboy::Palette *pal, m_Pals)
            delete pal;
        delete m_Image;
    }


    ///////////////////////////////////////////////////////////
    // Function type:  I/O
    // Contributers:   Pokedude
    // Last edit by:   Pokedude
    // Date of edit:   6/5/2016
    //
    ///////////////////////////////////////////////////////////
    bool Tileset::read(const qboy::Rom &rom, UInt32 offset)
    {
        if (!rom.seek(offset))
            AME_THROW(SET_ERROR_OFFSET, rom.redirected());


        // Reads the first two properties for the following determination
        m_IsCompressed = rom.readByte();
        m_IsPrimary = rom.readByte();
        rom.readHWord(); // padding


        // Determines all differences between FRLG and RSE
        int countPal;
        int countBlock;
        int palAdjustment;
        int uncompSize;
        if (rom.info().isFRLG())
        {
            if (!m_IsPrimary) // = 0 in the games -.-
            {
                countPal = 7;
                countBlock = 640;
                palAdjustment = 0;
                uncompSize = 40960;
            }
            else
            {
                countPal = 6;
                countBlock = 384;
                palAdjustment = 224;
                uncompSize = 24576;
            }
        }
        else
        {
            if (!m_IsPrimary) // = 0 in the games -.-
            {
                countPal = 6;
                countBlock = 512;
                palAdjustment = 0;
                uncompSize = 32768;
            }
            else
            {
                countPal = 7;
                countBlock = 512;
                palAdjustment = 192;
                uncompSize = 32768;
            }
        }


        // Reads all pointers within the tileset structure
        m_PtrImage = rom.readPointer();
        m_PtrPalette = rom.readPointer();
        m_PtrBlocks = rom.readPointer();

        if (rom.info().isFRLG()) // pointers reversed
        {
            m_PtrAnimations = rom.readPointer();
            m_PtrBehaviour = rom.readPointer();
        }
        else
        {
            m_PtrBehaviour = rom.readPointer();
            m_PtrAnimations = rom.readPointer();
        }


        // Determines whether all read pointers are valid
        if (!rom.checkOffset(m_PtrImage))
            AME_THROW(SET_ERROR_IMAGE, offset + 4);
        if (!rom.checkOffset(m_PtrPalette))
            AME_THROW(SET_ERROR_PALETTE, offset + 8);
        if (!rom.checkOffset(m_PtrBlocks))
            AME_THROW(SET_ERROR_BLOCKS, offset + 12);
        if (!rom.checkOffset(m_PtrAnimations))
            AME_THROW(SET_ERROR_ANIM, offset + 16);
        if (!rom.checkOffset(m_PtrBehaviour))
            AME_THROW(SET_ERROR_PROP, offset + 20);


        // Attempts to load the image
        if (m_IsCompressed)
        {
            if (!m_Image->readCompressed(rom, m_PtrImage, 128, true))
                AME_THROW(SET_ERROR_IMGDATA, m_PtrImage);
        }
        else
        {
            if (!m_Image->readUncompressed(rom, m_PtrImage, uncompSize, 128, true))
                AME_THROW(SET_ERROR_IMGDATA, m_PtrImage);
        }

        // Attempts to load the palettes
        for (int i = 0; i < countPal; i++)
        {
            qboy::Palette *pal = new qboy::Palette;
            pal->readUncompressed(rom, m_PtrPalette + palAdjustment + i * 32, 16);
            m_Pals.push_back(pal);
        }

        // Attempts to load the blocks
        rom.seek(m_PtrBlocks);
        for (int i = 0; i < countBlock; i++)
        {
            Block *block = new Block;
            m_Blocks.push_back(block);

            // Each block has 2 layers á 4 tiles
            for (int j = 0; j < 8; j++)
            {
                Tile tile;
                UInt16 data = rom.readHWord();

                // Extracts information from the hword
                tile.tile = (data & 0x3FF);
                tile.palette = ((data & 0xF800) >> 0xC);
                tile.flipX = ((data & 0x400) == 0x400);
                tile.flipY = ((data & 0x800) == 0x800);
                block->tiles[j] = tile;
            }
        }


        // Loading successful
        m_Offset = offset;
        return true;
    }


    ///////////////////////////////////////////////////////////
    // Function type:  Getter
    // Contributers:   Pokedude
    // Last edit by:   Pokedude
    // Date of edit:   6/10/2016
    //
    ///////////////////////////////////////////////////////////
    UInt32 Tileset::offset() const
    {
        return m_Offset;
    }


    ///////////////////////////////////////////////////////////
    // Function type:  Getter
    // Contributers:   Pokedude
    // Last edit by:   Pokedude
    // Date of edit:   6/7/2016
    //
    ///////////////////////////////////////////////////////////
    qboy::Image *Tileset::image() const
    {
        return m_Image;
    }

    ///////////////////////////////////////////////////////////
    // Function type:  Getter
    // Contributers:   Pokedude
    // Last edit by:   Pokedude
    // Date of edit:   6/7/2016
    //
    ///////////////////////////////////////////////////////////
    const QList<qboy::Palette *> &Tileset::palettes() const
    {
        return m_Pals;
    }

    ///////////////////////////////////////////////////////////
    // Function type:  Getter
    // Contributers:   Pokedude
    // Last edit by:   Pokedude
    // Date of edit:   6/7/2016
    //
    ///////////////////////////////////////////////////////////
    const QList<Block *> &Tileset::blocks() const
    {
        return m_Blocks;
    }
}
