#ifndef __THEIERE_H__
#define __THEIERE_H__

//
// une théière
//
class Theiere
{
    static FormeSphere *sphere; // une sphère centrée de rayon 1 pour les yeux
public:
    Theiere( float r = 1.0, float h = 0.0, float ang = 0.0, float vit = 1.0, float tai = 0.5,
             glm::vec3 coulCorps = glm::vec3( 1.0, 0.2, 0.0 ) )  // rouge légèrement orangé
        : vao(0), vboTheiereSommets(0), vboTheiereConnec(0), couleurCorps(coulCorps), rayon(r), hauteur(h), angle(ang), vitesse(vit), taille(tai)
    {
        // créer une théière graphique
        initialiserGraphique();
    }
    ~Theiere()
    {
        conclureGraphique();
    }
    void initialiserGraphique()
    {
        // créer quelques formes
        if ( sphere == NULL ) sphere = new FormeSphere( 1.0, 8, 8, true );

        // allouer les objets OpenGL
        glGenVertexArrays( 1, &vao );

        // initialiser le VAO pour la théière
        glBindVertexArray( vao );

        // créer le VBO pour les sommets
        glGenBuffers( 1, &vboTheiereSommets );
        glBindBuffer( GL_ARRAY_BUFFER, vboTheiereSommets );
        glBufferData( GL_ARRAY_BUFFER, sizeof(gTheiereSommets), gTheiereSommets, GL_STATIC_DRAW );
        // faire le lien avec l'attribut du nuanceur de sommets
        glVertexAttribPointer( locVertex, 3, GL_FLOAT, GL_FALSE, 0, 0 );
        glEnableVertexAttribArray(locVertex);

        // créer le VBO la connectivité
        glGenBuffers( 1, &vboTheiereConnec );
        glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, vboTheiereConnec );
        glBufferData( GL_ELEMENT_ARRAY_BUFFER, sizeof(gTheiereConnec), gTheiereConnec, GL_STATIC_DRAW );

        glBindVertexArray(0);
    }
    void conclureGraphique()
    {
        delete sphere;
    }

    // obtenir la matrice de modélisation qui représente la position courante de cette théière
    MatricePipeline obtenirMatriceCourante( )
    {
        MatricePipeline mtc;
        
        // amener le repère à la position courante de cette théière
        mtc.Rotate( angle, 0, 1, 0 );
        mtc.Translate( rayon, hauteur, 0.0 );

        // regarder vers l'avant du mouvement
        if (vitesse < 0){
            mtc.Rotate( 180, 0, 1, 0 );
        }

        // se positionner en haut de la théière (à +5.0 par rapport à la base), tout en ajustant selon la taille de cette théière
        // (le plus haut sommet de la théière est à y=+4.725, donc une translation de +5.0 amène le repère juste en haut)
        mtc.Translate( 0.0, 5.0 * taille, 0.0 );

        return mtc;
    }

    void afficher()
    {
        matrModel.PushMatrix(); { // sauvegarder la tranformation courante

            // amener le repère à la position courante de cette théière
            matrModel.Rotate( angle, 0, 1, 0 ); // tourner selon l'angle
            matrModel.Translate( rayon, hauteur, 0.0 ); // se déplacer dans la direction de l'axe des X ainsi tourné

            // afficher le corps : la théière
            // donner la couleur de sélection au besoin (partie 2)
            glVertexAttrib3fv( locColor, glm::value_ptr(couleurCorps) );
            matrModel.PushMatrix(); {
                // tourner la théière pour avoir son bec dans la bonne direction
                matrModel.Rotate( ( vitesse < 0.0 ) ? -90 : 90, 0, 1, 0 );
                // ajuster selon la taille
                matrModel.Scale( taille, taille, taille );
                glUniformMatrix4fv( locmatrModel, 1, GL_FALSE, matrModel ); // ==> Avant de tracer, on doit informer la carte graphique des changements faits à la matrice de modélisation
                glBindVertexArray( vao );
                glDrawElements( GL_TRIANGLES, sizeof(gTheiereConnec)/sizeof(GLuint), GL_UNSIGNED_INT, 0 );
                glBindVertexArray(0);

                // donner la couleur des yeux
                glVertexAttrib3f( locColor, 0.8, 0.8, 0.2 ); // jaune

                // afficher les yeux
                const GLfloat facteur[2] = { -1.0, 1.0 };
                for ( unsigned int i = 0; i < 2; ++i )
                {
                    matrModel.PushMatrix(); {
                        matrModel.Rotate( 30.0 * facteur[i], 0.0, 1.0, 0.0 );
                        matrModel.Translate( 2.6, 3.0, 0.0 );
                        matrModel.Scale( 0.4, 0.4, 0.4 );
                        glUniformMatrix4fv(locmatrModel, 1, GL_FALSE, matrModel);
                        sphere->afficher();
                    }matrModel.PopMatrix(); glUniformMatrix4fv( locmatrModel, 1, GL_FALSE, matrModel );
                }

            }matrModel.PopMatrix(); glUniformMatrix4fv( locmatrModel, 1, GL_FALSE, matrModel );

        }matrModel.PopMatrix(); glUniformMatrix4fv( locmatrModel, 1, GL_FALSE, matrModel );
        glUniformMatrix4fv( locmatrModel, 1, GL_FALSE, matrModel ); // informer ...
    }

    void avancerPhysique()
    {
        angle += Etat::sens * Etat::dt * vitesse;
    }

    // les variables de la théière
    GLuint vao = 0;
    GLuint vboTheiereSommets = 0;
    GLuint vboTheiereConnec = 0;
    glm::vec3 couleurCorps; // la couleur
    float rayon;            // en unités
    float hauteur;          // en unités
    float angle;            // en degrés
    float vitesse;          // en degrés/seconde
    float taille;           // en unités
};

FormeSphere* Theiere::sphere = NULL;

#endif
