#version 410

// Définition des paramètres des matériaux
layout (std140) uniform MaterialParameters
{
    vec4 emission;
    vec4 ambient;
    vec4 diffuse;
    vec4 specular;
    float shininess;
} FrontMaterial;

// Définition des paramètres globaux du modèle de lumière
layout (std140) uniform LightModelParameters
{
    vec4 ambient;       // couleur ambiante globale
    bool twoSide;       // éclairage sur les deux côtés ou un seul?
} LightModel;

layout (std140) uniform varsUnif
{
    // partie 1: illumination
    int typeIllumination;     // 0:Gouraud, 1:Phong
    bool utiliseBlinn;        // indique si on veut utiliser modèle spéculaire de Blinn ou Phong
    bool utiliseDirect;       // indique si on utilise un spot style Direct3D ou OpenGL
    bool afficheNormales;     // indique si on utilise les normales comme couleurs (utile pour le débogage)
    // partie 2: texture
    float tempsGlissement;    // temps de glissement
    int iTexCoul;             // numéro de la texture de couleurs appliquée
    // partie 3b: texture
    int iTexNorm;             // numéro de la texture de normales appliquée
};

uniform mat4 matrModel;
uniform mat4 matrVisu;
uniform mat4 matrProj;
uniform mat3 matrNormale;

/////////////////////////////////////////////////////////////////

layout(location=0) in vec4 Vertex;
layout(location=2) in vec3 Normal;
layout(location=8) in vec4 TexCoord;

out Attribs {
    vec4 couleur;
    vec3 normale;
    vec2 texCoord;
} AttribsOut;

void main( void )
{
    gl_Position = matrModel * Vertex;

    // Envoyer les attributs au nuanceur de contrôle de tessellation
    AttribsOut.normale = matrNormale * Normal;

    vec4 coul = FrontMaterial.emission + FrontMaterial.ambient * LightModel.ambient;
    AttribsOut.couleur = clamp( coul, 0.0, 1.0 );

    // Transmettre au nuanceur de fragments les coordonées de textures reçues
    AttribsOut.texCoord = TexCoord.st;
    // Deplacer la texture en fonction du temps
    AttribsOut.texCoord.s -= tempsGlissement;

}
