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
#include <AME/Algorithm/PaletteQuantizer.hpp>


namespace ame
{
    #define SIMILARITY_TOLERANCE_INTERVAL 10


    ///////////////////////////////////////////////////////////
    inline bool AME_PQ_IsSimilar(int color1, int color2, int level);
    inline int  AME_PQ_ColorDiff(int color1, int color2);


    ///////////////////////////////////////////////////////////
    // Function type:  Static
    // Contributors:   Pokedude
    // Last edit by:   Pokedude
    // Date of edit:   8/24/2016
    //
    ///////////////////////////////////////////////////////////
    PQImage PaletteQuantizer::reduce16(QImage &img)
    {
        return reducePriv(img, 16);
    }
    
    ///////////////////////////////////////////////////////////
    // Function type:  Static
    // Contributors:   Pokedude
    // Last edit by:   Pokedude
    // Date of edit:   8/24/2016
    //
    ///////////////////////////////////////////////////////////
    PQImage PaletteQuantizer::reduce256(QImage &img)
    {
        return reducePriv(img, 256);
    }
    
    ///////////////////////////////////////////////////////////
    // Function type:  Static
    // Contributors:   Pokedude
    // Last edit by:   Pokedude
    // Date of edit:   8/24/2016
    //
    ///////////////////////////////////////////////////////////
    PQImage PaletteQuantizer::reducePriv(QImage &img, int colors)
    {
        PQImage out;
        qboy::Color *outPal = new qboy::Color[colors];
        quint8 *outDat = new quint8[img.size().width()*img.size().height()];
        
        memset(outDat, 0, img.size().width()*img.size().height());
        out.count = colors;
        out.size = img.size();
        out.data = outDat;
        out.pal = outPal;
        
        
        // Converts the image to RGB8
        QList<int> rawColors;
        int bCount = img.size().width()*img.size().height();
        
        // Step 1: Counts the amount of colors of the raw image    
        for (int i = 0; i < bCount; i++)
        {
            int col = *(int*)(img.bits() + i*4);
            if (!rawColors.contains(col))
                rawColors.push_back(col);
        }
            
        
        // Step 2: Try to eliminate similar colors and average them
        //         as soon as their amount is under the desired amount.
        int level = 0;
        while (rawColors.size() > colors)
        {
            // s and t is 1 to exclude the BG color from the checks
            // provided that the first pixel is the BG color.
            for (int s = 1; s < rawColors.size(); s++)
            {
                for (int t = 1; t < rawColors.size(); t++)
                {
                    if (t == s)
                        continue;
                    
                    if (AME_PQ_IsSimilar(rawColors.at(s), rawColors.at(t), level))
                    {
                        // Remove one of the colors (preferrably 't' because of loop)
                        rawColors.removeAt(t);
                        
                        // Check if the desired amount is now reached and
                        // exit nested loop via GOTO if that is the case.
                        if (rawColors.size() <= colors)
                            goto Exit_Node;
                    }
                }
            }
            
            level++;
        }
               
    Exit_Node:
        
        // Step 3: Finds the nearest color values for each pixel (except BG)
        for (int i = 0; i < bCount; i++)
        {
            int col = *(int*)(img.constBits() + i*4);
            if (col != rawColors.at(0))
            {
                // Now get the closest match
                int closestIndex = 0;
                int closestMatch = 256;
                for (int j = 0; j < colors; j++)
                {
                    int diff;
                    if ((diff = AME_PQ_ColorDiff(col, rawColors.at(j))) < closestMatch)
                    {
                        closestMatch = diff;
                        closestIndex = j;
                    }
                }
                
                // Write the closest match (as index)
                outDat[i] = closestIndex;
            }
        }
        
        
        // Step 3: Finally, copy the final palette to the output structure
        for (int i = 0; i < colors; i++)
            outPal[i] = *(qboy::Color *) &rawColors.at(i);
        
        
        return out;
    }
    
    
    ///////////////////////////////////////////////////////////
    // Function type:  Static
    // Contributors:   Pokedude
    // Last edit by:   Pokedude
    // Date of edit:   8/24/2016
    //
    ///////////////////////////////////////////////////////////
    bool AME_PQ_IsSimilar(int color1, int color2, int level)
    {
        qboy::Color *c1 = (qboy::Color *) &color1;
        qboy::Color *c2 = (qboy::Color *) &color2;
        
        int range = SIMILARITY_TOLERANCE_INTERVAL * level;
        return  abs((int)c1->r - (int)c2->r) < range &&
                abs((int)c1->g - (int)c2->g) < range &&
                abs((int)c1->b - (int)c2->b) < range ;
    }
    
    ///////////////////////////////////////////////////////////
    // Function type:  Static
    // Contributors:   Pokedude
    // Last edit by:   Pokedude
    // Date of edit:   8/24/2016
    //
    ///////////////////////////////////////////////////////////
    int AME_PQ_ColorDiff(int color1, int color2)
    {
        qboy::Color *c1 = (qboy::Color *) &color1;
        qboy::Color *c2 = (qboy::Color *) &color2;
        
        int rd = (int)c1->r - (int)c2->r;
        int gd = (int)c1->g - (int)c2->g;
        int bd = (int)c1->b - (int)c2->b;
        
        return (int)(sqrt(rd*rd + gd*gd + bd*bd));
    }
}
