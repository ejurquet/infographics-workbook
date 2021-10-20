#ifndef __PIPELINE_H__
#define __PIPELINE_H__

// variables pour l'utilisation des nuanceurs
GLuint prog;      // votre programme de nuanceurs
GLint locVertex = -1;
GLint locColor = -1;
GLint locmatrModel = -1;
GLint locmatrVisu = -1;
GLint locmatrProj = -1;
GLuint indLightSource = -1;
GLuint indFrontMaterial = -1;
GLuint indLightModel = -1;

GLuint progBase;  // le programme de nuanceurs de base
GLint locColorBase = -1;
GLint locmatrModelBase = -1;
GLint locmatrVisuBase = -1;
GLint locmatrProjBase = -1;

// matrices du pipeline graphique
MatricePipeline matrModel, matrVisu, matrProj;

// Uniform Buffer Object (ubo) pour l'illumination
static GLuint ubo[3];

// définition des lumières
struct LightSourceParameters
{
    glm::vec4 ambient;
    glm::vec4 diffuse;
    glm::vec4 specular;
    glm::vec4 position;       // dans le repère du monde (il faudra convertir vers le repère de la caméra pour les calculs)
    glm::vec3 spotDirection;  // dans le repère du monde (il faudra convertir vers le repère de la caméra pour les calculs)
    float spotExposant;
    float spotAngleOuverture; // angle d'ouverture delta du spot ([0.0,90.0] ou 180.0)
    float constantAttenuation;
    float linearAttenuation;
    float quadraticAttenuation;
} LightSource = { glm::vec4( 1.0, 1.0, 1.0, 1.0 ),
                  glm::vec4( 1.0, 1.0, 1.0, 1.0 ),
                  glm::vec4( 1.0, 1.0, 1.0, 1.0 ),
                  glm::vec4( 1.8, 2.5, 2.0, 1.0 ),
                  glm::vec3( 0.0, 0.0, 1.0 ),
                  1.0,       // l'exposant du cône
                  180.0,     // l'angle du cône du spot
                  1., 0., 0. };

// définition du matériau
struct MaterialParameters
{
    glm::vec4 emission;
    glm::vec4 ambient;
    glm::vec4 diffuse;
    glm::vec4 specular;
    float shininess;
} FrontMaterial = { glm::vec4( 0.0, 0.0, 0.0, 1.0 ),
                    glm::vec4( 0.2, 0.2, 0.2, 1.0 ),
                    glm::vec4( 1.0, 1.0, 1.0, 1.0 ),
                    glm::vec4( 1.0, 1.0, 1.0, 1.0 ),
                    20.0 };

struct LightModelParameters
{
    glm::vec4 ambient; // couleur ambiante
    int localViewer;   // doit-on prendre en compte la position de l'observateur? (local ou à l'infini)
    int twoSide;       // éclairage sur les deux côtés ou un seul?
} LightModel = { glm::vec4(0,0,0,1), false, false };

#endif
