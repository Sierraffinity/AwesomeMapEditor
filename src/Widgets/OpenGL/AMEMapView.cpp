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
#include <AME/Widgets/OpenGL/AMEMapView.h>
#include <QBoy/OpenGL/GLErrors.hpp>


namespace ame
{
    ///////////////////////////////////////////////////////////
    // Local definitions
    //
    ///////////////////////////////////////////////////////////
    #define MV_VERTEX_ATTR      0
    #define MV_COORD_ATTR       1

    ///////////////////////////////////////////////////////////
    // Local buffers
    //
    ///////////////////////////////////////////////////////////
    UInt8 pixelBuffer[64];
    UInt8 blockBuffer[256];


    ///////////////////////////////////////////////////////////
    // Function type:  Constructor
    // Contributors:   Pokedude
    // Last edit by:   Pokedude
    // Date of edit:   6/17/2016
    //
    ///////////////////////////////////////////////////////////
    AMEMapView::AMEMapView(QWidget *parent)
        : QOpenGLWidget(parent),
          QOpenGLFunctions()
    {
        QSurfaceFormat format = this->format();
        format.setDepthBufferSize(24);
        format.setStencilBufferSize(8);
        format.setVersion(3, 3);
        format.setProfile(QSurfaceFormat::CompatibilityProfile);
        setFormat(format);
    }

    ///////////////////////////////////////////////////////////
    // Function type:  Destructor
    // Contributors:   Pokedude
    // Last edit by:   Pokedude
    // Date of edit:   6/17/2016
    //
    ///////////////////////////////////////////////////////////
    AMEMapView::~AMEMapView()
    {
        foreach (UInt32 id, m_MapTextures)
            glCheck(glDeleteTextures(1, &id));
        foreach (UInt32 id, m_PalTextures)
            glCheck(glDeleteTextures(1, &id));
        foreach (UInt32 id, m_VertexBuffers)
            glCheck(glDeleteBuffers(1, &id));
        foreach (UInt8 *v, m_BackPixelBuffers)
            delete v;
        foreach (UInt8 *v, m_ForePixelBuffers)
            delete v;

        glCheck(glDeleteBuffers(1, &m_IndexBuffer));
        delete m_PrimaryForeground;
        delete m_PrimaryBackground;
        delete m_SecondaryForeground;
        delete m_SecondaryBackground;

        m_Maps.clear();
        m_MapSizes.clear();
        m_MapPositions.clear();
        m_Palettes.clear();
        m_VAO.destroy();
        m_Program.removeAllShaders();
    }


    ///////////////////////////////////////////////////////////
    // Function type:  Virtual
    // Contributors:   Pokedude
    // Last edit by:   Pokedude
    // Date of edit:   6/17/2016
    //
    ///////////////////////////////////////////////////////////
    void AMEMapView::initializeGL()
    {
        // Initializes the OpenGL functions
        initializeOpenGLFunctions();
        qboy::GLErrors::Current = new qboy::GLErrors;

        // Initializes the shader program
        m_Program.addShaderFromSourceFile(QOpenGLShader::Vertex, ":/shaders/MapVertexShader.glsl");
        m_Program.addShaderFromSourceFile(QOpenGLShader::Fragment, ":/shaders/MapFragmentShader.glsl");
        m_Program.link();
        m_Program.bind();
        m_Program.setUniformValue("smp_texture", 0);
        m_Program.setUniformValue("smp_palette", 1);


        // Initializes the vao
        m_VAO.create();
        m_VAO.bind();


        // Enables blending
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    }

    ///////////////////////////////////////////////////////////
    // Function type:  Virtual
    // Contributors:   Pokedude
    // Last edit by:   Pokedude
    // Date of edit:   6/17/2016
    //
    ///////////////////////////////////////////////////////////
    void AMEMapView::resizeGL(int w, int h)
    {
        glCheck(glViewport(0, 0, w, h));
    }

    ///////////////////////////////////////////////////////////
    // Function type:  Virtual
    // Contributors:   Pokedude
    // Last edit by:   Pokedude
    // Date of edit:   6/17/2016
    //
    ///////////////////////////////////////////////////////////
    void AMEMapView::paintGL()
    {
        glCheck(glClearColor(0, 0, 1, 1));
        glCheck(glClear(GL_COLOR_BUFFER_BIT));

        // Binds the vertex array and the index buffer
        // (which is the same for all) initially.
        glCheck(m_VAO.bind());
        glCheck(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IndexBuffer));


        // Paints every texture
        for (int i = 0; i < m_Maps.size(); i++)
        {
            // Computes the MVP matrix with specific translation
            QMatrix4x4 mat_mvp;
            mat_mvp.setToIdentity();
            mat_mvp.ortho(0, width(), height(), 0, -1, 1);
            mat_mvp.translate(m_MapPositions.at(i).x(), m_MapPositions.at(i).y());

            // Binds the vertex buffer of the current texture
            glCheck(glBindBuffer(GL_ARRAY_BUFFER, m_VertexBuffers.at(i)));

            // Specifies the matrix and buffers within the shader program
            m_Program.bind();
            m_Program.setUniformValue("uni_mvp", mat_mvp);
            m_Program.enableAttributeArray(MV_VERTEX_ATTR);
            m_Program.enableAttributeArray(MV_COORD_ATTR);
            m_Program.setAttributeBuffer(MV_VERTEX_ATTR, GL_FLOAT, 0*sizeof(float), 2, 4*sizeof(float));
            m_Program.setAttributeBuffer(MV_COORD_ATTR,  GL_FLOAT, 2*sizeof(float), 2, 4*sizeof(float));

            // Binds the palette for the entire map
            glCheck(glActiveTexture(GL_TEXTURE1));
            glCheck(glBindTexture(GL_TEXTURE_2D, m_PalTextures.at(i)));

            // Draws the background
            glCheck(glActiveTexture(GL_TEXTURE0));
            glCheck(glBindTexture(GL_TEXTURE_2D, m_MapTextures.at(i*2)));
            glCheck(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, NULL));

            // Draws the foreground
            glCheck(glActiveTexture(GL_TEXTURE0));
            glCheck(glBindTexture(GL_TEXTURE_2D, m_MapTextures.at(i*2+1)));
            glCheck(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, NULL));
        }
    }


    ///////////////////////////////////////////////////////////
    // Function type:  Helper
    // Contributors:   Pokedude
    // Last edit by:   Pokedude
    // Date of edit:   6/17/2016
    //
    ///////////////////////////////////////////////////////////
    inline void flipVertically(UInt8 *dest, UInt8 *pixels)
    {
        for (int i = 0; i < 8; i++)
            for (int j = 0; j < 8; j++)
                dest[i + j * 8] = pixels[i + (8-1-j) * 8];
    }

    ///////////////////////////////////////////////////////////
    // Function type:  Helper
    // Contributors:   Pokedude
    // Last edit by:   Pokedude
    // Date of edit:   6/17/2016
    //
    ///////////////////////////////////////////////////////////
    inline void flipHorizontally(UInt8 *dest, UInt8 *pixels)
    {
        for (int i = 0; i < 64; i++)
            dest[i] = pixels[i - 2*(i%8) + 8-1];
    }

    ///////////////////////////////////////////////////////////
    // Function type:  Helper
    // Contributors:   Pokedude
    // Last edit by:   Pokedude
    // Date of edit:   6/17/2016
    //
    ///////////////////////////////////////////////////////////
    inline void extractTile(const QByteArray &img, Tile &tile)
    {
        unsigned char buffer[64];
        int x = (tile.tile % 16) * 8;
        int y = (tile.tile / 16) * 8;
        int pos = 0;

        for (int y2 = 0; y2 < 8; y2++)
            for (int x2 = 0; x2 < 8; x2++)
                pixelBuffer[pos++] = (UInt8)((img.at((x+x2) + (y+y2) * 128)) + (tile.palette * 16));

        if (tile.flipX)
        {
            flipHorizontally(buffer, pixelBuffer);
            if (tile.flipY)
                flipVertically(pixelBuffer, buffer);
            else
                memcpy(pixelBuffer, buffer, 64);
        }
        else if (tile.flipY)
        {
            flipVertically(buffer, pixelBuffer);
            memcpy(pixelBuffer, buffer, 64);
        }
    }

    ///////////////////////////////////////////////////////////
    // Function type:  Helper
    // Contributors:   Pokedude
    // Last edit by:   Pokedude
    // Date of edit:   6/17/2016
    //
    ///////////////////////////////////////////////////////////
    inline void extractBlock(UInt8 *pixels, UInt16 block)
    {
        int x = (block % 8) * 16;
        int y = (block / 8) * 16;
        int pos = 0;

        for (int y2 = 0; y2 < 16; y2++)
            for (int x2 = 0; x2 < 16; x2++)
                blockBuffer[pos++] = pixels[(x+x2) + (y+y2) * 128];
    }


    ///////////////////////////////////////////////////////////
    // Function type:  I/O
    // Contributors:   Pokedude
    // Last edit by:   Pokedude
    // Date of edit:   6/17/2016
    //
    ///////////////////////////////////////////////////////////
    bool AMEMapView::setMap(const qboy::Rom &rom, Map *mainMap)
    {
        const QSize mainSize = mainMap->header().size();
        const QList<Connection *> connex = mainMap->connections().connections();
        m_Maps.push_back(mainMap);


        // Retrieves every connected map
        for (int i = 0; i < connex.size(); i++)
        {
            Connection *current = connex[i];
            Map *map = dat_MapBankTable->banks().at(current->bank)->maps().at(current->map);
            m_Maps.push_back(map);
        }

        m_MapSizes.push_back(QSize(mainSize.width()*16, mainSize.height()*16));
        m_MapPositions.push_back(QPoint(0, 0));
        m_WidgetSize = QSize(m_MapSizes.at(0));


        // Calculates the positions for the maps
        int biggestLeftMap = 0; // width of the biggest left-connected map
        int biggestTopMap = 0;  // height of the biggest top-connected map
        for (int i = 0; i < connex.size(); i++)
        {
            Connection *current = connex[i];
            Map *map = m_Maps[i+1];
            QPoint translation;
            Int32 signedOff = (Int32)(current->offset)*16;

            // Determines the offset by direction
            QSize mapSize = QSize(map->header().size().width()*16, map->header().size().height()*16);
            if (current->direction == DIR_Left)
            {
                translation.setX(-mapSize.width());
                translation.setY(signedOff);
                m_WidgetSize.rwidth() += mapSize.width();

                if (mapSize.width() > biggestLeftMap)
                    biggestLeftMap = mapSize.width();
            }
            else if (current->direction == DIR_Right)
            {
                translation.setX(mainSize.width()*16);
                translation.setY(signedOff);
                m_WidgetSize.rwidth() += mapSize.width();
            }
            else if (current->direction == DIR_Up)
            {
                translation.setX(signedOff);
                translation.setY(-mapSize.height());
                m_WidgetSize.rheight() += mapSize.height();

                if (mapSize.height() > biggestTopMap)
                    biggestTopMap = mapSize.height();
            }
            else if (current->direction == DIR_Down)
            {
                translation.setX(signedOff);
                translation.setY(mainSize.height()*16);
                m_WidgetSize.rheight() += mapSize.height();
            }

            m_MapPositions.push_back(translation);
            m_MapSizes.push_back(mapSize);
        }

        // Adds the biggest left and top offsets to all positions
        // in order to not cause rendering fails on the widget
        for (int i = 0; i < m_MapPositions.size(); i++)
        {
            m_MapPositions[i].rx() += biggestLeftMap;
            m_MapPositions[i].ry() += biggestTopMap;
        }


        // Determines the block count for each game
        int blockCountPrimary;
        int blockCountSecondary;
        if (rom.info().isFRLG())
        {
            blockCountPrimary = 0x280;
            blockCountSecondary = 0x180;
        }
        else
        {
            blockCountPrimary = 0x200;
            blockCountSecondary = 0x200;
        }


        // Attempts to load all the blocksets
        QList<UInt8 *> blocksetBack;
        QList<UInt8 *> blocksetFore;
        for (int i = 0; i < m_Maps.size(); i++)
        {
            Tileset *primary = m_Maps[i]->header().primary();
            Tileset *secondary = m_Maps[i]->header().secondary();
            QVector<qboy::GLColor> palettes;

            // Retrieves the palettes, combines them, removes bg color
            for (int j = 0; j < primary->palettes().size(); j++)
            {
                palettes.append(primary->palettes().at(j)->rawGL());
                palettes[j * 16].a = 0.0f;
            }
            for (int j = 0; j < secondary->palettes().size(); j++)
            {
                palettes.append(secondary->palettes().at(j)->rawGL());
                palettes[(primary->palettes().size() * 16) + (j * 16)].a = 0.0f;
            }

            while (palettes.size() < 256) // align pal for OpenGL
                palettes.push_back({ 0.f, 0.f, 0.f, 0.f });

            m_Palettes.push_back(palettes);


            // Retrieves the raw pixel data of the tilesets
            const QByteArray &priRaw = primary->image()->raw();
            const QByteArray &secRaw = secondary->image()->raw();

            // Creates two buffers for the blockset pixels
            Int32 tilesetHeight1 = blockCountPrimary / 8 * 16;
            Int32 tilesetHeight2 = blockCountSecondary / 8 * 16;
            UInt8 *background1 = new UInt8[128 * tilesetHeight1];
            UInt8 *foreground1 = new UInt8[128 * tilesetHeight1];
            UInt8 *background2 = new UInt8[128 * tilesetHeight2];
            UInt8 *foreground2 = new UInt8[128 * tilesetHeight2];

            // Copy the tileset sizes for our main map
            if (i == 0)
            {
                m_PrimarySetSize = QSize(128, tilesetHeight1);
                m_SecondarySetSize = QSize(128, tilesetHeight2);
            }


            // Parses all the primary block data
            for (int j = 0; j < primary->blocks().size(); j++)
            {
                Block *curBlock = primary->blocks()[j];
                Int32 blockX = (j % 8) * 16;
                Int32 blockY = (j / 8) * 16;

                /* BACKGROUND */
                for (int k = 0; k < 4; k++)
                {
                    Tile tile = curBlock->tiles[k];
                    Int32 subX = ((k % 2) * 8) + blockX;
                    Int32 subY = ((k / 2) * 8) + blockY;

                    if (tile.tile >= blockCountPrimary)
                    {
                        tile.tile -= blockCountPrimary;
                        extractTile(secRaw, tile);
                    }
                    else
                    {
                        extractTile(priRaw, tile);
                    }

                    int pos = 0;
                    for (int y = 0; y < 8; y++)
                        for (int x = 0; x < 8; x++)
                            background1[(x+subX) + (y+subY) * 128] = pixelBuffer[pos++];
                }

                /* FOREGROUND */
                for (int k = 0; k < 4; k++)
                {
                    Tile tile = curBlock->tiles[k+4];
                    Int32 subX = ((k % 2) * 8) + blockX;
                    Int32 subY = ((k / 2) * 8) + blockY;;

                    if (tile.tile >= blockCountPrimary)
                    {
                        tile.tile -= blockCountPrimary;
                        extractTile(secRaw, tile);
                    }
                    else
                    {
                        extractTile(priRaw, tile);
                    }

                    int pos = 0;
                    for (int y = 0; y < 8; y++)
                        for (int x = 0; x < 8; x++)
                            foreground1[(x+subX) + (y+subY) * 128] = pixelBuffer[pos++];
                }
            }

            // Parses all the secondary block data
            for (int j = 0; j < secondary->blocks().size(); j++)
            {
                Block *curBlock = secondary->blocks()[j];
                Int32 blockX = (j % 8) * 16;
                Int32 blockY = (j / 8) * 16;

                /* BACKGROUND */
                for (int k = 0; k < 4; k++)
                {
                    Tile tile = curBlock->tiles[k];
                    Int32 subX = ((k % 2) * 8) + blockX;
                    Int32 subY = ((k / 2) * 8) + blockY;

                    if (tile.tile >= blockCountPrimary)
                    {
                        tile.tile -= blockCountPrimary;
                        extractTile(secRaw, tile);
                    }
                    else
                    {
                        extractTile(priRaw, tile);
                    }

                    int pos = 0;
                    for (int y = 0; y < 8; y++)
                        for (int x = 0; x < 8; x++)
                            background2[(x+subX) + (y+subY) * 128] = pixelBuffer[pos++];
                }

                /* FOREGROUND */
                for (int k = 0; k < 4; k++)
                {
                    Tile tile = curBlock->tiles[k+4];
                    Int32 subX = ((k % 2) * 8) + blockX;
                    Int32 subY = ((k / 2) * 8) + blockY;;

                    if (tile.tile >= blockCountPrimary)
                    {
                        tile.tile -= blockCountPrimary;
                        extractTile(secRaw, tile);
                    }
                    else
                    {
                        extractTile(priRaw, tile);
                    }

                    int pos = 0;
                    for (int y = 0; y < 8; y++)
                        for (int x = 0; x < 8; x++)
                            foreground2[(x+subX) + (y+subY) * 128] = pixelBuffer[pos++];
                }
            }


            // Adds the new blocksets
            blocksetBack.push_back(background1);
            blocksetBack.push_back(background2);
            blocksetFore.push_back(foreground1);
            blocksetFore.push_back(foreground2);
        }


        // Creates the images for the actual maps
        for (int i = 0; i < m_Maps.size(); i++)
        {
            MapHeader &header = m_Maps[i]->header();
            QSize mapSize = header.size();

            // Creates a new pixel buffer for the map
            UInt8 *backMapBuffer = new UInt8[mapSize.width()*16 * mapSize.height()*16];
            UInt8 *foreMapBuffer = new UInt8[mapSize.width()*16 * mapSize.height()*16];
            UInt8 *primaryBg = blocksetBack.at(i*2);
            UInt8 *secondaryBg = blocksetBack.at(i*2+1);
            UInt8 *primaryFg = blocksetFore.at(i*2);
            UInt8 *secondaryFg = blocksetFore.at(i*2+1);

            // Iterates through every map block and writes it to the map buffer
            for (int j = 0; j < header.blocks().size(); j++)
            {
                MapBlock block = *header.blocks().at(j);
                Int32 mapX = (j % mapSize.width()) * 16;
                Int32 mapY = (j / mapSize.width()) * 16;

                if (block.block >= blockCountPrimary)
                    extractBlock(secondaryBg, block.block - blockCountPrimary);
                else
                    extractBlock(primaryBg, block.block);

                int pos = 0;
                for (int y = 0; y < 16; y++)
                    for (int x = 0; x < 16; x++)
                        backMapBuffer[(x+mapX) + (y+mapY) * mapSize.width()*16] = blockBuffer[pos++];

                if (block.block >= blockCountPrimary)
                    extractBlock(secondaryFg, block.block - blockCountPrimary);
                else
                    extractBlock(primaryFg, block.block);

                pos = 0;
                for (int y = 0; y < 16; y++)
                    for (int x = 0; x < 16; x++)
                        foreMapBuffer[(x+mapX) + (y+mapY) * mapSize.width()*16] = blockBuffer[pos++];
            }

            // Appends the pixel buffers
            m_BackPixelBuffers.push_back(backMapBuffer);
            m_ForePixelBuffers.push_back(foreMapBuffer);
        }

        // Deletes all unnecessary blocksets
        for (int i = 0; i < blocksetBack.size(); i++)
        {
            if (i == 0)
            {
                m_PrimaryBackground = blocksetBack[0];
                m_PrimaryForeground = blocksetFore[0];
            }
            else if (i == 1)
            {
                m_SecondaryBackground = blocksetBack[1];
                m_SecondaryForeground = blocksetFore[1];
            }
            else
            {
                delete blocksetBack[i];
                delete blocksetFore[i];
            }

        }

        return true;
    }


    ///////////////////////////////////////////////////////////
    // Function type:  I/O
    // Contributors:   Pokedude
    // Last edit by:   Pokedude
    // Date of edit:   6/17/2016
    //
    ///////////////////////////////////////////////////////////
    void AMEMapView::makeGL()
    {
        makeCurrent();
        initializeOpenGLFunctions();

        // Creates, binds and buffers the static index buffer
        const unsigned int indices[] = { 0, 1, 2, 2, 3, 0 };
        glCheck(glGenBuffers(1, &m_IndexBuffer));
        glCheck(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IndexBuffer));
        glCheck(glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned)*6, indices, GL_STATIC_READ));

        // Creates vertex buffers (we only need 1 for 2 layers)
        for (int i = 0; i < m_Maps.size(); i++)
        {
            // Creates a constant vertex buffer
            float wid = (float)m_MapSizes.at(i).width();
            float hei = (float)m_MapSizes.at(i).height();
            float buffer[16] =
            {
            //  X    Y       U   V
                0,   0,      0,  0,
                wid, 0,      1,  0,
                wid, hei,    1,  1,
                0,   hei,    0,  1
            };

            // Buffers the vertices
            unsigned vbuffer = 0;
            glCheck(glGenBuffers(1, &vbuffer));
            glCheck(glBindBuffer(GL_ARRAY_BUFFER, vbuffer));
            glCheck(glBufferData(GL_ARRAY_BUFFER, sizeof(float)*16, buffer, GL_DYNAMIC_DRAW));
            m_VertexBuffers.push_back(vbuffer);


            // Creates foreground and background textures
            unsigned texFore = 0, texBack = 0;
            glCheck(glGenTextures(1, &texFore));
            glCheck(glGenTextures(1, &texBack));
            glCheck(glBindTexture(GL_TEXTURE_2D, texBack));
            glCheck(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST));
            glCheck(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST));
            glCheck(glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, (int)wid, (int)hei, 0, GL_RED, GL_UNSIGNED_BYTE, m_BackPixelBuffers[i]));
            glCheck(glBindTexture(GL_TEXTURE_2D, texFore));
            glCheck(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST));
            glCheck(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST));
            glCheck(glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, (int)wid, (int)hei, 0, GL_RED, GL_UNSIGNED_BYTE, m_ForePixelBuffers[i]));
            m_MapTextures.push_back(texBack);
            m_MapTextures.push_back(texFore);

            // Creates one palette texture for both layers
            unsigned texPal = 0;
            glCheck(glGenTextures(1, &texPal));
            glCheck(glBindTexture(GL_TEXTURE_2D, texPal));
            glCheck(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST));
            glCheck(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST));
            glCheck(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 256, 1, 0, GL_RGBA, GL_FLOAT, m_Palettes.at(i).data()));
            m_PalTextures.push_back(texPal);
        }

        // Sets the minimum size
        setMinimumSize(m_WidgetSize);
        doneCurrent();
    }
}
