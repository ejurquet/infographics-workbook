#ifndef __PIPELINE_H__
#define __PIPELINE_H__

// variables pour l'utilisation des nuanceurs
GLuint prog;      // votre programme de nuanceurs
GLint locVertex = -1;
GLint locColor = -1;
GLint locvitesse = -1;
GLint loctempsDeVieRestant = -1;
GLint locmatrModel = -1;
GLint locmatrVisu = -1;
GLint locmatrProj = -1;
GLint locleLutin = -1;
GLint loctexnumero = -1;
GLuint indvarsUnif = GL_INVALID_INDEX;
GLuint progRetroaction;  // votre programme de nuanceurs pour la rétroaction
GLint locVertexRetroaction = -1;
GLint locColorRetroaction = -1;
GLint locvitesseRetroaction = -1;
GLint loctempsDeVieRestantRetroaction = -1;
GLint locposPuitsRetroaction = -1;
GLint locbDimRetroaction = -1;
GLuint indvarsUnifRetroaction = GL_INVALID_INDEX;
GLuint progBase;  // le programme de nuanceurs de base
GLint locColorBase = -1;
GLint locmatrModelBase = -1;
GLint locmatrVisuBase = -1;
GLint locmatrProjBase = -1;

GLuint vao[2];
GLuint vbo[2];
GLuint tfo[1];
GLuint requete;
GLuint ubo;

// matrices du pipeline graphique
MatricePipeline matrModel, matrVisu, matrProj;

#endif
