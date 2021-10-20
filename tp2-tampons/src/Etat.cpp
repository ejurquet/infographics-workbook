#include "Etat.h"

SINGLETON_DECLARATION_CPP(Etat);

glm::vec4  Etat::bDim = glm::vec4( 10.0, 10.0, 10.0, 1.0 );
float      Etat::dt = float(1.0/60.0);
float      Etat::sens = float(+1.0);
bool       Etat::enmouvement = true;
bool       Etat::afficheAxes = true;
bool       Etat::culling = false;
GLenum     Etat::modePolygone = GL_FILL;
int        Etat::theiereChoisie = 0;
bool       Etat::illumination = false;
bool       Etat::debug = false;
glm::ivec2 Etat::sourisPosPrec = glm::ivec2(0);
