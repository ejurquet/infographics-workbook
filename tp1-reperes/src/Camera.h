#ifndef __CAMERA_H__
#define __CAMERA_H__

// variables pour définir le point de vue
class Camera
{
public:
    Camera() { initVar(); }
    void initVar() { theta = 0.0; phi = 0.0; dist = 40.0; modeLookAt = true; }
    void definir()
    {
        if ( modeLookAt )
        {
            matrVisu.LookAt( dist*cos(glm::radians(phi))*sin(glm::radians(theta)),
                             dist*sin(glm::radians(phi)) + 5, // <= prenez note du +5
                             dist*cos(glm::radians(phi))*cos(glm::radians(theta)),
                             0, 5, 0, // <= prenez note du +5
                             0, 1, 0 );
        }
        else
        {
            // Indice : l'appel ci-dessus est équivalent à
            // matrVisu.LookAt( dist*cos(glm::radians(theta))*sin(glm::radians(phi)),
            //                  dist*sin(glm::radians(theta))*sin(glm::radians(phi)),
            //                  dist*cos(glm::radians(phi)),
            //                  0., 0., 0.,
            //                  0., 0., 1. );
            // matrVisu.Translate( 0., -5., 0. );  // changer le point regardé

            matrVisu.LoadIdentity( );
            matrVisu.Translate( 0., 0., -dist );   // se déplacer à la distance de la caméra
            // utilisez matrVisu.Translate(), matrVisu.Rotate(), ...
	    //..
            matrVisu.Translate( 0., -5., 0. );     // changer le point regardé
        }
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
    bool modeLookAt;      // on utilise LookAt (au lieu de Rotate et Translate)
} camera;

#endif
