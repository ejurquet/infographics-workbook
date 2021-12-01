#version 410

uniform sampler2D leLutin;
uniform int texnumero;

in Attribs {
    vec4 couleur;
    vec2 texCoord;
} AttribsIn;

out vec4 FragColor;

void main( void )
{
    // Associer la couleur au fragment
    FragColor = AttribsIn.couleur;

    // doit-on utiliser la texture?
    if ( texnumero != 0 )
    {
        // Récupérer la texture
        vec4 couleurLutin = texture( leLutin, AttribsIn.texCoord );
        
        // Enlever le fond de la textures de lutin
        if ( couleurLutin.a < 0.1 ) discard;

        // Appliquer la couleur de lutin mélangée à la couleur initiale, comme couleur du fragment
        FragColor = vec4( mix( AttribsIn.couleur.rgb, couleurLutin.rgb, 0.6 ), AttribsIn.couleur.a );
    }
    
}
