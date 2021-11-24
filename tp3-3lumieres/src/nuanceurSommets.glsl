#version 410

// Définition des paramètres des sources de lumière
layout (std140) uniform LightSourceParameters
{
    vec4 ambient[3];
    vec4 diffuse[3];
    vec4 specular[3];
    vec4 position[3];      // dans le repère du monde
    vec3 spotDirection[3]; // dans le repère du monde
    float spotExponent;
    float spotAngleOuverture; // ([0.0,90.0] ou 180.0)
    float constantAttenuation;
    float linearAttenuation;
    float quadraticAttenuation;
} LightSource;

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
    vec3 lumiDir[3];
    vec3 normale;
    vec3 obsVec;
    vec2 texCoord;
} AttribsOut;

float calculerSpot( in vec3 D, in vec3 L, in vec3 N )
{
    float spotFacteur = 0.0;
    return( spotFacteur );
}

float attenuation = 1.0;
vec4 calculerReflexion( in int j, in vec3 L, in vec3 N, in vec3 O ) // pour la lumière j
{
    vec4 coul = vec4(0);

    // calculer l'éclairage seulement si le produit scalaire est positif
    float NdotL = max( 0.0, dot( N, L ) );
    if ( NdotL > 0.0 )
    {
        // calculer la composante diffuse
        coul += attenuation * FrontMaterial.diffuse * LightSource.diffuse[j] * NdotL;

        // calculer la composante spéculaire (Blinn ou Phong : spec = BdotN ou RdotO )
        float spec = ( utiliseBlinn ?
                       dot( normalize( L + O ), N ) : // dot( B, N )
                       dot( reflect( -L, N ), O ) ); // dot( R, O )
        if ( spec > 0 ) coul += attenuation * FrontMaterial.specular * LightSource.specular[j] * pow( spec, FrontMaterial.shininess );
    }

    return( coul );
}

void main( void )
{
    // appliquer la transformation standard du sommet (P * V * M * sommet)
    gl_Position = matrProj * matrVisu * matrModel * Vertex;

    // calcul de la composante ambiante du modèle
    vec4 coul = FrontMaterial.emission + FrontMaterial.ambient * LightModel.ambient;

    // calculer la normale (N) qui sera interpolée pour le nuanceur de fragments
    AttribsOut.normale = matrNormale * Normal;
    vec3 N = normalize( AttribsOut.normale );
    
    // calculer la position (P) du sommet (dans le repère de la caméra)
    vec3 pos = vec3( matrVisu * matrModel * Vertex );

    // Calculer le vecteur de direction vers l'observateur
    AttribsOut.obsVec = -pos;
    vec3 O = normalize(AttribsOut.obsVec);  // position de l'observateur

    // Calculer la couleur du sommet pour chaque source de lumière
    for(int j = 0; j < 3; j++) {

        // calculer le vecteur de la direction (L) de la lumière (dans le repère de la caméra)
        AttribsOut.lumiDir[j] = ( matrVisu * LightSource.position[j] ).xyz - pos;

        // Si l'illumination de Gouraud est choisie
        if (typeIllumination == 0) {
            // Calculer le vecteur vers la source lumineuse
            vec3 L = normalize( AttribsOut.lumiDir[j] );
            // Calculer la réflexion
            coul += calculerReflexion( j, L, N, O );
        }
    };

    AttribsOut.couleur = clamp( coul, 0.0, 1.0 );

    // Transmettre au nuanceur de fragments les coordonées de textures rçcues
    AttribsOut.texCoord = TexCoord.st;
    // Deplacer la texture en fonction du temps
    AttribsOut.texCoord.s -= tempsGlissement;

}
