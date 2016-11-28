#include "DirectXWidget.h"

DirectXWidget::DirectXWidget(QWidget* parent,
                             Qt::WindowFlags f)
:   QWidget(parent, f),
    m_pD3DD(NULL)
{
}

void DirectXWidget::initialize(LPDIRECT3D9 pD3D)
{
    if (pD3D != NULL)
    {
        D3DCAPS9 d3dCaps;
        pD3D->GetDeviceCaps( D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, &d3dCaps );

        D3DDISPLAYMODE d3ddm;
        pD3D->GetAdapterDisplayMode( D3DADAPTER_DEFAULT, &d3ddm );

        // Set up the structure used to create the D3DDevice
        D3DPRESENT_PARAMETERS d3dpp;
        ZeroMemory( &d3dpp, sizeof( d3dpp ) );
        d3dpp.Windowed = TRUE;
        d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
        d3dpp.BackBufferFormat = d3ddm.Format;
        d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;
        d3dpp.Flags = D3DPRESENTFLAG_LOCKABLE_BACKBUFFER;

        // Create the D3DDevice
        if ( FAILED( pD3D->CreateDevice( D3DADAPTER_DEFAULT,
                                         D3DDEVTYPE_HAL,
                                         (HWND)winId(),
                                         D3DCREATE_SOFTWARE_VERTEXPROCESSING,
                                         &d3dpp,
                                         &m_pD3DD
                                       )
                   )
           )
        {
            m_pD3DD = NULL;
            return;
        }
    }
}

DirectXWidget::~DirectXWidget(void)
{
    if (m_pD3DD != NULL)
        m_pD3DD->Release();
}

LPDIRECT3DDEVICE9 DirectXWidget::getDirect3dDevice()
{
    return m_pD3DD;
}

QPaintEngine* DirectXWidget::paintEngine() const
{
    return NULL;
}
