#include "Sprite.h"

#include "Paddle.h"


const unsigned int HEIGHT = 8;
const unsigned int WIDTH_NORMAL = 32;
const unsigned int WIDTH_SMALL = 24;
const unsigned int WIDTH_LARGE = 40;
const unsigned int FRAMES = 3;

Paddle::Paddle( Layer* gameLayer,
                Layer* shadowLayer,
                LPDIRECT3DDEVICE9 d3dd )
:   m_state(NORMAL)
{
    m_rect[NORMAL] = QRect(0, 0, WIDTH_NORMAL, HEIGHT);
    m_rect[SMALL] = QRect(0, 0, WIDTH_SMALL, HEIGHT);
    m_rect[LARGE] = QRect(0, 0, WIDTH_LARGE, HEIGHT);

    Sprite::FrameState* machine = new Sprite::FrameState[FRAMES*4];
    machine[0].frame = 0;
    machine[0].nextState = 0;
    for (unsigned int i = 1; i < FRAMES*4; i++)
    {
        machine[i].frame = (i+3)/4;
        machine[i].nextState = (i+1)%(FRAMES*4);
    }

    RECT texRect, normRect, smallRect, largeRect;
    normRect.left = m_rect[NORMAL].left();
    normRect.right = m_rect[NORMAL].right();
    normRect.top = m_rect[NORMAL].top();
    normRect.bottom = m_rect[NORMAL].bottom();

    smallRect.right = m_rect[SMALL].right();
    largeRect.right = m_rect[LARGE].right();
    
    texRect = normRect;
    texRect.bottom = HEIGHT * FRAMES;
    m_main[NORMAL] =
        new Sprite( L"tiles\\paddle normal.bmp",
                    texRect, normRect,
                    d3dd,
                    machine, FRAMES*4,
                    D3DCOLOR_COLORVALUE(0.0f,1.0f,0.0f,1.0f),
                    1.0f,
                    D3DXVECTOR3( (float)WIDTH_NORMAL/2.0f, 0.0f, 0.0f ) );

    texRect.right = WIDTH_SMALL;
    m_main[SMALL] =
        new Sprite( L"tiles\\paddle shrink.bmp",
                    texRect, smallRect,
                    d3dd,
                    machine, FRAMES*4,
                    D3DCOLOR_COLORVALUE(0.0f,1.0f,0.0f,1.0f),
                    1.0f,
                    D3DXVECTOR3( (float)WIDTH_SMALL/2.0f, 0.0f, 0.0f ) );

    texRect.right = WIDTH_LARGE;
    m_main[LARGE] =
        new Sprite( L"tiles\\paddle expand.bmp",
                    texRect, largeRect,
                    d3dd,
                    machine, FRAMES*4,
                    D3DCOLOR_COLORVALUE(0.0f,1.0f,0.0f,1.0f),
                    1.0f,
                    D3DXVECTOR3( (float)WIDTH_LARGE/2.0f, 0.0f, 0.0f ) );

    m_shadow[NORMAL] =
        new Sprite( L"tiles\\shadow paddle normal.bmp",
                    normRect, normRect,
                    d3dd,
                    NULL, 0,
                    D3DCOLOR_COLORVALUE(0.0f,1.0f,0.0f,1.0f),
                    0.5f,
                    D3DXVECTOR3( (float)WIDTH_NORMAL/2.0f, 0.0f, 0.0f ) );

    m_shadow[SMALL] =
        new Sprite( L"tiles\\shadow paddle shrink.bmp",
                    smallRect, smallRect,
                    d3dd,
                    NULL, 0,
                    D3DCOLOR_COLORVALUE(0.0f,1.0f,0.0f,1.0f),
                    0.5f,
                    D3DXVECTOR3( (float)WIDTH_NORMAL/2.0f, 0.0f, 0.0f ) );

    m_shadow[LARGE] =
        new Sprite( L"tiles\\shadow paddle expand.bmp",
                    largeRect, largeRect,
                    d3dd,
                    NULL, 0,
                    D3DCOLOR_COLORVALUE(0.0f,1.0f,0.0f,1.0f),
                    0.5f,
                    D3DXVECTOR3( (float)WIDTH_LARGE/2.0f, 0.0f, 0.0f ) );

    delete[] machine;

    initialize(m_main[NORMAL], m_shadow[NORMAL], gameLayer, shadowLayer);
}

Paddle::~Paddle(void)
{
    for (unsigned int i = 0; i < NUM_SIZETYPES; i++)
    {
        SizeType type = (SizeType)i;
        if (m_main[type] != NULL)
            delete m_main[type];
    }
}

QRect Paddle::getRectangle()
{
    return m_rect[m_state].translated((int)(getX() - (float)m_rect[m_state].width() / 2.0f), (int)getY());
}

void Paddle::setSizeType(Paddle::SizeType type)
{
    m_state = type;
    
    setGameSprite(m_main[m_state]);
    setShadowSprite(m_shadow[m_state]);
}
