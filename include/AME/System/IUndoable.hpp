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
// Copyright (C) 2016 diegoisawesome, Pokedude
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


#ifndef __AME_IUNDOABLE_HPP__
#define __AME_IUNDOABLE_HPP__


namespace ame
{
    ///////////////////////////////////////////////////////////
    /// \file    IUndoable.hpp
    /// \author  Pokedude
    /// \version 1.0.0.0
    /// \date    6/2/2016
    /// \brief   Provides an interface to undo/redo actions.
    ///
    /// Per map, a list of a list of write entries is stored.
    /// This enables easy undo- and redoing without changing
    /// the ROM's data constantly, thus preventing fragmentation.
    /// Write entries consist of an offset and the data itself.
    ///
    ///////////////////////////////////////////////////////////
    class IUndoable {
    public:

        ///////////////////////////////////////////////////////////
        /// \brief Undoes the most recent action.
        ///
        /// After undoing, removes this action from the undo-stack.
        /// It can then be redone by calling IUndoable::redo.
        ///
        ///////////////////////////////////////////////////////////
        virtual void undo() = 0;

        ///////////////////////////////////////////////////////////
        /// \brief Redoes the most recently undone action.
        ///
        /// After redoing, removes this action from the redo-stack.
        /// It can then be undone by calling IUndoable::undo.
        ///
        ///////////////////////////////////////////////////////////
        virtual void redo() = 0;
    };


    ///////////////////////////////////////////////////////////
    /// \brief Defines the most common undo/redo actions.
    ///
    /// By using actions, huge storage lists of previous states
    /// are avoided and performance is improved as side effect.
    ///
    ///////////////////////////////////////////////////////////
    enum UndoRedoAction
    {
        URA_ACTION_ADD      = 0,    ///< e.g. Table entry added
        URA_ACTION_REMOVE   = 1,    ///< e.g. Table entry removed
        URA_ACTION_MODIFY   = 2,    ///< e.g. Map block modified
        URA_ACTION_CLEAR    = 3,    ///< e.g. Entire table cleared
        URA_ACTION_WRITE    = 4,    ///< e.g. Writing process to file
        URA_ACTION_DELETE   = 5     ///< e.g. Deleting a map, tileset, etc
    };

}


#endif // __AME_IUNDOABLE_HPP__
