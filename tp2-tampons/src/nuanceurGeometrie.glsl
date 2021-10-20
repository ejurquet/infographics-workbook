#version 410

layout(triangles) in;
layout(triangle_strip, max_vertices = 3) out;

in Attribs {
    vec4 couleur;
} AttribsIn[];

out Attribs {
    vec4 couleur;
    vec3 lumiDir, obsVec, normale;
} AttribsOut;

void main()
{
    // Calculer le produit vectoriel des vecteurs à partirpour obtenir la normale
    vec3 arete1 = vec3( gl_in[1].gl_Position - gl_in[0].gl_Position );
    vec3 arete2 = vec3( gl_in[2].gl_Position - gl_in[0].gl_Position);
    vec3 normale = cross( arete1 , arete2 );

    // Émettre les sommets
    for ( int i = 0 ; i < gl_in.length() ; ++i )
    {
        // Récupérer la position
        gl_Position = gl_in[i].gl_Position;
        // Attribuer la couleur
        AttribsOut.couleur = AttribsIn[i].couleur;

        // Attribuer la normale
        AttribsOut.normale = normale;
        // Définir la direction du vecteur de lumière
        AttribsOut.lumiDir = vec3( 0.0, 0.0, 1.0 );
        // Définir la position de l'observateur
        AttribsOut.obsVec = vec3( 0.0, 0.0, 1.0 );

        // Émettre le sommet
        EmitVertex();
    }

}
