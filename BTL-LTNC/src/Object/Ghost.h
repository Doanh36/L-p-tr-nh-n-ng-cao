#ifndef GHOST_H
#define GHOST_H

#include "GameObject.h"
#include "TextureManager.h"
#include "Map.h"
// Không nhất thiết cần #include "Pacman.h" ở đây nếu chỉ lấy vị trí qua tham số hàm Update
#include <vector> // Cần cho việc tính toán đường đi
#include <cmath>
#include <cstdlib>


enum class GhostState {
    CHASE,
    SCATTER,
    FRIGHTENED,
    EATEN
};

class Ghost : public GameObject {
public:

    Ghost(Properties* props, Map* map);//, int ghostId);

    void Draw() override;
    void Update(float dt) override;
    void UpdateofGhost(float dt,const Vector2D& pacmanPos, const Vector2D& pacmanDir );
    void Clean() override;

    GhostState GetState() const { return m_State; }
    void SetState(GhostState state);

private:
    Map* m_Map;
    Vector2D m_Velocity;
    const float m_BaseSpeed = 100.0f;
    float m_CurrentSpeed;


    int m_GhostId;
    GhostState m_State;
    Vector2D m_TargetTile;

    Vector2D m_CurrentTile;
    Vector2D m_NextTile;
    bool m_IsMoving = false;

    void CalculateTarget(const Vector2D& pacmanPos, const Vector2D& pacmanDir);
    void CalculateDirection();
    bool CanTurn(int dirX, int dirY);
    float GetDistanceSq(const Vector2D& pos1, const Vector2D& pos2);
    bool IsAtTileCenter();

};

#endif // GHOST_H
