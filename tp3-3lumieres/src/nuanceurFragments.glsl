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

uniform sampler2D laTextureCoul;
uniform sampler2D laTextureNorm;

/////////////////////////////////////////////////////////////////

in Attribs {
    vec4 couleur;
    vec3 lumiDir[3];
    vec3 normale;
    vec3 obsVec;
    vec2 texCoord;
} AttribsIn;

out vec4 FragColor;

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
    vec4 coul = AttribsIn.couleur; // la composante ambiante déjà calculée (dans nuanceur de sommets)

    vec3 N = normalize( AttribsIn.normale );
    vec3 O = normalize( AttribsIn.obsVec );

    // Si une texture de normale est activée
    if (iTexNorm != 0) {
        // Modifier la normale en conséquence
        vec3 couleur = texture( laTextureNorm, AttribsIn.texCoord).rgb;
        vec3 dN = normalize( ( couleur - 0.5 ) * 2.0 );
        N = normalize( N + dN );
    }

    // Si l'illumination de Phong est choisie
    if (typeIllumination == 1) {

        // Pour chaque source de lumière
        for (int j = 0; j < 3; j++) {
            // Calculer le vecteur vers la source lumineuse
            vec3 L = normalize( AttribsIn.lumiDir[j] );
            // Calculer la réflexion
            coul += calculerReflexion( j, L, N, O );
        }

    }
    
    // echantilloner la couleur du fragment dans la texture
    vec4 couleurTexture = texture( laTextureCoul, AttribsIn.texCoord);

    // Afficher une texture si elle existe
    if (iTexCoul != 0) {
        coul = couleurTexture * coul;
        // Enlever les fragments fonces
        if ( length(couleurTexture.rgb) < 0.5 )
            discard;
    }

    // assigner la couleur au fragment
    FragColor = clamp( coul, 0.0, 1.0 );

    // Pour « voir » les normales, on peut remplacer la couleur du fragment par la normale.
    // (Les composantes de la normale variant entre -1 et +1, il faut
    // toutefois les convertir en une couleur entre 0 et +1 en faisant (N+1)/2.)
    if ( afficheNormales ) FragColor = clamp( vec4( (N+1)/2, AttribsIn.couleur.a ), 0.0, 1.0 );

}
