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


#ifndef __AME_MAPLAYOUTTABLE_HPP__
#define __AME_MAPLAYOUTTABLE_HPP__


///////////////////////////////////////////////////////////
// Include files
//
///////////////////////////////////////////////////////////
#include <AME/Mapping/Map.hpp>
#include <AME/Mapping/MappingErrors.hpp>


namespace ame
{
   ///////////////////////////////////////////////////////////
   /// \file    MapLayoutTable.hpp
   /// \author  Diegoisawesome
   /// \version 1.0.0.0
   /// \date    7/3/2016
   /// \brief   Contains multiple map layouts.
   ///
   /// The game does not care how many map layouts exist,
   /// it just crashes if an invalid one was specified. There
   /// are a few workarounds to determine the layout count, though.
   ///
   ///////////////////////////////////////////////////////////
   class MapLayoutTable /* : IUndoable */ {
   public:

       ///////////////////////////////////////////////////////////
       /// \brief Default constructor
       ///
       /// Initializes a new instance of ame::MapLayoutTable.
       /// Note: MapLayoutTable is designed to work with stack
       /// objects, but it is recommended to allocate instances
       /// on the global heap, because of the undo/redo system.
       ///
       ///////////////////////////////////////////////////////////
       MapLayoutTable();

       ///////////////////////////////////////////////////////////
       /// \brief Copy constructor
       ///
       /// Copies all members of the given ame::MapLayoutTable.
       /// Is only called by template code, not by actual AME code.
       ///
       ///////////////////////////////////////////////////////////
       MapLayoutTable(const MapLayoutTable &rvalue);

       ///////////////////////////////////////////////////////////
       /// \brief Assignment constructor
       ///
       /// Copies all members of the given ame::MapLayoutTable
       /// and stores them in a new class instance.
       ///
       ///////////////////////////////////////////////////////////
       MapLayoutTable &operator=(const MapLayoutTable &rvalue);

       ///////////////////////////////////////////////////////////
       /// \brief Destructor
       ///
       /// Deletes all the allocated layout header objects.
       ///
       ///////////////////////////////////////////////////////////
       ~MapLayoutTable();


       ///////////////////////////////////////////////////////////
       /// \brief Attempts to read the entire layout table.
       ///
       /// Retrieves the amount of layouts through an algorithm and
       /// attempts to load them all.
       ///
       /// \param rom Currently opened ROM file
       /// \param offset Offset of the layout table
       /// \returns true if the layouts were read correctly.
       ///
       ///////////////////////////////////////////////////////////
       bool read(const qboy::Rom &rom, UInt32 offset);


       ///////////////////////////////////////////////////////////
       /// \brief Retrieves the offset of the layout table.
       ///
       ///////////////////////////////////////////////////////////
       UInt32 offset() const;

       ///////////////////////////////////////////////////////////
       /// \brief Retrieves a constant reference to all layouts.
       ///
       ///////////////////////////////////////////////////////////
       const QList<MapHeader *> &mapHeaders() const;

       ///////////////////////////////////////////////////////////
       /// \brief Retrieves the amount of layouts in the layout table.
       ///
       ///////////////////////////////////////////////////////////
       UInt32 count() const;



   private:

       ///////////////////////////////////////////////////////////
       // Class members
       //
       ///////////////////////////////////////////////////////////
       UInt32 m_Offset;         ///< Offset of the map-bank
       Int32 m_Count;           ///< Amount of map layouts
       QList<MapHeader *> m_MapHeaders;     ///< Holds the actual maps
   };
}


#endif // __AME_MAPLAYOUTTABLE_HPP__
