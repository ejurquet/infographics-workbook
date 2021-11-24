#ifndef __ETAT_H__
#define __ETAT_H__

#include <GL/glew.h>
#include <glm/glm.hpp>
#include "inf2705-Singleton.h"

//
// variables d'état
//
class Etat : public Singleton<Etat>
{
    SINGLETON_DECLARATION_CLASSE(Etat);
public:
    static bool enPerspective;        // indique si on est en mode Perspective (true) ou Ortho (false)
    static bool enmouvement;          // le modèle est en mouvement/rotation automatique ou non
    static bool englissement;         // la texture glisse
    static bool afficheAxes;          // indique si on affiche les axes
    static int curLumi;               // la source lumineuse courante (celle qui peut être déplacée)
    static GLenum modePolygone;       // comment afficher les polygones
    static glm::ivec2 sourisPosPrec;  // la position de la souris
    static int modele;                // le modèle à afficher
    static float facteurDeform;       // facteur de déformation de la surface (voir Apprentissage supplémentaire)
    static bool utiliseTess;          // on utilise des nuanceurs de tessellation ?
    static GLfloat TessLevelInner;    // niveau de tessellation
    static GLfloat TessLevelOuter;    // niveau de tessellation
    static float temps;               // le temps courant (en secondes)
    static float dt;                  // intervalle entre chaque affichage (en secondes)
};

#endif
