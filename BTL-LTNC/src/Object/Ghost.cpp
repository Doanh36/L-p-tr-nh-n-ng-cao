#include "Ghost.h"

Ghost::Ghost(Properties* props, Map* map) : GameObject(props), m_Map(map), m_Velocity(-1,0), m_CurrentSpeed(100.0f){

}

void Ghost::Update(float dt){}

void Ghost::Draw()
{
    int drawX = m_Transform->position.X - TILE_SIZE / 2 + 146;
    int drawY = m_Transform->position.Y - TILE_SIZE / 2 + 14;

    TextureManager::GetInstance()->DrawFrame(m_TextureID,drawX, drawY,m_Width, m_Height,0, 0,m_Flip);
}

void Ghost::UpdateofGhost(float dt,const Vector2D& pacmanPos, const Vector2D& pacmanDir )
{
    int tileX = (m_Transform->position.X + TILE_SIZE / 2) / TILE_SIZE;
    int tileY = (m_Transform->position.Y + TILE_SIZE / 2) / TILE_SIZE;
    m_CurrentTile = Vector2D(tileX, tileY);

    if (IsAtTileCenter()) {
        CalculateTarget(pacmanPos, pacmanDir);
    }

    float newX = m_Transform->position.X + m_Velocity.X * m_CurrentSpeed * dt;
    float newY = m_Transform->position.Y + m_Velocity.Y * m_CurrentSpeed * dt;

    if (m_Map->CanMove(newX, newY)) {
        m_Transform->position.X = newX;
        m_Transform->position.Y = newY;
    } else {
        CalculateTarget(pacmanPos, pacmanDir);
    }

}

void Ghost::Clean()
{

}

bool Ghost::IsAtTileCenter() {
    int centerX = m_CurrentTile.X * TILE_SIZE + TILE_SIZE / 2;
    int centerY = m_CurrentTile.Y * TILE_SIZE + TILE_SIZE / 2;

    return abs(m_Transform->position.X - centerX) < 2 && abs(m_Transform->position.Y - centerY) < 2;
}

void Ghost::CalculateDirection()
{
    std::vector<Vector2D> possibleDirs;

    // 4 hướng: trái, phải, lên, xuống
    std::vector<Vector2D> directions = {
        Vector2D(-1, 0), // left
        Vector2D(1, 0),  // right
        Vector2D(0, -1), // up
        Vector2D(0, 1)   // down
    };

    for (const auto& dir : directions) {
        int nextX = m_CurrentTile.X + dir.X;
        int nextY = m_CurrentTile.Y + dir.Y;

        if ( m_Map->CanMove(nextX, nextY) )
            continue;

        // Loại trừ hướng ngược lại
        if (dir.X == -m_Velocity.X && dir.Y == -m_Velocity.Y)
            continue;

        possibleDirs.push_back(dir);
    }

    if (!possibleDirs.empty()) {
        int index = rand() % possibleDirs.size();
        m_Velocity = possibleDirs[index];
    } else {
        // Nếu kẹt, quay đầu
        m_Velocity.X = -m_Velocity.X;
        m_Velocity.Y = -m_Velocity.Y;
    }
}

void Ghost::CalculateTarget( const Vector2D& pacmanPos, const Vector2D& pacmanDir )
{
    std::vector<Vector2D> directions = {
        Vector2D(-1, 0), // left
        Vector2D(1, 0),  // right
        Vector2D(0, -1), // up
        Vector2D(0, 1)   // down
    };

    Vector2D bestDir = Vector2D(0, 0);
    float minDist = std::numeric_limits<float>::max();

    for (const auto& dir : directions) {
        int nextX = m_CurrentTile.X + dir.X;
        int nextY = m_CurrentTile.Y + dir.Y;

        // Nếu là tường, bỏ qua
        if (m_Map->CanMove(nextX, nextY))
            continue;

        // Tránh quay đầu lại
        if (dir.X == -m_Velocity.X && dir.Y == -m_Velocity.Y)
            continue;

        // Tính khoảng cách đến Pacman
        Vector2D nextTile(nextX, nextY);
        Vector2D pacmanTile(pacmanPos.X / TILE_SIZE, pacmanPos.Y / TILE_SIZE);
        float dist = GetDistanceSq(nextTile, pacmanTile);

        if (dist < minDist) {
            minDist = dist;
            bestDir = dir;
        }
    }

    if (bestDir.X != 0 || bestDir.Y != 0) {
        m_Velocity = bestDir;
    }
}

float Ghost::GetDistanceSq( const Vector2D& pos1, const Vector2D& pos2 )
{
    float dx = pos1.X - pos2.X;
    float dy = pos1.Y - pos2.Y;
    return dx * dx + dy * dy;
}
