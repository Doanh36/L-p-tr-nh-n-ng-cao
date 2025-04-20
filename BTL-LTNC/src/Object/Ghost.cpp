#include "Ghost.h"

Ghost::Ghost(Properties* props, Map* map)
    : GameObject(props), m_Map(map), m_LastDirection(NONE) {}

void Ghost::Draw() {
    int drawX = m_Transform->position.X - TILE_SIZE / 2 + 146;
    int drawY = m_Transform->position.Y - TILE_SIZE / 2 + 14;

    TextureManager::GetInstance()->DrawFrame(m_TextureID,drawX, drawY,m_Width, m_Height,0, 0,m_Flip);
}

void Ghost::Clean() {

}

void Ghost::Update(float dt)
{
    // Kiểm tra nếu đang đứng chính giữa tile
    int centerX = (int)(m_Transform->position.X + TILE_SIZE / 2) % TILE_SIZE;
    int centerY = (int)(m_Transform->position.Y + TILE_SIZE / 2) % TILE_SIZE;

    bool onTile = (centerX == 0) && (centerY == 0);

    if (onTile) {
        // Xác định ô lưới hiện tại
        int gridX = (m_Transform->position.X - TILE_SIZE / 2 ) / TILE_SIZE;
        int gridY = (m_Transform->position.Y - TILE_SIZE / 2 ) / TILE_SIZE;

        m_GridX = gridX;
        m_GridY = gridY;

        // Xét 4 hướng: trái, phải, lên, xuống
        const int dx[] = { -1, 1, 0, 0 };
        const int dy[] = { 0, 0, -1, 1 };

        int bestDir = -1;
        int minDistance = 1e9;

        for (int i = 0; i < 4; ++i) {

            if ((m_LastDirection == LEFT && i == RIGHT) || (m_LastDirection == RIGHT && i == LEFT) ||
    (m_LastDirection == UP && i == DOWN) || (m_LastDirection == DOWN && i == UP)) {
    continue;
}

            int newX = gridX + dx[i];
            int newY = gridY + dy[i];

            float pixelX = newX * TILE_SIZE + TILE_SIZE / 2.0f;
            float pixelY = newY * TILE_SIZE + TILE_SIZE / 2.0f;

            if (m_Map->CanMove(pixelX, pixelY)) {
                int dist = ManhattanDistance(newX, newY, m_TargetX, m_TargetY);
                // Ưu tiên hướng cũ nếu khoảng cách bằng nhau
            if (dist < minDistance || (dist == minDistance && i == m_LastDirection)) {
                minDistance = dist;
                bestDir = i;
            }
            }
        }

        if (bestDir != -1) {
            m_VelocityX = dx[bestDir] * m_Speed;
            m_VelocityY = dy[bestDir] * m_Speed;
m_LastDirection = static_cast<Direction>(bestDir);

        } else {
            // Không có hướng hợp lệ → đứng im
            m_VelocityX = 0;
            m_VelocityY = 0;
        }
    }

    // Giả lập bước tiếp theo để kiểm tra va chạm
    float nextX = m_Transform->position.X + m_VelocityX * dt;
    float nextY = m_Transform->position.Y + m_VelocityY * dt;

    if (m_Map->CanMove(nextX, nextY)) {
    m_Transform->position.X = nextX;
    m_Transform->position.Y = nextY;
    } else {
    // Gặp tường → dừng lại + căn lại vị trí về giữa tile
    m_VelocityX = 0;
    m_VelocityY = 0;

    // Ép ghost về giữa tile gần nhất
    int grid_X = static_cast<int>(m_Transform->position.X / TILE_SIZE);
    int grid_Y = static_cast<int>(m_Transform->position.Y / TILE_SIZE);
    m_Transform->position.X = grid_X * TILE_SIZE + TILE_SIZE / 2;
    m_Transform->position.Y = grid_Y * TILE_SIZE + TILE_SIZE / 2;

    std::cout << "[Ghost] Position: " << m_Transform->position.X
          << ", onTile: " << (onTile ? "YES" : "NO") << "\n";

}
}

int Ghost::ManhattanDistance(int x1, int y1, int x2, int y2) const {
    return abs(x1 - x2) + abs(y1 - y2);
}
