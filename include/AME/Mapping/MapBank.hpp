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


#ifndef __AME_MAPBANK_HPP__
#define __AME_MAPBANK_HPP__


///////////////////////////////////////////////////////////
// Include files
//
///////////////////////////////////////////////////////////
#include <AME/Mapping/Map.hpp>
#include <AME/Mapping/MappingErrors.hpp>


namespace ame
{
   ///////////////////////////////////////////////////////////
   /// \file    MapBank.hpp
   /// \author  Pokedude
   /// \version 1.0.0.0
   /// \date    6/15/2016
   /// \brief   Contains multiple maps.
   ///
   /// The game does not care how many maps one bank contains,
   /// it just crashes if an invalid one was specified. There
   /// are a few workarounds to determine the map count, though.
   ///
   ///////////////////////////////////////////////////////////
   class MapBank /* : IUndoable */ {
   public:

       ///////////////////////////////////////////////////////////
       /// \brief Default constructor
       ///
       /// Initializes a new instance of ame::MapBank.
       /// Note: MapBank is designed to work with stack
       /// objects, but it is recommended to allocate instances
       /// on the global heap, because of the undo/redo system.
       ///
       ///////////////////////////////////////////////////////////
       MapBank();

       ///////////////////////////////////////////////////////////
       /// \brief Copy constructor
       ///
       /// Copies all members of the given ame::MapBank.
       /// Is only called by template code, not by actual AME code.
       ///
       ///////////////////////////////////////////////////////////
       MapBank(const MapBank &rvalue);

       ///////////////////////////////////////////////////////////
       /// \brief Assignment constructor
       ///
       /// Copies all members of the given ame::MapBank
       /// and stores them in a new class instance.
       ///
       ///////////////////////////////////////////////////////////
       MapBank &operator=(const MapBank &rvalue);

       ///////////////////////////////////////////////////////////
       /// \brief Destructor
       ///
       /// Deletes all the allocated map objects.
       ///
       ///////////////////////////////////////////////////////////
       ~MapBank();


       ///////////////////////////////////////////////////////////
       /// \brief Attempts to read the entire bank.
       ///
       /// Retrieves the amount of maps through an algorithm and
       /// attempts to load them all.
       ///
       /// \param rom Currently opened ROM file
       /// \param offset Offset of the current bank
       /// \param next Offset of the next bank
       /// \returns true if the map was read correctly.
       ///
       ///////////////////////////////////////////////////////////
       bool read(const qboy::Rom &rom, UInt32 offset, UInt32 next);


       ///////////////////////////////////////////////////////////
       /// \brief Retrieves the offset of the bank.
       ///
       ///////////////////////////////////////////////////////////
       UInt32 offset() const;

       ///////////////////////////////////////////////////////////
       /// \brief Retrieves a constant reference to all maps.
       ///
       ///////////////////////////////////////////////////////////
       const QList<Map *> &maps() const;


   private:

       ///////////////////////////////////////////////////////////
       // Class members
       //
       ///////////////////////////////////////////////////////////
       UInt32 m_Offset;         ///< Offset of the map-bank
       UInt32 m_Count;          ///< Amount of maps in the bank
       QList<Map *> m_Maps;     ///< Holds the actual maps
   };
}


#endif // __AME_MAPBANK_HPP__
