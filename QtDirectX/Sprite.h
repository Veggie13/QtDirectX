#pragma once

#include <d3dx9.h>

#include "drawable.h"


class Sprite : public Drawable
{
public:
    struct FrameState
    {
        unsigned int frame;
        unsigned int nextState;
    };

    Sprite( LPCWSTR src, RECT textureSize, RECT spriteSize, LPDIRECT3DDEVICE9 d3dd,
            const FrameState* stateMachine, unsigned int nStates,
            D3DCOLOR bgColor, float alpha = 1.0f, D3DXVECTOR3 vCenter = D3DXVECTOR3(0.0f,0.0f,0.0f) );
    virtual ~Sprite(void);

    virtual void draw();

    void setPosition(float x, float y);
    float getX();
    float getY();

    void setState(unsigned int state);

private:
    float m_X, m_Y;

    RECT m_textureSize;
    RECT m_spriteSize;
    float m_alpha;
    
    LPDIRECT3DDEVICE9   m_d3dDevice;
    LPD3DXSPRITE        m_pSprite;
    LPDIRECT3DTEXTURE9  m_pSpriteTex;

    FrameState* m_state;
    FrameState* m_stateMachine;

    D3DXVECTOR3 m_vCenter;

};
