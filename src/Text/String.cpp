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
#include <AME/Text/String.hpp>
#include <AME/Text/Tables.hpp>
#include <AME/System/Configuration.hpp>
#include <QMap>


namespace ame
{
    ///////////////////////////////////////////////////////////
    // Helper functions
    //
    ///////////////////////////////////////////////////////////
    const QString convertRaw(UInt32 number)
    {
        // Prepares the conversion
        QString rawString("{raw:{0}}");
        QString replace = QString::number(number, 16);

        // Performs the conversion
        return QString(rawString.replace("{0}", replace.toUpper()));
    }


    ///////////////////////////////////////////////////////////
    // Member functions
    //
    ///////////////////////////////////////////////////////////
    const QString String::read(const qboy::Rom &rom, UInt32 offset)
    {
        // Firstly, determines whether the given rom is valid
        Q_ASSERT(rom.info().isLoaded() && rom.info().isValid());

        // Declares needed variables for the decoding process
        QList<UInt8> encoded;
        QString decoded;
        UInt8 readByte;

        // Declares the different dynamic tables (some are the same across all roms)
        QMap<UInt32, QString> *mapBuffers = NULL;
        QMap<UInt32, QString> *mapFunctions = NULL;

        // Determines the rom version and depending on that, loads the tables
        if (CONFIG(RomType) == RT_FRLG)
        {
            mapBuffers = &BufferSequencesFRLG;
            mapFunctions = &FunctionSequencesFRLG;
        }
        else if (CONFIG(RomType) == RT_RS)
        {
            mapBuffers = &BufferSequencesRSE;
            mapFunctions = &FunctionSequencesRS;
        }
        else
        {
            mapBuffers = &BufferSequencesRSE;
            mapFunctions = &FunctionSequencesE;
        }


        // Reads the whole Pokémon string, terminated by 0xFF
        Q_ASSERT(rom.seek(offset));
        while ((readByte = rom.readByte()) != 0xFF)
            encoded.push_back(readByte);

        // Iterates through every encoded character and interprets it
        int length = encoded.size();
        for (int i = 0; i < length;)
        {
            // Fetches the char at the current position
            UInt8 currentChar = encoded.at(i++);
            if (currentChar == 0xF8 || currentChar == 0xF9)
            {
                // Character might be a symbol
                UInt8 arg1 = encoded[i++];
                UInt32 search = ((currentChar << 8) | arg1);

                // Searches for the sequence in the symbol-map
                auto searchResult = SymbolSequences.find(search);
                if (searchResult != SymbolSequences.end())
                    decoded.push_back(searchResult.value());
                else
                    decoded.push_back(convertRaw(search));
            }
            else if (currentChar == 0xFD)
            {
                // Character might be a buffer
                UInt8 arg1 = encoded.at(i++);
                UInt32 search = ((currentChar << 8) | arg1);

                // Searches for the sequence in the dynamic buffer-map
                auto searchResult = mapBuffers->find(search);
                if (searchResult != mapBuffers->end())
                    decoded.push_back(searchResult.value());
                else
                    decoded.push_back(convertRaw(search));
            }
            else if (currentChar == 0xFC)
            {
                // Character might be an escape sequence
                UInt8 arg1 = encoded.at(i++);
                UInt32 search = 0;
                if (arg1 >= 1 && arg1 <= 6)
                {
                    // Might be a multi-byte function
                    UInt8 arg2 = encoded[i++];
                    search = ((currentChar << 16) | (arg1 << 8) | arg2);
                }
                else
                {
                    // Might be a single-byte function
                    search = ((currentChar << 8) | arg1);
                }

                // Searches for the sequence in the dynamic escape-map
                auto searchResult = mapFunctions->find(search);
                if (searchResult != mapFunctions->end())
                    decoded.push_back(searchResult.value());
                else
                    decoded.push_back(convertRaw(search));
            }
            else
            {
                // Is a single character for sure (exception-safe!)
                decoded.push_back(SingleSequences.value(currentChar));
            }
        }

        // Finished
        return decoded;
    }
}
