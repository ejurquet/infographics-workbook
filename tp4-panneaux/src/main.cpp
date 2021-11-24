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
#include "inf2705-texture.h"
#include "inf2705-forme.h"
#include "Etat.h"
#include "Pipeline.h"
#include "Camera.h"

// les formes
FormeSphere *demisphere = NULL;
FormeDisque *disque = NULL;

////////////////////////////////////////
// déclaration des variables globales //
////////////////////////////////////////

// les particules
struct Particule
{
    glm::vec3 position;         // en unités
    glm::vec4 couleur;          // couleur actuelle de la particule
    glm::vec3 vitesse;          // en unités/seconde
    GLfloat tempsDeVieRestant;  // temps de vie restant en secondes
    // (vous pouvez ajouter d'autres éléments, mais il faudra les prévoir dans les varyings)
};
// le tableau de particules
const unsigned int MAXNPARTICULES = 1000000;
static Particule part[MAXNPARTICULES] = {{{0,0,0},{0,0,0,0},{0,0,0},0}};
//   ( puisque tempsDeVieRestant == 0.0 au début, toutes les particules seront initialisées par le nuanceur de rétroaction)

// les variables uniformes
struct
{
    float tempsDeVieMax;       // temps de vie maximal (en secondes)
    float temps;               // le temps courant dans la simulation (en secondes)
    float dt;                  // intervalle entre chaque affichage (en secondes)
    float gravite;             // gravité utilisée dans le calcul de la position de la particule
    float pointsize;           // taille des points (en pixels)
} varsUnif = { 15.0, 0.0, float(1.0/60.0), float(10.0), 9.0 };

void calculerPhysique( )
{
    // ajuster le dt selon la fréquence d'affichage
    {
        static int tempsPrec = 0;
        // obtenir le temps depuis l'initialisation (en millisecondes)
        int tempsCour = FenetreTP::obtenirTemps();
        // calculer un nouveau dt (sauf la première fois)
        if ( tempsPrec ) varsUnif.dt = ( tempsCour - tempsPrec )/1000.0;
        // se préparer pour la prochaine fois
        tempsPrec = tempsCour;
    }

    if ( Etat::enmouvement )
    {
        // déplacer les particules en utilisant le nuanceur de sommets pour la rétroaction
        glUseProgram( progRetroaction );

        glUniform3fv( locposPuitsRetroaction, 1, glm::value_ptr(Etat::posPuits) );
        glUniform3fv( locbDimRetroaction, 1, glm::value_ptr(Etat::bDim) );

        // charger les blocs de variables uniformes (ubo)
        if ( indvarsUnifRetroaction != GL_INVALID_INDEX )
        {
            glBindBuffer( GL_UNIFORM_BUFFER, ubo );
            glBufferData( GL_UNIFORM_BUFFER, sizeof(varsUnif), &varsUnif, GL_DYNAMIC_COPY );
            glBindBuffer( GL_UNIFORM_BUFFER, 0 );
            const GLuint bindingIndex = 0;
            glBindBufferBase( GL_UNIFORM_BUFFER, bindingIndex, ubo );
            glUniformBlockBinding( prog, indvarsUnifRetroaction, bindingIndex );
        }

        // faire les transformations de retour (rétroaction)
        glBindBufferBase( GL_TRANSFORM_FEEDBACK_BUFFER, 0, vbo[1] );

        glBindVertexArray( vao[1] );         // sélectionner le second VAO
        // se préparer
        glBindBuffer( GL_ARRAY_BUFFER, vbo[0] );  // utiliser les valeurs courantes
        glVertexAttribPointer( locVertexRetroaction, 3, GL_FLOAT, GL_FALSE, sizeof(Particule), reinterpret_cast<void*>( offsetof(Particule,position) ) );
        glVertexAttribPointer( locColorRetroaction, 4, GL_FLOAT, GL_FALSE, sizeof(Particule), reinterpret_cast<void*>( offsetof(Particule,couleur) ) );
        glVertexAttribPointer( locvitesseRetroaction, 3, GL_FLOAT, GL_FALSE, sizeof(Particule), reinterpret_cast<void*>( offsetof(Particule,vitesse) ) );
        glVertexAttribPointer( loctempsDeVieRestantRetroaction, 1, GL_FLOAT, GL_FALSE, sizeof(Particule), reinterpret_cast<void*>( offsetof(Particule,tempsDeVieRestant) ) );

        // débuter la requête (si impression)
        if ( Etat::impression )
            glBeginQuery( GL_TRANSFORM_FEEDBACK_PRIMITIVES_WRITTEN, requete );

        // désactiver le tramage
        glEnable( GL_RASTERIZER_DISCARD );
        // débuter la rétroaction
        glBeginTransformFeedback( GL_POINTS );
        // « dessiner » pour la rétroaction
        glDrawArrays( GL_POINTS, 0, sizeof(part)/sizeof(Particule) );
        // terminer la rétroaction
        glEndTransformFeedback();
        // réactiver le tramage
        glDisable( GL_RASTERIZER_DISCARD );

        // terminer la requête (si impression)
        if ( Etat::impression )
            glEndQuery( GL_TRANSFORM_FEEDBACK_PRIMITIVES_WRITTEN );

        glBindVertexArray( 0 );              // désélectionner le VAO

        if ( Etat::impression )
        {
            glFlush(); // attendre que la carte graphique ait terminé le traitement
            // obtenir et imprimer les résultats
            GLuint nresul;
            glGetQueryObjectuiv( requete, GL_QUERY_RESULT, &nresul );
            std::cout << " Nombre total de particules (=nresul)=" << nresul << std::endl;

            if ( nresul )
            {
                const int NRETOUR = 10;
                Particule Retour[NRETOUR];
                if ( nresul > NRETOUR ) nresul = NRETOUR; // on veut seulement les NRETOUR premières particules
                glGetBufferSubData( GL_TRANSFORM_FEEDBACK_BUFFER, 0, nresul*sizeof(Particule), Retour );

                for ( unsigned int i = 0; i < nresul; ++i )
                    std::cout << "   part["<<i<<"]"
                              << " .position[]="
                              << " " << Retour[i].position[0]
                              << " " << Retour[i].position[1]
                              << " " << Retour[i].position[2]
                              << " .couleur[]="
                              << " " << Retour[i].couleur[0]
                              << " " << Retour[i].couleur[1]
                              << " " << Retour[i].couleur[2]
                              << " " << Retour[i].couleur[3]
                              << " .vitesse[]="
                              << " " << Retour[i].vitesse[0]
                              << " " << Retour[i].vitesse[1]
                              << " " << Retour[i].vitesse[2]
                              << " .tempsDeVieRestant[]="
                              << " " << Retour[i].tempsDeVieRestant
                              << std::endl;
            }

            Etat::impression = false;
        }

        // échanger les numéros des deux VBO
        std::swap( vbo[0], vbo[1] );

        // avancer le temps
        varsUnif.temps += varsUnif.dt;

        // faire varier la texture utilisée
        if ( Etat::boucleTexture )
            if ( fmod( varsUnif.temps, 5 ) <= varsUnif.dt )
                if ( ++Etat::texnumero > 3 ) Etat::texnumero = 1;

#if 0
        // faire varier la taille de la boite
        static int sensX = 1;
        Etat::bDim.x += sensX * 0.05;
        if ( Etat::bDim.x < 85. ) sensX = +1;
        else if ( Etat::bDim.x > 115. ) sensX = -1;

        static int sensY = 1;
        Etat::bDim.y += sensY * 0.03;
        if ( Etat::bDim.y < 85. ) sensY = +1;
        else if ( Etat::bDim.y > 115. ) sensY = -1;

        static int sensZ = 1;
        Etat::bDim.z += sensZ * 0.07;
        if ( Etat::bDim.z < 40. ) sensZ = +1;
        else if ( Etat::bDim.z > 100. ) sensZ = -1;

        Etat::verifierPositionPuits();
#endif

        FenetreTP::VerifierErreurGL("calculerPhysique");
    }
}

void chargerTextures()
{
    unsigned char *pixels;
    GLsizei largeur, hauteur;
    //if ( ( pixels = ChargerImage( "textures/echiquier.bmp", largeur, hauteur ) ) != NULL )
    if ( ( pixels = ChargerImage( "textures/oiseau.bmp", largeur, hauteur ) ) != NULL )
    {
        glGenTextures( 1, &Etat::textureOISEAU );
        glBindTexture( GL_TEXTURE_2D, Etat::textureOISEAU );
        glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA, largeur, hauteur, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels );
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
        //glGenerateMipmap( GL_TEXTURE_2D );
        glBindTexture( GL_TEXTURE_2D, 0 );
        delete[] pixels;
    }
    if ( ( pixels = ChargerImage( "textures/flocon.bmp", largeur, hauteur ) ) != NULL )
    {
        glGenTextures( 1, &Etat::textureFLOCON );
        glBindTexture( GL_TEXTURE_2D, Etat::textureFLOCON );
        glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA, largeur, hauteur, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels );
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
        //glGenerateMipmap( GL_TEXTURE_2D );
        glBindTexture( GL_TEXTURE_2D, 0 );
        delete[] pixels;
    }
}

void chargerNuanceurs()
{
    // charger le nuanceur de base
    std::cout << "Charger le nuanceur de base" << std::endl;
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
        //if ( ( locVertexBase = glGetAttribLocation( progBase, "Vertex" ) ) == -1 ) std::cerr << "!!! pas trouvé la \"Location\" de Vertex" << std::endl;
        if ( ( locColorBase = glGetAttribLocation( progBase, "Color" ) ) == -1 ) std::cerr << "!!! pas trouvé la \"Location\" de Color" << std::endl;
        if ( ( locmatrModelBase = glGetUniformLocation( progBase, "matrModel" ) ) == -1 ) std::cerr << "!!! pas trouvé la \"Location\" de matrModel" << std::endl;
        if ( ( locmatrVisuBase = glGetUniformLocation( progBase, "matrVisu" ) ) == -1 ) std::cerr << "!!! pas trouvé la \"Location\" de matrVisu" << std::endl;
        if ( ( locmatrProjBase = glGetUniformLocation( progBase, "matrProj" ) ) == -1 ) std::cerr << "!!! pas trouvé la \"Location\" de matrProj" << std::endl;
    }

    // charger le nuanceur de ce TP
    std::cout << "Charger le nuanceur de ce TP" << std::endl;
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
        if ( ( locVertex = glGetAttribLocation( prog, "Vertex" ) ) == -1 ) std::cerr << "!!! pas trouvé la \"Location\" de Vertex" << std::endl;
        if ( ( locColor = glGetAttribLocation( prog, "Color" ) ) == -1 ) std::cerr << "!!! pas trouvé la \"Location\" de Color" << std::endl;
        if ( ( locvitesse = glGetAttribLocation( prog, "vitesse" ) ) == -1 ) std::cerr << "!!! pas trouvé la \"Location\" de vitesse" << std::endl;
        if ( ( loctempsDeVieRestant = glGetAttribLocation( prog, "tempsDeVieRestant" ) ) == -1 ) std::cerr << "!!! pas trouvé la \"Location\" de tempsDeVieRestant" << std::endl;
        if ( ( locmatrModel = glGetUniformLocation( prog, "matrModel" ) ) == -1 ) std::cerr << "!!! pas trouvé la \"Location\" de matrModel" << std::endl;
        if ( ( locmatrVisu = glGetUniformLocation( prog, "matrVisu" ) ) == -1 ) std::cerr << "!!! pas trouvé la \"Location\" de matrVisu" << std::endl;
        if ( ( locmatrProj = glGetUniformLocation( prog, "matrProj" ) ) == -1 ) std::cerr << "!!! pas trouvé la \"Location\" de matrProj" << std::endl;
        if ( ( locleLutin = glGetUniformLocation( prog, "leLutin" ) ) == -1 ) std::cerr << "!!! pas trouvé la \"Location\" de leLutin" << std::endl;
        if ( ( loctexnumero = glGetUniformLocation( prog, "texnumero" ) ) == -1 ) std::cerr << "!!! pas trouvé la \"Location\" de texnumero" << std::endl;
        if ( ( indvarsUnif = glGetUniformBlockIndex( prog, "varsUnif" ) ) == GL_INVALID_INDEX ) std::cerr << "!!! pas trouvé l'\"index\" de varsUnif" << std::endl;
        //std::cout << " locVertex=" << locVertex << " locColor=" << locColor << " locvitesse=" << locvitesse << " loctempsDeVieRestant=" << loctempsDeVieRestant << std::endl;

        // charger les blocs de variables uniformes (ubo)
        if ( indvarsUnif != GL_INVALID_INDEX )
        {
            glBindBuffer( GL_UNIFORM_BUFFER, ubo );
            glBufferData( GL_UNIFORM_BUFFER, sizeof(varsUnif), &varsUnif, GL_DYNAMIC_COPY );
            glBindBuffer( GL_UNIFORM_BUFFER, 0 );
            const GLuint bindingIndex = 0;
            glBindBufferBase( GL_UNIFORM_BUFFER, bindingIndex, ubo );
            glUniformBlockBinding( prog, indvarsUnif, bindingIndex );
        }
    }

    // charger le nuanceur de sommets pour la rétroaction
    std::cout << "Charger le nuanceur de sommets pour la rétroaction" << std::endl;
    {
        // créer le programme
        progRetroaction = glCreateProgram();

        // attacher le nuanceur de sommets
        const GLchar *chainesSommets = ProgNuanceur::lireNuanceur( "nuanceurSommetsRetroaction.glsl" );
        if ( chainesSommets != NULL )
        {
            GLuint nuanceurObj = glCreateShader( GL_VERTEX_SHADER );
            glShaderSource( nuanceurObj, 1, &chainesSommets, NULL );
            glCompileShader( nuanceurObj );
            glAttachShader( progRetroaction, nuanceurObj );
            ProgNuanceur::afficherLogCompile( nuanceurObj );
            delete [] chainesSommets;
        }

        const GLchar* vars[] ={ "VertexMod", "ColorMod", "vitesseMod", "tempsDeVieRestantMod" };
        glTransformFeedbackVaryings( progRetroaction, sizeof(vars)/sizeof(vars[0]), vars, GL_INTERLEAVED_ATTRIBS );

        // faire l'édition des liens du programme
        glLinkProgram( progRetroaction );
        ProgNuanceur::afficherLogLink( progRetroaction );

        // demander la "Location" des variables
        if ( ( locVertexRetroaction = glGetAttribLocation( progRetroaction, "Vertex" ) ) == -1 ) std::cerr << "!!! pas trouvé la \"Location\" de Vertex" << std::endl;
        if ( ( locColorRetroaction = glGetAttribLocation( progRetroaction, "Color" ) ) == -1 ) std::cerr << "!!! pas trouvé la \"Location\" de Color" << std::endl;
        if ( ( locvitesseRetroaction = glGetAttribLocation( progRetroaction, "vitesse" ) ) == -1 ) std::cerr << "!!! pas trouvé la \"Location\" de vitesse" << std::endl;
        if ( ( loctempsDeVieRestantRetroaction = glGetAttribLocation( progRetroaction, "tempsDeVieRestant" ) ) == -1 ) std::cerr << "!!! pas trouvé la \"Location\" de tempsDeVieRestant" << std::endl;
        if ( ( locposPuitsRetroaction = glGetUniformLocation( progRetroaction, "posPuits" ) ) == -1 ) std::cerr << "!!! pas trouvé la \"Location\" de posPuits" << std::endl;
        if ( ( locbDimRetroaction = glGetUniformLocation( progRetroaction, "bDim" ) ) == -1 ) std::cerr << "!!! pas trouvé la \"Location\" de bDim" << std::endl;
        if ( ( indvarsUnifRetroaction = glGetUniformBlockIndex( prog, "varsUnif" ) ) == GL_INVALID_INDEX ) std::cerr << "!!! pas trouvé l'\"index\" de varsUnif" << std::endl;
        // std::cout << " locVertexRetroaction=" << locVertexRetroaction << " locColorRetroaction=" << locColorRetroaction << " locvitesseRetroaction=" << locvitesseRetroaction << " loctempsDeVieRestantRetroaction=" << loctempsDeVieRestantRetroaction << std::endl;

        // charger les blocs de variables uniformes (ubo)
        if ( indvarsUnifRetroaction != GL_INVALID_INDEX )
        {
            glBindBuffer( GL_UNIFORM_BUFFER, ubo );
            glBufferData( GL_UNIFORM_BUFFER, sizeof(varsUnif), &varsUnif, GL_DYNAMIC_COPY );
            glBindBuffer( GL_UNIFORM_BUFFER, 0 );
            const GLuint bindingIndex = 0;
            glBindBufferBase( GL_UNIFORM_BUFFER, bindingIndex, ubo );
            glUniformBlockBinding( prog, indvarsUnifRetroaction, bindingIndex );
        }
    }
}

void FenetreTP::initialiser()
{
    // donner la couleur de fond
    glm::vec4 couleurFond( 0.9, 0.9, 1.0, 1.0 );
    glClearColor( couleurFond.r, couleurFond.g, couleurFond.b, couleurFond.a );

    // activer les états openGL
    glEnable( GL_DEPTH_TEST );
    glEnable( GL_PROGRAM_POINT_SIZE );
    glPixelStorei( GL_UNPACK_ALIGNMENT, 1 );
    glEnable(GL_BLEND);
    glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );

    // charger les textures
    chargerTextures();

    // allouer les UBO pour les variables uniformes
    glGenBuffers( 1, &ubo );

    // charger les nuanceurs
    chargerNuanceurs();
    FenetreTP::VerifierErreurGL("après le chargement des nuanceurs");

    // Initialiser les formes pour les parois
    glUseProgram( progBase );
    demisphere = new FormeSphere( 1.0, 64, 64, true, false );
    disque = new FormeDisque( 0.0, 1.0, 64, 64 );

    // Initialiser les objets OpenGL
    glGenVertexArrays( 2, vao ); // générer deux VAOs
    glGenBuffers( 2, vbo );      // générer les VBOs
    glGenTransformFeedbacks( 1, tfo );

    // Initialiser le vao pour les particules
    // charger le VBO pour les valeurs modifiés
    glBindBuffer( GL_ARRAY_BUFFER, 0 );
    glBindTransformFeedback( GL_TRANSFORM_FEEDBACK, tfo[0] );
    glBindBuffer( GL_ARRAY_BUFFER, vbo[1] );
    glBufferData( GL_ARRAY_BUFFER, sizeof(part), NULL, GL_STREAM_DRAW ); // on ne donne rien sinon la taille
    FenetreTP::VerifierErreurGL("après le chargement de tfo[0]");

    glUseProgram( prog );
    // remplir les VBO et faire le lien avec les attributs du nuanceur de sommets
    glBindVertexArray( vao[0] );
    glBindBuffer( GL_ARRAY_BUFFER, vbo[0] );
    glBufferData( GL_ARRAY_BUFFER, sizeof(part), part, GL_STREAM_DRAW );
    glEnableVertexAttribArray(locVertex);
    glEnableVertexAttribArray(locColor);
    glEnableVertexAttribArray(locvitesse);
    glEnableVertexAttribArray(loctempsDeVieRestant);
    glBindVertexArray( 0 );
    FenetreTP::VerifierErreurGL("après les glVertexAttribPointer de vao[0]");

    // remplir les VBO pour les valeurs modifiées
    glBindVertexArray( vao[1] );
    glBindBuffer( GL_ARRAY_BUFFER, vbo[1] );
    glBufferData( GL_ARRAY_BUFFER, sizeof(part), part, GL_STREAM_DRAW ); // déjà fait ci-dessus
    glVertexAttribPointer( locVertexRetroaction, 3, GL_FLOAT, GL_FALSE, sizeof(Particule), reinterpret_cast<void*>( offsetof(Particule,position) ) );
    glEnableVertexAttribArray(locVertexRetroaction);
    glVertexAttribPointer( locColorRetroaction, 4, GL_FLOAT, GL_FALSE, sizeof(Particule), reinterpret_cast<void*>( offsetof(Particule,couleur) ) );
    glEnableVertexAttribArray(locColorRetroaction);
    glVertexAttribPointer( locvitesseRetroaction, 3, GL_FLOAT, GL_FALSE, sizeof(Particule), reinterpret_cast<void*>( offsetof(Particule,vitesse) ) );
    glEnableVertexAttribArray(locvitesseRetroaction);
    glVertexAttribPointer( loctempsDeVieRestantRetroaction, 1, GL_FLOAT, GL_FALSE, sizeof(Particule), reinterpret_cast<void*>( offsetof(Particule,tempsDeVieRestant) ) );
    glEnableVertexAttribArray(loctempsDeVieRestantRetroaction);
    glBindVertexArray( 0 );
    FenetreTP::VerifierErreurGL("après les glVertexAttribPointer de vao[1]");

    // Défaire tous les liens
    glBindBuffer( GL_ARRAY_BUFFER, 0 );

    // créer la requête afin d'obtenir un retour d'information lorsque souhaité
    glGenQueries( 1, &requete );

    FenetreTP::VerifierErreurGL("fin de initialiser");
}

void FenetreTP::conclure()
{
    glUseProgram( 0 );
    glDeleteVertexArrays( 2, vao );
    glDeleteBuffers( 2, vbo );
    glDeleteQueries( 1, &requete );
    glDeleteBuffers( 1, &ubo );
    delete demisphere;
    delete disque;
}

void definirProjection( int w, int h )
{
    matrProj.Perspective( 35.0, (GLdouble) w / (GLdouble) h, 50.0, 1000.0 );
}

void afficherDecoration()
{
    // utiliser le programme de base pour les décorations
    glUseProgram( progBase );
    glUniformMatrix4fv( locmatrProjBase, 1, GL_FALSE, matrProj ); // donner la projection courante

    // afficher la boîte (demi-sphère)
    matrModel.PushMatrix();{
        matrModel.Scale( Etat::bDim.x, Etat::bDim.y, Etat::bDim.z );
        glUniformMatrix4fv( locmatrModelBase, 1, GL_FALSE, matrModel );
        // la base de la boîte
        glVertexAttrib3f( locColorBase, 0.2, 0.2, 0.2 );
        disque->afficher();
        // les faces arrières de la demi-sphère qui sert de boîte
        glEnable( GL_CULL_FACE );
        glCullFace( GL_FRONT ); // on enlève les faces avant pour ne garder que les faces arrières
        glVertexAttrib3f( locColorBase, 0.4, 0.4, 0.5 );
        demisphere->afficher();
        glDisable( GL_CULL_FACE );
    }matrModel.PopMatrix(); glUniformMatrix4fv( locmatrModelBase, 1, GL_FALSE, matrModel );
}

void afficherModele()
{
    // afficher d'abord les décorations (en utilisant progBase)
    afficherDecoration();

    // afficher les particules (en utilisant prog)
    glUseProgram( prog );

    glBindVertexArray( vao[0] );
    // refaire le lien avec les attributs du nuanceur de sommets pour le vbo actuellement utilisé
    glBindBuffer( GL_ARRAY_BUFFER, vbo[0] );
    glVertexAttribPointer( locVertex, 3, GL_FLOAT, GL_FALSE, sizeof(Particule), reinterpret_cast<void*>( offsetof(Particule,position) ) );
    glVertexAttribPointer( locColor, 4, GL_FLOAT, GL_FALSE, sizeof(Particule), reinterpret_cast<void*>( offsetof(Particule,couleur) ) );
    glVertexAttribPointer( locvitesse, 3, GL_FLOAT, GL_FALSE, sizeof(Particule), reinterpret_cast<void*>( offsetof(Particule,vitesse) ) );
    glVertexAttribPointer( loctempsDeVieRestant, 1, GL_FLOAT, GL_FALSE, sizeof(Particule), reinterpret_cast<void*>( offsetof(Particule,tempsDeVieRestant) ) );

    switch ( Etat::texnumero ) // 0-aucune, 1-oiseau, 2-flocon
    {
    default: glBindTexture( GL_TEXTURE_2D, 0 ); break;
    case 1: glBindTexture( GL_TEXTURE_2D, Etat::textureOISEAU ); break;
    case 2: glBindTexture( GL_TEXTURE_2D, Etat::textureFLOCON ); break;
    }

    // tracer le résultat de la rétroaction
    //glDrawTransformFeedback( GL_POINTS, tfo[0] );
    glDrawArrays( GL_POINTS, 0, Etat::nparticules );

    glBindTexture( GL_TEXTURE_2D, 0 );
    glBindVertexArray( 0 );
}

void FenetreTP::afficherScene()
{
    // effacer l'écran et le tampon de profondeur
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

    glUseProgram( progBase );

    // définir le pipeline graphique
    definirProjection( largeur_, hauteur_ );
    glUniformMatrix4fv( locmatrProjBase, 1, GL_FALSE, matrProj );

    camera.definir();
    glUniformMatrix4fv( locmatrVisuBase, 1, GL_FALSE, matrVisu );

    matrModel.LoadIdentity();
    glUniformMatrix4fv( locmatrModelBase, 1, GL_FALSE, matrModel );

    // afficher les axes
    if ( Etat::afficheAxes ) FenetreTP::afficherAxes( 30. );

    // afficher les particules
    //glActiveTexture( GL_TEXTURE0 ); // activer la texture '0' (valeur de défaut)
    glUseProgram( prog );
    glUniformMatrix4fv( locmatrProj, 1, GL_FALSE, matrProj );
    glUniformMatrix4fv( locmatrVisu, 1, GL_FALSE, matrVisu );
    glUniformMatrix4fv( locmatrModel, 1, GL_FALSE, matrModel );
    glUniform1i( locleLutin, 0 ); // '0' => utilisation de GL_TEXTURE0
    glUniform1i( loctexnumero, Etat::texnumero );
    // charger les blocs de variables uniformes (ubo)
    if ( indvarsUnif != GL_INVALID_INDEX )
    {
        glBindBuffer( GL_UNIFORM_BUFFER, ubo );
        glBufferData( GL_UNIFORM_BUFFER, sizeof(varsUnif), &varsUnif, GL_DYNAMIC_COPY );
        glBindBuffer( GL_UNIFORM_BUFFER, 0 );
        const GLuint bindingIndex = 0;
        glBindBufferBase( GL_UNIFORM_BUFFER, bindingIndex, ubo );
        glUniformBlockBinding( prog, indvarsUnif, bindingIndex );
    }

    definirProjection( largeur_, hauteur_ );
    glUniformMatrix4fv( locmatrProj, 1, GL_FALSE, matrProj );
    afficherModele();

    VerifierErreurGL("fin de afficherScene");

    // permuter tampons avant et arrière
    swap();
}

void FenetreTP::redimensionner( GLsizei w, GLsizei h )
{
    glViewport( 0, 0, w, h );
}

void FenetreTP::clavier( TP_touche touche )
{
    // quelques variables pour n'imprimer qu'une seule fois la liste des touches lorsqu'une touche est invalide
    bool toucheValide = true; // on suppose que la touche est connue
    static bool listerTouchesAFAIRE = true; // si la touche est invalide, on imprimera la liste des touches

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

    case TP_j: // Incrémenter le nombre de particules
    case TP_CROCHETDROIT:
        {
            unsigned int nparticulesPrec = Etat::nparticules;
            Etat::nparticules *= 1.2;
            if ( Etat::nparticules > MAXNPARTICULES ) Etat::nparticules = MAXNPARTICULES;
            std::cout << " nparticules=" << Etat::nparticules << std::endl;
            // on met les nouvelles particules au puits
            // (glBindBuffer n'est pas très efficace, mais on ne fait pas ça souvent)
            glBindBuffer( GL_ARRAY_BUFFER, vbo[0] );
            Particule *ptr = (Particule*) glMapBuffer( GL_ARRAY_BUFFER, GL_WRITE_ONLY );
            for ( unsigned int i = nparticulesPrec ; i < Etat::nparticules ; ++i )
                ptr[i].tempsDeVieRestant = 0.0; // la particule sera initialisée par le nuanceur de sommets pour la rétroaction
            glUnmapBuffer( GL_ARRAY_BUFFER );
        }
        break;
    case TP_u: // Décrémenter le nombre de particules
    case TP_CROCHETGAUCHE:
        Etat::nparticules /= 1.2;
        if ( Etat::nparticules < 5 ) Etat::nparticules = 5;
        std::cout << " nparticules=" << Etat::nparticules << std::endl;
        break;

    case TP_i: // Imprimer les caractéristiques des 10 premières particules
        Etat::impression = true;
        break;

    case TP_PAGEPREC: // Augmenter la dimension de la boîte en X
        Etat::bDim.x += 3.0;
        std::cout << " Etat::bDim= " << Etat::bDim.x << " x " << Etat::bDim.y << " x " << Etat::bDim.z << std::endl;
        break;
    case TP_PAGESUIV: // Diminuer la dimension de la boîte en X
        if ( Etat::bDim.x > 12.5 ) Etat::bDim.x -= 3.0;
        Etat::verifierPositionPuits();
        std::cout << " Etat::bDim= " << Etat::bDim.x << " x " << Etat::bDim.y << " x " << Etat::bDim.z << std::endl;
        break;
    case TP_DROITE: // Augmenter la dimension de la boîte en Y
        Etat::bDim.y += 3.0;
        std::cout << " Etat::bDim= " << Etat::bDim.x << " x " << Etat::bDim.y << " x " << Etat::bDim.z << std::endl;
        break;
    case TP_GAUCHE: // Diminuer la dimension de la boîte en Y
        if ( Etat::bDim.y > 12.5 ) Etat::bDim.y -= 3.0;
        Etat::verifierPositionPuits();
        std::cout << " Etat::bDim= " << Etat::bDim.x << " x " << Etat::bDim.y << " x " << Etat::bDim.z << std::endl;
        break;
    case TP_HAUT: // Augmenter la dimension de la boîte en Z
        Etat::bDim.z += 3.0;
        std::cout << " Etat::bDim= " << Etat::bDim.x << " x " << Etat::bDim.y << " x " << Etat::bDim.z << std::endl;
        break;
    case TP_BAS: // Diminuer la dimension de la boîte en Z
        if ( Etat::bDim.z > 12.5 ) Etat::bDim.z -= 3.0;
        Etat::verifierPositionPuits();
        std::cout << " Etat::bDim= " << Etat::bDim.x << " x " << Etat::bDim.y << " x " << Etat::bDim.z << std::endl;
        break;

    case TP_0: // Remettre le puits à la position initiale
        Etat::posPuits = Etat::posPuitsOrig;
        break;

    case TP_PLUS: // Avancer la caméra
    case TP_EGAL:
        camera.dist -= 10.;
        if ( camera.dist < 20. ) camera.dist = 20.;
        std::cout << " camera.dist=" << camera.dist << std::endl;
        break;

    case TP_SOULIGNE:
    case TP_MOINS: // Reculer la caméra
        camera.dist += 25.;
        if ( camera.dist > 1000. - Etat::bDim.y ) camera.dist = 1000. - Etat::bDim.y;
        std::cout << " camera.dist=" << camera.dist << std::endl;
        break;

    case TP_b: // Incrémenter la gravité
        varsUnif.gravite += 2.5;
        std::cout << " varsUnif.gravite=" << varsUnif.gravite << std::endl;
        break;
    case TP_h: // Décrémenter la gravité
        varsUnif.gravite -= 2.5;
        if ( varsUnif.gravite < 0.0 ) varsUnif.gravite = 0.0;
        std::cout << " varsUnif.gravite=" << varsUnif.gravite << std::endl;
        break;

    case TP_l: // Incrémenter la durée de vie maximale
        varsUnif.tempsDeVieMax += 0.2;
        std::cout << " varsUnif.tempsDeVieMax=" << varsUnif.tempsDeVieMax << std::endl;
        break;
    case TP_k: // Décrémenter la durée de vie maximale
        varsUnif.tempsDeVieMax -= 0.2;
        if ( varsUnif.tempsDeVieMax < 1.0 ) varsUnif.tempsDeVieMax = 1.0;
        std::cout << " varsUnif.tempsDeVieMax=" << varsUnif.tempsDeVieMax << std::endl;
        break;

    case TP_f: // Incrémenter la taille des particules
        varsUnif.pointsize += 0.2;
        std::cout << " varsUnif.pointsize=" << varsUnif.pointsize << std::endl;
        break;
    case TP_d: // Décrémenter la taille des particules
        varsUnif.pointsize -= 0.2;
        if ( varsUnif.pointsize < 1.0 ) varsUnif.pointsize = 1.0;
        std::cout << " varsUnif.pointsize=" << varsUnif.pointsize << std::endl;
        break;

    case TP_t: // Changer la texture utilisée: 0-aucune, 1-oiseau, 2-flocon
        if ( ++Etat::texnumero > 2 ) Etat::texnumero = 0;
        std::cout << " Etat::texnumero=" << Etat::texnumero << std::endl;
        break;

    case TP_a: // Boucler sur les textures automatiquement
        Etat::boucleTexture = !Etat::boucleTexture;
        std::cout << " Etat::boucleTexture=" << Etat::boucleTexture << std::endl;
        break;

    case TP_g: // Permuter l'affichage en fil de fer ou plein
        Etat::modePolygone = ( Etat::modePolygone == GL_FILL ) ? GL_LINE : GL_FILL;
        glPolygonMode( GL_FRONT_AND_BACK, Etat::modePolygone );
        break;

    case TP_ESPACE: // Mettre en pause ou reprendre l'animation
        Etat::enmouvement = !Etat::enmouvement;
        break;

    default:
        std::cout << " touche inconnue : " << (char) touche << std::endl;
        toucheValide = false;
        break;
    }

    // n'imprimer qu'une seule fois la liste des touches lorsqu'une touche est invalide
    if ( toucheValide ) // si la touche est valide, ...
    {
        listerTouchesAFAIRE = true; // ... on imprimera la liste des touches à la prochaine touche invalide
    }
    else if ( listerTouchesAFAIRE ) // s'il faut imprimer la liste des touches ...
    {
        listerTouchesAFAIRE = false; // ... se souvenir que ça a été fait
        imprimerFichier( "touches.txt" );
    }

}

// ce qui est déplacé par la souris
static enum { deplaceCam, deplacePuits } deplace = deplaceCam;
static bool pressed = false;
void FenetreTP::sourisClic( int button, int state, int x, int y )
{
    pressed = ( state == TP_PRESSE );
    if ( pressed )
    {
        // on vient de presser la souris
        Etat::sourisPosPrec.x = x;
        Etat::sourisPosPrec.y = y;
        switch ( button )
        {
        case TP_BOUTON_GAUCHE: // Manipuler la caméra
        case TP_BOUTON_MILIEU:
            deplace = deplaceCam;
            break;
        case TP_BOUTON_DROIT: // Déplacer le puits
            deplace = deplacePuits;
            break;
        }
    }
    else
    {
        // on vient de relâcher la souris
    }
}

void FenetreTP::sourisMolette( int x, int y ) // Changer la distance de la caméra
{
    const int sens = +1;
    camera.dist -= 5. * sens*y;
    if ( camera.dist < 20. ) camera.dist = 20.;
    else if ( camera.dist > 1000. - Etat::bDim.y ) camera.dist = 1000. - Etat::bDim.y;
}

void FenetreTP::sourisMouvement( int x, int y )
{
    if ( pressed )
    {
        int dx = x - Etat::sourisPosPrec.x;
        int dy = y - Etat::sourisPosPrec.y;
        switch ( deplace )
        {
        case deplaceCam:
            camera.theta -= dx / 3.0;
            camera.phi   += dy / 3.0;
            break;
        case deplacePuits:
            {
                glm::mat4 VM = matrVisu*matrModel;
                glm::mat4 P = matrProj;
                glm::vec4 cloture( 0, 0, largeur_, hauteur_ );
                glm::vec3 ecranPosPrec = glm::project( glm::vec3(Etat::posPuits), VM, P, cloture );
                glm::vec3 ecranPos( x, hauteur_-y, ecranPosPrec.z );
                Etat::posPuits = glm::vec4( glm::unProject( ecranPos, VM, P, cloture ), 1.0 );
                Etat::verifierPositionPuits();
                std::cout << " Etat::posPuits=" << glm::to_string(Etat::posPuits) << std::endl;
            }
            break;
        }

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
