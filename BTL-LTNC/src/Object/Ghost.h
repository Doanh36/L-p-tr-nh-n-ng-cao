#ifndef GHOST_H
#define GHOST_H

#include "TextureManager.h"
#include "GameObject.h"
#include "Map.h"
#include <cmath>
#include <iostream>

enum Direction {
    LEFT = 0,
    RIGHT = 1,
    UP = 2,
    DOWN = 3,
    NONE = -1
};


class Ghost : public GameObject {
public:
    Ghost(Properties* props, Map* map);

    virtual void Update(float dt) override;
    virtual void Draw() override;
    virtual void Clean() override;

    // Tính khoảng cách Manhattan giữa ghost và Pacman
    int ManhattanDistance(int x1, int y1, int x2, int y2) const;

private:
    Map* m_Map;
    Direction m_LastDirection;


    // Pacman luôn đứng yên tại vị trí (1,1)
    const int m_TargetX = 1;
    const int m_TargetY = 1;

    // Tọa độ ghost hiện tại trên lưới
    int m_GridX;
    int m_GridY;

    float m_VelocityX = 0;
    float m_VelocityY = 0;

    const float m_Speed = 100.0f; // pixel/giây (tuỳ chỉnh tốc độ)
};

#endif // GHOST_H
