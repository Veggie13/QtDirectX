#pragma once

#include <d3dx9.h>

#include "GameObject.h"


class Layer;
class Sprite;

class Ball : public GameObject
{
public:
    static const unsigned int WIDTH;
    static const unsigned int HEIGHT;


    Ball(   Layer* gameLayer,
            Layer* shadowLayer,
            LPDIRECT3DDEVICE9 d3dd );
    virtual ~Ball(void);

    void setVelocity(float dxps, float dyps);
    float getXVelocity();
    float getYVelocity();

    float getTargetX(float dt);
    float getTargetY(float dt);

private:
    Sprite* m_main;
    Sprite* m_shadow;

    float m_dX;
    float m_dY;

};
