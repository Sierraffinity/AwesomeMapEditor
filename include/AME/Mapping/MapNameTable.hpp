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


#ifndef __AME_MAPNAMETABLE_HPP__
#define __AME_MAPNAMETABLE_HPP__


///////////////////////////////////////////////////////////
// Include files
//
///////////////////////////////////////////////////////////
#include <AME/Mapping/MapName.hpp>


namespace ame
{
   ///////////////////////////////////////////////////////////
   /// \file    MapNameTable.hpp
   /// \author  Diegoisawesome
   /// \version 1.0.0.0
   /// \date    7/1/2016
   /// \brief   Contains multiple map names.
   ///
   /// A class to store the table to the map names.
   ///
   ///////////////////////////////////////////////////////////
   class MapNameTable /* : IUndoable */ {
   public:

       ///////////////////////////////////////////////////////////
       /// \brief Default constructor
       ///
       /// Initializes a new instance of ame::MapNameTable.
       /// Note: MapNameTable is designed to work with stack
       /// objects, but it is recommended to allocate instances
       /// on the global heap, because of the undo/redo system.
       ///
       ///////////////////////////////////////////////////////////
       MapNameTable();

       ///////////////////////////////////////////////////////////
       /// \brief Copy constructor
       ///
       /// Copies all members of the given ame::MapNameTable.
       /// Is only called by template code, not by actual AME code.
       ///
       ///////////////////////////////////////////////////////////
       MapNameTable(const MapNameTable &rvalue);

       ///////////////////////////////////////////////////////////
       /// \brief Assignment constructor
       ///
       /// Copies all members of the given ame::MapNameTable
       /// and stores them in a new class instance.
       ///
       ///////////////////////////////////////////////////////////
       MapNameTable &operator=(const MapNameTable &rvalue);

       ///////////////////////////////////////////////////////////
       /// \brief Destructor
       ///
       /// Deletes all the allocated name objects.
       ///
       ///////////////////////////////////////////////////////////
       ~MapNameTable();


       ///////////////////////////////////////////////////////////
       /// \brief Attempts to read all map names.
       ///
       /// Reads all names located at the given offset.
       ///
       /// \param rom Currently opened ROM file
       /// \param offset Offset of the name table
       /// \returns true if the names were read correctly.
       ///
       ///////////////////////////////////////////////////////////
       bool read(const qboy::Rom &rom, UInt32 offset);


       ///////////////////////////////////////////////////////////
       /// \brief Retrieves the offset of the names.
       ///
       ///////////////////////////////////////////////////////////
       UInt32 offset() const;

       ///////////////////////////////////////////////////////////
       /// \brief Retrieves a constant reference to all map names.
       ///
       ///////////////////////////////////////////////////////////
       const QList<MapName *> &names() const;


   private:

       ///////////////////////////////////////////////////////////
       // Class members
       //
       ///////////////////////////////////////////////////////////
       UInt32 m_Offset;          ///< Offset of the name table
       QList<MapName *> m_Names; ///< Holds all the banks
   };
}


#endif // __AME_MAPNAMETABLE_HPP__
