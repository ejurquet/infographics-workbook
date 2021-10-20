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
    static glm::vec4 bDim;           // les dimensions de l'aquarium: une boite [-x,+x][-y,+y][-z,+z]
    static float dt;                 // intervalle entre chaque affichage (en secondes)
    static float sens;               // +1 ou -1 : le temps avance ou recule
    static bool enmouvement;         // le modèle est en mouvement/rotation automatique ou non
    static bool afficheAxes;         // indique si on affiche les axes
    static bool culling;             // indique si on veut ne pas afficher les faces arrières
    static GLenum modePolygone;      // comment afficher les polygones (GL_LINE ou GL_FILL)
    static int theiereChoisie;       // 0 pour la vue « normale »; 1 à 9 pour la vue de la théière 1 à 9
    static bool illumination;        // indique si on veut atténuer selon l'éloignement
    static bool debug;               // indique si on veut utiliser le debug (afficher ou ne pas afficher certains objets, faire ou ne pas faire certaines actions)
    static glm::ivec2 sourisPosPrec;
};

#endif
