#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_ttf.h>
#include <stdio.h>
#include <stdbool.h>
#include <time.h>

#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 720
#define LEVEL_WIDTH 1600
#define LEVEL_HEIGHT 1200

// Text rendering constants
#define FONT_SIZE 24
#define TEXT_COLOR_R 255
#define TEXT_COLOR_G 255
#define TEXT_COLOR_B 255
#define TEXT_POS_X 10
#define TEXT_POS_Y 10

// Game Guide Window Constants
#define GUIDE_WIDTH 400
#define GUIDE_HEIGHT 300
#define GUIDE_BG_R 0      // Set guide background to black (or 0 for transparent if supported)
#define GUIDE_BG_G 0
#define GUIDE_BG_B 0
#define GUIDE_TEXT_COLOR_R 200
#define GUIDE_TEXT_COLOR_G 200
#define GUIDE_TEXT_COLOR_B 200
#define GUIDE_TEXT_OFFSET_X 20
#define GUIDE_TEXT_OFFSET_Y 20

SDL_Surface *screen = NULL;
SDL_Surface *background = NULL;
SDL_Surface *coin_image = NULL;
SDL_Surface *character_image = NULL;
TTF_Font *font = NULL;
TTF_Font *guideFont = NULL;
SDL_Color textColor = {TEXT_COLOR_R, TEXT_COLOR_G, TEXT_COLOR_B};
SDL_Color guideTextColor = {GUIDE_TEXT_COLOR_R, GUIDE_TEXT_COLOR_G, GUIDE_TEXT_COLOR_B};
SDL_Surface *timeSurface = NULL;
SDL_Surface *guideSurface = NULL;
bool showGuide = false;
Uint32 pause_time = 0;

SDL_Event event;
bool running = true;
bool split_screen = false;

int cameraX1 = 0, cameraY1 = 0; // Camera for player 1
int cameraX2 = 50, cameraY2 = 50; // Camera for player 2 (initial offset)
Uint32 start_time;

// Coin animation variables
int coin_x = 600;
int coin_y = 725;
int coin_direction = 1;
int coin_move_distance = 100;

// Position du personnage (assuming one character for now)
int character_x = 300;
int character_y = 700;

SDL_Surface *load_image(const char *path) {
    SDL_Surface *loaded = IMG_Load(path);
    if (!loaded) {
        printf("Impossible de charger l'image %s: %s\n", path, IMG_GetError());
        return NULL;
    }
    SDL_Surface *optimized = SDL_DisplayFormatAlpha(loaded);
    SDL_FreeSurface(loaded);
    return optimized;
}
void apply_surface(int x, int y, SDL_Surface *src, SDL_Surface *dest, int w, int h) {
    SDL_Rect offset;
    offset.x = x;
    offset.y = y;
    SDL_BlitSurface(src, NULL, dest, &offset);
}

/*void apply_surface(int x, int y, SDL_Surface *src, SDL_Surface *dest, int w, int h) {
    SDL_Rect offset;
    offset.x = x;
    offset.y = y;
    SDL_BlitSurface(src, NULL, dest, &offset);

    SDL_Rect destRect;
    destRect.x = x;
    destRect.y = y;
    destRect.w = w;  // New width
    destRect.h = h;  // New height
    SDL_BlitScaled(src, NULL, dest, &destRect);
}*/

void handle_input() {
    const Uint8 *keys = SDL_GetKeyState(NULL);

    // Player 1 controls (Arrow keys)
    if (keys[SDLK_LEFT]) cameraX1 -= 5;
    if (keys[SDLK_RIGHT]) cameraX1 += 5;
    if (keys[SDLK_UP]) cameraY1 -= 5;
    if (keys[SDLK_DOWN]) cameraY1 += 5;

    // Player 2 controls (Q, D, Z, S)
    if (keys[SDLK_q]) cameraX2 -= 5;
    if (keys[SDLK_d]) cameraX2 += 5;
    if (keys[SDLK_z]) cameraY2 -= 5;
    if (keys[SDLK_s]) cameraY2 += 5;

    // Keep cameras within level bounds (for both players)
    if (cameraX1 < 0) cameraX1 = 0;
    if (cameraY1 < 0) cameraY1 = 0;
    if (cameraX1 > LEVEL_WIDTH - SCREEN_WIDTH) cameraX1 = LEVEL_WIDTH - SCREEN_WIDTH;
    if (cameraY1 > LEVEL_HEIGHT - SCREEN_HEIGHT / 2) cameraY1 = LEVEL_HEIGHT - SCREEN_HEIGHT / 2; // Adjusted for split screen height (TOP HALF)

    if (cameraX2 < 0) cameraX2 = 0;
    if (cameraY2 < SCREEN_HEIGHT / 2) cameraY2 = SCREEN_HEIGHT / 2; // Lower bound for player 2 (BOTTOM HALF starts at SCREEN_HEIGHT / 2)
    if (cameraX2 > LEVEL_WIDTH - SCREEN_WIDTH) cameraX2 = LEVEL_WIDTH - SCREEN_WIDTH;
    if (cameraY2 > LEVEL_HEIGHT) cameraY2 = LEVEL_HEIGHT; // Upper bound for player 2 (BOTTOM HALF goes up to LEVEL_HEIGHT)
}

void render_time() {
    if (showGuide) {
        return; // Don't render time when the guide is showing
    }

    Uint32 current_time = SDL_GetTicks();
    int elapsed_seconds = (current_time - start_time) / 1000;
    char time_text[32];
    sprintf(time_text, "Time: %d", elapsed_seconds);

    if (timeSurface != NULL) {
        SDL_FreeSurface(timeSurface);
    }

    timeSurface = TTF_RenderText_Solid(font, time_text, textColor);
    if (timeSurface == NULL) {
        fprintf(stderr, "TTF_RenderText_Solid: %s\n", TTF_GetError());
        running = false;
    }

    apply_surface(TEXT_POS_X, TEXT_POS_Y, timeSurface, screen, 100, 20); //normal
}

void render_game(SDL_Rect camera) {
    SDL_BlitSurface(background, &camera, screen, NULL);
    apply_surface(coin_x - camera.x, coin_y - camera.y, coin_image, screen, 25, 25); // Adjusted size to 25x25
    apply_surface(character_x - camera.x, character_y - camera.y, character_image, screen, 100, 100); //adjusted size.
}

void render_guide() {
    if (guideSurface != NULL) {
        SDL_FreeSurface(guideSurface);
        guideSurface = NULL;
    }

    guideSurface = SDL_CreateRGBSurface(SDL_SWSURFACE, GUIDE_WIDTH, GUIDE_HEIGHT, 32, 0, 0, 0, 0);
    if (guideSurface != NULL) {
        SDL_FillRect(guideSurface, NULL, SDL_MapRGB(guideSurface->format, GUIDE_BG_R, GUIDE_BG_G, GUIDE_BG_B));

        SDL_Surface *line1 = TTF_RenderText_Solid(guideFont, "Game Guide", guideTextColor);
        SDL_Surface *line2 = TTF_RenderText_Solid(guideFont, "Player 1:", guideTextColor);
        SDL_Surface *line3 = TTF_RenderText_Solid(guideFont, "  Arrows: Move", guideTextColor);
        SDL_Surface *line4 = TTF_RenderText_Solid(guideFont, "Player 2:", guideTextColor);
        SDL_Surface *line5 = TTF_RenderText_Solid(guideFont, "  Q/D/Z/S: Move", guideTextColor);
        SDL_Surface *line6 = TTF_RenderText_Solid(guideFont, "M: Toggle Split Screen", guideTextColor);
        SDL_Surface *line7 = TTF_RenderText_Solid(guideFont, "G: Toggle Game Guide", guideTextColor);
        SDL_Surface *line8 = TTF_RenderText_Solid(guideFont, "ESC: Quit", guideTextColor);

        apply_surface(GUIDE_TEXT_OFFSET_X, GUIDE_TEXT_OFFSET_Y, line1, guideSurface, 200, 20);
        apply_surface(GUIDE_TEXT_OFFSET_X, GUIDE_TEXT_OFFSET_Y + 30, line2, guideSurface, 100, 20);
        apply_surface(GUIDE_TEXT_OFFSET_X + 20, GUIDE_TEXT_OFFSET_Y + 50, line3, guideSurface, 150, 20);
        apply_surface(GUIDE_TEXT_OFFSET_X, GUIDE_TEXT_OFFSET_Y + 80, line4, guideSurface, 100, 20);
        apply_surface(GUIDE_TEXT_OFFSET_X + 20, GUIDE_TEXT_OFFSET_Y + 100, line5, guideSurface, 150, 20);
        apply_surface(GUIDE_TEXT_OFFSET_X, GUIDE_TEXT_OFFSET_Y + 130, line6, guideSurface, 200, 20);
        apply_surface(GUIDE_TEXT_OFFSET_X, GUIDE_TEXT_OFFSET_Y + 160, line7, guideSurface, 200, 20);
        apply_surface(GUIDE_TEXT_OFFSET_X, GUIDE_TEXT_OFFSET_Y + 190, line8, guideSurface, 200, 20);

        SDL_Rect guidePos;
        guidePos.x = (SCREEN_WIDTH - GUIDE_WIDTH) / 2;
        guidePos.y = (SCREEN_HEIGHT - GUIDE_HEIGHT) / 2;
        SDL_BlitSurface(guideSurface, NULL, screen, &guidePos);

        SDL_FreeSurface(line1);
        SDL_FreeSurface(line2);
        SDL_FreeSurface(line3);
        SDL_FreeSurface(line4);
        SDL_FreeSurface(line5);
        SDL_FreeSurface(line6);
        SDL_FreeSurface(line7);
        SDL_FreeSurface(line8);
    } else {
        fprintf(stderr, "SDL_CreateRGBSurface failed: %s\n", TTF_GetError());
        running = false;
    }
}

void update_coin() {
    if (!showGuide) {
        coin_x += coin_direction * 2;
        if (coin_direction == 1 && coin_x > 600 + coin_move_distance) {
            coin_direction = -1;
        } else if (coin_direction == -1 && coin_x < 600) {
            coin_direction = 1;
        }
    }
}

int main(int argc, char *argv[]) {
    SDL_Init(SDL_INIT_VIDEO);
    if (TTF_Init() == -1) {
        fprintf(stderr, "TTF_Init: %s\n", TTF_GetError());
        return 1;
    }

    screen = SDL_SetVideoMode(SCREEN_WIDTH, SCREEN_HEIGHT, 32, SDL_SWSURFACE);
    SDL_WM_SetCaption("Jeu Pixel 2D - SDL 1.2", NULL);

    background = load_image("bg.png");
    coin_image = load_image("coin.png");
    character_image = load_image("c.png");

    font = TTF_OpenFont("arial.ttf", FONT_SIZE);
    guideFont = TTF_OpenFont("arial.ttf", 20); // Smaller font for the guide
    if (font == NULL || guideFont == NULL) {
        fprintf(stderr, "TTF_OpenFont: %s\n", TTF_GetError());
        TTF_Quit();
        SDL_Quit();
        return 1;
    }

    if (!background || !coin_image || !character_image) {
        TTF_CloseFont(font);
        TTF_CloseFont(guideFont);
        TTF_Quit();
        SDL_Quit();
        return 1;
    }

    start_time = SDL_GetTicks();
    printf("LEVEL_WIDTH: %d, LEVEL_HEIGHT: %d, SCREEN_WIDTH: %d, SCREEN_HEIGHT: %d\n", LEVEL_WIDTH, LEVEL_HEIGHT, SCREEN_WIDTH, SCREEN_HEIGHT);

    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) running = false;
            if (event.type == SDL_KEYDOWN) {
                if (event.key.keysym.sym == SDLK_ESCAPE) running = false;
                if (event.key.keysym.sym == SDLK_m) split_screen = !split_screen;
                if (event.key.keysym.sym == SDLK_g) {
                    showGuide = !showGuide;
                    if (showGuide) {
                        pause_time = SDL_GetTicks(); // Record the time when the guide appears
                    } else {
                        Uint32 elapsed_pause = SDL_GetTicks() - pause_time;
                        start_time += elapsed_pause; // Add the pause duration to the start time
                        pause_time = 0;
                    }
                }
            }
        }

        handle_input();
        update_coin();

        // Fill the screen with black on each frame before rendering
        SDL_FillRect(screen, &screen->clip_rect, SDL_MapRGB(screen->format, 0x00, 0x00, 0x00));

        if (!split_screen) {
            SDL_Rect camera = {cameraX1, cameraY1, SCREEN_WIDTH, SCREEN_HEIGHT};
            render_game(camera);
            render_time();
        } else {
            // Camera for the first player (top half)
            SDL_Rect camera1 = {cameraX1, cameraY1, SCREEN_WIDTH, SCREEN_HEIGHT / 2};
            SDL_BlitSurface(background, &camera1, screen, &(SDL_Rect){0, 0, 0, 0});
            apply_surface(coin_x - camera1.x, coin_y - camera1.y, coin_image, screen, 25, 25); //adjusted size
            apply_surface(character_x - camera1.x, character_y - camera1.y, character_image, screen, 100, 100); //adjusted size

            // Camera for the second player (bottom half)
            SDL_Rect camera2 = {cameraX2, cameraY2, SCREEN_WIDTH, SCREEN_HEIGHT / 2};
            SDL_BlitSurface(background, &camera2, screen, &(SDL_Rect){0, SCREEN_HEIGHT / 2, 0, 0});
            apply_surface(coin_x - camera2.x, coin_y - camera2.y + SCREEN_HEIGHT / 2, coin_image, screen, 25, 25); //adjusted

            // Draw character for the second player ONLY if within their viewport
            int char2_screen_x = character_x - camera2.x;
            int char2_screen_y = character_y - camera2.y + SCREEN_HEIGHT / 2; // Corrected variable name here

            if (char2_screen_x >= 0 && char2_screen_x < SCREEN_WIDTH &&
                char2_screen_y >= SCREEN_HEIGHT / 2 && char2_screen_y < SCREEN_HEIGHT) {
                apply_surface(char2_screen_x, char2_screen_y, character_image, screen, 100, 100); //adjusted size
            }

            render_time();
            apply_surface(TEXT_POS_X, TEXT_POS_Y + SCREEN_HEIGHT / 2, timeSurface, screen, 100, 20);
        }

        if (showGuide) {
            render_guide();
        }

        SDL_Flip(screen);
        SDL_Delay(16);
    }

    SDL_FreeSurface(background);
    SDL_FreeSurface(coin_image);
    SDL_FreeSurface(character_image);
    SDL_FreeSurface(timeSurface);
    SDL_FreeSurface(guideSurface);
    TTF_CloseFont(font);
    TTF_CloseFont(guideFont);
    TTF_Quit();
    SDL_Quit();

    return 0;
}
