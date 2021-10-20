#version 410

////////////////////////////////////////////////////////////////////////////////

// Définition des paramètres des sources de lumière
layout (std140) uniform LightSourceParameters
{
    vec4 ambient;
    vec4 diffuse;
    vec4 specular;
    vec4 position;      // dans le repère du monde
    vec3 spotDirection; // dans le repère du monde
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
    vec4 ambient;       // couleur ambiante
    bool localViewer;   // observateur local ou à l'infini?
    bool twoSide;       // éclairage sur les deux côtés ou un seul?
} LightModel;

////////////////////////////////////////////////////////////////////////////////

uniform int illumination; // on veut calculer l'illumination ?

in Attribs {
    vec4 couleur;
    vec3 lumiDir, obsVec, normale;
} AttribsIn;

out vec4 FragColor;

void main( void )
{
    // La couleur du fragment est la couleur interpolée
    FragColor = AttribsIn.couleur;
    vec3 L = normalize( AttribsIn.lumiDir );    // vecteur vers la lumière
    vec3 N = normalize( gl_FrontFacing ? AttribsIn.normale : - AttribsIn.normale );    // vecteur normal
    vec3 O = normalize( AttribsIn.obsVec );     // position de l'observateur

    // Ajout de l'émission et du terme ambiant du modèle d'illumination
    vec4 coul = FrontMaterial.emission +
                FrontMaterial.ambient * LightModel.ambient;

    // Calculer la composante ambiante de la lumière
    coul += FrontMaterial.ambient * LightSource.ambient;

    // Récupérer l'alpha pour la couleur courante (important pour la transparence des miroirs)
    coul.a = AttribsIn.couleur.a;

    // Réaliser le produit scalaire pour le calcul de la réflexion diffuse
    float NdotL = max( 0.0, dot( N, L ) );
    // Calculer la composante diffuse de la lumière
    coul += AttribsIn.couleur * LightSource.diffuse * NdotL;

    // Calculer la composante speculaire selon Phong
    float spec = max ( 0.0, dot( reflect ( -L, N), O ) ); // Phong

    // Calculer la composante spéculaire de la lumière
    coul += FrontMaterial.specular * LightSource.specular * pow( spec, FrontMaterial.shininess );

    // Si l'illumination est activée on assigne la couleur du fragment qui est la couleur calculée
    if ( illumination == 1 ) {
        FragColor = clamp( coul, 0.0, 1.0 );
    } else {
        FragColor = AttribsIn.couleur;
    }
}
