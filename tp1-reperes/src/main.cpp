// Prénoms, noms et matricule des membres de l'équipe:
// - Ali AMEZIANE HASSANI (matricule1)
// - Eliès JURQUET (2162621)
//#pragma message (": *************** Identifiez les membres de l'équipe dans le fichier 'main.cpp' et commentez cette ligne. ***************")

#if defined(_WIN32) || defined(WIN32)
#pragma warning ( disable : 4244 4305 )
// warning C4244: 'conversion' conversion from 'double' to 'float', possible loss of data
// warning C4305: 'argument' : truncation from 'double' to 'float'
#endif

#include <iostream>
#include <math.h>
#include "inf2705-matrice.h"
#include "inf2705-nuanceur.h"
#include "inf2705-fenetre.h"
#include "inf2705-forme.h"
#include "inf2705-theiere.h"
#include "Etat.h"
#include "Pipeline.h"
#include "Camera.h"
#include "Helicoptere.h"

// l'helicoptere, le cube englobant
Helicoptere *helicoptere = NULL;
FormeCube *cubeFil = NULL;

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

    helicoptere->calculerPhysique();
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
        if ( ( locmatrModel = glGetUniformLocation( progBase, "matrModel" ) ) == -1 ) std::cerr << "!!! pas trouvé la \"Location\" de matrModel" << std::endl;
        if ( ( locmatrVisu = glGetUniformLocation( progBase, "matrVisu" ) ) == -1 ) std::cerr << "!!! pas trouvé la \"Location\" de matrVisu" << std::endl;
        if ( ( locmatrProj = glGetUniformLocation( progBase, "matrProj" ) ) == -1 ) std::cerr << "!!! pas trouvé la \"Location\" de matrProj" << std::endl;
    }
}

void FenetreTP::initialiser()
{
    // donner la couleur de fond
    glm::vec4 couleurFond( 0.15, 0.15, 0.15, 1.0 );
    glClearColor( couleurFond.r, couleurFond.g, couleurFond.b, couleurFond.a );

    // activer les états openGL
    glEnable( GL_DEPTH_TEST );

    // charger les nuanceurs
    chargerNuanceurs();
    glUseProgram( progBase );

    // créer l'helicoptere
    helicoptere = new Helicoptere();

    // créer un cube englobant
    cubeFil = new FormeCube( 1.0, false );
}

void FenetreTP::conclure()
{
    delete helicoptere;
    delete cubeFil;
}

void afficherDecoration()
{
    // afficher la boîte englobante
    GLint locColor = glGetAttribLocation( progBase, "Color" );
    glVertexAttrib3f( locColor, 1.0, 0.5, 0.5 ); // équivalent au glColor() de OpenGL 2.x
    matrModel.PushMatrix();{
        matrModel.Translate( 0, Etat::dimBoite/2, 0 );
        matrModel.Scale( Etat::dimBoite, Etat::dimBoite, Etat::dimBoite );
        glUniformMatrix4fv( locmatrModel, 1, GL_FALSE, matrModel );
        cubeFil->afficher();
    }matrModel.PopMatrix(); glUniformMatrix4fv( locmatrModel, 1, GL_FALSE, matrModel );
}

void FenetreTP::afficherScene()
{
    // effacer l'écran et le tampon de profondeur
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

    glUseProgram( progBase );

    // définir le pipeline graphique
    matrProj.Perspective( 20.0, (GLdouble) largeur_ / (GLdouble) hauteur_, 0.1, 100.0 );
    glUniformMatrix4fv( locmatrProj, 1, GL_FALSE, matrProj ); // informer la carte graphique des changements faits à la matrice

    camera.definir();
    glUniformMatrix4fv( locmatrVisu, 1, GL_FALSE, matrVisu ); // informer la carte graphique des changements faits à la matrice

    matrModel.LoadIdentity();
    glUniformMatrix4fv( locmatrModel, 1, GL_FALSE, matrModel ); // informer la carte graphique des changements faits à la matrice

    // afficher les axes
    if ( Etat::afficheAxes ) FenetreTP::afficherAxes();

    // tracer la boite englobante
    afficherDecoration();

    // afficher des triangles en plein ou en fil de fer ?
    glPolygonMode( GL_FRONT_AND_BACK, Etat::modePolygone );
    // ne pas afficher les triangles dont on voit la face arrière ? (touche 'c')
    if ( Etat::culling ) glEnable( GL_CULL_FACE ); else glDisable( GL_CULL_FACE );

    // tracer l'helicoptere
    helicoptere->afficher();

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

        // case TP_v: // Recharger les fichiers des nuanceurs et recréer le programme
        //    chargerNuanceurs();
        //    std::cout << "// Recharger nuanceurs" << std::endl;
        //    break;

    case TP_i: // Réinitiliaser le point de vue et l'hélicoptère
        camera.initVar(); helicoptere->initVar();
        break;
    case TP_g: // Permuter l'affichage en fil de fer ou plein
        Etat::modePolygone = ( Etat::modePolygone == GL_FILL ) ? GL_LINE : GL_FILL;
        break;
    case TP_c: // Permuter l'affichage des faces arrières
        Etat::culling = !Etat::culling;
        break;

    case TP_l: // Utiliser LookAt ou Translate+Rotate pour placer la caméra
        camera.modeLookAt = !camera.modeLookAt;
        std::cout << " camera.modeLookAt=" << camera.modeLookAt << std::endl;
        break;

    case TP_m: // Choisir le modèle affiché: cube, théière
        if ( ++Etat::modele > 2 ) Etat::modele = 1;
        std::cout << " Etat::modele=" << Etat::modele << std::endl;
        break;

    case TP_SOULIGNE:
    case TP_MOINS: // Reculer la caméra
        camera.dist += 0.1;
        break;
    case TP_PLUS: // Avancer la caméra
    case TP_EGAL:
        if ( camera.dist > 1.0 ) camera.dist -= 0.1;
        break;

    case TP_DROITE: // Déplacer l'hélicoptère vers +X
        if ( helicoptere->position.x + helicoptere->taille < Etat::dimBoite/2 ) helicoptere->position.x += 0.1;
        break;
    case TP_GAUCHE: // Déplacer l'hélicoptère vers -X
        if ( helicoptere->position.x - helicoptere->taille > -Etat::dimBoite/2 ) helicoptere->position.x -= 0.1;
        break;
    case TP_HAUT: // Déplacer l'hélicoptère vers +Y
        if ( helicoptere->position.y + helicoptere->taille < Etat::dimBoite ) helicoptere->position.y += 0.1;
        break;
    case TP_BAS: // Déplacer l'hélicoptère vers -Y
        if ( helicoptere->position.y - helicoptere->taille > 0.0 ) helicoptere->position.y -= 0.1;
        break;
    case TP_PAGEPREC: // Déplacer l'hélicoptère vers +Z
        if ( helicoptere->position.z + helicoptere->taille < Etat::dimBoite/2 ) helicoptere->position.z += 0.1;
        break;
    case TP_PAGESUIV: // Déplacer l'hélicoptère vers -Z
        if ( helicoptere->position.z - helicoptere->taille > -Etat::dimBoite/2 ) helicoptere->position.z -= 0.1;
        break;

    case TP_FIN:
    case TP_f: // Diminuer la taille de la tête
        if ( helicoptere->taille > 0.25 ) helicoptere->taille -= 0.05;
        helicoptere->verifierAngles();
        std::cout << " helicoptere->taille=" << helicoptere->taille << " helicoptere->angleCorps=" << helicoptere->angleCorps << " helicoptere->anglePale=" << helicoptere->anglePale << " helicoptere->anglePatte=" << helicoptere->anglePatte << std::endl;
        break;
    case TP_DEBUT:
    case TP_r: // Augmenter la taille de la tête
        helicoptere->taille += 0.05;
        helicoptere->verifierAngles();
        std::cout << " helicoptere->taille=" << helicoptere->taille << " helicoptere->angleCorps=" << helicoptere->angleCorps << " helicoptere->anglePale=" << helicoptere->anglePale << " helicoptere->anglePatte=" << helicoptere->anglePatte << std::endl;
        break;

    case TP_VIRGULE: // Diminuer l'angle de rotation de l'hélicoptère
        helicoptere->angleCorps -= 2.0;
        if ( helicoptere->angleCorps < 0.0 ) helicoptere->angleCorps = 0.0;
        std::cout << " helicoptere->taille=" << helicoptere->taille << " helicoptere->angleCorps=" << helicoptere->angleCorps << " helicoptere->anglePale=" << helicoptere->anglePale << " helicoptere->anglePatte=" << helicoptere->anglePatte << std::endl;
        break;
    case TP_POINT: // Augmenter l'angle de rotation de l'hélicoptère
        helicoptere->angleCorps += 2.0;
        if ( helicoptere->angleCorps > 30.0 ) helicoptere->angleCorps = 30.0;
        std::cout << " helicoptere->taille=" << helicoptere->taille << " helicoptere->angleCorps=" << helicoptere->angleCorps << " helicoptere->anglePale=" << helicoptere->anglePale << " helicoptere->anglePatte=" << helicoptere->anglePatte << std::endl;
        break;

    case TP_CROCHETDROIT:
    case TP_o: // Diminuer l'angle des pattes
        helicoptere->anglePatte -= 2.0;
        helicoptere->verifierAngles();
        std::cout << " helicoptere->taille=" << helicoptere->taille << " helicoptere->angleCorps=" << helicoptere->angleCorps << " helicoptere->anglePale=" << helicoptere->anglePale << " helicoptere->anglePatte=" << helicoptere->anglePatte << std::endl;
        break;
    case TP_CROCHETGAUCHE:
    case TP_p: // Augmenter l'angle des pattes
        helicoptere->anglePatte += 2.0;
        helicoptere->verifierAngles();
        std::cout << " helicoptere->taille=" << helicoptere->taille << " helicoptere->angleCorps=" << helicoptere->angleCorps << " helicoptere->anglePale=" << helicoptere->anglePale << " helicoptere->anglePatte=" << helicoptere->anglePatte << std::endl;
        break;

    case TP_j: // Diminuer l'angle des pales
        helicoptere->anglePale -= 2.0;
        helicoptere->verifierAngles();
        std::cout << " helicoptere->taille=" << helicoptere->taille << " helicoptere->angleCorps=" << helicoptere->angleCorps << " helicoptere->anglePale=" << helicoptere->anglePale << " helicoptere->anglePatte=" << helicoptere->anglePatte << std::endl;
        break;
    case TP_u: // Augmenter l'angle des pales
        helicoptere->anglePale += 2.0;
        helicoptere->verifierAngles();
        std::cout << " helicoptere->taille=" << helicoptere->taille << " helicoptere->angleCorps=" << helicoptere->angleCorps << " helicoptere->anglePale=" << helicoptere->anglePale << " helicoptere->anglePatte=" << helicoptere->anglePatte << std::endl;
        break;

    case TP_b: // Incrémenter la dimension de la boite
        Etat::dimBoite += 0.05;
        std::cout << " Etat::dimBoite=" << Etat::dimBoite << std::endl;
        break;
    case TP_h: // Décrémenter la dimension de la boite
        Etat::dimBoite -= 0.05;
        if ( Etat::dimBoite < 1.0 ) Etat::dimBoite = 1.0;
        std::cout << " Etat::dimBoite=" << Etat::dimBoite << std::endl;
        break;

    case TP_ESPACE: // Mettre en pause ou reprendre l'animation
        Etat::enmouvement = !Etat::enmouvement;
        break;

    case TP_s: // Sauvegarder une copie de la fenêtre dans un fichier
        sauvegarderFenetre( );
        break;

    default:
        std::cout << " touche inconnue : " << (char) touche << std::endl;
        imprimerFichier( "touches.txt" );
        break;
    }
}

glm::ivec2 sourisPosPrec(0,0);
static bool pressed = false;
void FenetreTP::sourisClic( int button, int state, int x, int y )
{
    // button est un parmi { TP_BOUTON_GAUCHE, TP_BOUTON_MILIEU, TP_BOUTON_DROIT }
    // state  est un parmi { TP_PRESSE, DL_RELEASED }
    pressed = ( state == TP_PRESSE );
    switch ( button )
    {
    case TP_BOUTON_GAUCHE: // Déplacer (modifier angles) la caméra
        sourisPosPrec.x = x;
        sourisPosPrec.y = y;
        break;
    }
}

void FenetreTP::sourisMolette( int x, int y )
{
    const int sens = +1;
    camera.dist -= 0.2 * sens*y;
    if ( camera.dist < 15.0 ) camera.dist = 15.0;
    else if ( camera.dist > 70.0 ) camera.dist = 70.0;
}

void FenetreTP::sourisMouvement( int x, int y )
{
    if ( pressed )
    {
        int dx = x - sourisPosPrec.x;
        int dy = y - sourisPosPrec.y;
        camera.theta -= dx / 3.0;
        camera.phi   += dy / 3.0;
        sourisPosPrec.x = x;
        sourisPosPrec.y = y;
        camera.verifierAngles();
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
