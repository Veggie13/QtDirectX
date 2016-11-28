#include "Layer.h"
#include "Sprite.h"

#include "Block.h"


Block::Block(   const Arena::Spec::BlockType& blockType,
                const QSize& blockSize,
                Layer* gameLayer,
                Layer* shadowLayer,
                LPDIRECT3DDEVICE9 d3dd )
:   m_strength(blockType.strength),
    m_points(blockType.points),
    m_main(NULL),
    m_shadow(NULL)
{
    m_rect.top = 0;
    m_rect.left = 0;
    m_rect.right = blockSize.width();
    m_rect.bottom = blockSize.height();

    RECT texSizeRect = m_rect;
    texSizeRect.bottom = m_strength * blockSize.height();

    Sprite::FrameState* machine = new Sprite::FrameState[m_strength];
    for (unsigned int i = 0; i < m_strength; i++)
    {
        machine[i].frame = m_strength - i - 1;
        machine[i].nextState = i;
    }

    wchar_t src[128];
    memset(src, 0, sizeof(src));
    blockType.source.toWCharArray(src);
    m_main =
        new Sprite( src,
                    texSizeRect, m_rect,
                    d3dd,
                    machine, m_strength,
                    D3DCOLOR_COLORVALUE(0.0f,1.0f,0.0f,1.0f) );
    m_main->setState(m_strength - 1);
    delete[] machine;

    memset(src, 0, sizeof(src));
    blockType.shadowSource.toWCharArray(src);
    m_shadow =
        new Sprite( src,
                    m_rect, m_rect,
                    d3dd,
                    NULL, 0,
                    D3DCOLOR_COLORVALUE(0.0f,1.0f,0.0f,1.0f),
                    0.5f );

    initialize(m_main, m_shadow, gameLayer, shadowLayer);
}

Block::~Block(void)
{
    if (m_main != NULL)
        delete m_main;
    if (m_shadow != NULL)
        delete m_shadow;
}

RECT Block::getRectangle()
{
    return m_rect;
}

bool Block::impact()
{
    if (--m_strength == 0)
        return true;

    m_main->setState(m_strength - 1);
    return false;
}
