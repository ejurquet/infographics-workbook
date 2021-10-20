// Prénoms, noms et matricule des membres de l'équipe:
// - Ali AMEZIANE HASSANI (2007033)
// - Eliès JURQUET (2162621)

#if defined(_WIN32) || defined(WIN32)
#pragma warning ( disable : 4244 4305 )
// warning C4244: 'conversion' conversion from 'double' to 'float', possible loss of data
// warning C4305: 'argument' : truncation from 'double' to 'float'
#endif

#include <stdlib.h>
#include <iostream>
#include "inf2705-matrice.h"
#include "inf2705-nuanceur.h"
#include "inf2705-fenetre.h"
#include "inf2705-forme.h"
#include "inf2705-theiere.h"
#include <glm/gtx/io.hpp>
#define SOL 1
#include "Etat.h"
#include "Pipeline.h"
#include "Camera.h"
#include "Aquarium.h"

static Aquarium *aquarium = NULL;

void calculerPhysique( )
{
    // ajuster le dt selon la fréquence d'affichage
    {
        static int tempsPrec = 0;
        // obtenir le temps depuis l'initialisation (en millisecondes)
        int tempsCour = FenetreTP::obtenirTemps();
        // calculer un nouveau dt (sauf la première fois)
        if ( tempsPrec ) Etat::dt = ( tempsCour - tempsPrec )/1000.0;
        // se préparer pour la prochaine fois
        tempsPrec = tempsCour;
    }

    if ( Etat::enmouvement )
    {
        if ( getenv("DEMO") != NULL )
        {
            camera.theta += 3.0 * Etat::dt;
            camera.verifierAngles();
        }
    }

    aquarium->calculerPhysique( );
}

void chargerNuanceurs()
{
    // charger le nuanceur de base
    {
        // créer le programme
        progBase = glCreateProgram();

        // attacher le nuanceur de sommets
        {
            GLuint nuanceurObj = glCreateShader( GL_VERTEX_SHADER );
            glShaderSource( nuanceurObj, 1, &ProgNuanceur::chainesSommetsBase, NULL );
            glCompileShader( nuanceurObj );
            glAttachShader( progBase, nuanceurObj );
            ProgNuanceur::afficherLogCompile( nuanceurObj );
        }
        // attacher le nuanceur de fragments
        {
            GLuint nuanceurObj = glCreateShader( GL_FRAGMENT_SHADER );
            glShaderSource( nuanceurObj, 1, &ProgNuanceur::chainesFragmentsBase, NULL );
            glCompileShader( nuanceurObj );
            glAttachShader( progBase, nuanceurObj );
            ProgNuanceur::afficherLogCompile( nuanceurObj );
        }

        // faire l'édition des liens du programme
        glLinkProgram( progBase );
        ProgNuanceur::afficherLogLink( progBase );

        // demander la "Location" des variables
        if ( ( locColorBase = glGetAttribLocation( progBase, "Color" ) ) == -1 ) std::cerr << "!!! pas trouvé la \"Location\" de Color" << std::endl;
        if ( ( locmatrModelBase = glGetUniformLocation( progBase, "matrModel" ) ) == -1 ) std::cerr << "!!! pas trouvé la \"Location\" de matrModel" << std::endl;
        if ( ( locmatrVisuBase = glGetUniformLocation( progBase, "matrVisu" ) ) == -1 ) std::cerr << "!!! pas trouvé la \"Location\" de matrVisu" << std::endl;
        if ( ( locmatrProjBase = glGetUniformLocation( progBase, "matrProj" ) ) == -1 ) std::cerr << "!!! pas trouvé la \"Location\" de matrProj" << std::endl;
    }

    // charger le nuanceur de ce TP
    {
        // créer le programme
        prog = glCreateProgram();

        // attacher le nuanceur de sommets
        const GLchar *chainesSommets = ProgNuanceur::lireNuanceur( "nuanceurSommets.glsl" );
        if ( chainesSommets != NULL )
        {
            GLuint nuanceurObj = glCreateShader( GL_VERTEX_SHADER );
            glShaderSource( nuanceurObj, 1, &chainesSommets, NULL );
            glCompileShader( nuanceurObj );
            glAttachShader( prog, nuanceurObj );
            ProgNuanceur::afficherLogCompile( nuanceurObj );
            delete [] chainesSommets;
        }
        // partie 3: remplir le fichier "nuanceurGeometrie.glsl"
        // attacher le nuanceur de géométrie
        const GLchar *chainesGeometrie = ProgNuanceur::lireNuanceur( "nuanceurGeometrie.glsl" );
        if ( chainesGeometrie != NULL )
        {
            GLuint nuanceurObj = glCreateShader( GL_GEOMETRY_SHADER );
            glShaderSource( nuanceurObj, 1, &chainesGeometrie, NULL );
            glCompileShader( nuanceurObj );
            glAttachShader( prog, nuanceurObj );
            ProgNuanceur::afficherLogCompile( nuanceurObj );
            delete [] chainesGeometrie;
        }
        // attacher le nuanceur de fragments
        const GLchar *chainesFragments = ProgNuanceur::lireNuanceur( "nuanceurFragments.glsl" );
        if ( chainesFragments != NULL )
        {
            GLuint nuanceurObj = glCreateShader( GL_FRAGMENT_SHADER );
            glShaderSource( nuanceurObj, 1, &chainesFragments, NULL );
            glCompileShader( nuanceurObj );
            glAttachShader( prog, nuanceurObj );
            ProgNuanceur::afficherLogCompile( nuanceurObj );
            delete [] chainesFragments;
        }

        // faire l'édition des liens du programme
        glLinkProgram( prog );
        ProgNuanceur::afficherLogLink( prog );

        // demander la "Location" des variables
        if ( ( locmatrModel = glGetUniformLocation( prog, "matrModel" ) ) == -1 ) std::cerr << "!!! pas trouvé la \"Location\" de matrModel" << std::endl;
        if ( ( locmatrVisu = glGetUniformLocation( prog, "matrVisu" ) ) == -1 ) std::cerr << "!!! pas trouvé la \"Location\" de matrVisu" << std::endl;
        if ( ( locmatrProj = glGetUniformLocation( prog, "matrProj" ) ) == -1 ) std::cerr << "!!! pas trouvé la \"Location\" de matrProj" << std::endl;
        if ( ( indLightSource = glGetUniformBlockIndex( prog, "LightSourceParameters" ) ) == GL_INVALID_INDEX ) std::cerr << "!!! pas trouvé l'\"index\" de LightSource" << std::endl;
        if ( ( indFrontMaterial = glGetUniformBlockIndex( prog, "MaterialParameters" ) ) == GL_INVALID_INDEX ) std::cerr << "!!! pas trouvé l'\"index\" de FrontMaterial" << std::endl;
        if ( ( indLightModel = glGetUniformBlockIndex( prog, "LightModelParameters" ) ) == GL_INVALID_INDEX ) std::cerr << "!!! pas trouvé l'\"index\" de LightModel" << std::endl;

        // charger les ubo (valeurs fixes qui ne varient pas dans ce TP)
        {
            glBindBuffer( GL_UNIFORM_BUFFER, ubo[0] );
            glBufferData( GL_UNIFORM_BUFFER, sizeof(LightSource), &LightSource, GL_DYNAMIC_COPY );
            glBindBuffer( GL_UNIFORM_BUFFER, 0 );
            const GLuint bindingIndex = 0;
            glBindBufferBase( GL_UNIFORM_BUFFER, bindingIndex, ubo[0] );
            glUniformBlockBinding( prog, indLightSource, bindingIndex );
        }
        {
            glBindBuffer( GL_UNIFORM_BUFFER, ubo[1] );
            glBufferData( GL_UNIFORM_BUFFER, sizeof(FrontMaterial), &FrontMaterial, GL_DYNAMIC_COPY );
            glBindBuffer( GL_UNIFORM_BUFFER, 0 );
            const GLuint bindingIndex = 1;
            glBindBufferBase( GL_UNIFORM_BUFFER, bindingIndex, ubo[1] );
            glUniformBlockBinding( prog, indFrontMaterial, bindingIndex );
        }
        {
            glBindBuffer( GL_UNIFORM_BUFFER, ubo[2] );
            glBufferData( GL_UNIFORM_BUFFER, sizeof(LightModel), &LightModel, GL_DYNAMIC_COPY );
            glBindBuffer( GL_UNIFORM_BUFFER, 0 );
            const GLuint bindingIndex = 2;
            glBindBufferBase( GL_UNIFORM_BUFFER, bindingIndex, ubo[2] );
            glUniformBlockBinding( prog, indLightModel, bindingIndex );
        }
    }
}

void FenetreTP::initialiser()
{
    // donner la couleur de fond
    glm::vec4 couleurFond( 0.1, 0.1, 0.1, 1.0 );
    glClearColor( couleurFond.r, couleurFond.g, couleurFond.b, couleurFond.a );

    // activer les états openGL
    glEnable( GL_DEPTH_TEST );

    // activer le mélange de couleur pour la transparence
    glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );

    // allouer les UBO pour les variables uniformes
    glGenBuffers( 3, ubo );

    // charger les nuanceurs
    chargerNuanceurs();
    glUseProgram( prog );

    // créer l'aquarium
    aquarium = new Aquarium();
}

void FenetreTP::conclure()
{
    delete aquarium;
    glDeleteBuffers( 3, ubo );
}

void FenetreTP::afficherScene( )
{
    // effacer les tampons de couleur, de profondeur et de stencil
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT );

    glUseProgram( progBase );

    // définir le pipeline graphique
    // ... la matrice de projection
    matrProj.Perspective( 50.0, (GLdouble) largeur_ / (GLdouble) hauteur_, 0.1, 100.0 );
    glUniformMatrix4fv( locmatrProjBase, 1, GL_FALSE, matrProj );

    // ... la matrice de visualisation (point de vue)
    if ( Etat::theiereChoisie > 0 )
        aquarium->positionnerCamera( Etat::theiereChoisie );
    else
        camera.definir();
    glUniformMatrix4fv( locmatrVisuBase, 1, GL_FALSE, matrVisu );

    // ... la matrice de modélisation
    matrModel.LoadIdentity();
    glUniformMatrix4fv( locmatrModelBase, 1, GL_FALSE, matrModel );

    // afficher les axes
    if ( Etat::afficheAxes ) FenetreTP::afficherAxes(4);

    // afficher l'aquarium
    aquarium->afficher();

    // permuter tampons avant et arrière
    swap();
}

void FenetreTP::redimensionner( GLsizei w, GLsizei h )
{
    glViewport( 0, 0, w, h );
}

void FenetreTP::clavier( TP_touche touche )
{
    switch ( touche )
    {
    case TP_ECHAP:
    case TP_q: // Quitter l'application
        quit();
        break;

    case TP_x: // Activer/désactiver l'affichage des axes
        Etat::afficheAxes = !Etat::afficheAxes;
        std::cout << "// Affichage des axes ? " << ( Etat::afficheAxes ? "OUI" : "NON" ) << std::endl;
        break;

    case TP_v: // Recharger les fichiers des nuanceurs et recréer le programme
        chargerNuanceurs();
        std::cout << "// Recharger nuanceurs" << std::endl;
        break;

    case TP_ESPACE: // Mettre en pause ou reprendre l'animation
        Etat::enmouvement = !Etat::enmouvement;
        break;

    case TP_CROCHETGAUCHE: // Inverser l'avancement du temps (il recule)
        Etat::sens = -1.0;
        std::cout << " Etat::sens= " << Etat::sens << " Etat::dt= " << Etat::dt << std::endl;
        break;
    case TP_CROCHETDROIT: // Remettre l'avancement du temps normalement
        Etat::sens = +1.0;
        std::cout << " Etat::sens= " << Etat::sens << " Etat::dt= " << Etat::dt << std::endl;
        break;

    case TP_g: // Permuter l'affichage en fil de fer ou plein
        Etat::modePolygone = ( Etat::modePolygone == GL_FILL ) ? GL_LINE : GL_FILL;
        std::cout << " Etat::modePolygone=" << ( Etat::modePolygone == GL_FILL ? "GL_LINE" : "GL_FILL" ) << std::endl;
        break;
    case TP_c: // Permuter l'affichage des faces arrières
        Etat::culling = !Etat::culling;
        std::cout << " Etat::culling=" << Etat::culling << std::endl;
        break;

    case TP_DROITE: // Augmenter la dimension de la boîte en X
        Etat::bDim.x += 0.1;
        std::cout << " Etat::bDim= " << Etat::bDim.x << " x " << Etat::bDim.y << " x " << Etat::bDim.z << std::endl;
        break;
    case TP_GAUCHE: // Diminuer la dimension de la boîte en X
        if ( Etat::bDim.x > 10.0 ) Etat::bDim.x -= 0.1;
        std::cout << " Etat::bDim= " << Etat::bDim.x << " x " << Etat::bDim.y << " x " << Etat::bDim.z << std::endl;
        break;
    case TP_HAUT: // Augmenter la dimension de la boîte en Z
        Etat::bDim.z += 0.1;
        std::cout << " Etat::bDim= " << Etat::bDim.x << " x " << Etat::bDim.y << " x " << Etat::bDim.z << std::endl;
        break;
    case TP_BAS: // Diminuer la dimension de la boîte en Z
        if ( Etat::bDim.z > 10.0 ) Etat::bDim.z -= 0.1;
        std::cout << " Etat::bDim= " << Etat::bDim.x << " x " << Etat::bDim.y << " x " << Etat::bDim.z << std::endl;
        break;

    case TP_PLUS: // Incrémenter la distance de la caméra
    case TP_EGAL:
        camera.dist--;
        std::cout << " camera.dist=" << camera.dist << std::endl;
        break;

    case TP_SOULIGNE:
    case TP_MOINS: // Décrémenter la distance de la caméra
        camera.dist++;
        std::cout << " camera.dist=" << camera.dist << std::endl;
        break;

    case TP_i: // Calculer ou non l'illumination de Phong
        Etat::illumination = !Etat::illumination;
        std::cout << " Etat::illumination=" << Etat::illumination << std::endl;
        break;

    case TP_0: // Afficher le point de vue « normal » (extérieur à l'aquarium)
    case TP_1: // Afficher le point de vue de la théière 1
    case TP_2: // Afficher le point de vue de la théière 2
    case TP_3: // Afficher le point de vue de la théière 3
    case TP_4: // Afficher le point de vue de la théière 4
    case TP_5: // Afficher le point de vue de la théière 5
    case TP_6: // Afficher le point de vue de la théière 6
    case TP_7: // Afficher le point de vue de la théière 7
    case TP_8: // Afficher le point de vue de la théière 8
    case TP_9: // Afficher le point de vue de la théière 9
        Etat::theiereChoisie = touche - TP_0;
        std::cout << " Etat::theiereChoisie=" << Etat::theiereChoisie << std::endl;
        break;

    case TP_d: // Mode debug : (ne pas utiliser le stencil)
        Etat::debug = !Etat::debug;
        std::cout << " Etat::debug=" << Etat::debug << std::endl;
        break;

    default:
        std::cout << " touche inconnue : " << (char) touche << std::endl;
        imprimerFichier( "touches.txt" );
        break;
    }
}

static bool presse = false;
void FenetreTP::sourisClic( int button, int state, int x, int y )
{
    presse = ( state == TP_PRESSE );
    if ( presse )
    {
        Etat::sourisPosPrec.x = x;
        Etat::sourisPosPrec.y = y;
    }
}

void FenetreTP::sourisMolette( int x, int y ) // Changer la distance de la caméra
{
    const int sens = +1;
    camera.dist -= 0.5 * sens*y;
    if ( camera.dist < 1.0 ) camera.dist = 1.0;
    else if ( camera.dist > 70.0 - Etat::bDim.y ) camera.dist = 70.0 - Etat::bDim.y;
}

void FenetreTP::sourisMouvement( int x, int y )
{
    if ( presse )
    {
        int dx = x - Etat::sourisPosPrec.x;
        int dy = y - Etat::sourisPosPrec.y;
        camera.theta -= dx / 3.0;
        camera.phi   += dy / 3.0;

        camera.verifierAngles();

        Etat::sourisPosPrec.x = x;
        Etat::sourisPosPrec.y = y;
    }
}

int main( int argc, char *argv[] )
{
    // créer une fenêtre
    FenetreTP fenetre( "INF2705 TP" );

    // allouer des ressources et définir le contexte OpenGL
    fenetre.initialiser();

    bool boucler = true;
    while ( boucler )
    {
        // mettre à jour la physique
        calculerPhysique( );

        // affichage
        fenetre.afficherScene();

        // récupérer les événements et appeler la fonction de rappel
        boucler = fenetre.gererEvenement();
    }

    // détruire les ressources OpenGL allouées
    fenetre.conclure();

    return 0;
}
