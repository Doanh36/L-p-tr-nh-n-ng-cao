#include "Pacman.h"
#include "TextureManager.h"


Pacman::Pacman(int x, int y, std::string textureID)
    : m_x(x), m_y(y), m_textureID(textureID), m_speed(2),
      m_frame(0), m_lastFrameTime(SDL_GetTicks()) {}

bool Pacman::CanMove( int newX, int newY, const int maze[MAP_HEIGHT][MAP_WIDTH] ) {
    const int offset = TILE_SIZE / 4;  // độ lệch để kiểm tra 2 điểm

    // Vị trí hai điểm phía trước (tùy hướng)
    int px1 = newX, py1 = newY;
    int px2 = newX, py2 = newY;

    switch (m_direction) {
        case UP:
            px1 -= offset; py1 -= TILE_SIZE / 2;
            px2 += offset; py2 -= TILE_SIZE / 2;
            break;
        case DOWN:
            px1 -= offset; py1 += TILE_SIZE / 2;
            px2 += offset; py2 += TILE_SIZE / 2;
            break;
        case LEFT:
            px1 -= TILE_SIZE / 2; py1 -= offset;
            px2 -= TILE_SIZE / 2; py2 += offset;
            break;
        case RIGHT:
            px1 += TILE_SIZE / 2; py1 -= offset;
            px2 += TILE_SIZE / 2; py2 += offset;
            break;
    }

    // Chuyển pixel sang tọa độ tile
    int tileX1 = px1 / TILE_SIZE;
    int tileY1 = py1 / TILE_SIZE;
    int tileX2 = px2 / TILE_SIZE;
    int tileY2 = py2 / TILE_SIZE;

    // Kiểm tra nằm trong bản đồ
    if (tileX1 < 0 || tileX1 >= MAP_WIDTH || tileY1 < 0 || tileY1 >= MAP_HEIGHT ||
        tileX2 < 0 || tileX2 >= MAP_WIDTH || tileY2 < 0 || tileY2 >= MAP_HEIGHT) {
        return false;
    }

    // Trả về true nếu cả hai điểm không va vào tường
    return (maze[tileY1][tileX1] == 0 && maze[tileY2][tileX2] == 0);

}

void Pacman::Update( const int maze[MAP_HEIGHT][MAP_WIDTH]) {

    if (SDL_GetTicks() - m_lastFrameTime > m_animDelay) {
        m_frame = (m_frame + 1) % 2;
        m_lastFrameTime = SDL_GetTicks();
    }

    // Gần trung tâm tile chưa?
    int tileCenterX = ((m_x + TILE_SIZE / 2) / TILE_SIZE) * TILE_SIZE;
    int tileCenterY = ((m_y + TILE_SIZE / 2) / TILE_SIZE) * TILE_SIZE;

    bool nearCenterX = abs(m_x - tileCenterX) < 2;
    bool nearCenterY = abs(m_y - tileCenterY) < 2;

    // Nếu gần tâm và có hướng mới, thử rẽ
    if (nearCenterX && nearCenterY) {
        int tryX = m_x;
        int tryY = m_y;

        switch (m_nextDirection) {
            case UP:    tryY -= m_speed; break;
            case DOWN:  tryY += m_speed; break;
            case LEFT:  tryX -= m_speed; break;
            case RIGHT: tryX += m_speed; break;
        }

        if (CanMove(tryX, tryY, maze)) {
            // Snap vào giữa tile và đổi hướng
            m_x = tileCenterX;
            m_y = tileCenterY;
            m_direction = m_nextDirection;
        }
    }

    // Di chuyển theo hướng hiện tại
    int newX = m_x, newY = m_y;

    switch (m_direction) {
        case UP:    newY -= m_speed; break;
        case DOWN:  newY += m_speed; break;
        case LEFT:  newX -= m_speed; break;
        case RIGHT: newX += m_speed; break;
    }

    if (CanMove(newX, newY, maze)) {
        m_x = newX;
        m_y = newY;
    }
}

void Pacman::Render(SDL_Renderer* renderer) {
    int drawX = m_x - TILE_SIZE / 2;
    int drawY = m_y - TILE_SIZE / 2;

    TextureManager::GetInstance()->DrawFrame(
        m_textureID,
        drawX, drawY,
        32, 32,         // kích thước frame
        0, m_frame,     // row, frame
        SDL_FLIP_NONE
    );
}

void Pacman::HandleInput(SDL_Event& e) {
    if (e.type == SDL_KEYDOWN) {
    switch (e.key.keysym.sym) {
        case SDLK_UP:    m_direction = UP; break;
        case SDLK_DOWN:  m_direction = DOWN; break;
        case SDLK_LEFT:  m_direction = LEFT; break;
        case SDLK_RIGHT: m_direction = RIGHT; break;
    }
}
}
