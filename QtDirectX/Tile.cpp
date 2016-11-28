#include <stdexcept>

#include "Tile.h"

// Our custom FVF, which describes our custom vertex structure
#define D3DFVF_CUSTOMVERTEX (D3DFVF_XYZRHW|D3DFVF_DIFFUSE|D3DFVF_TEX1)

Tile::Tile(LPCWSTR src, RECT rect, LPDIRECT3DDEVICE9 d3dd, float alpha)
:   m_d3dDevice(d3dd),
    m_vertexBuffer(NULL),
    m_texture(NULL),
    m_rect(rect),
    m_alpha(alpha)
{
    if (m_d3dDevice == NULL)
        return;

    HRESULT res;
    res = m_d3dDevice->CreateVertexBuffer(  sizeof(CUSTOM_VERTEX) * 4,
                                            NULL,
                                            D3DFVF_CUSTOMVERTEX,
                                            D3DPOOL_MANAGED,
                                            &m_vertexBuffer,
                                            NULL );
    if (FAILED(res))
    {
        m_d3dDevice = NULL;
        m_vertexBuffer = NULL;
        return;
    }

    D3DXIMAGE_INFO imgInfo;
    res = D3DXCreateTextureFromFileEx(  m_d3dDevice,
                                        src,
                                        (UINT) abs(m_rect.right - m_rect.left),
                                        (UINT) abs(m_rect.bottom - m_rect.top),
                                        1, 0,
                                        D3DFMT_UNKNOWN,
                                        D3DPOOL_MANAGED,
                                        D3DX_DEFAULT,
                                        D3DX_DEFAULT,
                                        D3DCOLOR_COLORVALUE(0.0f, 1.0f, 0.0f, 1.0f),
                                        &imgInfo,
                                        NULL,
                                        &m_texture );
    if (FAILED(res))
    {
        m_d3dDevice = NULL;
        m_vertexBuffer->Release();
        m_vertexBuffer = NULL;
        m_texture = NULL;
        return;
    }
}

Tile::~Tile(void)
{
    if (m_d3dDevice != NULL)
        m_d3dDevice->SetStreamSource(0, NULL, 0, 0);
    if (m_texture != NULL)
        m_texture->Release();
    if (m_vertexBuffer != NULL)
        m_vertexBuffer->Release();
}

void Tile::draw()
{
    HRESULT res;

    CUSTOM_VERTEX* vertices;
    res = m_vertexBuffer->Lock(0, 0, (void**)&vertices, NULL);
    if (FAILED(res))
        throw std::runtime_error("Could not lock vertex buffer.");

    vertices[0].color = D3DCOLOR_COLORVALUE(1.0f, 1.0f, 1.0f, m_alpha);
    vertices[0].x = (float) m_rect.left - 0.5f;
    vertices[0].y = (float) m_rect.top - 0.5f;
    vertices[0].z = 0.0f;
    vertices[0].rhw = 1.0f;
    vertices[0].u = 0.0f;
    vertices[0].v = 0.0f;

    vertices[1].color = D3DCOLOR_COLORVALUE(1.0f, 1.0f, 1.0f, m_alpha);
    vertices[1].x = (float) m_rect.right - 0.5f;
    vertices[1].y = (float) m_rect.top - 0.5f;
    vertices[1].z = 0.0f;
    vertices[1].rhw = 1.0f;
    vertices[1].u = 1.0f;
    vertices[1].v = 0.0f;

    vertices[2].color = D3DCOLOR_COLORVALUE(1.0f, 1.0f, 1.0f, m_alpha);
    vertices[2].x = (float) m_rect.right - 0.5f;
    vertices[2].y = (float) m_rect.bottom - 0.5f;
    vertices[2].z = 0.0f;
    vertices[2].rhw = 1.0f;
    vertices[2].u = 1.0f;
    vertices[2].v = 1.0f;

    vertices[3].color = D3DCOLOR_COLORVALUE(1.0f, 1.0f, 1.0f, m_alpha);
    vertices[3].x = (float) m_rect.left - 0.5f;
    vertices[3].y = (float) m_rect.bottom - 0.5f;
    vertices[3].z = 0.0f;
    vertices[3].rhw = 1.0f;
    vertices[3].u = 0.0f;
    vertices[3].v = 1.0f;

    m_vertexBuffer->Unlock();

    res = m_d3dDevice->SetStreamSource(0, m_vertexBuffer, 0, sizeof(CUSTOM_VERTEX));
    if (FAILED(res))
        throw std::runtime_error("Could not set the stream source.");

    m_d3dDevice->SetTexture(0, m_texture);
    m_d3dDevice->DrawPrimitive(D3DPT_TRIANGLEFAN, 0, 2);
}
