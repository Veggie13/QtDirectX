#pragma once

#include "Arena.h"

#include "GameObject.h"


class Layer;
class Sprite;

class Block : public GameObject
{
public:
    Block(  const Arena::Spec::BlockType& blockType,
            const QSize& blockSize,
            Layer* gameLayer,
            Layer* shadowLayer,
            LPDIRECT3DDEVICE9 d3dd );
    ~Block(void);

    RECT getRectangle();

    bool impact();

private:
    unsigned int m_strength;
    unsigned int m_points;

    RECT m_rect;

    Sprite* m_main;
    Sprite* m_shadow;

};
