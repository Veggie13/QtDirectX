#include <QMouseEvent>
#include <QPoint>

#include "Arena.h"
#include "Layer.h"
#include "Tile.h"

#include "TriangleApp.h"

// Our custom FVF, which describes our custom vertex structure
#define D3DFVF_CUSTOMVERTEX (D3DFVF_XYZRHW|D3DFVF_DIFFUSE|D3DFVF_TEX1)

const int HEIGHT = 160;
const int WIDTH = 240;

TriangleApp::TriangleApp(void)
:   m_pD3D(NULL),
    m_pSprite(NULL),
    m_pSpriteTex(NULL),
    m_pTile(NULL),
    m_pBars(NULL),
    m_pShad(NULL),
    m_arena(NULL),
    m_angle(0.0f)
{
    resize(WIDTH, HEIGHT);
    setMouseTracking(true);

    // Create the D3D object.
    m_pD3D = Direct3DCreate9( D3D_SDK_VERSION );

    initialize(m_pD3D);

    D3DXIMAGE_INFO d3dxImageInfo;
    HRESULT res = S_OK;
    res = D3DXCreateTextureFromFileEx( getDirect3dDevice(),
                             L"tiles\\side scroll.bmp",
                             240, // I had to set width manually. D3DPOOL_DEFAULT works for textures but causes problems for D3DXSPRITE.
                             160, // I had to set height manually. D3DPOOL_DEFAULT works for textures but causes problems for D3DXSPRITE.
                             1,   // Don't create mip-maps when you plan on using D3DXSPRITE. It throws off the pixel math for sprite animation.
                             0, //D3DPOOL_DEFAULT,
                             D3DFMT_UNKNOWN,
                             D3DPOOL_MANAGED, //D3DPOOL_DEFAULT,
                             D3DX_DEFAULT,
                             D3DX_DEFAULT,
                             D3DCOLOR_COLORVALUE(0.0f,1.0f,0.0f,1.0f),
                             &d3dxImageInfo,
                             NULL,
                             &m_pSpriteTex );
    D3DXCreateSprite(getDirect3dDevice(), &m_pSprite);

    m_eyeVec = D3DXVECTOR3(0,0,-103);
    m_lookVec = D3DXVECTOR3(0,0,0);
    m_upVec = D3DXVECTOR3(0,1,0);

    D3DXMatrixLookAtLH( &m_viewMat,
                        &m_eyeVec,
                        &m_lookVec,
                        &m_upVec );
    getDirect3dDevice()->SetTransform(D3DTS_VIEW, &m_viewMat);

    D3DXMatrixOrthoLH( &m_projectionMat,
                       (float) WIDTH,
                       (float) HEIGHT,
                       0.1,
                       200 );
    getDirect3dDevice()->SetTransform(D3DTS_PROJECTION, &m_projectionMat);

    getDirect3dDevice()->SetVertexShader(NULL);
    getDirect3dDevice()->SetFVF(D3DFVF_CUSTOMVERTEX);

    getDirect3dDevice()->SetRenderState( D3DRS_LIGHTING, FALSE );
    getDirect3dDevice()->SetRenderState( D3DRS_CULLMODE, D3DCULL_NONE );
    getDirect3dDevice()->SetRenderState( D3DRS_ALPHABLENDENABLE, TRUE );
    getDirect3dDevice()->SetRenderState( D3DRS_SRCBLEND, D3DBLEND_SRCALPHA );
    getDirect3dDevice()->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA );
    getDirect3dDevice()->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);

    RECT tileRect;
    tileRect.top = 0;
    tileRect.left = 0;
    tileRect.bottom = HEIGHT;
    tileRect.right = WIDTH;
    m_pTile = new Tile(L"tiles\\background - bitmap.bmp", tileRect, getDirect3dDevice());
    m_pBars = new Tile(L"tiles\\bars.bmp", tileRect, getDirect3dDevice());
    m_pShad = new Tile(L"tiles\\shadow main.bmp", tileRect, getDirect3dDevice(), 0.5f);

    Arena::Spec arenaSpec;
    arenaSpec.arenaSize = QSize(176,176);
    arenaSpec.blockSize = QSize(16,8);
    arenaSpec.blockTypes.append(Arena::Spec::BlockType());
    arenaSpec.blockTypes[0].name = QString("silver");
    arenaSpec.blockTypes[0].points = 50000;
    arenaSpec.blockTypes[0].strength = 2;
    arenaSpec.blockTypes[0].source = QString("tiles\\block silver.bmp");
    arenaSpec.blockTypes[0].shadowSource = QString("tiles\\shadow block.bmp");
    for (unsigned int x = 0; x < Arena::COLS; x++)
    {
        arenaSpec.blockMap[x][0] = 100;
        arenaSpec.blockMap[x][1] = 100;
        for (unsigned int y = 2; y < Arena::ROWS; y++)
            arenaSpec.blockMap[x][y] = 0;
    }
    arenaSpec.ballStartPosition = QPair<float, float>(20.0f, 100.0f);
    arenaSpec.ballStartVelocity = QPair<float, float>(30.0f, -30.0f);
    m_arena = new Arena(arenaSpec, getDirect3dDevice());

    m_arena->m_gameLayer->addDrawable(m_pBars);
    m_arena->m_shadowLayer->addDrawable(m_pShad);

    connect(&m_timer, SIGNAL( timeout() ), this, SLOT( renderD3D() ));
    m_timer.setSingleShot(false);
    m_timer.start( (int)(1000.0/60.0) );
    m_counter.start();
}

TriangleApp::~TriangleApp(void)
{
    m_timer.stop();

    if (m_arena != NULL)
        delete m_arena;

    if (m_pTile != NULL)
        delete m_pTile;
    if (m_pShad != NULL)
        delete m_pShad;
    if (m_pBars != NULL)
        delete m_pBars;

    if (m_pSpriteTex != NULL)
        m_pSpriteTex->Release();

    if (m_pSprite != NULL)
        m_pSprite->Release();

    if (m_pD3D != NULL)
        m_pD3D->Release();
}

void TriangleApp::showEvent(QShowEvent*)
{
    //renderD3D();
}

void TriangleApp::mouseMoveEvent(QMouseEvent* evt)
{
    QPoint mousePos = evt->pos();
    float x = (float)WIDTH * (float)mousePos.x() / (float)width();
    m_arena->setPaddlePosition(x);
}

void TriangleApp::renderD3D()
{
    static unsigned int offset = 0;

    if (isHidden())
        return;

    float dt = (float)m_counter.restart() / 1000.0f;
    //this->setWindowTitle(QString("ARKANOID (%1 fps)").arg((double)1.0f/dt, 0, (char)103, 3));
    if (!m_arena->recalc(dt))
        m_arena->restart();

    // Clear the backbuffer to a blue color
    getDirect3dDevice()->Clear( 0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB( 0, 0, 0 ), 1.0f, 0 );

    // Begin the scene
    if( SUCCEEDED( getDirect3dDevice()->BeginScene() ) )
    {
        m_pTile->draw();
        m_arena->m_shadowLayer->draw();
        m_arena->m_gameLayer->draw();

        RECT rcSource;
        rcSource.top = offset/2;
        rcSource.left = offset/2;
        rcSource.bottom = 96 + offset/2;
        rcSource.right = 48 + offset/2;

        offset = (offset + 1) % 64;

        D3DXVECTOR3 vCenter( 0.0f, 0.0f, 0.0f );
        D3DXVECTOR3 vPosition1( 192.0f,  0.0f, 0.0f );
        D3DXVECTOR3 vPosition2( 192.0f, 96.0f, 0.0f );

        m_pSprite->Begin( D3DXSPRITE_ALPHABLEND );
        {
            m_pSprite->Draw( m_pSpriteTex,
                             &rcSource,
                             &vCenter,
                             &vPosition1,
                             D3DCOLOR_COLORVALUE(1.0f,1.0f,1.0f,1.0f) );
            m_pSprite->Draw( m_pSpriteTex,
                             &rcSource,
                             &vCenter,
                             &vPosition2,
                             D3DCOLOR_COLORVALUE(1.0f,1.0f,1.0f,1.0f) );
        }
        m_pSprite->End();

        // End the scene
        getDirect3dDevice()->EndScene();
    }

    // Present the backbuffer contents to the display
    getDirect3dDevice()->Present( NULL, NULL, NULL, NULL );
}
