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
    static bool enmouvement;         // le modèle est en mouvement automatique ou non
    static bool afficheAxes;         // indique si on affiche les axes
    static bool culling;             // indique si on veut ne pas afficher les faces arrières
    static GLenum modePolygone;      // comment afficher les polygones (GL_LINE ou GL_FILL)
    static int modele;               // le modèle à afficher
    static double dimBoite;          // la dimension de la boite
    static bool corpsVersAvant;      // l'orientation du corps est toujours vers l'avant?
    static float dt;                 // intervalle entre chaque affichage (en secondes)
};

#endif
