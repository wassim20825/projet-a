#ifndef PERSO_H
#define PERSO_H

#include "SDL/SDL.h"
#include "SDL/SDL_image.h"
#include "SDL/SDL_ttf.h"


 
typedef struct {
    SDL_Surface *spriteVie;  // ton image de cœur, par ex.
    SDL_Rect    posVie;      // où l’afficher
    int         nbVies;      // nombre de vies actuelles
} vie;

typedef struct  {
    TTF_Font   *police;      // la police pour le texte
    SDL_Color   couleur;     // couleur du texte
    SDL_Rect    posScore;    // où l’afficher
    int         points;      // valeur du score
} score;

typedef struct {
    SDL_Rect position;
    SDL_Surface *sprite;
     vie         v;          // la struct vie
    score       s;          // la struct score;
    double vitesse;
    double acceleration;
    int sol;
} personnage;

typedef struct {
    SDL_Surface *spriteVie;  // ton image de cœur, par ex.
    SDL_Rect    posVie;      // où l’afficher
    int         nbVies;      // nombre de vies actuelles
} vie_ennemi;

typedef struct  {
    TTF_Font   *police;      // la police pour le texte
    SDL_Color   couleur;     // couleur du texte
    SDL_Rect    posScore;    // où l’afficher
    int         points;      // valeur du score
} score_ennemi;

typedef struct {
    SDL_Rect position;
    SDL_Surface *sprite;
    vie_ennemi         v;          // la struct vie
    score_ennemi       s;          // la struct score;
    double vitesse;
    double acceleration;
    int sol;
} ennemi;

void initialiser_personnage(personnage *p);
void initialiser_ennemi(ennemi *e);
void afficher_perso(personnage p, SDL_Surface *ecran, SDL_Surface *background, SDL_Rect positionFond);
void afficher_ennemi(ennemi e, SDL_Surface *ecran, SDL_Surface *background, SDL_Rect positionFond);
void animperso(int *i, SDL_Event *event, personnage *p);
void jump(personnage *p);
void deplacerperso(personnage *p, int *continuer, SDL_Event *event);
 

#endif
