#include "Etat.h"

SINGLETON_DECLARATION_CPP(Etat);

bool         Etat::enmouvement = true;
bool         Etat::impression = false;
bool         Etat::afficheAxes = true;
GLenum       Etat::modePolygone = GL_FILL;
unsigned int Etat::nparticules = 400;
int          Etat::texnumero = 1;
GLuint       Etat::textureOISEAU = 0;
GLuint       Etat::textureFLOCON = 0;
bool         Etat::boucleTexture = false;
glm::ivec2   Etat::sourisPosPrec = glm::ivec2(0);
int          Etat::modele = 1;
glm::vec3    Etat::posPuitsOrig = glm::vec3( 20.0, 10.0, 10.0 );
glm::vec3    Etat::posPuits = posPuitsOrig;
glm::vec3    Etat::bDim = glm::vec3( 75., 110., 105. );
