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
    FragColor = AttribsIn.couleur;

    // doit-on utiliser la texture?
    if ( texnumero != 0 )
    {
        vec4 couleurLutin = texture( leLutin, AttribsIn.texCoord );
        
        if ( couleurLutin.a < 0.1 ) discard;

        FragColor = vec4( mix( AttribsIn.couleur.rgb, couleurLutin.rgb, 0.6 ), AttribsIn.couleur.a );
    }
    
}
