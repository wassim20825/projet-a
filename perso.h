#ifndef PERSO_H
#define PERSO_H

#include "SDL/SDL.h"
#include "SDL/SDL_image.h"
#include "SDL/SDL_ttf.h"


/**
* @struct vie
* @brief struct for vie
*/ 
typedef struct {
    SDL_Surface *spriteVie;/*!< surface*/   
    SDL_Rect    posVie;    /*!< rectangle*/    
    int         nbVies;    /*!< int*/    
} vie;
/**
* @struct score
* @brief struct for score
*/ 
typedef struct  {
    TTF_Font   *police; /*!< texte*/       
    SDL_Color   couleur;  /*!< couleur*/     
    SDL_Rect    posScore; /*!< rectangle*/    
    int         points;   /*!< int*/    
} score;
/**
* @struct personnage
* @brief struct for personnage
*/ 
typedef struct {
    SDL_Rect position;/*!< rectangle*/
    SDL_Surface *sprite;/*!< surface*/
     vie         v;   /*!< vie*/       // la struct vie
    score       s;   /*!< score*/       // la struct score;
    double vitesse;/*!< double*/
    double acceleration;/*!< double*/
    int sol;/*!< int*/
} personnage;

 

void initialiser_personnage(personnage *p);
 
void afficher_perso( personnage p, SDL_Surface *ecran);
void animperso2(int *i, SDL_Event *event, personnage *p); 
void animperso(int *i, SDL_Event *event, personnage *p);
void jump(personnage *p);
void deplacerperso(personnage *p, int *continuer, SDL_Event *event);
void deplacerperso2(personnage *p, int *continuer, SDL_Event *event); 

#endif
