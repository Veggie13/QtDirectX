#pragma once

#include <d3dx9.h>

#include <QRect>

#include "GameObject.h"


class Layer;
class Sprite;

class Paddle : public GameObject
{
public:
    enum SizeType { NORMAL=0, SMALL, LARGE, NUM_SIZETYPES };

    Paddle( Layer* gameLayer,
            Layer* shadowLayer,
            LPDIRECT3DDEVICE9 d3dd );
    virtual ~Paddle(void);

    QRect getRectangle();

    void setSizeType(SizeType type);
    SizeType sizeType();

private:
    QRect m_rect[NUM_SIZETYPES];

    Sprite* m_main[NUM_SIZETYPES];
    Sprite* m_shadow[NUM_SIZETYPES];

    SizeType m_state;

};
