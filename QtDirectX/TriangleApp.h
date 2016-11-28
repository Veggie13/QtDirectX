#pragma once
#include "directxwidget.h"

#include <QTime>
#include <QTimer>


class Arena;
class Tile;

class TriangleApp : public DirectXWidget
{
    Q_OBJECT

public:
    TriangleApp(void);
    virtual ~TriangleApp(void);

protected:
    virtual void showEvent(QShowEvent*);
    virtual void mouseMoveEvent(QMouseEvent*);

protected slots:
    virtual void renderD3D();

private:
    LPDIRECT3D9  m_pD3D;
    LPD3DXSPRITE m_pSprite;
    LPDIRECT3DTEXTURE9 m_pSpriteTex;

    Tile* m_pTile;
    Tile* m_pBars;
    Tile* m_pShad;

    QTimer  m_timer;
    QTime   m_counter;

    float m_angle;

    D3DXMATRIX  m_worldMat;
    D3DXMATRIX  m_projectionMat;
    D3DXMATRIX  m_viewMat;

    D3DXVECTOR3 m_eyeVec;
    D3DXVECTOR3 m_lookVec;
    D3DXVECTOR3 m_upVec;

    Arena* m_arena;

};
