#version 410

layout(points) in;
layout(triangle_strip, max_vertices = 4) out;

uniform mat4 matrProj;

uniform int texnumero;

layout (std140) uniform varsUnif
{
    float tempsDeVieMax;       // temps de vie maximal (en secondes)
    float temps;               // le temps courant dans la simulation (en secondes)
    float dt;                  // intervalle entre chaque affichage (en secondes)
    float gravite;             // gravité utilisée dans le calcul de la position de la particule
    float pointsize;           // taille des points (en pixels)
};

in Attribs {
    vec4 couleur;
    float tempsDeVieRestant;
    float sens; // du vol (partie 3)
    float hauteur; // de la particule dans le repère du monde (partie 3)
} AttribsIn[];

out Attribs {
    vec4 couleur;
    vec2 texCoord;
} AttribsOut;

// la hauteur minimale en-dessous de laquelle les lutins ne tournent plus (partie 3)
const float hauteurInerte = 8.0;

void main()
{
    // Similaire a l'exemple 10 sur les panneaux
    vec2 coins[4];
    coins[0] = vec2( -0.5, 0.5 );
    coins[1] = vec2( -0.5, -0.5 );
    coins[2] = vec2( 0.5, 0.5 );
    coins[3] = vec2( 0.5, -0.5 );
    
    // à partir du point, créer quatre points qui servent de coin aux deux triangles
    for ( int i = 0 ; i < 4 ; ++i) {

        vec2 decalage = coins[i]; // on positionne successivement aux quatre coins

        // Tournoiement du flocon
        if (texnumero == 2 && AttribsIn[0].hauteur >= hauteurInerte) {
            // Calculer la matrice de rotation pour faire tourner le flocon autour de son centre (axe x)
            mat2 matrRot = mat2( cos(6.0 * AttribsIn[0].tempsDeVieRestant), sin(6.0 * AttribsIn[0].tempsDeVieRestant),
                                -sin(6.0 * AttribsIn[0].tempsDeVieRestant), cos(6.0 * AttribsIn[0].tempsDeVieRestant) );
            // Calculer le decalage en consequence
            decalage = matrRot * coins[i];
        }

        // Calculer la position du coin i
        vec4 pos = vec4( gl_in[0].gl_Position.xy + gl_in[0].gl_PointSize * decalage, gl_in[0].gl_Position.zw );

        // assigner la position du point
        gl_Position = matrProj * pos;  // on termine la transformation débutée dans le nuanceur de sommets

        // assigner la couleur courante
        AttribsOut.couleur = AttribsIn[0].couleur;
        // Faire fondre les lutins en fin de vie
        if (AttribsIn[0].tempsDeVieRestant < 4.0) {
            AttribsOut.couleur.a = AttribsIn[0].couleur.a - ( (4.0 - AttribsIn[0].tempsDeVieRestant) / 4.0 ) * AttribsIn[0].couleur.a;
        }

        // fixer les coordonnées de texture    
        AttribsOut.texCoord = coins[i] + vec2( 0.5, 0.5 );   // on utilise coins[] pour définir des coordonnées de texture
        
        // Vol de l'oiseau
        if (texnumero == 1) {
            const float nlutins = 16.0; // 16 positions de vol dans la texture
            int num = 8;    // la frame 8 est donnée par défaut
            // Si on est au dessus de la hauteur d'inerte, on donne la bonne frame
            if (AttribsIn[0].hauteur >= hauteurInerte) {
                num = int( mod( 18.0 * AttribsIn[0].tempsDeVieRestant, nlutins));    // 18 Hz
            }
            AttribsOut.texCoord.s = ( AttribsIn[0].sens * AttribsOut.texCoord.s + num) / nlutins;
        }

        EmitVertex();
    }
}
