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
#include <AME/Widgets/OpenGL/AMEBorderView.h>
#include <AME/Widgets/OpenGL/AMEOpenGLShared.hpp>
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
    // Function type:  Constructor
    // Contributors:   Pokedude
    // Last edit by:   Pokedude
    // Date of edit:   6/19/2016
    //
    ///////////////////////////////////////////////////////////
    AMEBorderView::AMEBorderView(QWidget *parent)
        : QOpenGLWidget(parent),
          QOpenGLFunctions(),
          m_Foreground(0),
          m_Background(0),
          m_IsInit(false)
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
    // Date of edit:   6/19/2016
    //
    ///////////////////////////////////////////////////////////
    AMEBorderView::~AMEBorderView()
    {
        if (m_IsInit)
            freeGL();
    }


    ///////////////////////////////////////////////////////////
    // Function type:  Virtual
    // Contributors:   Pokedude
    // Last edit by:   Pokedude
    // Date of edit:   6/19/2016
    //
    ///////////////////////////////////////////////////////////
    void AMEBorderView::initializeGL()
    {
        if (qboy::GLErrors::Current == NULL)
            qboy::GLErrors::Current = new qboy::GLErrors;

        initializeOpenGLFunctions();

        // Enables blending
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    }

    ///////////////////////////////////////////////////////////
    // Function type:  Virtual
    // Contributors:   Pokedude
    // Last edit by:   Pokedude
    // Date of edit:   6/19/2016
    //
    ///////////////////////////////////////////////////////////
    void AMEBorderView::resizeGL(int w, int h)
    {
        glCheck(glViewport(0, 0, w, h));
    }

    ///////////////////////////////////////////////////////////
    // Function type:  Virtual
    // Contributors:   Pokedude
    // Last edit by:   Pokedude
    // Date of edit:   6/19/2016
    //
    ///////////////////////////////////////////////////////////
    void AMEBorderView::paintGL()
    {
        glCheck(glClearColor(1, 1, 1, 1));
        glCheck(glClear(GL_COLOR_BUFFER_BIT));

        if (!m_Foreground)
            return;

        // Binds the vertex array, vertex buffer and the index buffer
        glCheck(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IndexBuffer));
        glCheck(glBindBuffer(GL_ARRAY_BUFFER, m_VertexBuffer));


        // Paints the primary background and foreground
        QMatrix4x4 mat_mvp;
        mat_mvp.setToIdentity();
        mat_mvp.ortho(0, width(), height(), 0, -1, 1);

        // Specifies the matrix and buffers within the shader program
        m_Program.bind();
        m_Program.setUniformValue("uni_mvp", mat_mvp);
        m_Program.setUniformValue("is_background", true);
        m_Program.enableAttributeArray(MV_VERTEX_ATTR);
        m_Program.enableAttributeArray(MV_COORD_ATTR);
        m_Program.setAttributeBuffer(MV_VERTEX_ATTR, GL_FLOAT, 0*sizeof(float), 2, 4*sizeof(float));
        m_Program.setAttributeBuffer(MV_COORD_ATTR,  GL_FLOAT, 2*sizeof(float), 2, 4*sizeof(float));

        // Draws them
        glCheck(glActiveTexture(GL_TEXTURE1));
        glCheck(glBindTexture(GL_TEXTURE_2D, m_PalTexture));
        glCheck(glActiveTexture(GL_TEXTURE0));
        glCheck(glBindTexture(GL_TEXTURE_2D, m_BackTexture));
        glCheck(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, NULL));

        m_Program.setUniformValue("is_background", false);
        glCheck(glBindTexture(GL_TEXTURE_2D, m_ForeTexture));
        glCheck(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, NULL));
    }


    ///////////////////////////////////////////////////////////
    // Function type:  Helper
    // Contributors:   Pokedude
    // Last edit by:   Pokedude
    // Date of edit:   6/19/2016
    //
    ///////////////////////////////////////////////////////////
    inline void extractBorderBlock(UInt8 *dest, UInt8 *pixels, UInt16 block)
    {
        int x = (block % 8) * 16;
        int y = (block / 8) * 16;
        int pos = 0;

         for (int y2 = 0; y2 < 16; y2++)
             for (int x2 = 0; x2 < 16; x2++)
                 dest[pos++] = pixels[(x+x2) + (y+y2) * 128];
    }


    ///////////////////////////////////////////////////////////
    // Function type:  Setter
    // Contributors:   Pokedude
    // Last edit by:   Pokedude
    // Date of edit:   6/19/2016
    //
    ///////////////////////////////////////////////////////////
    void AMEBorderView::setMapView(AMEMapView *view, bool layout)
    {
        makeCurrent();
        initializeOpenGLFunctions();
        m_IsInit = true;


        // Fetches the border and the blockset pixel buffers
        MapBorder *pbd;
        if (!layout)
            pbd = &view->mainMap()->header().border();
        else
            pbd = &view->border();

        MapBorder &border = *pbd;
        QList<UInt8 *> setPixels = view->mainPixels();
        UInt8 *primaryBg = setPixels[0];
        UInt8 *primaryFg = setPixels[1];
        UInt8 *secondaryBg = setPixels[2];
        UInt8 *secondaryFg = setPixels[3];
        UInt8 blockBuffer[256];

        // Initializes the border block pixel buffers
        m_Background = new UInt8[border.width()*border.height()*256];
        m_Foreground = new UInt8[border.width()*border.height()*256];


        // Fetches every block and copies it to the pixel buffers
        for (int i = 0; i < border.blocks().size(); i++)
        {
            MapBlock block = *border.blocks().at(i);
            Int32 mapX = (i % border.width()) * 16;
            Int32 mapY = (i / border.width()) * 16;

            if (block.block >= view->primaryBlockCount())
                extractBorderBlock(blockBuffer, secondaryBg, block.block - view->primaryBlockCount());
            else
                extractBorderBlock(blockBuffer, primaryBg, block.block);

            int pos = 0;
            for (int y = 0; y < 16; y++)
                for (int x = 0; x < 16; x++)
                    m_Background[(x+mapX) + (y+mapY) * border.width()*16] = blockBuffer[pos++];


             if (block.block >= view->primaryBlockCount())
                 extractBorderBlock(blockBuffer, secondaryFg, block.block - view->primaryBlockCount());
             else
                 extractBorderBlock(blockBuffer, primaryFg, block.block);

             pos = 0;
             for (int y = 0; y < 16; y++)
                 for (int x = 0; x < 16; x++)
                     m_Foreground[(x+mapX) + (y+mapY) * border.width()*16] = blockBuffer[pos++];
        }


        // Creates the static vertex buffer
        float wid = (float)border.width()*16;
        float hei = (float)border.height()*16;
        float buffer[16] =
        {
        //  X    Y       U   V
            0,   0,      0,  0,
            wid, 0,      1,  0,
            wid, hei,    1,  1,
            0,   hei,    0,  1
        };


        // Creates the necessary OpenGL objects for rendering
        const unsigned int indices[] = { 0, 1, 2, 2, 3, 0 };
        glCheck(m_VAO.bind());
        glCheck(glGenBuffers(1, &m_IndexBuffer));
        glCheck(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IndexBuffer));
        glCheck(glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned)*6, indices, GL_STATIC_DRAW));

        glCheck(glGenBuffers(1, &m_VertexBuffer));
        glCheck(glBindBuffer(GL_ARRAY_BUFFER, m_VertexBuffer));
        glCheck(glBufferData(GL_ARRAY_BUFFER, sizeof(float)*16, buffer, GL_DYNAMIC_DRAW));

        glCheck(glGenTextures(1, &m_BackTexture));
        glCheck(glGenTextures(1, &m_ForeTexture));
        glCheck(glBindTexture(GL_TEXTURE_2D, m_BackTexture));
        glCheck(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST));
        glCheck(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST));
        glCheck(glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, (int)wid, (int)hei, 0, GL_RED, GL_UNSIGNED_BYTE, m_Background));
        glCheck(glBindTexture(GL_TEXTURE_2D, m_ForeTexture));
        glCheck(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST));
        glCheck(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST));
        glCheck(glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, (int)wid, (int)hei, 0, GL_RED, GL_UNSIGNED_BYTE, m_Foreground));
        glCheck(glGenTextures(1, &m_PalTexture));
        glCheck(glBindTexture(GL_TEXTURE_2D, m_PalTexture));
        glCheck(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST));
        glCheck(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST));
        glCheck(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 256, 1, 0, GL_RGBA, GL_FLOAT, view->mainPalettes()->data()));


        // Sets the minimum size
        setMinimumSize(border.width()*16, border.height()*16);
        doneCurrent();
    }


    ///////////////////////////////////////////////////////////
    // Function type:  I/O
    // Contributors:   Pokedude
    // Last edit by:   Pokedude
    // Date of edit:   6/20/2016
    //
    ///////////////////////////////////////////////////////////
    void AMEBorderView::freeGL()
    {
        makeCurrent();


        delete m_Foreground;
        delete m_Background;

        glCheck(glDeleteBuffers(1, &m_VertexBuffer));
        glCheck(glDeleteTextures(1, &m_BackTexture));
        glCheck(glDeleteTextures(1, &m_ForeTexture));
        glCheck(glDeleteTextures(1, &m_PalTexture));
        glCheck(glDeleteBuffers(1, &m_IndexBuffer));

        m_Foreground = NULL;
        m_Background = NULL;


        doneCurrent();
        repaint();
    }
}
