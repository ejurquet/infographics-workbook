#ifndef __CAMERA_H__
#define __CAMERA_H__

//
// variables pour définir le point de vue
//
class Camera
{
public:
    Camera() { theta = 0.0; phi = 0.0; dist = 40.0; }
    void definir()
    {
        matrVisu.LookAt( dist*cos(glm::radians(phi))*sin(glm::radians(theta)),
                         dist*sin(glm::radians(phi)),
                         dist*cos(glm::radians(phi))*cos(glm::radians(theta)),
                         0, 0, 0,
                         0, 1, 0 );
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
