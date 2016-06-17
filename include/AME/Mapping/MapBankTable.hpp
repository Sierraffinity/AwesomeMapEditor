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


#ifndef __AME_MAPBANKTABLE_HPP__
#define __AME_MAPBANKTABLE_HPP__


///////////////////////////////////////////////////////////
// Include files
//
///////////////////////////////////////////////////////////
#include <AME/Mapping/MapBank.hpp>
#include <AME/Mapping/MappingErrors.hpp>


namespace ame
{
   ///////////////////////////////////////////////////////////
   /// \file    MapBankTable.hpp
   /// \author  Pokedude
   /// \version 1.0.0.0
   /// \date    6/15/2016
   /// \brief   Contains multiple banks.
   ///
   /// The game again does not care how many banks exist,
   /// therefore we need another counting algorithm.
   ///
   ///////////////////////////////////////////////////////////
   class MapBankTable /* : IUndoable */ {
   public:

       ///////////////////////////////////////////////////////////
       /// \brief Default constructor
       ///
       /// Initializes a new instance of ame::MapBankTable.
       /// Note: MapBankTable is designed to work with stack
       /// objects, but it is recommended to allocate instances
       /// on the global heap, because of the undo/redo system.
       ///
       ///////////////////////////////////////////////////////////
       MapBankTable();

       ///////////////////////////////////////////////////////////
       /// \brief Copy constructor
       ///
       /// Copies all members of the given ame::MapBankTable.
       /// Is only called by template code, not by actual AME code.
       ///
       ///////////////////////////////////////////////////////////
       MapBankTable(const MapBankTable &rvalue);

       ///////////////////////////////////////////////////////////
       /// \brief Assignment constructor
       ///
       /// Copies all members of the given ame::MapBankTable
       /// and stores them in a new class instance.
       ///
       ///////////////////////////////////////////////////////////
       MapBankTable &operator=(const MapBankTable &rvalue);

       ///////////////////////////////////////////////////////////
       /// \brief Destructor
       ///
       /// Deletes all the allocated bank objects.
       ///
       ///////////////////////////////////////////////////////////
       ~MapBankTable();


       ///////////////////////////////////////////////////////////
       /// \brief Attempts to read all map banks.
       ///
       /// Reads all banks located at the given offset.
       ///
       /// \param rom Currently opened ROM file
       /// \param offset Offset of the bank table
       /// \returns true if the map was read correctly.
       ///
       ///////////////////////////////////////////////////////////
       bool read(const qboy::Rom &rom, UInt32 offset);


       ///////////////////////////////////////////////////////////
       /// \brief Retrieves the offset of the bank.
       ///
       ///////////////////////////////////////////////////////////
       UInt32 offset() const;

       ///////////////////////////////////////////////////////////
       /// \brief Retrieves a constant reference to all maps.
       ///
       ///////////////////////////////////////////////////////////
       const QList<MapBank *> &banks() const;


   private:

       ///////////////////////////////////////////////////////////
       // Class members
       //
       ///////////////////////////////////////////////////////////
       UInt32 m_Offset;          ///< Offset of the bank table
       Int32 m_Count;           ///< Amount of banks in the table
       QList<MapBank *> m_Banks; ///< Holds all the banks
   };
}


#endif // __AME_MAPBANKTABLE_HPP__
