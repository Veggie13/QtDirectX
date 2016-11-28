#pragma once

#include <d3dx9.h>

#include "drawable.h"


class Tile : public Drawable
{
public:
    Tile(LPCWSTR src, RECT rect, LPDIRECT3DDEVICE9 d3dd, float alpha = 1.0f);
    virtual ~Tile(void);

    virtual void draw();

private:
    LPDIRECT3DDEVICE9       m_d3dDevice;
    LPDIRECT3DVERTEXBUFFER9 m_vertexBuffer;
    LPDIRECT3DTEXTURE9      m_texture;
    RECT                    m_rect;
    float                   m_alpha;

    // A structure for our custom vertex type
    struct CUSTOM_VERTEX
    {
        FLOAT x, y, z, rhw; // The transformed position for the vertex
        DWORD color;        // The vertex color
        FLOAT u, v;
    };

};
