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


#ifndef __AME_PALETTEQUANTIZER_HPP__
#define __AME_PALETTEQUANTIZER_HPP__


///////////////////////////////////////////////////////////
// Include files
//
///////////////////////////////////////////////////////////
#include <QBoy/Config.hpp>
#include <QBoy/Graphics/Color.hpp>
#include <QtWidgets>


namespace ame
{
    ///////////////////////////////////////////////////////////
    /// \struct  PQImage
    /// \brief   Defines an image with quantized palette.
    ///
    ///////////////////////////////////////////////////////////
    struct PQImage
    {
        QSize size;
        quint8 *data;
        quint32 count;
        qboy::Color *pal;
        
        
        PQImage() { }
        ~PQImage()
        {
            delete[] data;
            delete[] pal;
        }
    };


    ///////////////////////////////////////////////////////////
    /// \file    PaletteQuantizer.hpp
    /// \author  Pokedude
    /// \version 1.0.0.0
    /// \date    8/24/2016
    /// \brief   
    ///
    /// Reduces images to either 16 or 256 colors and can also
    /// apply a palette to them without changing their look.
    ///
    ///////////////////////////////////////////////////////////
    class PaletteQuantizer {
    public:
        
        ///////////////////////////////////////////////////////////
        /// \brief    Reduces an image to 16 colors.
        /// \param    img QImage to convert
        /// \returns  an instance of PQImage.
        /// 
        ///////////////////////////////////////////////////////////
        static PQImage reduce16(QImage &img);
        
        ///////////////////////////////////////////////////////////
        /// \brief    Reduces an image to 256 colors.
        /// \param    img QImage to convert
        /// \returns  an instance of PQImage.
        /// 
        ///////////////////////////////////////////////////////////
        static PQImage reduce256(QImage &img);
        
        ///////////////////////////////////////////////////////////
        /// \brief   Specifies a new palette for the given PQImage.
        /// \param   img PQImage instance; will be modified
        /// \param   pal New palette to use for this PQImage
        /// 
        ///////////////////////////////////////////////////////////
        static void setPalette(PQImage &img, QColor *pal);
        
        
    private:
        
        static PQImage reducePriv(QImage &img, int colors);
    };
}


#endif // __AME_PALETTEQUANTIZER_HPP__
