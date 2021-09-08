#include "Etat.h"

SINGLETON_DECLARATION_CPP(Etat);

bool       Etat::enmouvement = false;
bool       Etat::afficheAxes = true;
bool       Etat::culling = false;
GLenum     Etat::modePolygone = GL_LINE;
int        Etat::modele = 1;
double     Etat::dimBoite = 10.0;
bool       Etat::corpsVersAvant = false;
float      Etat::dt = float(1.0/60.0);
