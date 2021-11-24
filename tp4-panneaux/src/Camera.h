#ifndef __CAMERA_H__
#define __CAMERA_H__

//
// variables pour définir le point de vue
//
class Camera
{
public:
    Camera() { theta = 0.0; phi = 0.0; dist = 250.0; }
    void definir()
    {
        glm::vec3 ptVise = glm::vec3( 0.0, 0.0, 0.5*Etat::bDim.z ); // un point à mi-hauteur du modèle
        matrVisu.LookAt( ptVise.y + dist*cos(glm::radians(phi))*cos(glm::radians(theta)),
                         ptVise.x + dist*cos(glm::radians(phi))*sin(glm::radians(theta)),
                         ptVise.z + dist*sin(glm::radians(phi)),
                         ptVise.x, ptVise.y, ptVise.z,
                         0.0, 0.0, 1.0 );
    }
    void verifierAngles() // vérifier que les angles ne débordent pas les valeurs permises
    {
        const GLdouble MINPHI = -90.0 + 0.01, MAXPHI = 90.0 - 0.01;
        phi = glm::clamp( phi, MINPHI, MAXPHI );
        if ( theta > 360.0 ) theta -= 360.0; else if ( theta < 0.0 ) theta += 360.0;
    }
    double theta;         // angle de rotation de la caméra (coord. sphériques)
    double phi;           // angle de rotation de la caméra (coord. sphériques)
    double dist;          // distance (coord. sphériques)
} camera;

#endif
