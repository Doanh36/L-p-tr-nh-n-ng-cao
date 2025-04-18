#include "Engine.h"
#include "TextureManager.h"
#include "Vector2D.h"
#include "Transform.h"
#include "Pacman.h"
#include "Map.h"
#include "Ghost.h"

Engine* Engine::s_Instance = nullptr;

Properties* pacmanProps = new Properties(std::string("pacman"), m_x, m_y, TILE_SIZE, TILE_SIZE);
Map* m_Map = new Map();
Pacman* pacman = new Pacman(pacmanProps, m_Map);

Properties* ghostProps = new Properties("ghost", 304, 240, TILE_SIZE, TILE_SIZE);
Ghost* ghost = new Ghost(ghostProps, m_Map);

bool Engine::Init(){
    if ( SDL_Init(SDL_INIT_VIDEO) != 0 && IMG_Init( IMG_INIT_JPG | IMG_INIT_PNG ) ) {
        SDL_Log( "Failed to initialize SDL: %s" , SDL_GetError() ) ;
        return false;
    }

    m_Window = SDL_CreateWindow( " Pac-man " , SDL_WINDOWPOS_CENTERED , SDL_WINDOWPOS_CENTERED , SCREEN_WIDTH , SCREEN_HEIGHT ,0);
    if ( m_Window == nullptr ) {
        SDL_Log( "Failed to create Window: %s" , SDL_GetError() ) ;
        return false;
    }

    m_Renderer = SDL_CreateRenderer( m_Window , -1 , SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC );
    if ( m_Renderer == nullptr ) {
        SDL_Log( "Failed to create Renderer: %s" , SDL_GetError() ) ;
        return false;
    }

    TextureManager::GetInstance()->Load( "pacman" , "assets/pacman.png");
    TextureManager::GetInstance()->Load( "ghost" , "assets/blinky icon.png" );

    Transform tf;
    tf.Log();
    return m_IsRunning = true;
}

void Engine::Update(){
    pacman->Update(0.016f);
    ghost->UpdateofGhost(0.016f, pacman->GetPosition(), pacman->GetDirection());
}

void Engine::Render(){

    SDL_SetRenderDrawColor( m_Renderer, 0 , 0 , 0 , 0 );
    SDL_RenderClear(m_Renderer);

    m_Map->Draw(m_Renderer);

    pacman->Draw();

    ghost->Draw();

    SDL_RenderPresent(m_Renderer);
}

void Engine::Events(){
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
            case SDL_QUIT:
                Quit();
                break;

            case SDL_KEYDOWN:
            case SDL_KEYUP:
                pacman->HandleInput(event);
                break;
        }
    }
}

bool Engine::Clean(){
    TextureManager::GetInstance()->Clean();
    delete gameMap;
    SDL_DestroyRenderer ( m_Renderer );
    SDL_DestroyWindow ( m_Window );
    IMG_Quit();
    SDL_Quit();

}

void Engine::Quit(){
    m_IsRunning = false;
}
