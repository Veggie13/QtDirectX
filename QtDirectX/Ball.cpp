#include "Layer.h"
#include "Sprite.h"

#include "Ball.h"


const unsigned int Ball::WIDTH = 5;
const unsigned int Ball::HEIGHT = 4;

Ball::Ball( Layer* gameLayer,
            Layer* shadowLayer,
            LPDIRECT3DDEVICE9 d3dd )
:   m_main(NULL),
    m_shadow(NULL)
{
    RECT ballRect;
    ballRect.top = 0;
    ballRect.left = 0;
    ballRect.bottom = HEIGHT;
    ballRect.right = WIDTH;

    Sprite* gameSprite =
        new Sprite( L"tiles\\ball.bmp",
                    ballRect, ballRect,
                    d3dd,
                    NULL, 0,
                    D3DCOLOR_COLORVALUE(0.0f,1.0f,0.0f,1.0f) );
    Sprite* shadowSprite =
        new Sprite( L"tiles\\shadow ball.bmp",
                    ballRect, ballRect,
                    d3dd,
                    NULL, 0,
                    D3DCOLOR_COLORVALUE(0.0f,1.0f,0.0f,1.0f),
                    0.5f );

    initialize(gameSprite, shadowSprite, gameLayer, shadowLayer);
}

Ball::~Ball(void)
{
    if (m_main != NULL)
        delete m_main;
    if (m_shadow != NULL)
        delete m_shadow;
}

void Ball::setVelocity(float dxps, float dyps)
{
    m_dX = dxps;
    m_dY = dyps;
}

float Ball::getXVelocity()
{
    return m_dX;
}

float Ball::getYVelocity()
{
    return m_dY;
}

float Ball::getTargetX(float dt)
{
    return getX() + m_dX * dt;
}

float Ball::getTargetY(float dt)
{
    return getY() + m_dY * dt;
}
