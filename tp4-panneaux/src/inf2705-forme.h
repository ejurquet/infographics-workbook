#ifndef __INF2705_FORME_H__
#define __INF2705_FORME_H__

///////////////////////////////////////////////////////////////////////////
/// @class Forme
/// @brief Fonctions variées pour afficher des formes connues
///
///        Fonctions variées pour afficher des formes connues
///
/// @author Benoit Ozell, INF2705
///////////////////////////////////////////////////////////////////////////

#include <GL/glew.h>
#include <iostream>
#include <string.h>
#include <glm/glm.hpp>
#include <glm/gtx/string_cast.hpp>

#if defined( linux )
# define AVEC_OPENGL2 1
#endif

#if defined(_WIN32) || defined(WIN32)
#pragma warning ( disable : 4305 )
// warning C4305: 'argument' : truncation from 'double' to 'const GLfloat'
#endif


#if defined( AVEC_OPENGL2)

/* Copyright (c) Mark J. Kilgard, 1994, 1997. */

/**
   (c) Copyright 1993, Silicon Graphics, Inc.

   ALL RIGHTS RESERVED

   Permission to use, copy, modify, and distribute this software
   for any purpose and without fee is hereby granted, provided
   that the above copyright notice appear in all copies and that
   both the copyright notice and this permission notice appear in
   supporting documentation, and that the name of Silicon
   Graphics, Inc. not be used in advertising or publicity
   pertaining to distribution of the software without specific,
   written prior permission.

   THE MATERIAL EMBODIED ON THIS SOFTWARE IS PROVIDED TO YOU
   "AS-IS" AND WITHOUT WARRANTY OF ANY KIND, EXPRESS, IMPLIED OR
   OTHERWISE, INCLUDING WITHOUT LIMITATION, ANY WARRANTY OF
   MERCHANTABILITY OR FITNESS FOR A PARTICULAR PURPOSE.  IN NO
   EVENT SHALL SILICON GRAPHICS, INC.  BE LIABLE TO YOU OR ANYONE
   ELSE FOR ANY DIRECT, SPECIAL, INCIDENTAL, INDIRECT OR
   CONSEQUENTIAL DAMAGES OF ANY KIND, OR ANY DAMAGES WHATSOEVER,
   INCLUDING WITHOUT LIMITATION, LOSS OF PROFIT, LOSS OF USE,
   SAVINGS OR REVENUE, OR THE CLAIMS OF THIRD PARTIES, WHETHER OR
   NOT SILICON GRAPHICS, INC.  HAS BEEN ADVISED OF THE POSSIBILITY
   OF SUCH LOSS, HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
   ARISING OUT OF OR IN CONNECTION WITH THE POSSESSION, USE OR
   PERFORMANCE OF THIS SOFTWARE.

   US Government Users Restricted Rights

   Use, duplication, or disclosure by the Government is subject to
   restrictions set forth in FAR 52.227.19(c)(2) or subparagraph
   (c)(1)(ii) of the Rights in Technical Data and Computer
   Software clause at DFARS 252.227-7013 and/or in similar or
   successor clauses in the FAR or the DOD or NASA FAR
   Supplement.  Unpublished-- rights reserved under the copyright
   laws of the United States.  Contractor/manufacturer is Silicon
   Graphics, Inc., 2011 N.  Shoreline Blvd., Mountain View, CA
   94039-7311.

   OpenGL(TM) is a trademark of Silicon Graphics, Inc.
*/

#include <math.h>
#include <stdio.h>
#include <assert.h>

static GLfloat dodec[20][3];

static void initDodecahedron(void)
{
    GLfloat alpha, beta;

    alpha = sqrt(2.0 / (3.0 + sqrt(5.0)));
    beta = 1.0 + sqrt(6.0 / (3.0 + sqrt(5.0)) - 2.0 + 2.0 * sqrt(2.0 / (3.0 + sqrt(5.0))));
    dodec[0][0] = -alpha; dodec[0][1] = 0; dodec[0][2] = beta;
    dodec[1][0] = alpha; dodec[1][1] = 0; dodec[1][2] = beta;
    dodec[2][0] = -1; dodec[2][1] = -1; dodec[2][2] = -1;
    dodec[3][0] = -1; dodec[3][1] = -1; dodec[3][2] = 1;
    dodec[4][0] = -1; dodec[4][1] = 1; dodec[4][2] = -1;
    dodec[5][0] = -1; dodec[5][1] = 1; dodec[5][2] = 1;
    dodec[6][0] = 1; dodec[6][1] = -1; dodec[6][2] = -1;
    dodec[7][0] = 1; dodec[7][1] = -1; dodec[7][2] = 1;
    dodec[8][0] = 1; dodec[8][1] = 1; dodec[8][2] = -1;
    dodec[9][0] = 1; dodec[9][1] = 1; dodec[9][2] = 1;
    dodec[10][0] = beta; dodec[10][1] = alpha; dodec[10][2] = 0;
    dodec[11][0] = beta; dodec[11][1] = -alpha; dodec[11][2] = 0;
    dodec[12][0] = -beta; dodec[12][1] = alpha; dodec[12][2] = 0;
    dodec[13][0] = -beta; dodec[13][1] = -alpha; dodec[13][2] = 0;
    dodec[14][0] = -alpha; dodec[14][1] = 0; dodec[14][2] = -beta;
    dodec[15][0] = alpha; dodec[15][1] = 0; dodec[15][2] = -beta;
    dodec[16][0] = 0; dodec[16][1] = beta; dodec[16][2] = alpha;
    dodec[17][0] = 0; dodec[17][1] = beta; dodec[17][2] = -alpha;
    dodec[18][0] = 0; dodec[18][1] = -beta; dodec[18][2] = alpha;
    dodec[19][0] = 0; dodec[19][1] = -beta; dodec[19][2] = -alpha;

}

#define DIFF3(_a,_b,_c) {                       \
        (_c)[0] = (_a)[0] - (_b)[0];            \
        (_c)[1] = (_a)[1] - (_b)[1];            \
        (_c)[2] = (_a)[2] - (_b)[2];            \
    }

static void crossprod(GLfloat v1[3], GLfloat v2[3], GLfloat prod[3])
{
    GLfloat p[3];         /* in case prod == v1 or v2 */

    p[0] = v1[1] * v2[2] - v2[1] * v1[2];
    p[1] = v1[2] * v2[0] - v2[2] * v1[0];
    p[2] = v1[0] * v2[1] - v2[0] * v1[1];
    prod[0] = p[0];
    prod[1] = p[1];
    prod[2] = p[2];
}

static void normalize(GLfloat v[3])
{
    GLfloat d;

    d = sqrt(v[0] * v[0] + v[1] * v[1] + v[2] * v[2]);
    if (d == 0.0)
    {
        printf("normalize: zero length vector");
        v[0] = d = 1.0;
    }
    d = 1 / d;
    v[0] *= d;
    v[1] *= d;
    v[2] *= d;
}

static void pentagon(int a, int b, int c, int d, int e, GLenum shadeType)
{
    GLfloat n0[3], d1[3], d2[3];

    DIFF3(dodec[a], dodec[b], d1);
    DIFF3(dodec[b], dodec[c], d2);
    crossprod(d1, d2, n0);
    normalize(n0);

    glBegin(shadeType);
    glNormal3fv(n0);
    glVertex3fv(&dodec[a][0]);
    glVertex3fv(&dodec[b][0]);
    glVertex3fv(&dodec[c][0]);
    glVertex3fv(&dodec[d][0]);
    glVertex3fv(&dodec[e][0]);
    glEnd();
}

static void dodecahedron(GLenum type)
{
    static int inited = 0;

    if (inited == 0)
    {
        inited = 1;
        initDodecahedron();
    }
    pentagon(0, 1, 9, 16, 5, type);
    pentagon(1, 0, 3, 18, 7, type);
    pentagon(1, 7, 11, 10, 9, type);
    pentagon(11, 7, 18, 19, 6, type);
    pentagon(8, 17, 16, 9, 10, type);
    pentagon(2, 14, 15, 6, 19, type);
    pentagon(2, 13, 12, 4, 14, type);
    pentagon(2, 19, 18, 3, 13, type);
    pentagon(3, 0, 5, 12, 13, type);
    pentagon(6, 15, 8, 10, 11, type);
    pentagon(4, 17, 8, 15, 14, type);
    pentagon(4, 12, 5, 16, 17, type);
}

void shapesWireDodecahedron(void)
{
    dodecahedron(GL_LINE_LOOP);
}

void shapesSolidDodecahedron(void)
{
    dodecahedron(GL_TRIANGLE_FAN);
}

static void recorditem(GLfloat * n1, GLfloat * n2, GLfloat * n3,
                       GLenum shadeType)
{
    GLfloat q0[3], q1[3];

    DIFF3(n1, n2, q0);
    DIFF3(n2, n3, q1);
    crossprod(q0, q1, q1);
    normalize(q1);

    glBegin(shadeType);
    glNormal3fv(q1);
    glVertex3fv(n1);
    glVertex3fv(n2);
    glVertex3fv(n3);
    glEnd();
}

static void subdivide(GLfloat * v0, GLfloat * v1, GLfloat * v2,
                      GLenum shadeType)
{
    int depth = 1;
    for (int i = 0; i < depth; i++)
    {
        for (int j = 0; i + j < depth; j++)
        {
            GLfloat w0[3], w1[3], w2[3];
            int k = depth - i - j;
            for (int n = 0; n < 3; n++)
            {
                w0[n] = (i * v0[n] + j * v1[n] + k * v2[n]) / depth;
                w1[n] = ((i + 1) * v0[n] + j * v1[n] + (k - 1) * v2[n]) / depth;
                w2[n] = (i * v0[n] + (j + 1) * v1[n] + (k - 1) * v2[n]) / depth;
            }
            GLfloat l;
            l = sqrt(w0[0] * w0[0] + w0[1] * w0[1] + w0[2] * w0[2]);
            w0[0] /= l;
            w0[1] /= l;
            w0[2] /= l;
            l = sqrt(w1[0] * w1[0] + w1[1] * w1[1] + w1[2] * w1[2]);
            w1[0] /= l;
            w1[1] /= l;
            w1[2] /= l;
            l = sqrt(w2[0] * w2[0] + w2[1] * w2[1] + w2[2] * w2[2]);
            w2[0] /= l;
            w2[1] /= l;
            w2[2] /= l;
            recorditem(w1, w0, w2, shadeType);
        }
    }
}

static void drawtriangle(int i, GLfloat data[][3], int ndx[][3],
                         GLenum shadeType)
{
    GLfloat *x0 = data[ndx[i][0]];
    GLfloat *x1 = data[ndx[i][1]];
    GLfloat *x2 = data[ndx[i][2]];
    subdivide(x0, x1, x2, shadeType);
}

/* octahedron data: The octahedron produced is centered at the
   origin and has radius 1.0 */
static GLfloat odata[6][3] =
{
    {1.0, 0.0, 0.0},
    {-1.0, 0.0, 0.0},
    {0.0, 1.0, 0.0},
    {0.0, -1.0, 0.0},
    {0.0, 0.0, 1.0},
    {0.0, 0.0, -1.0}
};

static int ondex[8][3] =
{
    {0, 4, 2},
    {1, 2, 4},
    {0, 3, 4},
    {1, 4, 3},
    {0, 2, 5},
    {1, 5, 2},
    {0, 5, 3},
    {1, 3, 5}
};

static void octahedron(GLenum shadeType)
{
    for (int i = 7; i >= 0; i--)
    {
        drawtriangle(i, odata, ondex, shadeType);
    }
}

void shapesWireOctahedron(void)
{
    octahedron(GL_LINE_LOOP);
}

void shapesSolidOctahedron(void)
{
    octahedron(GL_TRIANGLES);
}


/* icosahedron data: These numbers are rigged to make an
   icosahedron of radius 1.0 */

#define X .525731112119133606
#define Z .850650808352039932

static GLfloat idata[12][3] =
{
    {-X, 0, Z},
    {X, 0, Z},
    {-X, 0, -Z},
    {X, 0, -Z},
    {0, Z, X},
    {0, Z, -X},
    {0, -Z, X},
    {0, -Z, -X},
    {Z, X, 0},
    {-Z, X, 0},
    {Z, -X, 0},
    {-Z, -X, 0}
};

static int connectivity[20][3] =
{
    {0, 4, 1},
    {0, 9, 4},
    {9, 5, 4},
    {4, 5, 8},
    {4, 8, 1},
    {8, 10, 1},
    {8, 3, 10},
    {5, 3, 8},
    {5, 2, 3},
    {2, 7, 3},
    {7, 10, 3},
    {7, 6, 10},
    {7, 11, 6},
    {11, 0, 6},
    {0, 1, 6},
    {6, 1, 10},
    {9, 0, 11},
    {9, 11, 2},
    {9, 2, 5},
    {7, 2, 11},
};

static void icosahedron(GLenum shadeType)
{
    for (int i = 19; i >= 0; i--)
    {
        drawtriangle(i, idata, connectivity, shadeType);
    }
}

void shapesWireIcosahedron(void)
{
    icosahedron(GL_LINE_LOOP);
}

void shapesSolidIcosahedron(void)
{
    icosahedron(GL_TRIANGLES);
}


/* tetrahedron data: */

#define T   1.73205080756887729

static GLfloat tdata[4][3] =
{
    {T, T, T},
    {T, -T, -T},
    {-T, T, -T},
    {-T, -T, T}
};
#undef T

static int tndex[4][3] =
{
    {0, 1, 3},
    {2, 1, 0},
    {3, 2, 0},
    {1, 2, 3}
};

static void tetrahedron(GLenum shadeType)
{
    for (int i = 3; i >= 0; i--)
        drawtriangle(i, tdata, tndex, shadeType);
}

void shapesWireTetrahedron(void)
{
    tetrahedron(GL_LINE_LOOP);
}

void shapesSolidTetrahedron(void)
{
    tetrahedron(GL_TRIANGLES);
}
#endif


//////////////////////////////////////////////////////////

class FormeBase2705
{
public:
    FormeBase2705( bool plein = true )
        : plein_(plein),
        vao(0), locVertex(-1), locNormal(-1), locTexCoord(-1)
    {
        glGenVertexArrays( 1, &vao );
    }
    ~FormeBase2705()
    {
        glDeleteVertexArrays( 1, &vao );
    }
protected:
    bool obtenirAttributs( )
    {
        GLint prog = 0; glGetIntegerv( GL_CURRENT_PROGRAM, &prog );
        if ( prog <= 0 )
        {
            std::cerr << "Pas de programme actif!" << std::endl;
            return(false);
        }

        locVertex = glGetAttribLocation( prog, "Vertex" );
        if ( locVertex == -1 )
        {
            std::cerr << "Pas de nuanceur de sommets!" << std::endl;
            return(false);
        }
        locNormal = glGetAttribLocation( prog, "Normal" );
        locTexCoord = glGetAttribLocation( prog, "TexCoord" );
        if ( locTexCoord < 0 ) locTexCoord = glGetAttribLocation( prog, "TexCoord0" );
        if ( locTexCoord < 0 ) locTexCoord = glGetAttribLocation( prog, "MultiTexCoord0" );
        return(true);
    }

    bool plein_;
    GLuint vao;
    GLint locVertex, locNormal, locTexCoord;
};

//////////

#define AJOUTE(X,Y,Z,NX,NY,NZ,S,T)                                      \
    {                                                                   \
        if ( locTexCoord >= 0 )                                         \
        { texcoord[2*nsommets+0] = GLfloat(S); texcoord[2*nsommets+1] = GLfloat(T); } \
        if ( locNormal >= 0 )                                           \
        { normales[3*nsommets+0] = GLfloat(NX); normales[3*nsommets+1] = GLfloat(NY); normales[3*nsommets+2] = GLfloat(NZ); } \
        { sommets[3*nsommets+0] = GLfloat(X); sommets[3*nsommets+1] = GLfloat(Y); sommets[3*nsommets+2] = GLfloat(Z); ++nsommets; } \
    }

//////////

class FormeQuad : public FormeBase2705
{
public:
    FormeQuad( GLfloat taille = 1.0,
               bool plein = true )
        : FormeBase2705( plein )
    {
        if ( obtenirAttributs( ) )
        {
            /*         +Y            */
            /*   3+-----------+2     */
            /*    |           |      */
            /*    |           |      */
            /*    |           |  +X  */
            /*    |           |      */
            /*    |           |      */
            /*    |           |      */
            /*   0+-----------+1     */
            /*                       */

            static GLint faces[4] = { 0, 1, 2, 3 };
            static GLfloat n[3] = { 0.0, 0.0, 1.0 };
            GLfloat v[4][3];
            v[3][0] = v[0][0] = 0.0;
            v[1][0] = v[2][0] = taille;
            v[0][1] = v[1][1] = 0.0;
            v[2][1] = v[3][1] = taille;
            v[3][2] = v[2][2] = v[1][2] = v[0][2] = 0.0;
            GLfloat t[4][2];
            t[3][0] = t[0][0] = 0.;
            t[1][0] = t[2][0] = 1.;
            t[0][1] = t[1][1] = 0.;
            t[2][1] = t[3][1] = 1.;

            // initialisation
            const int TAILLEMAX = 100;
            assert( TAILLEMAX > 6*6 );
            GLfloat sommets[3*TAILLEMAX], normales[3*TAILLEMAX], texcoord[2*TAILLEMAX];
            int nsommets = 0;

            if ( plein_ )
            {
                AJOUTE( v[faces[0]][0], v[faces[0]][1], v[faces[0]][2], n[0], n[1], n[2], t[faces[0]][0], t[faces[0]][1] );
                AJOUTE( v[faces[1]][0], v[faces[1]][1], v[faces[1]][2], n[0], n[1], n[2], t[faces[1]][0], t[faces[1]][1] );
                AJOUTE( v[faces[3]][0], v[faces[3]][1], v[faces[3]][2], n[0], n[1], n[2], t[faces[3]][0], t[faces[3]][1] );
                AJOUTE( v[faces[2]][0], v[faces[2]][1], v[faces[2]][2], n[0], n[1], n[2], t[faces[2]][0], t[faces[2]][1] );
            }
            else
            {
                AJOUTE( v[faces[0]][0], v[faces[0]][1], v[faces[0]][2], n[0], n[1], n[2], t[faces[0]][0], t[faces[0]][1] );
                AJOUTE( v[faces[1]][0], v[faces[1]][1], v[faces[1]][2], n[0], n[1], n[2], t[faces[1]][0], t[faces[1]][1] );
                AJOUTE( v[faces[2]][0], v[faces[2]][1], v[faces[2]][2], n[0], n[1], n[2], t[faces[2]][0], t[faces[2]][1] );
                AJOUTE( v[faces[3]][0], v[faces[3]][1], v[faces[3]][2], n[0], n[1], n[2], t[faces[3]][0], t[faces[3]][1] );
            }
            nsommets_ = nsommets;
            assert( TAILLEMAX >= nsommets );

            // remplir VBOs
            glBindVertexArray( vao );
            glGenBuffers( 3, vbo );

            glBindBuffer( GL_ARRAY_BUFFER, vbo[0] );
            glBufferData( GL_ARRAY_BUFFER, 3*nsommets*sizeof(GLfloat), NULL, GL_STATIC_DRAW );
            glBufferSubData( GL_ARRAY_BUFFER, 0, 3*nsommets*sizeof(GLfloat), sommets );
            glVertexAttribPointer( locVertex, 3, GL_FLOAT, GL_FALSE, 0, 0 );
            glEnableVertexAttribArray(locVertex);

            if ( locNormal >= 0 )
            {
                glBindBuffer( GL_ARRAY_BUFFER, vbo[1] );
                glBufferData( GL_ARRAY_BUFFER, 3*nsommets*sizeof(GLfloat), NULL, GL_STATIC_DRAW );
                glBufferSubData( GL_ARRAY_BUFFER, 0, 3*nsommets*sizeof(GLfloat), normales );
                glVertexAttribPointer( locNormal, 3, GL_FLOAT, GL_FALSE, 0, 0 );
                glEnableVertexAttribArray(locNormal);
            }

            if ( locTexCoord >= 0 )
            {
                glBindBuffer( GL_ARRAY_BUFFER, vbo[2] );
                glBufferData( GL_ARRAY_BUFFER, 2*nsommets*sizeof(GLfloat), NULL, GL_STATIC_DRAW );
                glBufferSubData( GL_ARRAY_BUFFER, 0, 2*nsommets*sizeof(GLfloat), texcoord );
                glVertexAttribPointer( locTexCoord, 2, GL_FLOAT, GL_FALSE, 0, 0 );
                glEnableVertexAttribArray(locTexCoord);
            }

            glBindVertexArray( 0 );
        }
    }
    ~FormeQuad()
    {
        glDeleteBuffers( 3, vbo );
    }
    void afficher()
    {
        glBindVertexArray( vao );
        if ( plein_ )
            glDrawArrays( GL_TRIANGLE_STRIP, 0, nsommets_ );
        else
            glDrawArrays( GL_LINE_LOOP, 0, nsommets_ );
        glBindVertexArray( 0 );
    }
private:
    GLint nsommets_;
    GLuint vbo[3];
};
//////////

class FormeTri : public FormeBase2705
{
public:
    FormeTri( GLfloat taille = 1.0,
               bool plein = true )
        : FormeBase2705( plein )
    {
        if ( obtenirAttributs( ) )
        {
            /*         +Y           */
            /*   2+-                */
            /*    | \               */
            /*    |   \             */
            /*    |     \      +X   */
            /*    |       \         */
            /*    |         \       */
            /*    |           \     */
            /*   0+------------+1   */
            /*                      */

            static GLint faces[3] = { 0, 1, 2 };
            static GLfloat n[3] = { 0.0, 0.0, 1.0 };
            GLfloat v[4][3];
            v[2][0] = v[0][0] = 0.0;
            v[1][0] = taille;
            v[0][1] = v[1][1] = 0.0;
            v[2][1] = taille;
            v[2][2] = v[1][2] = v[0][2] = 0.0;
            GLfloat t[4][2];
            t[2][0] = t[0][0] = 0.;
            t[1][0] = 1.;
            t[0][1] = t[1][1] = 0.;
            t[2][1] = 1.;

            // initialisation
            const int TAILLEMAX = 100;
            assert( TAILLEMAX > 6*6 );
            GLfloat sommets[3*TAILLEMAX], normales[3*TAILLEMAX], texcoord[2*TAILLEMAX];
            int nsommets = 0;

            if ( plein_ )
            {
                AJOUTE( v[faces[0]][0], v[faces[0]][1], v[faces[0]][2], n[0], n[1], n[2], t[faces[0]][0], t[faces[0]][1] );
                AJOUTE( v[faces[1]][0], v[faces[1]][1], v[faces[1]][2], n[0], n[1], n[2], t[faces[1]][0], t[faces[1]][1] );
                AJOUTE( v[faces[2]][0], v[faces[2]][1], v[faces[2]][2], n[0], n[1], n[2], t[faces[2]][0], t[faces[2]][1] );
            }
            else
            {
                AJOUTE( v[faces[0]][0], v[faces[0]][1], v[faces[0]][2], n[0], n[1], n[2], t[faces[0]][0], t[faces[0]][1] );
                AJOUTE( v[faces[1]][0], v[faces[1]][1], v[faces[1]][2], n[0], n[1], n[2], t[faces[1]][0], t[faces[1]][1] );
                AJOUTE( v[faces[2]][0], v[faces[2]][1], v[faces[2]][2], n[0], n[1], n[2], t[faces[2]][0], t[faces[2]][1] );
            }
            nsommets_ = nsommets;
            assert( TAILLEMAX >= nsommets );

            // remplir VBOs
            glBindVertexArray( vao );
            glGenBuffers( 3, vbo );

            glBindBuffer( GL_ARRAY_BUFFER, vbo[0] );
            glBufferData( GL_ARRAY_BUFFER, 3*nsommets*sizeof(GLfloat), NULL, GL_STATIC_DRAW );
            glBufferSubData( GL_ARRAY_BUFFER, 0, 3*nsommets*sizeof(GLfloat), sommets );
            glVertexAttribPointer( locVertex, 3, GL_FLOAT, GL_FALSE, 0, 0 );
            glEnableVertexAttribArray(locVertex);

            if ( locNormal >= 0 )
            {
                glBindBuffer( GL_ARRAY_BUFFER, vbo[1] );
                glBufferData( GL_ARRAY_BUFFER, 3*nsommets*sizeof(GLfloat), NULL, GL_STATIC_DRAW );
                glBufferSubData( GL_ARRAY_BUFFER, 0, 3*nsommets*sizeof(GLfloat), normales );
                glVertexAttribPointer( locNormal, 3, GL_FLOAT, GL_FALSE, 0, 0 );
                glEnableVertexAttribArray(locNormal);
            }

            if ( locTexCoord >= 0 )
            {
                glBindBuffer( GL_ARRAY_BUFFER, vbo[2] );
                glBufferData( GL_ARRAY_BUFFER, 2*nsommets*sizeof(GLfloat), NULL, GL_STATIC_DRAW );
                glBufferSubData( GL_ARRAY_BUFFER, 0, 2*nsommets*sizeof(GLfloat), texcoord );
                glVertexAttribPointer( locTexCoord, 2, GL_FLOAT, GL_FALSE, 0, 0 );
                glEnableVertexAttribArray(locTexCoord);
            }

            glBindVertexArray( 0 );
        }
    }
    ~FormeTri()
    {
        glDeleteBuffers( 3, vbo );
    }
    void afficher()
    {
        glBindVertexArray( vao );
        if ( plein_ )
            glDrawArrays( GL_TRIANGLE_STRIP, 0, nsommets_ );
        else
            glDrawArrays( GL_LINE_LOOP, 0, nsommets_ );
        glBindVertexArray( 0 );
    }
private:
    GLint nsommets_;
    GLuint vbo[3];
};

//////////

class FormeCube : public FormeBase2705
{
public:
    FormeCube( GLfloat taille = 1.0,
               bool plein = true )
        : FormeBase2705( plein )
    {
        if ( obtenirAttributs( ) )
        {
            /*         +Y                    */
            /*   3+-----------+2             */
            /*    |\          |\             */
            /*    | \         | \            */
            /*    |  \        |  \           */
            /*    |  7+-----------+6         */
            /*    |   |       |   |          */
            /*    |   |       |   |          */
            /*   0+---|-------+1  |          */
            /*     \  |        \  |     +X   */
            /*      \ |         \ |          */
            /*       \|          \|          */
            /*       4+-----------+5         */
            /*             +Z                */

            static GLint faces[6][4] =
            {
                { 3, 2, 1, 0 },
                { 5, 4, 0, 1 },
                { 6, 5, 1, 2 },
                { 7, 6, 2, 3 },
                { 4, 7, 3, 0 },
                { 4, 5, 6, 7 }
            };
            static GLfloat n[6][3] =
            {
                {  0.0,  0.0, -1.0 },
                {  0.0, -1.0,  0.0 },
                {  1.0,  0.0,  0.0 },
                {  0.0,  1.0,  0.0 },
                { -1.0,  0.0,  0.0 },
                {  0.0,  0.0,  1.0 }
            };
            GLfloat v[8][3];
            v[4][0] = v[7][0] = v[3][0] = v[0][0] = -taille / 2;
            v[6][0] = v[5][0] = v[1][0] = v[2][0] = +taille / 2;
            v[5][1] = v[4][1] = v[0][1] = v[1][1] = -taille / 2;
            v[7][1] = v[6][1] = v[2][1] = v[3][1] = +taille / 2;
            v[3][2] = v[2][2] = v[1][2] = v[0][2] = -taille / 2;
            v[4][2] = v[5][2] = v[6][2] = v[7][2] = +taille / 2;
            GLfloat t[8][2];
            t[4][0] = t[7][0] = t[3][0] = t[0][0] = 0.;
            t[6][0] = t[5][0] = t[1][0] = t[2][0] = 1.;
            t[5][1] = t[4][1] = t[0][1] = t[1][1] = 0.;
            t[7][1] = t[6][1] = t[2][1] = t[3][1] = 1.;

            // initialisation
            const int TAILLEMAX = 100;
            assert( TAILLEMAX > 6*6 );
            GLfloat sommets[3*TAILLEMAX], normales[3*TAILLEMAX], texcoord[2*TAILLEMAX];
            int nsommets = 0;

            if ( plein_ )
            {
                for ( int i = 0 ; i < 6 ; ++i )
                {
                    AJOUTE( v[faces[i][0]][0], v[faces[i][0]][1], v[faces[i][0]][2], n[i][0], n[i][1], n[i][2], t[faces[i][0]][0], t[faces[i][0]][1] );
                    AJOUTE( v[faces[i][1]][0], v[faces[i][1]][1], v[faces[i][1]][2], n[i][0], n[i][1], n[i][2], t[faces[i][1]][0], t[faces[i][1]][1] );
                    AJOUTE( v[faces[i][2]][0], v[faces[i][2]][1], v[faces[i][2]][2], n[i][0], n[i][1], n[i][2], t[faces[i][2]][0], t[faces[i][2]][1] );
                    AJOUTE( v[faces[i][2]][0], v[faces[i][2]][1], v[faces[i][2]][2], n[i][0], n[i][1], n[i][2], t[faces[i][2]][0], t[faces[i][2]][1] );
                    AJOUTE( v[faces[i][3]][0], v[faces[i][3]][1], v[faces[i][3]][2], n[i][0], n[i][1], n[i][2], t[faces[i][3]][0], t[faces[i][3]][1] );
                    AJOUTE( v[faces[i][0]][0], v[faces[i][0]][1], v[faces[i][0]][2], n[i][0], n[i][1], n[i][2], t[faces[i][0]][0], t[faces[i][0]][1] );
                }
            }
            else
            {
                for ( int i = 0 ; i < 6 ; ++i )
                {
                    AJOUTE( v[faces[i][0]][0], v[faces[i][0]][1], v[faces[i][0]][2], n[i][0], n[i][1], n[i][2], t[faces[i][0]][0], t[faces[i][0]][1] );
                    AJOUTE( v[faces[i][1]][0], v[faces[i][1]][1], v[faces[i][1]][2], n[i][0], n[i][1], n[i][2], t[faces[i][1]][0], t[faces[i][1]][1] );
                    AJOUTE( v[faces[i][2]][0], v[faces[i][2]][1], v[faces[i][2]][2], n[i][0], n[i][1], n[i][2], t[faces[i][2]][0], t[faces[i][2]][1] );
                    AJOUTE( v[faces[i][3]][0], v[faces[i][3]][1], v[faces[i][3]][2], n[i][0], n[i][1], n[i][2], t[faces[i][3]][0], t[faces[i][3]][1] );
                }
            }
            nsommets_ = nsommets;
            assert( TAILLEMAX >= nsommets );

            // remplir VBOs
            glBindVertexArray( vao );
            glGenBuffers( 3, vbo );

            glBindBuffer( GL_ARRAY_BUFFER, vbo[0] );
            glBufferData( GL_ARRAY_BUFFER, 3*nsommets*sizeof(GLfloat), NULL, GL_STATIC_DRAW );
            glBufferSubData( GL_ARRAY_BUFFER, 0, 3*nsommets*sizeof(GLfloat), sommets );
            glVertexAttribPointer( locVertex, 3, GL_FLOAT, GL_FALSE, 0, 0 );
            glEnableVertexAttribArray(locVertex);

            if ( locNormal >= 0 )
            {
                glBindBuffer( GL_ARRAY_BUFFER, vbo[1] );
                glBufferData( GL_ARRAY_BUFFER, 3*nsommets*sizeof(GLfloat), NULL, GL_STATIC_DRAW );
                glBufferSubData( GL_ARRAY_BUFFER, 0, 3*nsommets*sizeof(GLfloat), normales );
                glVertexAttribPointer( locNormal, 3, GL_FLOAT, GL_FALSE, 0, 0 );
                glEnableVertexAttribArray(locNormal);
            }

            if ( locTexCoord >= 0 )
            {
                glBindBuffer( GL_ARRAY_BUFFER, vbo[2] );
                glBufferData( GL_ARRAY_BUFFER, 2*nsommets*sizeof(GLfloat), NULL, GL_STATIC_DRAW );
                glBufferSubData( GL_ARRAY_BUFFER, 0, 2*nsommets*sizeof(GLfloat), texcoord );
                glVertexAttribPointer( locTexCoord, 2, GL_FLOAT, GL_FALSE, 0, 0 );
                glEnableVertexAttribArray(locTexCoord);
            }

            glBindVertexArray( 0 );
        }
    }
    ~FormeCube()
    {
        glDeleteBuffers( 3, vbo );
    }
    void afficher()
    {
        glBindVertexArray( vao );
        if ( plein_ )
            glDrawArrays( GL_TRIANGLES, 0, nsommets_ );
        else
            for ( int i = 0 ; i < 6 ; ++i ) glDrawArrays( GL_LINE_LOOP, 4*i, 4 );
        glBindVertexArray( 0 );
    }
private:
    GLint nsommets_;
    GLuint vbo[3];
};

//////////

class FormeSphere : public FormeBase2705
{
public:
    FormeSphere( GLdouble radius, GLint slices, GLint stacks,
                 bool plein = true, bool entiere = true )
        : FormeBase2705( plein )
    {
        if ( obtenirAttributs( ) )
        {
            // initialisation
            const int TAILLEMAX = 10000;
            assert( TAILLEMAX > 2*(slices+1)*(stacks+2) );
            GLfloat sommets[3*TAILLEMAX], normales[3*TAILLEMAX], texcoord[2*TAILLEMAX];
            int nsommets = 0;

            // variables locales
            const GLdouble drho = M_PI / stacks;
            const GLdouble dtheta = 2.0 * M_PI / slices;

            GLint imin, imax;
            if ( locTexCoord >= 0 ) { imin = 0; imax = stacks; } else { imin = 1; imax = stacks - 1; }
            if ( !entiere ) imax = imax/2 + 1; // pour se rendre seulement à la moitié supérieure

            /* texturing: s goes from 0.0/0.25/0.5/0.75/1.0 at +y/+x/-y/-x/+y axis */
            /* t goes from -1.0/+1.0 at z = -radius/+radius (linear along longitudes) */
            /* cannot use triangle fan on texturing (s coord. at top/bottom tip varies) */

            nsommets = 0;
            {
                GLdouble t = 1.0;
                GLdouble ds = 1.0 / slices;
                GLdouble dt = 1.0 / stacks;
                for ( GLint i = imin; i < imax; i++ )
                {
                    GLdouble rho = i * drho;
                    GLdouble s = 0.0;
                    for ( GLint j = 0; j <= slices; j++ )
                    {
                        GLdouble x, y, z;
                        GLdouble theta = (j == slices) ? 0.0 : j * dtheta;
                        x = -sin(theta) * sin(rho);
                        y = cos(theta) * sin(rho);
                        z = cos(rho);
                        AJOUTE( x * radius, y * radius, z * radius, x, y, z, s, t );

                        x = -sin(theta) * sin(rho + drho);
                        y = cos(theta) * sin(rho + drho);
                        z = cos(rho + drho);
                        AJOUTE( x * radius, y * radius, z * radius, x, y, z, s, t-dt );
                        s += ds;
                    }
                    t -= dt;
                }
            }
            nsommetsStrip_ = nsommets;

            if ( !(locTexCoord >= 0) )
            {
                AJOUTE( 0.0, 0.0, radius, 0.0, 0.0, 1.0, 0, 0 );
                for ( GLint j = 0; j <= slices; j++ )
                {
                    GLdouble x, y, z;
                    GLdouble theta = (j == slices) ? 0.0 : j * dtheta;
                    x = -sin(theta) * sin(drho);
                    y = cos(theta) * sin(drho);
                    z = cos(drho);
                    AJOUTE( x * radius, y * radius, z * radius, x, y, z, 0, 0 );
                }
            }
            nsommetsFan_ = nsommets - nsommetsStrip_;

            if ( !(locTexCoord >= 0) && entiere )
            {
                AJOUTE( 0.0, 0.0, -radius, 0.0, 0.0, -1.0, 0, 0 );
                GLdouble rho = M_PI - drho;
                for ( GLint j = slices; j >= 0; j-- )
                {
                    GLdouble x, y, z;
                    GLdouble theta = (j == slices) ? 0.0 : j * dtheta;
                    x = -sin(theta) * sin(rho);
                    y = cos(theta) * sin(rho);
                    z = cos(rho);
                    AJOUTE( x * radius, y * radius, z * radius, x, y, z, 0, 0 );
                }
            }
            assert( TAILLEMAX >= nsommets );

            // remplir VBOs
            glBindVertexArray( vao );
            glGenBuffers( 3, vbo );

            glBindBuffer( GL_ARRAY_BUFFER, vbo[0] );
            glBufferData( GL_ARRAY_BUFFER, 3*nsommets*sizeof(GLfloat), NULL, GL_STATIC_DRAW );
            glBufferSubData( GL_ARRAY_BUFFER, 0, 3*nsommets*sizeof(GLfloat), sommets );
            glVertexAttribPointer( locVertex, 3, GL_FLOAT, GL_FALSE, 0, 0 );
            glEnableVertexAttribArray(locVertex);

            if ( locNormal >= 0 )
            {
                glBindBuffer( GL_ARRAY_BUFFER, vbo[1] );
                glBufferData( GL_ARRAY_BUFFER, 3*nsommets*sizeof(GLfloat), NULL, GL_STATIC_DRAW );
                glBufferSubData( GL_ARRAY_BUFFER, 0, 3*nsommets*sizeof(GLfloat), normales );
                glVertexAttribPointer( locNormal, 3, GL_FLOAT, GL_FALSE, 0, 0 );
                glEnableVertexAttribArray(locNormal);
            }

            if ( locTexCoord >= 0 )
            {
                glBindBuffer( GL_ARRAY_BUFFER, vbo[2] );
                glBufferData( GL_ARRAY_BUFFER, 2*nsommets*sizeof(GLfloat), NULL, GL_STATIC_DRAW );
                glBufferSubData( GL_ARRAY_BUFFER, 0, 2*nsommets*sizeof(GLfloat), texcoord );
                glVertexAttribPointer( locTexCoord, 2, GL_FLOAT, GL_FALSE, 0, 0 );
                glEnableVertexAttribArray(locTexCoord);
            }

            glBindVertexArray( 0 );
        }
    }
    ~FormeSphere()
    {
        glDeleteBuffers( 3, vbo );
    }
    void afficher()
    {
        glBindVertexArray( vao );
        glDrawArrays( GL_TRIANGLE_STRIP, 0, nsommetsStrip_ );
        if ( !(locTexCoord >= 0) )
        {
            glDrawArrays( GL_TRIANGLE_FAN, nsommetsStrip_, nsommetsFan_ );
            glDrawArrays( GL_TRIANGLE_FAN, nsommetsStrip_+nsommetsFan_, nsommetsFan_ );
        }
        glBindVertexArray( 0 );
    }
private:
    GLint nsommetsStrip_, nsommetsFan_;
    GLuint vbo[3];
};

//////////

class FormeTore : public FormeBase2705
{
public:
    FormeTore( GLdouble innerRadius, GLdouble outerRadius, GLint nsides, GLint rings,
               bool plein = true )
        : FormeBase2705( plein )
    {
        if ( obtenirAttributs( ) )
        {
            // initialisation
            const int TAILLEMAX = 10000;
            assert( TAILLEMAX > 2*(nsides+1)*(rings) );
            GLfloat sommets[3*TAILLEMAX], normales[3*TAILLEMAX], texcoord[2*TAILLEMAX];
            int nsommets = 0;

            // variables locales
            const GLdouble ringDelta = 2.0 * M_PI / rings;
            const GLdouble sideDelta = 2.0 * M_PI / nsides;

            GLdouble theta = 0.0;
            GLdouble cosTheta = 1.0;
            GLdouble sinTheta = 0.0;
            for ( int i = rings - 1; i >= 0; i-- )
            {
                GLdouble theta1 = theta + ringDelta;
                GLdouble cosTheta1 = cos(theta1);
                GLdouble sinTheta1 = sin(theta1);
                GLdouble phi = 0.0;
                for ( int j = nsides; j >= 0; j-- )
                {
                    phi += sideDelta;
                    GLdouble cosPhi = cos(phi);
                    GLdouble sinPhi = sin(phi);
                    GLdouble dist = outerRadius + innerRadius * cosPhi;

                    AJOUTE( cosTheta1 * dist, -sinTheta1 * dist, innerRadius * sinPhi,
                            cosTheta1 * cosPhi, -sinTheta1 * cosPhi, sinPhi,
                            (4.0*(i))/rings, (4.0*j)/nsides );

                    AJOUTE( cosTheta * dist, -sinTheta * dist,  innerRadius * sinPhi,
                            cosTheta * cosPhi, -sinTheta * cosPhi, sinPhi,
                            (4.0*(i+1))/rings, (4.0*j)/nsides );
                }
                theta = theta1;
                cosTheta = cosTheta1;
                sinTheta = sinTheta1;
            }
            nsommets_ = nsommets;
            assert( TAILLEMAX >= nsommets );

            // remplir VBOs
            glBindVertexArray( vao );
            glGenBuffers( 3, vbo );

            glBindBuffer( GL_ARRAY_BUFFER, vbo[0] );
            glBufferData( GL_ARRAY_BUFFER, 3*nsommets*sizeof(GLfloat), NULL, GL_STATIC_DRAW );
            glBufferSubData( GL_ARRAY_BUFFER, 0, 3*nsommets*sizeof(GLfloat), sommets );
            glVertexAttribPointer( locVertex, 3, GL_FLOAT, GL_FALSE, 0, 0 );
            glEnableVertexAttribArray(locVertex);

            if ( locNormal >= 0 )
            {
                glBindBuffer( GL_ARRAY_BUFFER, vbo[1] );
                glBufferData( GL_ARRAY_BUFFER, 3*nsommets*sizeof(GLfloat), NULL, GL_STATIC_DRAW );
                glBufferSubData( GL_ARRAY_BUFFER, 0, 3*nsommets*sizeof(GLfloat), normales );
                glVertexAttribPointer( locNormal, 3, GL_FLOAT, GL_FALSE, 0, 0 );
                glEnableVertexAttribArray(locNormal);
            }

            if ( locTexCoord >= 0 )
            {
                glBindBuffer( GL_ARRAY_BUFFER, vbo[2] );
                glBufferData( GL_ARRAY_BUFFER, 2*nsommets*sizeof(GLfloat), NULL, GL_STATIC_DRAW );
                glBufferSubData( GL_ARRAY_BUFFER, 0, 2*nsommets*sizeof(GLfloat), texcoord );
                glVertexAttribPointer( locTexCoord, 2, GL_FLOAT, GL_FALSE, 0, 0 );
                glEnableVertexAttribArray(locTexCoord);
            }

            glBindVertexArray( 0 );
        }
    }
    ~FormeTore()
    {
        glDeleteBuffers( 3, vbo );
    }
    void afficher()
    {
        glBindVertexArray( vao );
        glDrawArrays( GL_TRIANGLE_STRIP, 0, nsommets_ );
        glBindVertexArray( 0 );
    }
private:
    GLint nsommets_;
    GLuint vbo[3];
};

//////////

class FormeCylindre : public FormeBase2705
{
public:
    FormeCylindre( GLdouble base, GLdouble top, GLdouble height, GLint slices, GLint stacks,
                   bool plein = true )
        : FormeBase2705( plein )
    {
        if ( obtenirAttributs( ) )
        {
            // initialisation
            const int TAILLEMAX = 10000;
            assert( TAILLEMAX > 2*(slices+1)*(stacks) + 2*(slices+1) + 2 );
            GLfloat sommets[3*TAILLEMAX], normales[3*TAILLEMAX], texcoord[2*TAILLEMAX];
            int nsommets = 0;

            // variables locales
            const GLdouble da = 2.0 * M_PI / slices;
            {
                // le cylindre
                const GLdouble dr = (top - base) / stacks;
                const GLdouble nz = (base - top) / height;
                const GLdouble dz = height / stacks;
                const GLdouble ds = 1.0 / slices;
                const GLdouble dt = 1.0 / stacks;
                GLdouble t = 0.0;
                GLdouble z = 0.0;
                GLdouble r = base;
                for ( int j = 0; j < stacks; j++ )
                {
                    GLdouble s = 0.0;
                    for ( int i = 0; i <= slices; i++ )
                    {
                        GLdouble a = ( i == slices ) ? 0.0 : i * da;
                        GLdouble x = sin( a );
                        GLdouble y = cos( a );
                        AJOUTE( x * r, y * r, z,  x, y, nz, s, t );
                        AJOUTE( x * (r + dr), y * (r + dr), z + dz, x, y, nz, s, t + dt );
                        s += ds;
                    }
                    r += dr;
                    t += dt;
                    z += dz;
                }
                nsommetsCyl_ = nsommets;
            }
            {
                // les deux bouts avec des disques
                /* texture of a shapesDisk is a cut out of the texture unit square
                 * x, y in [-outerRadius, +outerRadius]; s, t in [0, 1] (linear mapping) */
                AJOUTE( 0.0, 0.0, 0.0,  0.0, 0.0, -1.0,  0.5, 0.5 );
                for ( int i = slices; i >= 0; i-- )
                {
                    GLdouble a = ( i == slices ) ? 0.0 : i * -da;
                    GLdouble x = sin( a );
                    GLdouble y = cos( a );
                    AJOUTE( base*x, base*y, 0.0,  0.0, 0.0, -1.0,  0.5*(1.0-x), 0.5*(1.0+y) );
                }
                nsommetsBout_ = nsommets - nsommetsCyl_;
                AJOUTE( 0.0, 0.0, height,  0.0, 0.0, +1.0,  0.5, 0.5 );
                for ( int i = slices; i >= 0; i-- )
                {
                    GLdouble a = ( i == slices ) ? 0.0 : i * da;
                    GLdouble x = sin( a );
                    GLdouble y = cos( a );
                    AJOUTE( top*x, top*y, height,  0.0, 0.0, +1.0,  0.5*(1.0-x), 0.5*(1.0+y) );
                }
            }
            assert( TAILLEMAX >= nsommets );

            // remplir VBOs
            glBindVertexArray( vao );
            glGenBuffers( 3, vbo );

            glBindBuffer( GL_ARRAY_BUFFER, vbo[0] );
            glBufferData( GL_ARRAY_BUFFER, 3*nsommets*sizeof(GLfloat), NULL, GL_STATIC_DRAW );
            glBufferSubData( GL_ARRAY_BUFFER, 0, 3*nsommets*sizeof(GLfloat), sommets );
            glVertexAttribPointer( locVertex, 3, GL_FLOAT, GL_FALSE, 0, 0 );
            glEnableVertexAttribArray(locVertex);

            if ( locNormal >= 0 )
            {
                glBindBuffer( GL_ARRAY_BUFFER, vbo[1] );
                glBufferData( GL_ARRAY_BUFFER, 3*nsommets*sizeof(GLfloat), NULL, GL_STATIC_DRAW );
                glBufferSubData( GL_ARRAY_BUFFER, 0, 3*nsommets*sizeof(GLfloat), normales );
                glVertexAttribPointer( locNormal, 3, GL_FLOAT, GL_FALSE, 0, 0 );
                glEnableVertexAttribArray(locNormal);
            }

            if ( locTexCoord >= 0 )
            {
                glBindBuffer( GL_ARRAY_BUFFER, vbo[2] );
                glBufferData( GL_ARRAY_BUFFER, 2*nsommets*sizeof(GLfloat), NULL, GL_STATIC_DRAW );
                glBufferSubData( GL_ARRAY_BUFFER, 0, 2*nsommets*sizeof(GLfloat), texcoord );
                glVertexAttribPointer( locTexCoord, 2, GL_FLOAT, GL_FALSE, 0, 0 );
                glEnableVertexAttribArray(locTexCoord);
            }

            glBindVertexArray( 0 );
        }
    }
    ~FormeCylindre()
    {
        glDeleteBuffers( 3, vbo );
    }
    void afficher()
    {
        glBindVertexArray( vao );
        glDrawArrays( GL_TRIANGLE_STRIP, 0, nsommetsCyl_ );
        glDrawArrays( GL_TRIANGLE_FAN, nsommetsCyl_, nsommetsBout_ );
        glDrawArrays( GL_TRIANGLE_FAN, nsommetsCyl_+nsommetsBout_, nsommetsBout_ );
        glBindVertexArray( 0 );
    }
private:
    GLint nsommetsCyl_, nsommetsBout_;
    GLuint vbo[3];
};

//////////

class FormeDisque : public FormeBase2705
{
public:
    FormeDisque( GLdouble inner, GLdouble outer, GLint slices, GLint loops,
                 bool plein = true )
        : FormeBase2705( plein )
    {
        if ( obtenirAttributs( ) )
        {
            // initialisation
            const int TAILLEMAX = 10000;
            assert( TAILLEMAX > 2*(slices+1)*(loops) );
            GLfloat sommets[3*TAILLEMAX], normales[3*TAILLEMAX], texcoord[2*TAILLEMAX];
            int nsommets = 0;

            // variables locales
            const GLdouble da = 2.0 * M_PI / slices;
            const GLdouble dr = (outer - inner) / loops;

            /* texture of a shapesDisk is a cut out of the texture unit square
             * x, y in [-outer, +outer]; s, t in [0, 1] * (linear mapping) */
            GLdouble r1 = inner;
            for ( int l = 0; l < loops; l++ )
            {
                GLdouble r2 = r1 + dr;
                for ( int i = slices; i >= 0; i-- )
                {
                    GLdouble a = ( i == slices ) ? 0.0 : i * da;
                    GLdouble x = sin( a );
                    GLdouble y = cos( a );
                    AJOUTE( r2*x, r2*y, 0.0, 0.0, 0.0, +1.0, 0.5*( 1.0 - x*r2/outer ), 0.5*( 1.0 + y*r2/outer ) );
                    AJOUTE( r1*x, r1*y, 0.0, 0.0, 0.0, +1.0, 0.5*( 1.0 - x*r1/outer ), 0.5*( 1.0 + y*r1/outer ) );
                }
                r1 = r2;
            }
            nsommets_ = nsommets;
            assert( TAILLEMAX >= nsommets );

            // remplir VBOs
            glBindVertexArray( vao );
            glGenBuffers( 3, vbo );

            glBindBuffer( GL_ARRAY_BUFFER, vbo[0] );
            glBufferData( GL_ARRAY_BUFFER, 3*nsommets*sizeof(GLfloat), NULL, GL_STATIC_DRAW );
            glBufferSubData( GL_ARRAY_BUFFER, 0, 3*nsommets*sizeof(GLfloat), sommets );
            glVertexAttribPointer( locVertex, 3, GL_FLOAT, GL_FALSE, 0, 0 );
            glEnableVertexAttribArray(locVertex);

            if ( locNormal >= 0 )
            {
                glBindBuffer( GL_ARRAY_BUFFER, vbo[1] );
                glBufferData( GL_ARRAY_BUFFER, 3*nsommets*sizeof(GLfloat), NULL, GL_STATIC_DRAW );
                glBufferSubData( GL_ARRAY_BUFFER, 0, 3*nsommets*sizeof(GLfloat), normales );
                glVertexAttribPointer( locNormal, 3, GL_FLOAT, GL_FALSE, 0, 0 );
                glEnableVertexAttribArray(locNormal);
            }

            if ( locTexCoord >= 0 )
            {
                glBindBuffer( GL_ARRAY_BUFFER, vbo[2] );
                glBufferData( GL_ARRAY_BUFFER, 2*nsommets*sizeof(GLfloat), NULL, GL_STATIC_DRAW );
                glBufferSubData( GL_ARRAY_BUFFER, 0, 2*nsommets*sizeof(GLfloat), texcoord );
                glVertexAttribPointer( locTexCoord, 2, GL_FLOAT, GL_FALSE, 0, 0 );
                glEnableVertexAttribArray(locTexCoord);
            }

            glBindVertexArray( 0 );
        }
    }
    ~FormeDisque()
    {
        glDeleteBuffers( 3, vbo );
    }
    void afficher()
    {
        glBindVertexArray( vao );
        glDrawArrays( GL_TRIANGLE_STRIP, 0, nsommets_ );
        glBindVertexArray( 0 );
    }
private:
    GLint nsommets_;
    GLuint vbo[3];
};

//////////

class FormeIcosaedre : public FormeBase2705
{
public:
    FormeIcosaedre( bool plein = true )
        : FormeBase2705( plein )
    {
    }
    ~FormeIcosaedre()
    {
    }
    void afficher()
    {
#if defined( AVEC_OPENGL2)
        shapesSolidIcosahedron( );
#endif
    }
private:
};

//////////

class FormeDodecaedre : public FormeBase2705
{
public:
    FormeDodecaedre( bool plein = true )
        : FormeBase2705( plein )
    {
    }
    ~FormeDodecaedre()
    {
    }
    void afficher()
    {
#if defined( AVEC_OPENGL2)
        shapesSolidDodecahedron( );
#endif
    }
private:
};

//////////
#include <stdio.h>
#include <string>
#include <vector>

class FormeObj : public FormeBase2705
{
public:
    FormeObj( const char *fichierObj,
              bool plein = true )
        : FormeBase2705( plein ), nsommets_(0), nconnec_(0), min(0.0), max(0.0)
    {
        if ( obtenirAttributs( ) )
        {
            std::vector<float> sommetsObj;
            std::vector<unsigned int> connecObj;
            if ( lireOBJ( fichierObj, sommetsObj, connecObj ) )
            {
                nsommets_ = (GLint)(sommetsObj.size()/3);
                float *sommets = new float[sommetsObj.size()];
                {
                    int i = 0;
                    std::vector<float>::iterator ite;
                    for( ite = sommetsObj.begin(); ite != sommetsObj.end(); ite++ ) { sommets[i++] = *ite; }
                }
                for ( int i = 0 ; i < nsommets_ ; ++i )
                {
                    if ( min.x > sommets[3*i+0] ) min.x = sommets[3*i+0];
                    else if ( max.x < sommets[3*i+0] ) max.x = sommets[3*i+0];
                    if ( min.y > sommets[3*i+1] ) min.y = sommets[3*i+1];
                    else if ( max.y < sommets[3*i+1] ) max.y = sommets[3*i+1];
                    if ( min.z > sommets[3*i+2] ) min.z = sommets[3*i+2];
                    else if ( max.z < sommets[3*i+2] ) max.z = sommets[3*i+2];
                }
                std::cout << "//@inf2705-forme," << __LINE__ << ";"
                          << " min=" <<  glm::to_string(min)
                          << " max=" <<  glm::to_string(max) << std::endl;

                nconnec_ = (GLint)(connecObj.size()/3);
                unsigned int *connec = new unsigned int[connecObj.size()];
                {
                    int i = 0;
                    std::vector<unsigned int>::iterator ite;
                    for( ite = connecObj.begin(); ite != connecObj.end(); ite++ ) { connec[i++] = *ite-1; }
                }
                // for ( int i = 0 ; i < nconnec_ ; ++i )
                // {
                //    //  if ( connec[i] <= 0 ) std::cerr << "//@inf2705-forme," << __LINE__ << ";" << " i=" << i << " connec[i]=" << connec[i] << std::endl;
                //    if ( connec[i] >= (unsigned int) (nsommets_-14) ) std::cerr << "//@inf2705-forme," << __LINE__ << ";" << " i=" << i << " connec[i]=" << connec[i] << std::endl;
                // }

                // remplir VBOs
                glBindVertexArray( vao );
                glGenBuffers( 4, vbo );

                glBindBuffer( GL_ARRAY_BUFFER, vbo[0] );
                glBufferData( GL_ARRAY_BUFFER, 3*nsommets_*sizeof(GLfloat), NULL, GL_STATIC_DRAW );
                glBufferSubData( GL_ARRAY_BUFFER, 0, 3*nsommets_*sizeof(GLfloat), sommets );
                glVertexAttribPointer( locVertex, 3, GL_FLOAT, GL_FALSE, 0, 0 );
                glEnableVertexAttribArray(locVertex);

                // if ( locNormal >= 0 )
                // {
                //    glBindBuffer( GL_ARRAY_BUFFER, vbo[1] );
                //    glBufferData( GL_ARRAY_BUFFER, 3*nsommets*sizeof(GLfloat), NULL, GL_STATIC_DRAW );
                //    glBufferSubData( GL_ARRAY_BUFFER, 0, 3*nsommets*sizeof(GLfloat), normales );
                //    glVertexAttribPointer( locNormal, 3, GL_FLOAT, GL_FALSE, 0, 0 );
                //    glEnableVertexAttribArray(locNormal);
                // }

                // if ( locTexCoord >= 0 )
                // {
                //    glBindBuffer( GL_ARRAY_BUFFER, vbo[2] );
                //    glBufferData( GL_ARRAY_BUFFER, 2*nsommets*sizeof(GLfloat), NULL, GL_STATIC_DRAW );
                //    glBufferSubData( GL_ARRAY_BUFFER, 0, 2*nsommets*sizeof(GLfloat), texcoord );
                //    glVertexAttribPointer( locTexCoord, 2, GL_FLOAT, GL_FALSE, 0, 0 );
                //    glEnableVertexAttribArray(locTexCoord);
                // }

                glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, vbo[3] );
                glBufferData( GL_ELEMENT_ARRAY_BUFFER, 3*nconnec_*sizeof(unsigned int), connec, GL_STATIC_DRAW );

                glBindVertexArray( 0 );

                delete [] sommets;
                delete [] connec;
            }
        }
    }
    ~FormeObj()
    {
        glDeleteBuffers( 4, vbo );
    }
    void afficher()
    {
        glBindVertexArray( vao );
        glDrawElements( GL_TRIANGLES, 3*nconnec_, GL_UNSIGNED_INT, 0 );
        glBindVertexArray( 0 );
    }
private:
    bool lireOBJ( const char *fichierObj, std::vector<float> & sommetsObj, std::vector<unsigned int> & connecObj )
    {
        printf( "Lecture de \"%s\" ...\n", fichierObj );

        FILE *fp = fopen( fichierObj, "r" );
        if ( fp == NULL )
        {
            perror( fichierObj );
            return false;
        }

        while( 1 )
        {
            char type[128];
            if ( fscanf( fp, "%s", type ) == EOF ) break; // c'est terminé

            char ligne[1000];
            fgets( ligne, 1000, fp );

            if ( strcmp( type, "v" ) == 0  )
            {
                float x[3];
                /*int nlu =*/ sscanf( ligne, "%f %f %f\n", &x[0], &x[1], &x[2] );
                sommetsObj.push_back(x[0]);
                sommetsObj.push_back(x[1]);
                sommetsObj.push_back(x[2]);
            }
            else if ( strcmp( type, "f" ) == 0  )
            {
                unsigned int vertexIndex[3];
                int nlu = sscanf( ligne, "%u/%*d/%*d %u/%*d/%*d %u/%*d/%*d\n", &vertexIndex[0], &vertexIndex[1], &vertexIndex[2] );
                if ( nlu != 3 )
                    nlu = sscanf( ligne, "%u//%*d %u//%*d %u//%*d\n", &vertexIndex[0], &vertexIndex[1], &vertexIndex[2] );
                if ( nlu != 3 )
                    nlu = sscanf( ligne, "%u %u %u\n", &vertexIndex[0], &vertexIndex[1], &vertexIndex[2] );
                if ( nlu != 3 )
                {
                    printf("Incapable de lire la connectivité :-(\n");
                    return false;
                }
                connecObj.push_back( vertexIndex[0] );
                connecObj.push_back( vertexIndex[1] );
                connecObj.push_back( vertexIndex[2] );
            }
            // else if ( strcmp( type, "vt" ) == 0  )
            // {
            // }
            // else if ( strcmp( type, "vn" ) == 0  )
            // {
            // }
        }

        fclose(fp);
        printf( "Lecture de \"%s\" terminé.\n", fichierObj );
        return true;
    }

    GLint nsommets_;
    GLint nconnec_;
    glm::vec3 min, max;
    GLuint vbo[4];
};

//////////

// théière avec base en (0,0,0), bec orienté en (1,0,0) et haut orienté en (0,1,0)
class FormeTheiere : public FormeBase2705
{
public:
    FormeTheiere( )
        : FormeBase2705( ) //, npts_(npts)
    {
        if ( obtenirAttributs( ) )
        {
            // initialisation

            // initialiser le VAO pour la théière
            glBindVertexArray( vao );
            glGenBuffers( 4, vbo );

            // créer le VBO pour les sommets
            glBindBuffer( GL_ARRAY_BUFFER, vbo[0] );
            glBufferData( GL_ARRAY_BUFFER, sizeof(gTheiereSommets), NULL, GL_STATIC_DRAW );
            glBufferSubData( GL_ARRAY_BUFFER, 0, sizeof(gTheiereSommets), gTheiereSommets );
            glVertexAttribPointer( locVertex, 3, GL_FLOAT, GL_FALSE, 0, 0 );
            glEnableVertexAttribArray(locVertex);

            if ( locNormal >= 0 )
            {
                glBindBuffer( GL_ARRAY_BUFFER, vbo[1] );
                glBufferData( GL_ARRAY_BUFFER, sizeof(gTheiereNormales), NULL, GL_STATIC_DRAW );
                glBufferSubData( GL_ARRAY_BUFFER, 0, sizeof(gTheiereNormales), gTheiereNormales );
                glVertexAttribPointer( locNormal, 3, GL_FLOAT, GL_FALSE, 0, 0 );
                glEnableVertexAttribArray(locNormal);
            }

            if ( locTexCoord >= 0 ) // on prend les normales, c'est joli et pas si mal :)
            {
                glBindBuffer( GL_ARRAY_BUFFER, vbo[2] );
                glBufferData( GL_ARRAY_BUFFER, sizeof(gTheiereNormales), NULL, GL_STATIC_DRAW );
                glBufferSubData( GL_ARRAY_BUFFER, 0, sizeof(gTheiereNormales), gTheiereNormales );
                glVertexAttribPointer( locTexCoord, 3, GL_FLOAT, GL_FALSE, 0, 0 );
                glEnableVertexAttribArray(locTexCoord);
            }

            // créer le VBO la connectivité
            glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, vbo[3] );
            glBufferData( GL_ELEMENT_ARRAY_BUFFER, sizeof(gTheiereConnec), gTheiereConnec, GL_STATIC_DRAW );

            glBindVertexArray(0);
        }
    }
    ~FormeTheiere()
    {
        glDeleteBuffers( 4, vbo );
    }
    void afficher()
    {
        glBindVertexArray( vao );
        glDrawElements( GL_TRIANGLES, sizeof(gTheiereConnec)/sizeof(GLuint), GL_UNSIGNED_INT, 0 );
        glBindVertexArray(0);
    }
private:

    GLuint vbo[4];
    // de glut_teapot :
    // les coordonnées des 530 sommets
    const GLfloat gTheiereSommets[530*3] =
    {
2.1, 3.6, 0.0,
2.071, 3.711, 0.0,
2.105, 3.748, 0.0,
2.174, 3.711, 0.0,
2.25, 3.6, 0.0,
1.937, 3.6, 0.8242,
1.91, 3.711, 0.8128,
1.942, 3.748, 0.8261,
2.005, 3.711, 0.8532,
2.076, 3.6, 0.8831,
1.491, 3.6, 1.491,
1.47, 3.711, 1.47,
1.494, 3.748, 1.494,
1.543, 3.711, 1.543,
1.597, 3.6, 1.597,
0.8242, 3.6, 1.937,
0.8128, 3.711, 1.91,
0.8261, 3.748, 1.942,
0.8532, 3.711, 2.005,
0.8831, 3.6, 2.076,
0.0, 3.6, 2.1,
0.0, 3.711, 2.071,
0.0, 3.748, 2.105,
0.0, 3.711, 2.174,
0.0, 3.6, 2.25,
-0.8812, 3.6, 1.937,
-0.8368, 3.711, 1.91,
-0.8332, 3.748, 1.942,
-0.8541, 3.711, 2.005,
-0.8831, 3.6, 2.076,
-1.542, 3.6, 1.491,
-1.492, 3.711, 1.47,
-1.501, 3.748, 1.494,
-1.544, 3.711, 1.543,
-1.597, 3.6, 1.597,
-1.956, 3.6, 0.8242,
-1.918, 3.711, 0.8128,
-1.944, 3.748, 0.8261,
-2.006, 3.711, 0.8532,
-2.076, 3.6, 0.8831,
-2.1, 3.6, 0.0,
-2.071, 3.711, 0.0,
-2.105, 3.748, 0.0,
-2.174, 3.711, 0.0,
-2.25, 3.6, 0.0,
-1.937, 3.6, -0.8242,
-1.91, 3.711, -0.8128,
-1.942, 3.748, -0.8261,
-2.005, 3.711, -0.8532,
-2.076, 3.6, -0.8831,
-1.491, 3.6, -1.491,
-1.47, 3.711, -1.47,
-1.494, 3.748, -1.494,
-1.543, 3.711, -1.543,
-1.597, 3.6, -1.597,
-0.8242, 3.6, -1.937,
-0.8128, 3.711, -1.91,
-0.8261, 3.748, -1.942,
-0.8532, 3.711, -2.005,
-0.8831, 3.6, -2.076,
0.0, 3.6, -2.1,
0.0, 3.711, -2.071,
0.0, 3.748, -2.105,
0.0, 3.711, -2.174,
0.0, 3.6, -2.25,
0.8242, 3.6, -1.937,
0.8128, 3.711, -1.91,
0.8261, 3.748, -1.942,
0.8532, 3.711, -2.005,
0.8831, 3.6, -2.076,
1.491, 3.6, -1.491,
1.47, 3.711, -1.47,
1.494, 3.748, -1.494,
1.543, 3.711, -1.543,
1.597, 3.6, -1.597,
1.937, 3.6, -0.8242,
1.91, 3.711, -0.8128,
1.942, 3.748, -0.8261,
2.005, 3.711, -0.8532,
2.076, 3.6, -0.8831,
2.525, 3.011, 0.0,
2.766, 2.433, 0.0,
2.936, 1.876, 0.0,
3.0, 1.35, 0.0,
2.33, 3.011, 0.9912,
2.551, 2.433, 1.086,
2.708, 1.876, 1.152,
2.767, 1.35, 1.178,
1.793, 3.011, 1.793,
1.964, 2.433, 1.964,
2.084, 1.876, 2.084,
2.13, 1.35, 2.13,
0.9912, 3.011, 2.33,
1.086, 2.433, 2.551,
1.152, 1.876, 2.708,
1.178, 1.35, 2.767,
0.0, 3.011, 2.525,
0.0, 2.433, 2.766,
0.0, 1.876, 2.936,
0.0, 1.35, 3.0,
-0.9912, 3.011, 2.33,
-1.086, 2.433, 2.551,
-1.152, 1.876, 2.708,
-1.178, 1.35, 2.767,
-1.793, 3.011, 1.793,
-1.964, 2.433, 1.964,
-2.084, 1.876, 2.084,
-2.13, 1.35, 2.13,
-2.33, 3.011, 0.9912,
-2.551, 2.433, 1.086,
-2.708, 1.876, 1.152,
-2.767, 1.35, 1.178,
-2.525, 3.011, 0.0,
-2.766, 2.433, 0.0,
-2.936, 1.876, 0.0,
-3.0, 1.35, 0.0,
-2.33, 3.011, -0.9912,
-2.551, 2.433, -1.086,
-2.708, 1.876, -1.152,
-2.767, 1.35, -1.178,
-1.793, 3.011, -1.793,
-1.964, 2.433, -1.964,
-2.084, 1.876, -2.084,
-2.13, 1.35, -2.13,
-0.9912, 3.011, -2.33,
-1.086, 2.433, -2.551,
-1.152, 1.876, -2.708,
-1.178, 1.35, -2.767,
0.0, 3.011, -2.525,
0.0, 2.433, -2.766,
0.0, 1.876, -2.936,
0.0, 1.35, -3.0,
0.9912, 3.011, -2.33,
1.086, 2.433, -2.551,
1.152, 1.876, -2.708,
1.178, 1.35, -2.767,
1.793, 3.011, -1.793,
1.964, 2.433, -1.964,
2.084, 1.876, -2.084,
2.13, 1.35, -2.13,
2.33, 3.011, -0.9912,
2.551, 2.433, -1.086,
2.708, 1.876, -1.152,
2.767, 1.35, -1.178,
2.883, 0.9053, 0.0,
2.625, 0.5766, 0.0,
2.367, 0.3533, 0.0,
2.25, 0.225, 0.0,
2.659, 0.9053, 1.132,
2.422, 0.5766, 1.03,
2.184, 0.3533, 0.9291,
2.076, 0.225, 0.8831,
2.047, 0.9053, 2.047,
1.864, 0.5766, 1.864,
1.681, 0.3533, 1.681,
1.597, 0.225, 1.597,
1.132, 0.9053, 2.659,
1.03, 0.5766, 2.422,
0.9291, 0.3533, 2.184,
0.8831, 0.225, 2.076,
0.0, 0.9053, 2.883,
0.0, 0.5766, 2.625,
0.0, 0.3533, 2.367,
0.0, 0.225, 2.25,
-1.132, 0.9053, 2.659,
-1.03, 0.5766, 2.422,
-0.9291, 0.3533, 2.184,
-0.8831, 0.225, 2.076,
-2.047, 0.9053, 2.047,
-1.864, 0.5766, 1.864,
-1.681, 0.3533, 1.681,
-1.597, 0.225, 1.597,
-2.659, 0.9053, 1.132,
-2.422, 0.5766, 1.03,
-2.184, 0.3533, 0.9291,
-2.076, 0.225, 0.8831,
-2.883, 0.9053, 0.0,
-2.625, 0.5766, 0.0,
-2.367, 0.3533, 0.0,
-2.25, 0.225, 0.0,
-2.659, 0.9053, -1.132,
-2.422, 0.5766, -1.03,
-2.184, 0.3533, -0.9291,
-2.076, 0.225, -0.8831,
-2.047, 0.9053, -2.047,
-1.864, 0.5766, -1.864,
-1.681, 0.3533, -1.681,
-1.597, 0.225, -1.597,
-1.132, 0.9053, -2.659,
-1.03, 0.5766, -2.422,
-0.9291, 0.3533, -2.184,
-0.8831, 0.225, -2.076,
0.0, 0.9053, -2.883,
0.0, 0.5766, -2.625,
0.0, 0.3533, -2.367,
0.0, 0.225, -2.25,
1.132, 0.9053, -2.659,
1.03, 0.5766, -2.422,
0.9291, 0.3533, -2.184,
0.8831, 0.225, -2.076,
2.047, 0.9053, -2.047,
1.864, 0.5766, -1.864,
1.681, 0.3533, -1.681,
1.597, 0.225, -1.597,
2.659, 0.9053, -1.132,
2.422, 0.5766, -1.03,
2.184, 0.3533, -0.9291,
2.076, 0.225, -0.8831,
2.199, 0.1424, 0.0,
1.927, 0.07031, 0.0,
1.253, 0.01934, 0.0,
0.0, 0.0, 0.0,
2.029, 0.1424, 0.8631,
1.777, 0.07031, 0.7562,
1.156, 0.01934, 0.4919,
1.561, 0.1424, 1.561,
1.368, 0.07031, 1.368,
0.8899, 0.01934, 0.8899,
0.8631, 0.1424, 2.029,
0.7562, 0.07031, 1.777,
0.4919, 0.01934, 1.156,
0.0, 0.1424, 2.199,
0.0, 0.07031, 1.927,
0.0, 0.01934, 1.253,
-0.8631, 0.1424, 2.029,
-0.7562, 0.07031, 1.777,
-0.4919, 0.01934, 1.156,
-1.561, 0.1424, 1.561,
-1.368, 0.07031, 1.368,
-0.8899, 0.01934, 0.8899,
-2.029, 0.1424, 0.8631,
-1.777, 0.07031, 0.7562,
-1.156, 0.01934, 0.4919,
-2.199, 0.1424, 0.0,
-1.927, 0.07031, 0.0,
-1.253, 0.01934, 0.0,
-2.029, 0.1424, -0.8631,
-1.777, 0.07031, -0.7562,
-1.156, 0.01934, -0.4919,
-1.561, 0.1424, -1.561,
-1.368, 0.07031, -1.368,
-0.8899, 0.01934, -0.8899,
-0.8631, 0.1424, -2.029,
-0.7562, 0.07031, -1.777,
-0.4919, 0.01934, -1.156,
0.0, 0.1424, -2.199,
0.0, 0.07031, -1.927,
0.0, 0.01934, -1.253,
0.8631, 0.1424, -2.029,
0.7562, 0.07031, -1.777,
0.4919, 0.01934, -1.156,
1.561, 0.1424, -1.561,
1.368, 0.07031, -1.368,
0.8899, 0.01934, -0.8899,
2.029, 0.1424, -0.8631,
1.777, 0.07031, -0.7562,
1.156, 0.01934, -0.4919,
-2.4, 3.038, 0.0,
-3.101, 3.032, 0.0,
-3.619, 2.995, 0.0,
-3.94, 2.895, 0.0,
-4.05, 2.7, 0.0,
-2.377, 3.09, 0.2531,
-3.122, 3.084, 0.2531,
-3.669, 3.041, 0.2531,
-4.005, 2.926, 0.2531,
-4.12, 2.7, 0.2531,
-2.325, 3.206, 0.3375,
-3.168, 3.198, 0.3375,
-3.778, 3.143, 0.3375,
-4.15, 2.993, 0.3375,
-4.275, 2.7, 0.3375,
-2.273, 3.322, 0.2531,
-3.214, 3.313, 0.2531,
-3.888, 3.244, 0.2531,
-4.294, 3.06, 0.2531,
-4.43, 2.7, 0.2531,
-2.25, 3.375, 0.0,
-3.234, 3.364, 0.0,
-3.938, 3.291, 0.0,
-4.359, 3.09, 0.0,
-4.5, 2.7, 0.0,
-2.273, 3.322, -0.2531,
-3.214, 3.313, -0.2531,
-3.888, 3.244, -0.2531,
-4.294, 3.06, -0.2531,
-4.43, 2.7, -0.2531,
-2.325, 3.206, -0.3375,
-3.168, 3.198, -0.3375,
-3.778, 3.143, -0.3375,
-4.15, 2.993, -0.3375,
-4.275, 2.7, -0.3375,
-2.377, 3.09, -0.2531,
-3.122, 3.084, -0.2531,
-3.669, 3.041, -0.2531,
-4.005, 2.926, -0.2531,
-4.12, 2.7, -0.2531,
-3.991, 2.394, 0.0,
-3.806, 2.025, 0.0,
-3.48, 1.656, 0.0,
-3.0, 1.35, 0.0,
-4.055, 2.365, 0.2531,
-3.852, 1.98, 0.2531,
-3.496, 1.6, 0.2531,
-2.977, 1.28, 0.2531,
-4.196, 2.3, 0.3375,
-3.952, 1.881, 0.3375,
-3.531, 1.478, 0.3375,
-2.925, 1.125, 0.3375,
-4.336, 2.235, 0.2531,
-4.051, 1.782, 0.2531,
-3.566, 1.356, 0.2531,
-2.873, 0.9703, 0.2531,
-4.4, 2.205, 0.0,
-4.097, 1.737, 0.0,
-3.582, 1.3, 0.0,
-2.85, 0.9, 0.0,
-4.336, 2.235, -0.2531,
-4.051, 1.782, -0.2531,
-3.566, 1.356, -0.2531,
-2.873, 0.9703, -0.2531,
-4.196, 2.3, -0.3375,
-3.952, 1.881, -0.3375,
-3.531, 1.478, -0.3375,
-2.925, 1.125, -0.3375,
-4.055, 2.365, -0.2531,
-3.852, 1.98, -0.2531,
-3.496, 1.6, -0.2531,
-2.977, 1.28, -0.2531,
2.55, 2.137, 0.0,
3.27, 2.303, 0.0,
3.581, 2.7, 0.0,
3.752, 3.182, 0.0,
4.05, 3.6, 0.0,
2.55, 1.944, 0.5569,
3.324, 2.159, 0.5028,
3.652, 2.617, 0.3839,
3.838, 3.151, 0.265,
4.191, 3.6, 0.2109,
2.55, 1.519, 0.7425,
3.445, 1.844, 0.6704,
3.806, 2.433, 0.5119,
4.027, 3.085, 0.3533,
4.5, 3.6, 0.2813,
2.55, 1.093, 0.5569,
3.566, 1.529, 0.5028,
3.961, 2.249, 0.3839,
4.215, 3.018, 0.265,
4.809, 3.6, 0.2109,
2.55, 0.9, 0.0,
3.621, 1.385, 0.0,
4.031, 2.166, 0.0,
4.301, 2.988, 0.0,
4.95, 3.6, 0.0,
2.55, 1.093, -0.5569,
3.566, 1.529, -0.5028,
3.961, 2.249, -0.3839,
4.215, 3.018, -0.265,
4.809, 3.6, -0.2109,
2.55, 1.519, -0.7425,
3.445, 1.844, -0.6704,
3.806, 2.433, -0.5119,
4.027, 3.085, -0.3533,
4.5, 3.6, -0.2813,
2.55, 1.944, -0.5569,
3.324, 2.159, -0.5028,
3.652, 2.617, -0.3839,
3.838, 3.151, -0.265,
4.191, 3.6, -0.2109,
4.158, 3.663, 0.0,
4.238, 3.684, 0.0,
4.261, 3.663, 0.0,
4.2, 3.6, 0.0,
4.308, 3.666, 0.1978,
4.379, 3.689, 0.1687,
4.381, 3.668, 0.1397,
4.294, 3.6, 0.1266,
4.64, 3.673, 0.2637,
4.69, 3.7, 0.225,
4.645, 3.677, 0.1863,
4.5, 3.6, 0.1688,
4.971, 3.68, 0.1978,
5.001, 3.711, 0.1687,
4.909, 3.687, 0.1397,
4.706, 3.6, 0.1266,
5.122, 3.683, 0.0,
5.142, 3.716, 0.0,
5.029, 3.691, 0.0,
4.8, 3.6, 0.0,
4.971, 3.68, -0.1978,
5.001, 3.711, -0.1687,
4.909, 3.687, -0.1397,
4.706, 3.6, -0.1266,
4.64, 3.673, -0.2637,
4.69, 3.7, -0.225,
4.645, 3.677, -0.1863,
4.5, 3.6, -0.1688,
4.308, 3.666, -0.1978,
4.379, 3.689, -0.1687,
4.381, 3.668, -0.1397,
4.294, 3.6, -0.1266,
0.0, 4.725, 0.0,
0.5109, 4.651, 0.0,
0.4875, 4.472, 0.0,
0.2953, 4.25, 0.0,
0.3, 4.05, 0.0,
0.4715, 4.651, 0.2011,
0.4499, 4.472, 0.1918,
0.2725, 4.25, 0.1161,
0.2768, 4.05, 0.1178,
0.3632, 4.651, 0.3632,
0.3465, 4.472, 0.3465,
0.2098, 4.25, 0.2098,
0.213, 4.05, 0.213,
0.2011, 4.651, 0.4715,
0.1918, 4.472, 0.4499,
0.1161, 4.25, 0.2725,
0.1178, 4.05, 0.2768,
0.0, 4.651, 0.5109,
0.0, 4.472, 0.4875,
0.0, 4.25, 0.2953,
0.0, 4.05, 0.3,
-0.2011, 4.651, 0.4715,
-0.1918, 4.472, 0.4499,
-0.1161, 4.25, 0.2725,
-0.1178, 4.05, 0.2768,
-0.3632, 4.651, 0.3632,
-0.3465, 4.472, 0.3465,
-0.2098, 4.25, 0.2098,
-0.213, 4.05, 0.213,
-0.4715, 4.651, 0.2011,
-0.4499, 4.472, 0.1918,
-0.2725, 4.25, 0.1161,
-0.2768, 4.05, 0.1178,
-0.5109, 4.651, 0.0,
-0.4875, 4.472, 0.0,
-0.2953, 4.25, 0.0,
-0.3, 4.05, 0.0,
-0.4715, 4.651, -0.2011,
-0.4499, 4.472, -0.1918,
-0.2725, 4.25, -0.1161,
-0.2768, 4.05, -0.1178,
-0.3632, 4.651, -0.3632,
-0.3465, 4.472, -0.3465,
-0.2098, 4.25, -0.2098,
-0.213, 4.05, -0.213,
-0.2011, 4.651, -0.4715,
-0.1918, 4.472, -0.4499,
-0.1161, 4.25, -0.2725,
-0.1178, 4.05, -0.2768,
0.0, 4.651, -0.5109,
0.0, 4.472, -0.4875,
0.0, 4.25, -0.2953,
0.0, 4.05, -0.3,
0.2011, 4.651, -0.4715,
0.1918, 4.472, -0.4499,
0.1161, 4.25, -0.2725,
0.1178, 4.05, -0.2768,
0.3632, 4.651, -0.3632,
0.3465, 4.472, -0.3465,
0.2098, 4.25, -0.2098,
0.213, 4.05, -0.213,
0.4715, 4.651, -0.2011,
0.4499, 4.472, -0.1918,
0.2725, 4.25, -0.1161,
0.2768, 4.05, -0.1178,
0.6844, 3.916, 0.0,
1.237, 3.825, 0.0,
1.734, 3.734, 0.0,
1.95, 3.6, 0.0,
0.6313, 3.916, 0.2686,
1.142, 3.825, 0.4857,
1.6, 3.734, 0.6807,
1.799, 3.6, 0.7654,
0.4859, 3.916, 0.4859,
0.8786, 3.825, 0.8786,
1.231, 3.734, 1.231,
1.385, 3.6, 1.385,
0.2686, 3.916, 0.6313,
0.4857, 3.825, 1.142,
0.6807, 3.734, 1.6,
0.7654, 3.6, 1.799,
0.0, 3.916, 0.6844,
0.0, 3.825, 1.237,
0.0, 3.734, 1.734,
0.0, 3.6, 1.95,
-0.2686, 3.916, 0.6313,
-0.4857, 3.825, 1.142,
-0.6807, 3.734, 1.6,
-0.7654, 3.6, 1.799,
-0.4859, 3.916, 0.4859,
-0.8786, 3.825, 0.8786,
-1.231, 3.734, 1.231,
-1.385, 3.6, 1.385,
-0.6313, 3.916, 0.2686,
-1.142, 3.825, 0.4857,
-1.6, 3.734, 0.6807,
-1.799, 3.6, 0.7654,
-0.6844, 3.916, 0.0,
-1.237, 3.825, 0.0,
-1.734, 3.734, 0.0,
-1.95, 3.6, 0.0,
-0.6313, 3.916, -0.2686,
-1.142, 3.825, -0.4857,
-1.6, 3.734, -0.6807,
-1.799, 3.6, -0.7654,
-0.4859, 3.916, -0.4859,
-0.8786, 3.825, -0.8786,
-1.231, 3.734, -1.231,
-1.385, 3.6, -1.385,
-0.2686, 3.916, -0.6313,
-0.4857, 3.825, -1.142,
-0.6807, 3.734, -1.6,
-0.7654, 3.6, -1.799,
0.0, 3.916, -0.6844,
0.0, 3.825, -1.237,
0.0, 3.734, -1.734,
0.0, 3.6, -1.95,
0.2686, 3.916, -0.6313,
0.4857, 3.825, -1.142,
0.6807, 3.734, -1.6,
0.7654, 3.6, -1.799,
0.4859, 3.916, -0.4859,
0.8786, 3.825, -0.8786,
1.231, 3.734, -1.231,
1.385, 3.6, -1.385,
0.6313, 3.916, -0.2686,
1.142, 3.825, -0.4857,
1.6, 3.734, -0.6807,
1.799, 3.6, -0.7654
    };

    // les normales des 530 sommets
    const GLfloat gTheiereNormales[530*3] =
    {
-0.9647, -0.2552, -0.0635,
-0.9701, 0.2423, 0.0084,
-0.1648, 0.985, 0.0501,
0.6655, 0.7462, 0.0117,
0.8677, 0.4969, 0.0026,
-0.8666, -0.2559, -0.4282,
-0.8995, 0.2429, -0.3629,
-0.1717, 0.985, -0.0162,
0.6096, 0.747, 0.265,
0.8003, 0.4978, 0.334,
-0.6369, -0.2562, -0.727,
-0.6919, 0.2428, -0.6798,
-0.1518, 0.985, -0.081,
0.4612, 0.7474, 0.478,
0.6111, 0.4984, 0.6148,
-0.3094, -0.2556, -0.9159,
-0.3787, 0.2424, -0.8931,
-0.1096, 0.985, -0.1324,
0.243, 0.747, 0.6187,
0.329, 0.4978, 0.8023,
0.0569, -0.2712, -0.9608,
-0.0092, 0.2338, -0.9722,
-0.0505, 0.9852, -0.1635,
-0.0117, 0.7461, 0.6656,
-0.0026, 0.4969, 0.8677,
0.4086, -0.3626, -0.8375,
0.3709, 0.1524, -0.916,
0.0244, 0.9805, -0.1949,
-0.2671, 0.7429, 0.6137,
-0.3341, 0.4972, 0.8006,
0.7041, -0.3863, -0.5956,
0.7058, 0.0906, -0.7025,
0.1278, 0.9696, -0.2085,
-0.4848, 0.7385, 0.4683,
-0.6154, 0.497, 0.6117,
0.9074, -0.3096, -0.2839,
0.9159, 0.1555, -0.3699,
0.1833, 0.9735, -0.1365,
-0.6252, 0.7406, 0.246,
-0.8029, 0.4968, 0.3293,
0.9643, -0.2551, 0.0705,
0.9737, 0.2276, -0.0003,
0.179, 0.9825, -0.0513,
-0.6671, 0.7448, -0.0116,
-0.8679, 0.4966, -0.0026,
0.8666, -0.2559, 0.4282,
0.8995, 0.2429, 0.3629,
0.1717, 0.985, 0.0162,
-0.6096, 0.747, -0.265,
-0.8003, 0.4978, -0.334,
0.6369, -0.2562, 0.727,
0.6919, 0.2428, 0.6798,
0.1518, 0.985, 0.081,
-0.4612, 0.7474, -0.478,
-0.6111, 0.4984, -0.6148,
0.3094, -0.2556, 0.9159,
0.3787, 0.2424, 0.8931,
0.1096, 0.985, 0.1324,
-0.243, 0.747, -0.6187,
-0.329, 0.4978, -0.8023,
-0.0635, -0.2552, 0.9647,
0.0084, 0.2423, 0.9701,
0.0501, 0.985, 0.1648,
0.0117, 0.7462, -0.6655,
0.0026, 0.4969, -0.8677,
-0.4282, -0.2559, 0.8666,
-0.3629, 0.2429, 0.8995,
-0.0162, 0.985, 0.1717,
0.265, 0.747, -0.6096,
0.334, 0.4978, -0.8003,
-0.727, -0.2562, 0.6369,
-0.6798, 0.2428, 0.6919,
-0.081, 0.985, 0.1518,
0.478, 0.7474, -0.4612,
0.6148, 0.4984, -0.6111,
-0.9159, -0.2556, 0.3094,
-0.8931, 0.2424, 0.3787,
-0.1324, 0.985, 0.1096,
0.6187, 0.747, -0.243,
0.8023, 0.4978, -0.329,
0.9148, 0.4037, 0.0005,
0.9411, 0.338, 0.001,
0.9782, 0.2075, 0.0011,
0.9976, -0.0678, -0.0008,
0.8448, 0.4045, 0.3501,
0.8689, 0.3387, 0.3607,
0.9033, 0.2079, 0.375,
0.9221, -0.0681, 0.3807,
0.646, 0.4051, 0.6468,
0.6644, 0.3392, 0.6659,
0.6907, 0.2083, 0.6924,
0.706, -0.0681, 0.7048,
0.3491, 0.4045, 0.8452,
0.3587, 0.3388, 0.8697,
0.3729, 0.208, 0.9042,
0.3822, -0.0678, 0.9215,
-0.0005, 0.4037, 0.9148,
-0.001, 0.338, 0.9411,
-0.0011, 0.2075, 0.9782,
0.0008, -0.0678, 0.9976,
-0.3501, 0.4045, 0.8448,
-0.3607, 0.3387, 0.8689,
-0.375, 0.2079, 0.9033,
-0.3807, -0.0681, 0.9221,
-0.6468, 0.4051, 0.646,
-0.6659, 0.3392, 0.6644,
-0.6924, 0.2083, 0.6907,
-0.7048, -0.0681, 0.706,
-0.8452, 0.4045, 0.3491,
-0.8697, 0.3388, 0.3587,
-0.9042, 0.208, 0.3729,
-0.9215, -0.0678, 0.3822,
-0.9148, 0.4037, -0.0005,
-0.9411, 0.338, -0.001,
-0.9782, 0.2075, -0.0011,
-0.9976, -0.0678, 0.0008,
-0.8448, 0.4045, -0.3501,
-0.8689, 0.3387, -0.3607,
-0.9033, 0.2079, -0.375,
-0.9221, -0.0681, -0.3807,
-0.646, 0.4051, -0.6468,
-0.6644, 0.3392, -0.6659,
-0.6907, 0.2083, -0.6924,
-0.706, -0.0681, -0.7048,
-0.3491, 0.4045, -0.8452,
-0.3587, 0.3388, -0.8697,
-0.3729, 0.208, -0.9042,
-0.3822, -0.0678, -0.9215,
0.0005, 0.4037, -0.9148,
0.001, 0.338, -0.9411,
0.0011, 0.2075, -0.9782,
-0.0008, -0.0678, -0.9976,
0.3501, 0.4045, -0.8448,
0.3607, 0.3387, -0.8689,
0.375, 0.2079, -0.9033,
0.3807, -0.0681, -0.9221,
0.6468, 0.4051, -0.646,
0.6659, 0.3392, -0.6644,
0.6924, 0.2083, -0.6907,
0.7048, -0.0681, -0.706,
0.8452, 0.4045, -0.3491,
0.8697, 0.3388, -0.3587,
0.9042, 0.208, -0.3729,
0.9215, -0.0678, -0.3822,
0.8951, -0.4457, -0.0058,
0.7239, -0.6898, -0.0042,
0.6976, -0.7164, 0.0027,
0.7981, -0.6024, 0.0036,
0.829, -0.4466, 0.3365,
0.6698, -0.6907, 0.2723,
0.6428, -0.7172, 0.2688,
0.7354, -0.6034, 0.3082,
0.6366, -0.447, 0.6283,
0.5139, -0.6913, 0.5078,
0.4903, -0.7178, 0.4942,
0.5609, -0.604, 0.5661,
0.3476, -0.4464, 0.8245,
0.2803, -0.6907, 0.6665,
0.2637, -0.7172, 0.6449,
0.3014, -0.6034, 0.7382,
0.0058, -0.4457, 0.8951,
0.0042, -0.6898, 0.7239,
-0.0027, -0.7164, 0.6976,
-0.0036, -0.6024, 0.7981,
-0.3365, -0.4466, 0.829,
-0.2723, -0.6907, 0.6698,
-0.2688, -0.7172, 0.6428,
-0.3082, -0.6034, 0.7354,
-0.6283, -0.447, 0.6366,
-0.5078, -0.6913, 0.5139,
-0.4942, -0.7178, 0.4903,
-0.5661, -0.604, 0.5609,
-0.8245, -0.4464, 0.3476,
-0.6665, -0.6907, 0.2803,
-0.6449, -0.7172, 0.2637,
-0.7382, -0.6034, 0.3014,
-0.8951, -0.4457, 0.0058,
-0.7239, -0.6898, 0.0042,
-0.6976, -0.7164, -0.0027,
-0.7981, -0.6024, -0.0036,
-0.829, -0.4466, -0.3365,
-0.6698, -0.6907, -0.2723,
-0.6428, -0.7172, -0.2688,
-0.7354, -0.6034, -0.3082,
-0.6366, -0.447, -0.6283,
-0.5139, -0.6913, -0.5078,
-0.4903, -0.7178, -0.4942,
-0.5609, -0.604, -0.5661,
-0.3476, -0.4464, -0.8245,
-0.2803, -0.6907, -0.6665,
-0.2637, -0.7172, -0.6449,
-0.3014, -0.6034, -0.7382,
-0.0058, -0.4457, -0.8951,
-0.0042, -0.6898, -0.7239,
0.0027, -0.7164, -0.6976,
0.0036, -0.6024, -0.7981,
0.3365, -0.4466, -0.829,
0.2723, -0.6907, -0.6698,
0.2688, -0.7172, -0.6428,
0.3082, -0.6034, -0.7354,
0.6283, -0.447, -0.6366,
0.5078, -0.6913, -0.5139,
0.4942, -0.7178, -0.4903,
0.5661, -0.604, -0.5609,
0.8245, -0.4464, -0.3476,
0.6665, -0.6907, -0.2803,
0.6449, -0.7172, -0.2637,
0.7382, -0.6034, -0.3014,
0.5936, -0.8044, -0.0208,
0.1662, -0.986, -0.0059,
0.0514, -0.9986, -0.0029,
0, -1, 0,
0.556, -0.8049, 0.2071,
0.1556, -0.9861, 0.0578,
0.0486, -0.9986, 0.0168,
0.4337, -0.8052, 0.4041,
0.1213, -0.9861, 0.1128,
0.0383, -0.9986, 0.0341,
0.2461, -0.805, 0.5397,
0.0689, -0.9861, 0.1509,
0.0223, -0.9986, 0.0462,
0.0208, -0.8044, 0.5936,
0.0059, -0.986, 0.1662,
0.0029, -0.9986, 0.0514,
-0.2071, -0.8049, 0.556,
-0.0578, -0.9861, 0.1556,
-0.0168, -0.9986, 0.0486,
-0.4041, -0.8052, 0.4337,
-0.1128, -0.9861, 0.1213,
-0.0341, -0.9986, 0.0383,
-0.5397, -0.805, 0.2461,
-0.1509, -0.9861, 0.0689,
-0.0462, -0.9986, 0.0223,
-0.5936, -0.8044, 0.0208,
-0.1662, -0.986, 0.0059,
-0.0514, -0.9986, 0.0029,
-0.556, -0.8049, -0.2071,
-0.1556, -0.9861, -0.0578,
-0.0486, -0.9986, -0.0168,
-0.4337, -0.8052, -0.4041,
-0.1213, -0.9861, -0.1128,
-0.0383, -0.9986, -0.0341,
-0.2461, -0.805, -0.5397,
-0.0689, -0.9861, -0.1509,
-0.0223, -0.9986, -0.0462,
-0.0208, -0.8044, -0.5936,
-0.0059, -0.986, -0.1662,
-0.0029, -0.9986, -0.0514,
0.2071, -0.8049, -0.556,
0.0578, -0.9861, -0.1556,
0.0168, -0.9986, -0.0486,
0.4041, -0.8052, -0.4337,
0.1128, -0.9861, -0.1213,
0.0341, -0.9986, -0.0383,
0.5397, -0.805, -0.2461,
0.1509, -0.9861, -0.0689,
0.0462, -0.9986, -0.0223,
0.0077, -0.9976, 0.0684,
0.0405, -0.9991, -0.0018,
0.1912, -0.9815, -0.0025,
0.6355, -0.772, 0.0006,
0.9879, -0.1547, -0.0033,
0.0065, -0.7656, 0.6431,
0.035, -0.8413, 0.5393,
0.163, -0.8182, 0.5512,
0.4857, -0.6243, 0.6117,
0.762, -0.146, 0.6308,
-0.0024, 0.2379, 0.9712,
-0.0114, 0.0022, 0.9999,
-0.0391, -0.0113, 0.9991,
-0.0591, -0.0663, 0.996,
-0.01, -0.0605, 0.9981,
-0.0094, 0.903, 0.4295,
-0.0502, 0.8385, 0.5425,
-0.2257, 0.795, 0.5629,
-0.5856, 0.5198, 0.6218,
-0.7759, 0.0432, 0.6292,
-0.0105, 0.9975, -0.0685,
-0.0568, 0.9983, 0.0009,
-0.2667, 0.9637, -0.001,
-0.7355, 0.6774, -0.0026,
-0.997, 0.0763, 0.0019,
-0.0077, 0.7656, -0.6432,
-0.0425, 0.8398, -0.5411,
-0.1985, 0.8026, -0.5623,
-0.5406, 0.5651, -0.623,
-0.7731, 0.0861, -0.6283,
0.0019, -0.2377, -0.9713,
0.0085, -0.0031, -0.9999,
0.0282, 0.0023, -0.9995,
0.047, 0.0466, -0.9978,
0.0089, 0.0486, -0.9987,
0.0071, -0.9029, -0.4296,
0.0375, -0.8401, -0.5411,
0.173, -0.8158, -0.5518,
0.5337, -0.5852, -0.6104,
0.7711, -0.0741, -0.6323,
0.9456, 0.3251, -0.0052,
0.8236, 0.5668, -0.0138,
0.6441, 0.7648, -0.0112,
0.5315, 0.8438, -0.0735,
0.7463, 0.254, 0.615,
0.6657, 0.4653, 0.5832,
0.54, 0.6414, 0.5448,
0.4821, 0.7724, 0.4133,
0.0056, -0.0358, 0.9993,
0.0058, -0.0438, 0.999,
0.0179, -0.0369, 0.9991,
0.1233, 0.199, 0.9722,
-0.7216, -0.3071, 0.6204,
-0.6023, -0.5297, 0.5971,
-0.4734, -0.6798, 0.5599,
-0.3762, -0.6681, 0.6418,
-0.9269, -0.3751, 0.0038,
-0.7543, -0.6564, 0.0105,
-0.5694, -0.8219, 0.0081,
-0.4801, -0.8741, 0.0733,
-0.7367, -0.275, -0.6176,
-0.623, -0.5156, -0.5881,
-0.4856, -0.6774, -0.5524,
-0.4373, -0.7921, -0.4257,
-0.0024, 0.0291, -0.9995,
0.0062, 0.0309, -0.9995,
-0.0015, 0.0295, -0.9995,
-0.109, -0.1969, -0.9743,
0.7383, 0.2702, -0.6179,
0.6486, 0.4756, -0.5941,
0.5153, 0.6516, -0.5564,
0.3993, 0.6582, -0.6381,
-0.2299, 0.9679, 0.1004,
-0.5396, 0.8418, -0.0094,
-0.8707, 0.4915, 0.0133,
-0.8758, 0.4808, 0.0415,
-0.6543, 0.7561, 0.0071,
-0.1316, 0.6339, 0.762,
-0.3232, 0.6846, 0.6533,
-0.6088, 0.4806, 0.6311,
-0.6164, 0.4198, 0.6661,
-0.4472, 0.5952, 0.6675,
0.13, -0.1396, 0.9816,
0.1846, 0.1002, 0.9776,
0.142, 0.1881, 0.9718,
0.0123, 0.1386, 0.9902,
-0.0084, 0.1303, 0.9914,
0.3582, -0.7656, 0.5343,
0.5874, -0.5124, 0.6263,
0.7836, -0.2411, 0.5724,
0.6588, -0.3366, 0.6727,
0.4259, -0.5474, 0.7203,
0.4089, -0.9069, -0.1015,
0.6894, -0.7242, 0.0091,
0.9241, -0.3816, -0.0169,
0.8523, -0.5219, -0.0338,
0.5758, -0.8175, -0.0017,
0.2934, -0.5928, -0.7499,
0.5642, -0.5426, -0.6222,
0.7706, -0.2424, -0.5893,
0.6637, -0.2967, -0.6865,
0.4479, -0.5145, -0.7311,
0.0286, 0.1639, -0.986,
0.0911, 0.0518, -0.9944,
0.1635, 0.1348, -0.9772,
0.1093, 0.1415, -0.9838,
0.0396, 0.153, -0.9874,
-0.1796, 0.814, -0.5522,
-0.3969, 0.6342, -0.6634,
-0.6215, 0.4454, -0.6443,
-0.5876, 0.4472, -0.6742,
-0.4119, 0.6135, -0.6736,
-0.3724, 0.9278, -0.0202,
0.1921, 0.9724, -0.1316,
0.9994, 0.0313, 0.0095,
0.6882, -0.703, 0.179,
-0.2727, 0.8505, 0.4496,
0.0178, 0.9861, -0.1651,
0.4543, 0.2732, -0.8478,
0.5171, -0.4876, -0.7033,
-0.0081, 0.5671, 0.8235,
-0.0522, 0.9982, 0.0278,
-0.0213, 0.6301, -0.7761,
0.0724, 0.1198, -0.9901,
0.4921, -0.247, 0.8346,
0.1084, 0.9431, 0.314,
-0.2274, 0.8822, -0.4122,
-0.2483, 0.6806, -0.6892,
0.6888, -0.7219, -0.0648,
0.6044, 0.7397, 0.2957,
-0.293, 0.9558, 0.0209,
-0.37, 0.9238, -0.0973,
0.4909, -0.1864, -0.8509,
0.3734, 0.876, -0.3051,
-0.2198, 0.8494, 0.4796,
-0.3131, 0.8154, 0.4868,
0.0262, 0.5183, -0.8547,
0.0349, 0.9969, -0.0698,
0.0123, 0.5735, 0.819,
-0.05, 0.3155, 0.9475,
-0.2496, 0.8321, -0.4951,
0.148, 0.9838, 0.1002,
0.4863, 0.2366, 0.8411,
0.3547, -0.3173, 0.8794,
0, 1, 0,
0.8938, 0.4452, 0.0532,
0.9116, -0.4109, -0.0077,
0.9402, -0.3403, 0.0085,
0.8044, 0.5934, -0.0262,
0.8055, 0.4452, 0.3909,
0.8453, -0.4121, 0.3399,
0.8655, -0.3412, 0.3666,
0.7532, 0.5939, 0.2824,
0.5938, 0.4451, 0.6702,
0.6497, -0.4127, 0.6383,
0.6585, -0.342, 0.6703,
0.5875, 0.5937, 0.5497,
0.2904, 0.4454, 0.8468,
0.3548, -0.4118, 0.8393,
0.3508, -0.3415, 0.8719,
0.3321, 0.5936, 0.733,
-0.0532, 0.4452, 0.8938,
0.0077, -0.4109, 0.9116,
-0.0085, -0.3403, 0.9402,
0.0262, 0.5934, 0.8044,
-0.3909, 0.4452, 0.8055,
-0.3399, -0.4121, 0.8453,
-0.3666, -0.3412, 0.8655,
-0.2824, 0.5939, 0.7532,
-0.6702, 0.4451, 0.5938,
-0.6383, -0.4127, 0.6497,
-0.6703, -0.342, 0.6585,
-0.5497, 0.5937, 0.5875,
-0.8468, 0.4454, 0.2904,
-0.8393, -0.4118, 0.3548,
-0.8719, -0.3415, 0.3508,
-0.733, 0.5936, 0.3321,
-0.8938, 0.4452, -0.0532,
-0.9116, -0.4109, 0.0077,
-0.9402, -0.3403, -0.0085,
-0.8044, 0.5934, 0.0262,
-0.8055, 0.4452, -0.3909,
-0.8453, -0.4121, -0.3399,
-0.8655, -0.3412, -0.3666,
-0.7532, 0.5939, -0.2824,
-0.5938, 0.4451, -0.6702,
-0.6497, -0.4127, -0.6383,
-0.6585, -0.342, -0.6703,
-0.5875, 0.5937, -0.5497,
-0.2904, 0.4454, -0.8468,
-0.3548, -0.4118, -0.8393,
-0.3508, -0.3415, -0.8719,
-0.3321, 0.5936, -0.733,
0.0532, 0.4452, -0.8938,
-0.0077, -0.4109, -0.9116,
0.0085, -0.3403, -0.9402,
-0.0262, 0.5934, -0.8044,
0.3909, 0.4452, -0.8055,
0.3399, -0.4121, -0.8453,
0.3666, -0.3412, -0.8655,
0.2824, 0.5939, -0.7532,
0.6702, 0.4451, -0.5938,
0.6383, -0.4127, -0.6497,
0.6703, -0.342, -0.6585,
0.5497, 0.5937, -0.5875,
0.8468, 0.4454, -0.2904,
0.8393, -0.4118, -0.3548,
0.8719, -0.3415, -0.3508,
0.733, 0.5936, -0.3321,
0.2464, 0.9691, -0.0054,
0.1719, 0.985, 0.0005,
0.3595, 0.933, 0.0115,
0.5263, 0.8495, -0.0346,
0.2294, 0.9692, 0.0888,
0.1583, 0.9851, 0.066,
0.3271, 0.9333, 0.1478,
0.4991, 0.8499, 0.1684,
0.1774, 0.9693, 0.1697,
0.1207, 0.9852, 0.1215,
0.2452, 0.9334, 0.2615,
0.3955, 0.8505, 0.3465,
0.099, 0.9692, 0.2251,
0.065, 0.9851, 0.1587,
0.1263, 0.9332, 0.3361,
0.233, 0.8502, 0.4719,
0.0054, 0.9691, 0.2464,
-0.0005, 0.985, 0.1719,
-0.0115, 0.933, 0.3595,
0.0346, 0.8495, 0.5263,
-0.0888, 0.9692, 0.2294,
-0.066, 0.9851, 0.1583,
-0.1478, 0.9333, 0.3271,
-0.1684, 0.8499, 0.4991,
-0.1697, 0.9693, 0.1774,
-0.1215, 0.9852, 0.1207,
-0.2615, 0.9334, 0.2452,
-0.3465, 0.8505, 0.3955,
-0.2251, 0.9692, 0.099,
-0.1587, 0.9851, 0.065,
-0.3361, 0.9332, 0.1263,
-0.4719, 0.8502, 0.233,
-0.2464, 0.9691, 0.0054,
-0.1719, 0.985, -0.0005,
-0.3595, 0.933, -0.0115,
-0.5263, 0.8495, 0.0346,
-0.2294, 0.9692, -0.0888,
-0.1583, 0.9851, -0.066,
-0.3271, 0.9333, -0.1478,
-0.4991, 0.8499, -0.1684,
-0.1774, 0.9693, -0.1697,
-0.1207, 0.9852, -0.1215,
-0.2452, 0.9334, -0.2615,
-0.3955, 0.8505, -0.3465,
-0.099, 0.9692, -0.2251,
-0.065, 0.9851, -0.1587,
-0.1263, 0.9332, -0.3361,
-0.233, 0.8502, -0.4719,
-0.0054, 0.9691, -0.2464,
0.0005, 0.985, -0.1719,
0.0115, 0.933, -0.3595,
-0.0346, 0.8495, -0.5263,
0.0888, 0.9692, -0.2294,
0.066, 0.9851, -0.1583,
0.1478, 0.9333, -0.3271,
0.1684, 0.8499, -0.4991,
0.1697, 0.9693, -0.1774,
0.1215, 0.9852, -0.1207,
0.2615, 0.9334, -0.2452,
0.3465, 0.8505, -0.3955,
0.2251, 0.9692, -0.099,
0.1587, 0.9851, -0.065,
0.3361, 0.9332, -0.1263,
0.4719, 0.8502, -0.233,
    };

    // les faces des 1024 triangles
    const GLuint gTheiereConnec[1024*3] =
    {
0, 5, 6,
6, 1, 0,
1, 6, 7,
7, 2, 1,
2, 7, 8,
8, 3, 2,
3, 8, 9,
9, 4, 3,
5, 10, 11,
11, 6, 5,
6, 11, 12,
12, 7, 6,
7, 12, 13,
13, 8, 7,
8, 13, 14,
14, 9, 8,
10, 15, 16,
16, 11, 10,
11, 16, 17,
17, 12, 11,
12, 17, 18,
18, 13, 12,
13, 18, 19,
19, 14, 13,
15, 20, 21,
21, 16, 15,
16, 21, 22,
22, 17, 16,
17, 22, 23,
23, 18, 17,
18, 23, 24,
24, 19, 18,
20, 25, 26,
26, 21, 20,
21, 26, 27,
27, 22, 21,
22, 27, 28,
28, 23, 22,
23, 28, 29,
29, 24, 23,
25, 30, 31,
31, 26, 25,
26, 31, 32,
32, 27, 26,
27, 32, 33,
33, 28, 27,
28, 33, 34,
34, 29, 28,
30, 35, 36,
36, 31, 30,
31, 36, 37,
37, 32, 31,
32, 37, 38,
38, 33, 32,
33, 38, 39,
39, 34, 33,
35, 40, 41,
41, 36, 35,
36, 41, 42,
42, 37, 36,
37, 42, 43,
43, 38, 37,
38, 43, 44,
44, 39, 38,
40, 45, 46,
46, 41, 40,
41, 46, 47,
47, 42, 41,
42, 47, 48,
48, 43, 42,
43, 48, 49,
49, 44, 43,
45, 50, 51,
51, 46, 45,
46, 51, 52,
52, 47, 46,
47, 52, 53,
53, 48, 47,
48, 53, 54,
54, 49, 48,
50, 55, 56,
56, 51, 50,
51, 56, 57,
57, 52, 51,
52, 57, 58,
58, 53, 52,
53, 58, 59,
59, 54, 53,
55, 60, 61,
61, 56, 55,
56, 61, 62,
62, 57, 56,
57, 62, 63,
63, 58, 57,
58, 63, 64,
64, 59, 58,
60, 65, 66,
66, 61, 60,
61, 66, 67,
67, 62, 61,
62, 67, 68,
68, 63, 62,
63, 68, 69,
69, 64, 63,
65, 70, 71,
71, 66, 65,
66, 71, 72,
72, 67, 66,
67, 72, 73,
73, 68, 67,
68, 73, 74,
74, 69, 68,
70, 75, 76,
76, 71, 70,
71, 76, 77,
77, 72, 71,
72, 77, 78,
78, 73, 72,
73, 78, 79,
79, 74, 73,
75, 0, 1,
1, 76, 75,
76, 1, 2,
2, 77, 76,
77, 2, 3,
3, 78, 77,
78, 3, 4,
4, 79, 78,
4, 9, 84,
84, 80, 4,
80, 84, 85,
85, 81, 80,
81, 85, 86,
86, 82, 81,
82, 86, 87,
87, 83, 82,
9, 14, 88,
88, 84, 9,
84, 88, 89,
89, 85, 84,
85, 89, 90,
90, 86, 85,
86, 90, 91,
91, 87, 86,
14, 19, 92,
92, 88, 14,
88, 92, 93,
93, 89, 88,
89, 93, 94,
94, 90, 89,
90, 94, 95,
95, 91, 90,
19, 24, 96,
96, 92, 19,
92, 96, 97,
97, 93, 92,
93, 97, 98,
98, 94, 93,
94, 98, 99,
99, 95, 94,
24, 29, 100,
100, 96, 24,
96, 100, 101,
101, 97, 96,
97, 101, 102,
102, 98, 97,
98, 102, 103,
103, 99, 98,
29, 34, 104,
104, 100, 29,
100, 104, 105,
105, 101, 100,
101, 105, 106,
106, 102, 101,
102, 106, 107,
107, 103, 102,
34, 39, 108,
108, 104, 34,
104, 108, 109,
109, 105, 104,
105, 109, 110,
110, 106, 105,
106, 110, 111,
111, 107, 106,
39, 44, 112,
112, 108, 39,
108, 112, 113,
113, 109, 108,
109, 113, 114,
114, 110, 109,
110, 114, 115,
115, 111, 110,
44, 49, 116,
116, 112, 44,
112, 116, 117,
117, 113, 112,
113, 117, 118,
118, 114, 113,
114, 118, 119,
119, 115, 114,
49, 54, 120,
120, 116, 49,
116, 120, 121,
121, 117, 116,
117, 121, 122,
122, 118, 117,
118, 122, 123,
123, 119, 118,
54, 59, 124,
124, 120, 54,
120, 124, 125,
125, 121, 120,
121, 125, 126,
126, 122, 121,
122, 126, 127,
127, 123, 122,
59, 64, 128,
128, 124, 59,
124, 128, 129,
129, 125, 124,
125, 129, 130,
130, 126, 125,
126, 130, 131,
131, 127, 126,
64, 69, 132,
132, 128, 64,
128, 132, 133,
133, 129, 128,
129, 133, 134,
134, 130, 129,
130, 134, 135,
135, 131, 130,
69, 74, 136,
136, 132, 69,
132, 136, 137,
137, 133, 132,
133, 137, 138,
138, 134, 133,
134, 138, 139,
139, 135, 134,
74, 79, 140,
140, 136, 74,
136, 140, 141,
141, 137, 136,
137, 141, 142,
142, 138, 137,
138, 142, 143,
143, 139, 138,
79, 4, 80,
80, 140, 79,
140, 80, 81,
81, 141, 140,
141, 81, 82,
82, 142, 141,
142, 82, 83,
83, 143, 142,
83, 87, 148,
148, 144, 83,
144, 148, 149,
149, 145, 144,
145, 149, 150,
150, 146, 145,
146, 150, 151,
151, 147, 146,
87, 91, 152,
152, 148, 87,
148, 152, 153,
153, 149, 148,
149, 153, 154,
154, 150, 149,
150, 154, 155,
155, 151, 150,
91, 95, 156,
156, 152, 91,
152, 156, 157,
157, 153, 152,
153, 157, 158,
158, 154, 153,
154, 158, 159,
159, 155, 154,
95, 99, 160,
160, 156, 95,
156, 160, 161,
161, 157, 156,
157, 161, 162,
162, 158, 157,
158, 162, 163,
163, 159, 158,
99, 103, 164,
164, 160, 99,
160, 164, 165,
165, 161, 160,
161, 165, 166,
166, 162, 161,
162, 166, 167,
167, 163, 162,
103, 107, 168,
168, 164, 103,
164, 168, 169,
169, 165, 164,
165, 169, 170,
170, 166, 165,
166, 170, 171,
171, 167, 166,
107, 111, 172,
172, 168, 107,
168, 172, 173,
173, 169, 168,
169, 173, 174,
174, 170, 169,
170, 174, 175,
175, 171, 170,
111, 115, 176,
176, 172, 111,
172, 176, 177,
177, 173, 172,
173, 177, 178,
178, 174, 173,
174, 178, 179,
179, 175, 174,
115, 119, 180,
180, 176, 115,
176, 180, 181,
181, 177, 176,
177, 181, 182,
182, 178, 177,
178, 182, 183,
183, 179, 178,
119, 123, 184,
184, 180, 119,
180, 184, 185,
185, 181, 180,
181, 185, 186,
186, 182, 181,
182, 186, 187,
187, 183, 182,
123, 127, 188,
188, 184, 123,
184, 188, 189,
189, 185, 184,
185, 189, 190,
190, 186, 185,
186, 190, 191,
191, 187, 186,
127, 131, 192,
192, 188, 127,
188, 192, 193,
193, 189, 188,
189, 193, 194,
194, 190, 189,
190, 194, 195,
195, 191, 190,
131, 135, 196,
196, 192, 131,
192, 196, 197,
197, 193, 192,
193, 197, 198,
198, 194, 193,
194, 198, 199,
199, 195, 194,
135, 139, 200,
200, 196, 135,
196, 200, 201,
201, 197, 196,
197, 201, 202,
202, 198, 197,
198, 202, 203,
203, 199, 198,
139, 143, 204,
204, 200, 139,
200, 204, 205,
205, 201, 200,
201, 205, 206,
206, 202, 201,
202, 206, 207,
207, 203, 202,
143, 83, 144,
144, 204, 143,
204, 144, 145,
145, 205, 204,
205, 145, 146,
146, 206, 205,
206, 146, 147,
147, 207, 206,
147, 151, 212,
212, 208, 147,
208, 212, 213,
213, 209, 208,
209, 213, 214,
214, 210, 209,
210, 214, 211,
211, 211, 210,
151, 155, 215,
215, 212, 151,
212, 215, 216,
216, 213, 212,
213, 216, 217,
217, 214, 213,
214, 217, 211,
211, 211, 214,
155, 159, 218,
218, 215, 155,
215, 218, 219,
219, 216, 215,
216, 219, 220,
220, 217, 216,
217, 220, 211,
211, 211, 217,
159, 163, 221,
221, 218, 159,
218, 221, 222,
222, 219, 218,
219, 222, 223,
223, 220, 219,
220, 223, 211,
211, 211, 220,
163, 167, 224,
224, 221, 163,
221, 224, 225,
225, 222, 221,
222, 225, 226,
226, 223, 222,
223, 226, 211,
211, 211, 223,
167, 171, 227,
227, 224, 167,
224, 227, 228,
228, 225, 224,
225, 228, 229,
229, 226, 225,
226, 229, 211,
211, 211, 226,
171, 175, 230,
230, 227, 171,
227, 230, 231,
231, 228, 227,
228, 231, 232,
232, 229, 228,
229, 232, 211,
211, 211, 229,
175, 179, 233,
233, 230, 175,
230, 233, 234,
234, 231, 230,
231, 234, 235,
235, 232, 231,
232, 235, 211,
211, 211, 232,
179, 183, 236,
236, 233, 179,
233, 236, 237,
237, 234, 233,
234, 237, 238,
238, 235, 234,
235, 238, 211,
211, 211, 235,
183, 187, 239,
239, 236, 183,
236, 239, 240,
240, 237, 236,
237, 240, 241,
241, 238, 237,
238, 241, 211,
211, 211, 238,
187, 191, 242,
242, 239, 187,
239, 242, 243,
243, 240, 239,
240, 243, 244,
244, 241, 240,
241, 244, 211,
211, 211, 241,
191, 195, 245,
245, 242, 191,
242, 245, 246,
246, 243, 242,
243, 246, 247,
247, 244, 243,
244, 247, 211,
211, 211, 244,
195, 199, 248,
248, 245, 195,
245, 248, 249,
249, 246, 245,
246, 249, 250,
250, 247, 246,
247, 250, 211,
211, 211, 247,
199, 203, 251,
251, 248, 199,
248, 251, 252,
252, 249, 248,
249, 252, 253,
253, 250, 249,
250, 253, 211,
211, 211, 250,
203, 207, 254,
254, 251, 203,
251, 254, 255,
255, 252, 251,
252, 255, 256,
256, 253, 252,
253, 256, 211,
211, 211, 253,
207, 147, 208,
208, 254, 207,
254, 208, 209,
209, 255, 254,
255, 209, 210,
210, 256, 255,
256, 210, 211,
211, 211, 256,
257, 262, 263,
263, 258, 257,
258, 263, 264,
264, 259, 258,
259, 264, 265,
265, 260, 259,
260, 265, 266,
266, 261, 260,
262, 267, 268,
268, 263, 262,
263, 268, 269,
269, 264, 263,
264, 269, 270,
270, 265, 264,
265, 270, 271,
271, 266, 265,
267, 272, 273,
273, 268, 267,
268, 273, 274,
274, 269, 268,
269, 274, 275,
275, 270, 269,
270, 275, 276,
276, 271, 270,
272, 277, 278,
278, 273, 272,
273, 278, 279,
279, 274, 273,
274, 279, 280,
280, 275, 274,
275, 280, 281,
281, 276, 275,
277, 282, 283,
283, 278, 277,
278, 283, 284,
284, 279, 278,
279, 284, 285,
285, 280, 279,
280, 285, 286,
286, 281, 280,
282, 287, 288,
288, 283, 282,
283, 288, 289,
289, 284, 283,
284, 289, 290,
290, 285, 284,
285, 290, 291,
291, 286, 285,
287, 292, 293,
293, 288, 287,
288, 293, 294,
294, 289, 288,
289, 294, 295,
295, 290, 289,
290, 295, 296,
296, 291, 290,
292, 257, 258,
258, 293, 292,
293, 258, 259,
259, 294, 293,
294, 259, 260,
260, 295, 294,
295, 260, 261,
261, 296, 295,
261, 266, 301,
301, 297, 261,
297, 301, 302,
302, 298, 297,
298, 302, 303,
303, 299, 298,
299, 303, 304,
304, 300, 299,
266, 271, 305,
305, 301, 266,
301, 305, 306,
306, 302, 301,
302, 306, 307,
307, 303, 302,
303, 307, 308,
308, 304, 303,
271, 276, 309,
309, 305, 271,
305, 309, 310,
310, 306, 305,
306, 310, 311,
311, 307, 306,
307, 311, 312,
312, 308, 307,
276, 281, 313,
313, 309, 276,
309, 313, 314,
314, 310, 309,
310, 314, 315,
315, 311, 310,
311, 315, 316,
316, 312, 311,
281, 286, 317,
317, 313, 281,
313, 317, 318,
318, 314, 313,
314, 318, 319,
319, 315, 314,
315, 319, 320,
320, 316, 315,
286, 291, 321,
321, 317, 286,
317, 321, 322,
322, 318, 317,
318, 322, 323,
323, 319, 318,
319, 323, 324,
324, 320, 319,
291, 296, 325,
325, 321, 291,
321, 325, 326,
326, 322, 321,
322, 326, 327,
327, 323, 322,
323, 327, 328,
328, 324, 323,
296, 261, 297,
297, 325, 296,
325, 297, 298,
298, 326, 325,
326, 298, 299,
299, 327, 326,
327, 299, 300,
300, 328, 327,
329, 334, 335,
335, 330, 329,
330, 335, 336,
336, 331, 330,
331, 336, 337,
337, 332, 331,
332, 337, 338,
338, 333, 332,
334, 339, 340,
340, 335, 334,
335, 340, 341,
341, 336, 335,
336, 341, 342,
342, 337, 336,
337, 342, 343,
343, 338, 337,
339, 344, 345,
345, 340, 339,
340, 345, 346,
346, 341, 340,
341, 346, 347,
347, 342, 341,
342, 347, 348,
348, 343, 342,
344, 349, 350,
350, 345, 344,
345, 350, 351,
351, 346, 345,
346, 351, 352,
352, 347, 346,
347, 352, 353,
353, 348, 347,
349, 354, 355,
355, 350, 349,
350, 355, 356,
356, 351, 350,
351, 356, 357,
357, 352, 351,
352, 357, 358,
358, 353, 352,
354, 359, 360,
360, 355, 354,
355, 360, 361,
361, 356, 355,
356, 361, 362,
362, 357, 356,
357, 362, 363,
363, 358, 357,
359, 364, 365,
365, 360, 359,
360, 365, 366,
366, 361, 360,
361, 366, 367,
367, 362, 361,
362, 367, 368,
368, 363, 362,
364, 329, 330,
330, 365, 364,
365, 330, 331,
331, 366, 365,
366, 331, 332,
332, 367, 366,
367, 332, 333,
333, 368, 367,
333, 338, 373,
373, 369, 333,
369, 373, 374,
374, 370, 369,
370, 374, 375,
375, 371, 370,
371, 375, 376,
376, 372, 371,
338, 343, 377,
377, 373, 338,
373, 377, 378,
378, 374, 373,
374, 378, 379,
379, 375, 374,
375, 379, 380,
380, 376, 375,
343, 348, 381,
381, 377, 343,
377, 381, 382,
382, 378, 377,
378, 382, 383,
383, 379, 378,
379, 383, 384,
384, 380, 379,
348, 353, 385,
385, 381, 348,
381, 385, 386,
386, 382, 381,
382, 386, 387,
387, 383, 382,
383, 387, 388,
388, 384, 383,
353, 358, 389,
389, 385, 353,
385, 389, 390,
390, 386, 385,
386, 390, 391,
391, 387, 386,
387, 391, 392,
392, 388, 387,
358, 363, 393,
393, 389, 358,
389, 393, 394,
394, 390, 389,
390, 394, 395,
395, 391, 390,
391, 395, 396,
396, 392, 391,
363, 368, 397,
397, 393, 363,
393, 397, 398,
398, 394, 393,
394, 398, 399,
399, 395, 394,
395, 399, 400,
400, 396, 395,
368, 333, 369,
369, 397, 368,
397, 369, 370,
370, 398, 397,
398, 370, 371,
371, 399, 398,
399, 371, 372,
372, 400, 399,
401, 401, 406,
406, 402, 401,
402, 406, 407,
407, 403, 402,
403, 407, 408,
408, 404, 403,
404, 408, 409,
409, 405, 404,
401, 401, 410,
410, 406, 401,
406, 410, 411,
411, 407, 406,
407, 411, 412,
412, 408, 407,
408, 412, 413,
413, 409, 408,
401, 401, 414,
414, 410, 401,
410, 414, 415,
415, 411, 410,
411, 415, 416,
416, 412, 411,
412, 416, 417,
417, 413, 412,
401, 401, 418,
418, 414, 401,
414, 418, 419,
419, 415, 414,
415, 419, 420,
420, 416, 415,
416, 420, 421,
421, 417, 416,
401, 401, 422,
422, 418, 401,
418, 422, 423,
423, 419, 418,
419, 423, 424,
424, 420, 419,
420, 424, 425,
425, 421, 420,
401, 401, 426,
426, 422, 401,
422, 426, 427,
427, 423, 422,
423, 427, 428,
428, 424, 423,
424, 428, 429,
429, 425, 424,
401, 401, 430,
430, 426, 401,
426, 430, 431,
431, 427, 426,
427, 431, 432,
432, 428, 427,
428, 432, 433,
433, 429, 428,
401, 401, 434,
434, 430, 401,
430, 434, 435,
435, 431, 430,
431, 435, 436,
436, 432, 431,
432, 436, 437,
437, 433, 432,
401, 401, 438,
438, 434, 401,
434, 438, 439,
439, 435, 434,
435, 439, 440,
440, 436, 435,
436, 440, 441,
441, 437, 436,
401, 401, 442,
442, 438, 401,
438, 442, 443,
443, 439, 438,
439, 443, 444,
444, 440, 439,
440, 444, 445,
445, 441, 440,
401, 401, 446,
446, 442, 401,
442, 446, 447,
447, 443, 442,
443, 447, 448,
448, 444, 443,
444, 448, 449,
449, 445, 444,
401, 401, 450,
450, 446, 401,
446, 450, 451,
451, 447, 446,
447, 451, 452,
452, 448, 447,
448, 452, 453,
453, 449, 448,
401, 401, 454,
454, 450, 401,
450, 454, 455,
455, 451, 450,
451, 455, 456,
456, 452, 451,
452, 456, 457,
457, 453, 452,
401, 401, 458,
458, 454, 401,
454, 458, 459,
459, 455, 454,
455, 459, 460,
460, 456, 455,
456, 460, 461,
461, 457, 456,
401, 401, 462,
462, 458, 401,
458, 462, 463,
463, 459, 458,
459, 463, 464,
464, 460, 459,
460, 464, 465,
465, 461, 460,
401, 401, 402,
402, 462, 401,
462, 402, 403,
403, 463, 462,
463, 403, 404,
404, 464, 463,
464, 404, 405,
405, 465, 464,
405, 409, 470,
470, 466, 405,
466, 470, 471,
471, 467, 466,
467, 471, 472,
472, 468, 467,
468, 472, 473,
473, 469, 468,
409, 413, 474,
474, 470, 409,
470, 474, 475,
475, 471, 470,
471, 475, 476,
476, 472, 471,
472, 476, 477,
477, 473, 472,
413, 417, 478,
478, 474, 413,
474, 478, 479,
479, 475, 474,
475, 479, 480,
480, 476, 475,
476, 480, 481,
481, 477, 476,
417, 421, 482,
482, 478, 417,
478, 482, 483,
483, 479, 478,
479, 483, 484,
484, 480, 479,
480, 484, 485,
485, 481, 480,
421, 425, 486,
486, 482, 421,
482, 486, 487,
487, 483, 482,
483, 487, 488,
488, 484, 483,
484, 488, 489,
489, 485, 484,
425, 429, 490,
490, 486, 425,
486, 490, 491,
491, 487, 486,
487, 491, 492,
492, 488, 487,
488, 492, 493,
493, 489, 488,
429, 433, 494,
494, 490, 429,
490, 494, 495,
495, 491, 490,
491, 495, 496,
496, 492, 491,
492, 496, 497,
497, 493, 492,
433, 437, 498,
498, 494, 433,
494, 498, 499,
499, 495, 494,
495, 499, 500,
500, 496, 495,
496, 500, 501,
501, 497, 496,
437, 441, 502,
502, 498, 437,
498, 502, 503,
503, 499, 498,
499, 503, 504,
504, 500, 499,
500, 504, 505,
505, 501, 500,
441, 445, 506,
506, 502, 441,
502, 506, 507,
507, 503, 502,
503, 507, 508,
508, 504, 503,
504, 508, 509,
509, 505, 504,
445, 449, 510,
510, 506, 445,
506, 510, 511,
511, 507, 506,
507, 511, 512,
512, 508, 507,
508, 512, 513,
513, 509, 508,
449, 453, 514,
514, 510, 449,
510, 514, 515,
515, 511, 510,
511, 515, 516,
516, 512, 511,
512, 516, 517,
517, 513, 512,
453, 457, 518,
518, 514, 453,
514, 518, 519,
519, 515, 514,
515, 519, 520,
520, 516, 515,
516, 520, 521,
521, 517, 516,
457, 461, 522,
522, 518, 457,
518, 522, 523,
523, 519, 518,
519, 523, 524,
524, 520, 519,
520, 524, 525,
525, 521, 520,
461, 465, 526,
526, 522, 461,
522, 526, 527,
527, 523, 522,
523, 527, 528,
528, 524, 523,
524, 528, 529,
529, 525, 524,
465, 405, 466,
466, 526, 465,
526, 466, 467,
467, 527, 526,
527, 467, 468,
468, 528, 527,
528, 468, 469,
469, 529, 528
    };
};

//////////

#endif
