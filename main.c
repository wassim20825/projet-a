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
#include "SDL/SDL.h"
#include "SDL/SDL_image.h"
#include <stdio.h>
#include <stdlib.h>
#include "perso.h"

int main(int argc, char *argv[])
{
    // Initialisation de la SDL
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        printf("Erreur SDL_Init : %s\n", SDL_GetError());
        return EXIT_FAILURE;
    }

    // Création de la fenêtre
    SDL_Surface *ecran = SDL_SetVideoMode(1048, 737, 32, SDL_HWSURFACE);
    if (!ecran) {
        printf("Erreur création écran : %s\n", SDL_GetError());
        SDL_Quit();
        return EXIT_FAILURE;
    }
//eka lktiba li tjik fou9 l'ecran
    SDL_WM_SetCaption("wassim", NULL);

    // Chargement du fond d'écran
    SDL_Surface *background = IMG_Load("back.png");
    if (!background) {
        printf("Erreur chargement background : %s\n", IMG_GetError());
        SDL_Quit();
        return EXIT_FAILURE;
    }
 if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        printf("Erreur SDL : %s\n", SDL_GetError());
        return 1;
    }

    if (TTF_Init() == -1) {
        printf("Erreur TTF_Init : %s\n", TTF_GetError());
        return 1;
    }

    // Position du fond postion background
    SDL_Rect positionFond = {0, 0};

    // Initialisation du personnage
    personnage p;
    personnage p2;
    initialiser_personnage(&p2);
  p2.position.x = 100;
    p2.position.y = 567;
     
//bch n3aytou lil fonction intialiser
    initialiser_personnage(&p);
     
//if load el taswira me sarecch
      if (!p.sprite || !p2.sprite) {
        SDL_FreeSurface(background);
        SDL_Quit();
        return EXIT_FAILURE;
    }
       
//bch yefhem illi taswira bel couleur
    SDL_SetColorKey(p.sprite, SDL_SRCCOLORKEY, SDL_MapRGB(p.sprite->format, 0, 0, 255));
    SDL_SetColorKey(p2.sprite, SDL_SRCCOLORKEY, SDL_MapRGB(p2.sprite->format, 0, 0, 255));
     

    // Boucle de jeu
    int continuer = 1;
    SDL_Event event;
    int i = 0;
    int j=0;
    while (continuer) {
    SDL_FillRect(ecran, NULL, SDL_MapRGB(ecran->format, 0, 0, 0)); // Effacer l'écran
    SDL_BlitSurface(background, NULL, ecran, &positionFond);
     
        deplacerperso(&p, &continuer, &event);
        animperso(&i, &event, &p); 
        deplacerperso2(&p2, &continuer, &event);
        animperso2(&j, &event, &p2);       
        afficher_perso(p, ecran   );
        afficher_perso(p2, ecran );




//bch yatla3 l fenetre
        SDL_Flip(ecran);
    }

    // Libération de la mémoire
    SDL_FreeSurface(p.sprite);
    SDL_FreeSurface(background);
    TTF_Quit();
    SDL_Quit();

    return EXIT_SUCCESS;
}


