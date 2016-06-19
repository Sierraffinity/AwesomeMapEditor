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
#include <QBoy/Text/String.hpp>
#include <AME/Structures/StructureErrors.hpp>
#include <AME/Structures/PokemonTable.hpp>
#include <AME/System/Configuration.hpp>
#include <AME/Widgets/Misc/Messages.hpp>


namespace ame
{
    ///////////////////////////////////////////////////////////
    // Function type:  Constructor
    // Contributors:   Pokedude
    // Last edit by:   Pokedude
    // Date of edit:   6/19/2016
    //
    ///////////////////////////////////////////////////////////
    PokemonTable::PokemonTable()
    {
    }

    ///////////////////////////////////////////////////////////
    // Function type:  Constructor
    // Contributors:   Pokedude
    // Last edit by:   Pokedude
    // Date of edit:   6/19/2016
    //
    ///////////////////////////////////////////////////////////
    PokemonTable::PokemonTable(const PokemonTable &rvalue)
        : m_Names(rvalue.m_Names),
          m_Images(rvalue.m_Images)
    {
    }

    ///////////////////////////////////////////////////////////
    // Function type:  Constructor
    // Contributors:   Pokedude
    // Last edit by:   Pokedude
    // Date of edit:   6/19/2016
    //
    ///////////////////////////////////////////////////////////
    PokemonTable &PokemonTable::operator=(const PokemonTable &rvalue)
    {
        m_Names = rvalue.m_Names;
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
    PokemonTable::~PokemonTable()
    {
        m_Names.clear();
        m_Images.clear();
    }


    ///////////////////////////////////////////////////////////
    // Function type:  I/O
    // Contributors:   Pokedude
    // Last edit by:   Pokedude
    // Date of edit:   6/19/2016
    //
    ///////////////////////////////////////////////////////////
    bool PokemonTable::read(const qboy::Rom &rom)
    {
        // Checks all the configuration pointers
        if (!rom.checkOffset(CONFIG(PokemonNames)))
            AME_THROW2(PKM_ERROR_NAMES);
        if (!rom.checkOffset(CONFIG(PokemonIcons)))
            AME_THROW2(PKM_ERROR_ICONS);
        if (!rom.checkOffset(CONFIG(PokemonPals)))
            AME_THROW2(PKM_ERROR_PALS);
        if (!rom.checkOffset(CONFIG(PokemonUsage)))
            AME_THROW2(PKM_ERROR_USAGE);


        // Attempts to read all the icon images
        for (unsigned i = 0; i < CONFIG(PokemonCount); i++)
        {
            if (!rom.seek(CONFIG(PokemonIcons) + i * 4))
                AME_THROW2(PKM_ERROR_ICONS);

            unsigned ptrImage = rom.readPointerRef();
            if (!rom.checkOffset(ptrImage))
                AME_THROW(PKM_ERROR_IMAGE, rom.redirected());

            m_Images.push_back(QImage(32, 32, QImage::Format_Indexed8));
            QImage &current = m_Images[i];
            qboy::Image image;

            // Decodes the image
            image.readUncompressed(rom, ptrImage, 512, 32, true);
            memcpy(current.bits(), image.raw().data(), 1024);
        }


        // Attempts to read all the palettes
        QList<QVector<QRgb>> colorTables;
        for (int i = 0; i < 3; i++)
        {
            qboy::Palette palette;
            palette.readUncompressed(rom, CONFIG(PokemonPals) + i * 32, 16);
            colorTables.push_back(QVector<QRgb>());

            // Converts the palette to a color table
            for (int j = 0; j < 16; j++)
            {
                const qboy::Color &color = palette.raw().at(j);
                colorTables[i].push_back(QColor::fromRgb(color.r, color.g, color.b).rgb());
            }
        }


        // Attempts to read all the palette usages
        for (unsigned i = 0; i < CONFIG(PokemonCount); i++)
        {
            QImage &current = m_Images[i];
            UInt8 usage;

            if (!rom.seek(CONFIG(PokemonUsage) + i))
                AME_THROW2(PKM_ERROR_USAGE);

            if ((usage = rom.readByte()) > 2)
                AME_THROW(PKM_ERROR_ENTRY, rom.offset()-1);

            // Stores the color table in the current QImage
            current.setColorTable(colorTables.at(usage));
        }


        // Attempts to read all the pokemon names
        for (unsigned i = 0; i < CONFIG(PokemonCount); i++)
        {
            m_Names.push_back(qboy::String::read(rom, CONFIG(PokemonNames) + i * 11));
        }

        // Loading successful
        return true;
    }


    ///////////////////////////////////////////////////////////
    // Function type:  Getter
    // Contributors:   Pokedude
    // Last edit by:   Pokedude
    // Date of edit:   6/19/2016
    //
    ///////////////////////////////////////////////////////////
    const QList<QImage> &PokemonTable::images() const
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
    const QStringList &PokemonTable::names() const
    {
        return m_Names;
    }
}
