#ifndef __AQUARIUM_H__
#define __AQUARIUM_H__

#include <vector>
#include <iterator>
#include "Theiere.h"

//
// l'aquarium
//
class Aquarium
{
    static FormeCube *cubeFil;
    static FormeQuad *quad;
public:
    Aquarium( )
        : locillumination(-1)
    {
        // les hauteurs variées des théières
        float hauteur[] = { -9.9, -6.0, 4.0, 3.0, -2.0, -8.0, -5.0, -3.0, 8.0, 7.0, -1.0, 1.0, 5.0, 6.0, 2.0, -4.0, -9.0, -7.0 };

        // créer un aquarium graphique
        glUseProgram( prog );
        initialiserGraphique();

        // initialiser la génération de valeurs aléatoires pour la création de théières
        srand( time(NULL) );

        // remplir l'aquarium
        for ( unsigned int i = 0 ; i < sizeof(hauteur)/sizeof(hauteur[0]) ; ++i )
        {
            // rayon (distance au centre de rotation) aléatoire
            float rayon = glm::mix( 2.0, 9.0, rand()/((double)RAND_MAX) );

            // un angle aléatoire en degrés
            float angle = glm::mix( 0, 360, rand()/((double)RAND_MAX) );

            // vitesse de rotation aléatoire
            float vit = glm::mix( -8.0, 8.0, rand()/((double)RAND_MAX) );
            vit += 4.0 * glm::sign(vit); // ajouter ou soustraire 4.0 selon le signe de vit afin d'avoir : 4.0 <= abs(vit) <= 12.0

            // taille aléatoire
            float taille = glm::mix( 0.1, 0.3, rand()/((double)RAND_MAX) );

            // couleur rouge légèrement orangé
            glm::vec3 couleur = glm::vec3( 1.0, 0.2, 0.0 );
            if ( i < 5 ) // les 5 premières ont une couleur spéciale pour bien les identifier
            {
                glm::vec3 couleurs[5] = { { 0, 1, 1 }, { 1, 0, 1 }, { 1, 1, 0 }, { 0, 0, 1 }, { 0, 1, 0 } }; // cyan, magenta, jaune, bleu, vert
                couleur = couleurs[i];
            }

            // créer une nouvelle théière
            Theiere *theiere = new Theiere( rayon, hauteur[i], angle, vit, taille, couleur );

            // ajouter cette théière dans la liste
            theieres.push_back( theiere );
        }

        // créer quelques autres formes
        glUseProgram( progBase );
        if ( cubeFil == NULL ) cubeFil = new FormeCube( 1.0, false );
        if ( quad == NULL ) quad = new FormeQuad( 1.0, true );
    }
    ~Aquarium()
    {
        conclureGraphique();
        // vider l'aquarium
        while ( !theieres.empty() ) theieres.pop_back( );
    }
    void initialiserGraphique()
    {
        GLint prog = 0; glGetIntegerv( GL_CURRENT_PROGRAM, &prog );
        if ( prog <= 0 )
        {
            std::cerr << "Pas de programme actif!" << std::endl;
            locVertex = locColor = -1;
            return;
        }
        if ( ( locVertex = glGetAttribLocation( prog, "Vertex" ) ) == -1 ) std::cerr << "!!! pas trouvé la \"Location\" de Vertex" << std::endl;
        if ( ( locColor = glGetAttribLocation( prog, "Color" ) ) == -1 ) std::cerr << "!!! pas trouvé la \"Location\" de Color" << std::endl;
        if ( ( locillumination = glGetUniformLocation( prog, "illumination" ) ) == -1 ) std::cerr << "!!! pas trouvé la \"Location\" de illumination" << std::endl;
    }
    void conclureGraphique()
    {
        delete cubeFil;
        delete quad;
    }

    void afficherParoiXpos()
    {
        matrModel.PushMatrix();{
            matrModel.Scale( 2*Etat::bDim.x, 2*Etat::bDim.y, 2*Etat::bDim.z );
            matrModel.Translate( 0.5, 0.0, 0.0 );
            matrModel.Rotate( -90.0, 0.0, 1.0, 0.0 );
            glUniformMatrix4fv( locmatrModelBase, 1, GL_FALSE, matrModel );
            quad->afficher();
        }matrModel.PopMatrix(); glUniformMatrix4fv( locmatrModelBase, 1, GL_FALSE, matrModel );
    }
    void afficherParoiXneg()
    {
        matrModel.PushMatrix();{
            matrModel.Scale( 2*Etat::bDim.x, 2*Etat::bDim.y, 2*Etat::bDim.z );
            matrModel.Translate( -0.5, 0.0, 0.0 );
            matrModel.Rotate( 90.0, 0.0, 1.0, 0.0 );
            glUniformMatrix4fv( locmatrModelBase, 1, GL_FALSE, matrModel );
            quad->afficher();
        }matrModel.PopMatrix(); glUniformMatrix4fv( locmatrModelBase, 1, GL_FALSE, matrModel );
    }
    void afficherParoiYpos()
    {
        matrModel.PushMatrix();{
            matrModel.Scale( 2*Etat::bDim.x, 2*Etat::bDim.y, 2*Etat::bDim.z );
            matrModel.Translate( 0.0, 0.5, 0.0 );
            matrModel.Rotate( 90.0, 1.0, 0.0, 0.0 );
            glUniformMatrix4fv( locmatrModelBase, 1, GL_FALSE, matrModel );
            quad->afficher();
        }matrModel.PopMatrix(); glUniformMatrix4fv( locmatrModelBase, 1, GL_FALSE, matrModel );
    }
    void afficherParoiYneg()
    {
        matrModel.PushMatrix();{
            matrModel.Scale( 2*Etat::bDim.x, 2*Etat::bDim.y, 2*Etat::bDim.z );
            matrModel.Translate( 0.0, -0.5, 0.0 );
            matrModel.Rotate( -90.0, 1.0, 0.0, 0.0 );
            glUniformMatrix4fv( locmatrModelBase, 1, GL_FALSE, matrModel );
            quad->afficher();
        }matrModel.PopMatrix(); glUniformMatrix4fv( locmatrModelBase, 1, GL_FALSE, matrModel );
    }
    void afficherParoiZneg()
    {
        matrModel.PushMatrix();{
            matrModel.Scale( 2*Etat::bDim.x, 2*Etat::bDim.y, 2*Etat::bDim.z );
            matrModel.Translate( 0.0, 0.0, -0.5 );
            glUniformMatrix4fv( locmatrModelBase, 1, GL_FALSE, matrModel );
            quad->afficher();
        }matrModel.PopMatrix(); glUniformMatrix4fv( locmatrModelBase, 1, GL_FALSE, matrModel );
    }
    void afficherParoiZpos()
    {
        matrModel.PushMatrix();{
            matrModel.Scale( 2*Etat::bDim.x, 2*Etat::bDim.y, 2*Etat::bDim.z );
            matrModel.Translate( 0.0, 0.0, 0.5 );
            matrModel.Rotate( -180.0, 1.0, 0.0, 0.0 );
            glUniformMatrix4fv( locmatrModelBase, 1, GL_FALSE, matrModel );
            quad->afficher();
        }matrModel.PopMatrix(); glUniformMatrix4fv( locmatrModelBase, 1, GL_FALSE, matrModel );
    }
    void afficherCoins()
    {
        matrModel.PushMatrix();{
            matrModel.Scale( 2*Etat::bDim.x, 2*Etat::bDim.y, 2*Etat::bDim.z );
            glUniformMatrix4fv( locmatrModelBase, 1, GL_FALSE, matrModel );
            cubeFil->afficher();
        }matrModel.PopMatrix(); glUniformMatrix4fv( locmatrModelBase, 1, GL_FALSE, matrModel );
    }
    void afficherParois()
    {
        // tracer les coins de l'aquarium avec le programme de base
        glVertexAttrib3f( locColorBase, 1.0, 1.0, 1.0 ); // blanc
        glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
        afficherCoins();

        // tracer les parois de verre de l'aquarium
        glEnable( GL_BLEND );
        glEnable( GL_CULL_FACE ); glCullFace( GL_BACK ); // ne pas afficher les faces arrière

        glVertexAttrib4f( locColorBase, 0.5, 0.2, 0.2, 0.6 ); // rougeâtre
        afficherParoiXpos(); // paroi en +X

        glVertexAttrib4f( locColorBase, 0.2, 0.5, 0.2, 0.6 ); // verdâtre
        afficherParoiZpos(); // paroi en +Z

        glVertexAttrib4f( locColorBase, 0.2, 0.2, 0.5, 0.6 ); // bleuté
        afficherParoiXneg(); // paroi en -X

        glVertexAttrib4f( locColorBase, 0.5, 0.5, 0.2, 0.6 ); // jaunâtre
        afficherParoiZneg(); // paroi en -Z

        glVertexAttrib4f( locColorBase, 0.7, 0.7, 0.7, 0.7 ); // gris pâle, plus opaque
        afficherParoiYpos(); // paroi en -Y

        glVertexAttrib4f( locColorBase, 0.4, 0.4, 0.4, 0.7 ); // gris foncé, plus opaque
        afficherParoiYneg(); // paroi en -Y

        glDisable( GL_CULL_FACE );
        glDisable( GL_BLEND );
    }

    void positionnerCamera( int theiereChoisie )
    {
        // Récupérer la théière choisie
        Theiere *theiere = theieres[theiereChoisie-1];
        // Obtenir sa matrice de modélisation
        MatricePipeline mtc = theiere->obtenirMatriceCourante();
        // Fixer le point de vue avec la matrice obtenue
        matrVisu.setMatr( glm::inverse( glm::mat4(mtc) ) );
    }

    void afficherToutesLesTheieres()
    {
        glVertexAttrib4f( locColor, 1.0, 1.0, 1.0, 1.0 );

        for ( std::vector<Theiere*>::iterator it = theieres.begin() ; it != theieres.end() ; it++ )
        {
            (*it)->afficher();
        }
    }

    // afficher le contenu de l'aquarium
    void afficherContenu( GLenum ordre = GL_CCW )
    {
        glUseProgram( prog );

        // afficher des triangles en plein ou en fil de fer ?
        glPolygonMode( GL_FRONT_AND_BACK, Etat::modePolygone );
        // ne pas afficher les triangles dont on voit la face arrière ? (touche 'c')
        if ( Etat::culling ) glEnable( GL_CULL_FACE );
        if ( Etat::culling ) glCullFace( ordre == GL_CCW ? GL_BACK : GL_FRONT );

        // afficher les théières
        afficherToutesLesTheieres();

        if ( Etat::culling ) glDisable( GL_CULL_FACE );
    }

    // afficher l'aquarium
    void afficher()
    {
        // tracer l'aquarium avec le programme de nuanceur de base
        glUseProgram( progBase );
        glUniformMatrix4fv( locmatrProjBase, 1, GL_FALSE, matrProj );
        glUniformMatrix4fv( locmatrVisuBase, 1, GL_FALSE, matrVisu );
        glUniformMatrix4fv( locmatrModelBase, 1, GL_FALSE, matrModel );

        // 1) Remplir le stencil avec le miroir en affichant les parois de
        // façon similaire à ce qui est fait dans afficherParois().
        // On veut des 1,2,4,8 ou 16 pour tous les pixels des miroirs et
        // des 0 partout ailleurs, mais on ne veut rien tracer à l'écran
        // pour le moment: on s'organise pour que le test du stencil ne
        // passe jamais tout en remplissant le stencil de valeurs.
        glEnable( GL_CULL_FACE ); 
        glCullFace( GL_BACK ); // ne pas afficher les faces arrière
        glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );

        // Remplir le test du stencil avec les miroirs, sans rien tracer pour le moment
        // Si le debug est activé (debug == 1), alors on n'utilise pas le stencil
        if ( Etat::debug == 0 ) {
            glEnable( GL_STENCIL_TEST );
        }
        glStencilOp( GL_REPLACE, GL_REPLACE, GL_REPLACE );

        glStencilFunc( GL_NEVER, 1, 1 );
        afficherParoiZpos(); // paroi en +Z

        glStencilFunc( GL_NEVER, 2, 2 );
        afficherParoiXpos(); // paroi en +X

        glStencilFunc( GL_NEVER, 4, 4 );
        afficherParoiZneg(); // paroi en -Z

        glStencilFunc( GL_NEVER, 8, 8 );
        afficherParoiXneg(); // paroi en -X

        glStencilFunc( GL_NEVER, 16, 16 );
        afficherParoiYneg(); // paroi en -Y

        glDisable( GL_CULL_FACE );

        // 2) Maintenant que le stencil est rempli de valeurs aux positions des miroirs,
        // on trace les scènes réfléchies.
        // On fait en sorte que seulement la région du stencil avec des 1,2,4,8 soit tracée
        // (c'est-à-dire seulement la région du miroir)
        // tracer les théières avec le programme de nuanceur de ce TP
        glUseProgram( prog );
        glUniformMatrix4fv( locmatrProj, 1, GL_FALSE, matrProj );
        glUniformMatrix4fv( locmatrVisu, 1, GL_FALSE, matrVisu );
        glUniformMatrix4fv( locmatrModel, 1, GL_FALSE, matrModel );
        glUniform1i( locillumination, Etat::illumination );

        // Garder les valeurs si le test échoue
        glStencilOp( GL_KEEP, GL_KEEP, GL_KEEP );

        // Tracer le contenu inversé sur les 5 miroirs
        
        // reflet en +Z
        glStencilFunc( GL_EQUAL, 1, 1 );
        matrModel.PushMatrix();{
            // faire des translations pour amener les plans de réflexion aux positions des miroirs
            matrModel.Translate( 0, 0, Etat::bDim.z );
            // faire une réflexion par -1, selon le plan orthogonal
            matrModel.Scale( 1, 1, -1 );
            // faire la translation inverse
            matrModel.Translate( 0, 0, -Etat::bDim.z );
            afficherToutesLesTheieres();
        }matrModel.PopMatrix();
        glUniformMatrix4fv( locmatrModel, 1, GL_FALSE, matrModel);

        // reflet en +X
        glStencilFunc( GL_EQUAL, 2, 2 );
        matrModel.PushMatrix();{
            matrModel.Translate( Etat::bDim.x, 0, 0 );
            matrModel.Scale( -1, 1, 1 );
            matrModel.Translate( -Etat::bDim.x, 0, 0 );
            afficherToutesLesTheieres();
        }matrModel.PopMatrix();
        glUniformMatrix4fv( locmatrModel, 1, GL_FALSE, matrModel);

        // reflet en -Z
        glStencilFunc( GL_EQUAL, 4, 4 );
        matrModel.PushMatrix();{
            matrModel.Translate( 0, 0, -Etat::bDim.z );
            matrModel.Scale( 1, 1, -1 );
            matrModel.Translate( 0, 0, Etat::bDim.z );
            afficherToutesLesTheieres();
        }matrModel.PopMatrix();
        glUniformMatrix4fv( locmatrModel, 1, GL_FALSE, matrModel);

        // reflet en -X
        glStencilFunc( GL_EQUAL, 8, 8 );
        matrModel.PushMatrix();{
            matrModel.Translate( -Etat::bDim.x, 0, 0 );
            matrModel.Scale( -1, 1, 1 );
            matrModel.Translate( Etat::bDim.x, 0, 0 );
            afficherToutesLesTheieres();
        }matrModel.PopMatrix();
        glUniformMatrix4fv( locmatrModel, 1, GL_FALSE, matrModel);

        // reflet en -Y
        glStencilFunc( GL_EQUAL, 16, 16 );
        matrModel.PushMatrix();{
            matrModel.Translate( 0, -Etat::bDim.y, 0 );
            matrModel.Scale( 1, -1, 1 );
            matrModel.Translate( 0, Etat::bDim.y, 0 );
            afficherToutesLesTheieres();
        }matrModel.PopMatrix();
        glUniformMatrix4fv( locmatrModel, 1, GL_FALSE, matrModel);

        glDisable( GL_STENCIL_TEST );

        // avant d'enfin tracer le contenu intérieur
        afficherContenu( );

        // afficher les parois de l'aquarium
        glUseProgram( progBase );

        afficherParois( );
    }

    void calculerPhysique( )
    {
        if ( Etat::enmouvement )
        {
            if ( getenv("DEMO") != NULL )
            {
                // faire varier la taille de la boite automatiquement pour la démo
                static int sensX = 1;
                Etat::bDim.x += sensX * 0.1 * Etat::dt;
                if ( Etat::bDim.x < 8.0 ) sensX = +1;
                else if ( Etat::bDim.x > 12.0 ) sensX = -1;

                static int sensZ = -1;
                Etat::bDim.z += sensZ * 0.07 * Etat::dt;
                if ( Etat::bDim.z < 8.0 ) sensZ = +1;
                else if ( Etat::bDim.z > 12.0 ) sensZ = -1;
            }

            for ( std::vector<Theiere*>::iterator it = theieres.begin() ; it != theieres.end() ; it++ )
            {
                (*it)->avancerPhysique();
            }
        }
    }

    GLint locillumination;

    // la liste des théières
    std::vector<Theiere*> theieres;
};

FormeCube* Aquarium::cubeFil = NULL;
FormeQuad* Aquarium::quad = NULL;

#endif
