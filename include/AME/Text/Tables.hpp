///////////////////////////////////////////////////////////
//
// QBoy: GameboyAdvance library
// Copyright (C) 2015-2016 Pokedude
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
///////////////////////////////////////////////////////////


#ifndef __AME_TABLES_HPP__
#define __AME_TABLES_HPP__


///////////////////////////////////////////////////////////
// Include files
//
///////////////////////////////////////////////////////////
#include <QBoy/Config.hpp>
#include <QString>
#include <QMap>


namespace ame
{
    ///////////////////////////////////////////////////////////
    /// \author Pokedude
    /// \date   04/05/2016
    /// \file   Tables.hpp
    /// \brief  Defines various tables for de- and encoding.
    ///
    /// Currently supported table types:
    /// * Single characters
    /// * Functions (no char) (FRLG/RSE)
    /// * Symbol characters
    /// * Buffer representations (FRLG/RSE)
    ///
    ///////////////////////////////////////////////////////////

    extern QMap<UInt32, QString> SingleSequences;
    extern QMap<UInt32, QString> FunctionSequencesFRLG;
    extern QMap<UInt32, QString> FunctionSequencesRS;
    extern QMap<UInt32, QString> FunctionSequencesE;
    extern QMap<UInt32, QString> SymbolSequences;
    extern QMap<UInt32, QString> BufferSequencesFRLG;
    extern QMap<UInt32, QString> BufferSequencesRSE;
}


#endif  // __AME_TABLES_HPP__
