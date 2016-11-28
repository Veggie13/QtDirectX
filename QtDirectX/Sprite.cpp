#include "Sprite.h"

Sprite::Sprite( LPCWSTR src, RECT textureSize, RECT spriteSize, LPDIRECT3DDEVICE9 d3dd,
                const FrameState* stateMachine, unsigned int nStates,
                D3DCOLOR bgColor, float alpha, D3DXVECTOR3 vCenter )
:   m_X(0.0f), m_Y(0.0f),
    m_textureSize(textureSize),
    m_spriteSize(spriteSize),
    m_alpha(alpha),
    m_d3dDevice(d3dd),
    m_pSprite(NULL),
    m_pSpriteTex(NULL),
    m_state(NULL),
    m_stateMachine(NULL),
    m_vCenter(vCenter)
{
    if (m_d3dDevice == NULL)
        return;

    HRESULT res = S_OK;

    D3DXIMAGE_INFO d3dxImageInfo;
    res = D3DXCreateTextureFromFileEx(
                            m_d3dDevice,
                            src,
                            (UINT) abs(m_textureSize.right - m_textureSize.left),
                            (UINT) abs(m_textureSize.bottom - m_textureSize.top),
                            1,   // Don't create mip-maps when you plan on using D3DXSPRITE. It throws off the pixel math for sprite animation.
                            0, //D3DPOOL_DEFAULT,
                            D3DFMT_UNKNOWN,
                            D3DPOOL_MANAGED, //D3DPOOL_DEFAULT,
                            D3DX_DEFAULT,
                            D3DX_DEFAULT,
                            bgColor,
                            &d3dxImageInfo,
                            NULL,
                            &m_pSpriteTex );
    if (FAILED(res))
    {
        m_d3dDevice = NULL;
        return;
    }

    res = D3DXCreateSprite(m_d3dDevice, &m_pSprite);
    if (FAILED(res))
    {
        m_d3dDevice = NULL;
        m_pSpriteTex->Release();
        m_pSpriteTex = NULL;
        return;
    }

    if (nStates == 0 || stateMachine == NULL)
    {
        m_stateMachine = new FrameState;
        m_stateMachine->frame = 0;
        m_stateMachine->nextState = 0;
    }
    else
    {
        m_stateMachine = new FrameState[nStates];
        memcpy(m_stateMachine, stateMachine, nStates * sizeof(FrameState));
    }

    m_state = m_stateMachine;
}

Sprite::~Sprite(void)
{
    if (m_pSpriteTex != NULL)
        m_pSpriteTex->Release();
    if (m_pSprite != NULL)
        m_pSprite->Release();

    if (m_stateMachine != NULL)
        delete[] m_stateMachine;
}

void Sprite::draw()
{
    RECT rcSource;
    rcSource.top = m_state->frame * abs(m_spriteSize.bottom - m_spriteSize.top);
    rcSource.left = 0;
    rcSource.bottom = (m_state->frame + 1) * abs(m_spriteSize.bottom - m_spriteSize.top);
    rcSource.right = m_spriteSize.right;

    D3DXVECTOR3 vPosition( m_X, m_Y, 0.0f );

    m_pSprite->Begin( D3DXSPRITE_ALPHABLEND );
    {
        m_pSprite->Draw( m_pSpriteTex,
                         &rcSource,
                         &m_vCenter,
                         &vPosition,
                         D3DCOLOR_COLORVALUE(1.0f,1.0f,1.0f,m_alpha) );
    }
    m_pSprite->End();

    m_state = m_stateMachine + m_state->nextState;
}

void Sprite::setPosition(float x, float y)
{
    m_X = x;
    m_Y = y;
}

float Sprite::getX()
{
    return m_X;
}

float Sprite::getY()
{
    return m_Y;
}

void Sprite::setState(unsigned int state)
{
    m_state = m_stateMachine + state;
}
