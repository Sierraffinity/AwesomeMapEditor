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


#ifndef __AME_ERRORSTACK_HPP__
#define __AME_ERRORSTACK_HPP__


///////////////////////////////////////////////////////////
// Include files
//
///////////////////////////////////////////////////////////
#include <QBoy/Config.hpp>
#include <QStringList>


namespace ame
{
    ///////////////////////////////////////////////////////////
    /// \file    ErrorStack.hpp
    /// \author  Pokedude
    /// \version 1.0.0.0
    /// \date    6/2/2016
    /// \brief   Stores errors thrown during loading/writing.
    ///
    /// This class is capable of tracking errors, storing them
    /// in a list and retrieving them alltogether to display
    /// them in an error box eventually. Can also generate a 
    /// log file that can be shared across the community.
    ///
    ///////////////////////////////////////////////////////////
    class ErrorStack {
    public:

        ///////////////////////////////////////////////////////////
        /// \brief Puts an error on the stack.
        ///
        /// When using the overload and passing a non-zero argument
        /// to it, the string defined in 'error' will be converted
        /// so that it contains the offset afterwards.
        ///
        /// \param error Raw or template error string
        /// \param offset If not zero, converts template string
        /// \returns always false in order to maintain code flow.
        ///
        ///////////////////////////////////////////////////////////
        static bool add(QString method, QString error, UInt32 offset = 0);

        ///////////////////////////////////////////////////////////
        /// \brief Retrieves all errors on the stack.
        ///
        ///////////////////////////////////////////////////////////
        static const QStringList &errors();

        ///////////////////////////////////////////////////////////
        /// \brief Retrieves all erroneous methods (with class).
        ///
        ///////////////////////////////////////////////////////////
        static const QStringList &methods();

        ///////////////////////////////////////////////////////////
        /// \brief Puts the errors into one, copyable log string.
        ///
        ///////////////////////////////////////////////////////////
        static const QString log();

        ///////////////////////////////////////////////////////////
        /// \brief Clears the error log.
        ///
        ///////////////////////////////////////////////////////////
        static void clear();
    };


    #define AME_THROW(err, off)         { rom.clearCache(); return ErrorStack::add(Q_FUNC_INFO, err, off); }
    #define AME_THROW2(err)             { rom.clearCache(); return ErrorStack::add(Q_FUNC_INFO, err); }
    #define AME_SILENT_THROW(err, off)  { ErrorStack::add(Q_FUNC_INFO, err, off); }
    #define AME_SILENT_THROW2(err)      { ErrorStack::add(Q_FUNC_INFO, err); }
}


#endif // __AME_ERRORSTACK_HPP__
