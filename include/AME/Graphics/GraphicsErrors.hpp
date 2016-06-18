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


#ifndef __AME_GRAPHICSERRORS_HPP__
#define __AME_GRAPHICSERRORS_HPP__


///////////////////////////////////////////////////////////
// Include files
//
///////////////////////////////////////////////////////////
#include <AME/System/ErrorStack.hpp>


namespace ame
{
    ///////////////////////////////////////////////////////////
    /// \file    GraphicsErrors.hpp
    /// \author  Pokedude
    /// \version 1.0.0.0
    /// \date    6/7/2016
    /// \brief   Defines all errors for 'Graphics' classes.
    ///
    ///////////////////////////////////////////////////////////


    ///////////////////////////////////////////////////////////
    // Class: Tileset
    //
    ///////////////////////////////////////////////////////////
    #define SET_ERROR_OFFSET    "The location of the tileset (ref: 0x%offset%) \nPlease make sure that your pointer at the\ngiven offset is valid and contains valid data."
    #define SET_ERROR_IMAGE     "The location of the tileset image (ref: 0x%offset%) \nPlease make sure that your pointer at the\ngiven offset is valid and contains valid data."
    #define SET_ERROR_PALETTE   "The location of the tileset palettes (ref: 0x%offset%) \nPlease make sure that your pointer at the\ngiven offset is valid and contains valid data."
    #define SET_ERROR_BLOCKS    "The location of the tileset blocks (ref: 0x%offset%) \nPlease make sure that your pointer at the\ngiven offset is valid and contains valid data."
    #define SET_ERROR_ANIM      "The location of the tileset animations (ref: 0x%offset%) \nPlease make sure that your pointer at the\ngiven offset is valid and contains valid data."
    #define SET_ERROR_PROP      "The location of the tileset properties (ref: 0x%offset%) \nPlease make sure that your pointer at the\ngiven offset is valid and contains valid data."
    #define SET_ERROR_IMGDATA   "The image data of this tileset (loc: 0x%offset%) is broken.\nMake sure that the image at the given offset\nconsists of valid LZ77-compressed data."
}


#endif // __AME_GRAPHICSERRORS_HPP__
