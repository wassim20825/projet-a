/**
* @file main.c
* @brief tache perso.
* @author wassim saidi
* @version 0.1
* @date Mai 10, 2025
*
* Code tache perso
*
*/
/**
* @file perso.c
*/
#include "perso.h"
#include <stdio.h>
/**
* @brief To initialize the perso p.
* @param p personnage
* @return Nothing
*/
 void initialiser_personnage(personnage *p)
{
    // Initialisation de la position du personnage
    p->position.x = 10;
    p->position.y = 567;
    p->position.w = 40;
    p->position.h = 80;

    // Initialisation des attributs physiques
    p->acceleration = 3;
    p->vitesse = 10;
    p->sol = 1;

    // Chargement du sprite du personnage
    p->sprite = IMG_Load("R1.png");
    if (p->sprite == NULL) {
        printf("Erreur chargement sprite personnage : %s\n", IMG_GetError());
    }

    // Initialisation des vies
    p->v.nbVies = 3;  // Par exemple, 3 vies au départ
    p->v.posVie.x = 830;
    p->v.posVie.y = 10;
    p->v.spriteVie = IMG_Load("coeur.png");
    
    if (p->v.spriteVie == NULL) {
        printf("Erreur chargement sprite vie : %s\n", IMG_GetError());
    }

    // Initialisation du score
    p->s.points = 0;
    p->s.posScore.x = 20;
    p->s.posScore.y = 20;
    p->s.couleur.r = 255;
    p->s.couleur.g = 255;
    p->s.couleur.b = 255;
    p->s.police = TTF_OpenFont("/usr/share/fonts/truetype/ubuntu/UbuntuMono-B.ttf", 24);
    if (p->s.police == NULL) {
        printf("Erreur chargement police : %s\n", TTF_GetError());
    }
}

 
/**
* @brief To affiche the perso p.
* @param p personnage ,ecran,background,position
* @return Nothing
*/
  void afficher_perso(personnage p, SDL_Surface *ecran)
{
    // Affichage du personnage
    if (p.sprite) {
        SDL_BlitSurface(p.sprite, NULL, ecran, &p.position);
    }

    // Affichage des vies (cœurs)
    if (p.v.spriteVie) {
        SDL_Rect pos = p.v.posVie;
        for (int i = 0; i < p.v.nbVies; i++) {
            SDL_BlitSurface(p.v.spriteVie, NULL, ecran, &pos);
            pos.x += p.v.spriteVie->w -35;  // Espace entre les cœurs
        }
    }

    // Affichage du score
    if (p.s.police) {
        char texte[20];
        sprintf(texte, "Score : %d", p.s.points); // Conversion du score en texte

        SDL_Surface *texteSurface = TTF_RenderText_Blended(p.s.police, texte, p.s.couleur);
        if (texteSurface) {
            SDL_BlitSurface(texteSurface, NULL, ecran, &p.s.posScore);
            SDL_FreeSurface(texteSurface);
        } else {
            printf("Erreur affichage texte score : %s\n", TTF_GetError());
        }
    }
}
 
/**
* @brief pour annimer perso p avec les fleches.
* @param i ,event ,personnage p
* @return Nothing
*/
void animperso(int *i, SDL_Event *event, personnage *p)
{
    char ch[20];
    SDL_PollEvent(event);
    switch (event->type) {
//maaneha bch nenzel alaa bouton fil clavier
        case SDL_KEYDOWN:
            switch (event->key.keysym.sym) {
                case SDLK_RIGHT:
		//i howa compteur i++ maaneha i=i+1
                    (*i)++;
                    if (*i > 9) *i = 1;
                    sprintf(ch, "R%d.png", *i);
                    SDL_Delay(150);
                    p->sprite = IMG_Load(ch);
                    break;
                case SDLK_LEFT:
                    (*i)++;
                    if (*i > 9) *i = 1;
                    sprintf(ch, "L%d.png", *i);
                    SDL_Delay(150);
                    p->sprite = IMG_Load(ch);
                    break;
            }
            break;
    }
}
/**
* @brief pour annimer perso p2 avec les boutons(z-q-s-d).
* @param i ,event ,personnage p2
* @return Nothing
*/
void animperso2(int *i, SDL_Event *event, personnage *p)
{
    char ch[20];
    SDL_PollEvent(event); // Vérifier les événements clavier

    switch (event->type) {
        case SDL_KEYDOWN:
            switch (event->key.keysym.sym) {
                case SDLK_d: // Droite
                    (*i)++;
                    if (*i > 9) *i = 1; // Limiter les frames à 9 max (par exemple)
                    sprintf(ch, "R%d.png", *i);  // Sprite de marche droite
                    SDL_Delay(150); // Délai pour l'animation
                    p->sprite = IMG_Load(ch);  // Charger le sprite correspondant
                    break;

                case SDLK_a: // Gauche
                    (*i)++;
                    if (*i > 9) *i = 1;
                    sprintf(ch, "L%d.png", *i);
                    SDL_Delay(150);
                    p->sprite = IMG_Load(ch);
                    break;

                
            }
            break;
    }
}
/**
* @brief pour sauter.
* @param personnage p2
* @return Nothing
*/
 void jump (personnage *p)
{ //int gravite = 1 ;
  
while( p->position.y!= p->sol) 
 p->position.y+=p->vitesse;
;}
/**
* @brief pour deplacer perso p avec les fleches.
* @param i ,event ,personnage p
* @return Nothing
*/  
void deplacerperso(personnage *p, int *continuer, SDL_Event *event)
{
    static double vitesse_y = 0;           // Vitesse verticale pour le saut
    const double gravite = 0.5;            // Gravité
    const double impulsion_saut = -10.0;   // Impulsion vers le haut
    const int sol_y = 567;                 // Position du sol (à adapter à ton jeu)
   
    static int direction = 1;  // 1 pour droite, -1 pour gauche (direction du mouvement)

    SDL_PollEvent(event);
    switch (event->type)
    {
        case SDL_QUIT:
            (*continuer) = 0;
            break;

        case SDL_KEYDOWN:
            switch (event->key.keysym.sym)
            {
                case SDLK_RIGHT:
                    direction = 1;  // La direction devient droite
                    if (p->position.x + p->position.w < 1048) // Limite à droite
                    {
                        if (p->acceleration > 0)
                            p->acceleration--; // Décroître l'accélération

                        p->position.x += p->vitesse + p->acceleration * 0.2; // Déplacement vers la droite
                    }
                    break;

                case SDLK_LEFT:
                    direction = -1;  // La direction devient gauche
                    if (p->position.x > 0) // Limite à gauche
                    {
                        if (p->acceleration > 0)
                            p->acceleration--; // Décroître l'accélération

                        p->position.x -= p->vitesse + p->acceleration * 0.2; // Déplacement vers la gauche
                    }
                    break;

                case SDLK_SPACE:
                    // Lorsque l'on appuie sur "Espace", le personnage doit accélérer dans la direction choisie
                    p->acceleration = 0.6;
                    if (direction == 1)  // Si direction droite
                    {
                        p->position.x += p->vitesse + p->acceleration * 0.2; // Déplacer à droite
                    }
                    else if (direction == -1)  // Si direction gauche
                    {
                        p->position.x -= p->vitesse + p->acceleration * 0.2; // Déplacer à gauche
                    }
                    break;

                case SDLK_UP:
                    if (p->sol == 1) // Si sur le sol
                    {
                        vitesse_y = impulsion_saut;
                        p->sol = 0;
                    }
                    break;
            }
            break;
    }

    // Appliquer la gravité si en saut
    if (p->sol == 0)
    {
        p->position.y += vitesse_y;
        vitesse_y += gravite;

        if (p->position.y >= sol_y)
        {
            p->position.y = sol_y;
            vitesse_y = 0;
            p->sol = 1;
        }
    }

     
}
/**
* @brief pour deplacer perso p2 avec les boutons(z-q-s-d).
* @param i ,event ,personnage p2
* @return Nothing
*/
void deplacerperso2(personnage *p, int *continuer, SDL_Event *event)
{
    static double vitesse_y = 0;
    const double gravite = 0.5;
    const double impulsion_saut = -10.0;
    const int sol_y = 567;
    static int direction = 1;

    switch (event->type)
    {
        case SDL_QUIT:
            (*continuer) = 0;
            break;

        case SDL_KEYDOWN:
            switch (event->key.keysym.sym)
            {
                case SDLK_d:  // Droite
                    direction = 1;
                    if (p->position.x + p->position.w < 1048)
                    {
                        if (p->acceleration > 0)
                            p->acceleration--;

                        p->position.x += p->vitesse + p->acceleration * 0.2;
                    }
                    break;

                case SDLK_a:  // Gauche
                    direction = -1;
                    if (p->position.x > 0)
                    {
                        if (p->acceleration > 0)
                            p->acceleration--;

                        p->position.x -= p->vitesse + p->acceleration * 0.2;
                    }
                    break;

                case SDLK_x:  // Boost
                    p->acceleration = 0.6;
                    if (direction == 1)
                        p->position.x += p->vitesse + p->acceleration * 0.2;
                    else
                        p->position.x -= p->vitesse + p->acceleration * 0.2;
                    break;

                case SDLK_w:  // Saut
                    if (p->sol == 1)
                    {
                        vitesse_y = impulsion_saut;
                        p->sol = 0;
                    }
                    break;
            }
            break;
    }

    // Gravité
    if (p->sol == 0)
    {
        p->position.y += vitesse_y;
        vitesse_y += gravite;

        if (p->position.y >= sol_y)
        {
            p->position.y = sol_y;
            vitesse_y = 0;
            p->sol = 1;
        }
    }
}
  
 





