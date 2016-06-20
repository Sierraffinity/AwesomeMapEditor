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
#include <AME/Graphics/PropertyTable.hpp>


namespace ame
{
    ///////////////////////////////////////////////////////////
    // Function type:  Constructor
    // Contributors:   Pokedude
    // Last edit by:   Pokedude
    // Date of edit:   6/20/2016
    //
    ///////////////////////////////////////////////////////////
    PropertyTable::PropertyTable()
    {
    }

    ///////////////////////////////////////////////////////////
    // Function type:  Constructor
    // Contributors:   Pokedude
    // Last edit by:   Pokedude
    // Date of edit:   6/20/2016
    //
    ///////////////////////////////////////////////////////////
    PropertyTable::PropertyTable(const PropertyTable &rvalue)
        : m_Properties(rvalue.m_Properties)
    {
    }

    ///////////////////////////////////////////////////////////
    // Function type:  Constructor
    // Contributors:   Pokedude
    // Last edit by:   Pokedude
    // Date of edit:   6/20/2016
    //
    ///////////////////////////////////////////////////////////
    PropertyTable &PropertyTable::operator=(const PropertyTable &rvalue)
    {
        m_Properties = rvalue.m_Properties;
        return *this;
    }

    ///////////////////////////////////////////////////////////
    // Function type:  Destructor
    // Contributors:   Pokedude
    // Last edit by:   Pokedude
    // Date of edit:   6/20/2016
    //
    ///////////////////////////////////////////////////////////
    PropertyTable::~PropertyTable()
    {
        foreach (Property *prop, m_Properties)
            delete prop;

        m_Properties.clear();
    }


    ///////////////////////////////////////////////////////////
    // Function type:  I/O
    // Contributors:   Pokedude
    // Last edit by:   Pokedude
    // Date of edit:   6/20/2016
    //
    ///////////////////////////////////////////////////////////
    bool PropertyTable::read(const qboy::Rom &rom, UInt32 offset, Int32 blocks)
    {
        if (!rom.seek(offset))
            AME_THROW(PPT_ERROR_OFFSET, rom.redirected());


        // Reads all properties
        for (int i = 0; i < blocks; i++)
        {
            Property *prop = new Property;
            if (!rom.info().isFRLG())
            {
                prop->behaviour = rom.readByte();
                prop->background = rom.readByte();
            }
            else
            {
                prop->behaviour = rom.readHWord();  // Lower -> behaviour
                prop->background = rom.readHWord(); // Upper -> background
            }

            // Adds the property
            m_Properties.push_back(prop);
        }


        m_Offset = offset;
        return true;
    }


    ///////////////////////////////////////////////////////////
    // Function type:  Getter
    // Contributors:   Pokedude
    // Last edit by:   Pokedude
    // Date of edit:   6/20/2016
    //
    ///////////////////////////////////////////////////////////
    const QList<Property *> &PropertyTable::properties() const
    {
        return m_Properties;
    }
}
