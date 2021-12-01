#version 410

uniform vec3 bDim, posPuits;

layout (std140) uniform varsUnif
{
    float tempsDeVieMax;       // temps de vie maximal (en secondes)
    float temps;               // le temps courant dans la simulation (en secondes)
    float dt;                  // intervalle entre chaque affichage (en secondes)
    float gravite;             // gravité utilisée dans le calcul de la position de la particule
    float pointsize;           // taille des points (en pixels)
};

layout(location=6) in vec3 Vertex;
layout(location=7) in vec4 Color;
layout(location=8) in vec3 vitesse;
layout(location=9) in float tempsDeVieRestant;

out vec3 VertexMod;
out vec4 ColorMod;
out vec3 vitesseMod;
out float tempsDeVieRestantMod;

// Produire une valeur un peu aléatoire
uint seed = uint(temps * 1000.0) + uint(gl_VertexID);
uint randhash( ) // entre  0 et UINT_MAX
{
    uint i=((seed++)^12345391u)*2654435769u;
    i ^= (i<<6u)^(i>>26u); i *= 2654435769u; i += (i<<5u)^(i>>12u);
    return i;
}
float aleatoire( ) // entre  0 et 1
{
    const float UINT_MAX = 4294967295.0;
    return float(randhash()) / UINT_MAX;
}

void main( void )
{
    if ( tempsDeVieRestant <= 0.0 )
    {
        // couleur aléatoire par interpolation linéaire entre COULMIN et COULMAX
        const float COULMIN = 0.2; // valeur minimale d'une composante de couleur lorsque la particule (re)naît
        const float COULMAX = 0.9; // valeur maximale d'une composante de couleur lorsque la particule (re)naît

        // faire renaitre la particule au puits
        VertexMod = posPuits;

        // assigner un vitesse (pseudo) aléatoire
        vitesseMod = vec3( mix( -25.0, 25.0, aleatoire() ),   // entre -25 et 25
                           mix( -25.0, 25.0, aleatoire() ),   // entre -25 et 25
                           mix(  25.0, 50.0, aleatoire() ) ); // entre  25 et 50
        //vitesseMod = vec3( 0.0, 30.0, 50.0 );

        // nouveau temps de vie (pseudo) aléatoire
        tempsDeVieRestantMod = aleatoire() * tempsDeVieMax; // valeur entre 0 et tempsDeVieMax secondes

        // couleur (pseudo) aléatoire par interpolation linéaire entre COULMIN et COULMAX
        ColorMod = vec4( mix( COULMIN, COULMAX, aleatoire() ),
                        mix( COULMIN, COULMAX, aleatoire() ),
                        mix( COULMIN, COULMAX, aleatoire() ),
                        1.0 );
    }
    else
    {
        // avancer la particule (méthode de Euler)
        VertexMod = Vertex + dt * vitesse;
        vitesseMod = vitesse;

        // diminuer son temps de vie
        tempsDeVieRestantMod = tempsDeVieRestant - dt;

        // garder la couleur courante
        ColorMod = Color;

        // gérer la collision avec la demi-sphère
        // mettre à l'échelle la position de la particule
        vec3 posSphUnitaire = VertexMod / bDim;
        // Multiplier chaque composante de la vitesse par la dimension correspondante de la demi-sphère
        vec3 vitSphUnitaire = vitesseMod * bDim;
        float dist = length( posSphUnitaire );
        if (dist >= 1.0) // la particule est sortie de la bulle
        {
            VertexMod = ( 2.0 - dist ) * VertexMod;
            // Normaliser N
            vec3 N = posSphUnitaire / dist;
            // Réfléchir la vitesse
            vec3 vitReflechieSphUnitaire = reflect( vitSphUnitaire, N );
            vitesseMod = vitReflechieSphUnitaire / bDim;
            // Vitesse divisée par 2
            vitesseMod /= 2.0;
        }

        // gérer la collision avec le sol
        float hauteurPlancher = 0.5 * pointsize;  // hauteur minimale à laquelle une collision avec le plancher survient
        // la particule est-elle en collision avec le plancher ?
        if ( VertexMod.z < hauteurPlancher )
        {
            // Ramener la particule au sol
            VertexMod.z = hauteurPlancher + (hauteurPlancher - VertexMod.z);
            // Inverser la vitesse verticale de la particule
            vitesseMod.z = - vitesseMod.z;
            // Vitesse divisée par 2
            vitesseMod /= 2.0;
        }

        // appliquer la gravité
        vitesseMod.z -= gravite * dt;
    }

}
