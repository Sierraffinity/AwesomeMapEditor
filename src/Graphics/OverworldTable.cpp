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
#include <AME/Graphics/OverworldTable.hpp>
#include <AME/System/Configuration.hpp>


namespace ame
{
    ///////////////////////////////////////////////////////////
    // Function type:  Constructor
    // Contributors:   Pokedude
    // Last edit by:   Pokedude
    // Date of edit:   6/19/2016
    //
    ///////////////////////////////////////////////////////////
    OverworldTable::OverworldTable()
    {
    }

    ///////////////////////////////////////////////////////////
    // Function type:  Constructor
    // Contributors:   Pokedude
    // Last edit by:   Pokedude
    // Date of edit:   6/19/2016
    //
    ///////////////////////////////////////////////////////////
    OverworldTable::OverworldTable(const OverworldTable &rvalue)
        : m_Images(rvalue.m_Images)
    {
    }

    ///////////////////////////////////////////////////////////
    // Function type:  Constructor
    // Contributors:   Pokedude
    // Last edit by:   Pokedude
    // Date of edit:   6/19/2016
    //
    ///////////////////////////////////////////////////////////
    OverworldTable &OverworldTable::operator=(const OverworldTable &rvalue)
    {
        m_Images = rvalue.m_Images;
        return *this;
    }

    ///////////////////////////////////////////////////////////
    // Function type:  Destructor
    // Contributors:   Pokedude
    // Last edit by:   Pokedude
    // Date of edit:   6/19/2016
    //
    ///////////////////////////////////////////////////////////
    OverworldTable::~OverworldTable()
    {
        m_Palettes = m_Palettes.toSet().toList();
        m_Images = m_Images.toSet().toList();
        
        foreach (qboy::Palette *pal, m_Palettes)
            delete pal;
        
        foreach (qboy::Image *img, m_Images)
            delete img;
        
        m_Palettes.clear();
        m_Images.clear();
    }


    ///////////////////////////////////////////////////////////
    // Function type:  I/O
    // Contributors:   Pokedude
    // Last edit by:   Pokedude
    // Date of edit:   6/19/2016
    //
    ///////////////////////////////////////////////////////////
    bool OverworldTable::read(const qboy::Rom &rom)
    {
        // Attempts to load all the overworld palettes
        QMap<UInt16, qboy::Palette *> paletteMap;
        bool reachedLimiter = false;
        for (int i = 0; !reachedLimiter; i++)
        {
            if (!rom.seek(CONFIG(OverworldPals) + i * 8))
                AME_THROW2(OWT_ERROR_OFFSET);


            // Reads the palette pointer
            unsigned ptrPal = rom.readPointerRef();
            if (!rom.checkOffset(ptrPal))
                AME_THROW(OWT_ERROR_PALETTE, rom.redirected());

            // Reads the palette index
            unsigned short idxPal = rom.readHWord();
            rom.readBytes(2); // unimportant data

            // Determines whether this is the last palette entry
            if (rom.readWord() == 0x0 && rom.readWord() == 0x0)
                reachedLimiter = true;

            // Loads the palette
            qboy::Palette *palette = new qboy::Palette;
            palette->readUncompressed(rom, ptrPal, 16);

            // Adds the necessary values to the map
            paletteMap.insert(idxPal, palette);
        }


        // Attempts to load the overworld images
        for (unsigned i = 0; i < CONFIG(OverworldCount); i++)
        {
            if (!rom.seek(CONFIG(Overworlds) + i * 4))
                AME_THROW2(OWT_ERROR_OFFSET);

            // Reads the OW structure pointer
            unsigned ptrStruct = rom.readPointerRef();
            if (!rom.seek(ptrStruct + 2))
                AME_THROW(OWT_ERROR_STRUCT, rom.redirected());


            // Reads the palette and the sprite sizes
            UInt16 idxPal = rom.readHWord();
            rom.readWord(); // unimportant
            UInt16 width = rom.readHWord();  // in pixels
            UInt16 height = rom.readHWord(); // in pixels
            rom.readBytes(16); // unimportant
            UInt32 ptrFrames = rom.readPointerRef();

            if (!rom.seek(ptrFrames))
                AME_THROW(OWT_ERROR_FRAMES, rom.redirected());
            if (!paletteMap.contains(idxPal))
                AME_THROW(OWT_ERROR_INDEX, idxPal);

            unsigned ptrImage = rom.readPointerRef();
            if (!rom.checkOffset(ptrImage))
                AME_THROW(OWT_ERROR_SPRITE, rom.redirected());

            // Reads the actual image
            qboy::Image *image = new qboy::Image;
            image->readUncompressed(rom, ptrImage, width*height/2, width, true);

            m_Images.push_back(image);
            m_Palettes.push_back(paletteMap.value(idxPal));
        }


        return true;
    }


    ///////////////////////////////////////////////////////////
    // Function type:  Getter
    // Contributors:   Pokedude
    // Last edit by:   Pokedude
    // Date of edit:   6/19/2016
    //
    ///////////////////////////////////////////////////////////
    const QList<qboy::Image *> &OverworldTable::images() const
    {
        return m_Images;
    }

    ///////////////////////////////////////////////////////////
    // Function type:  Getter
    // Contributors:   Pokedude
    // Last edit by:   Pokedude
    // Date of edit:   6/19/2016
    //
    ///////////////////////////////////////////////////////////
    const QList<qboy::Palette *> &OverworldTable::palettes() const
    {
        return m_Palettes;
    }
}
