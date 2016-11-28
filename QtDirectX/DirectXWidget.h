#pragma once

#include <QWidget>

#include <d3dx9.h>


class DirectXWidget : public QWidget
{
public:
    DirectXWidget(QWidget* parent = NULL, Qt::WindowFlags f = 0);
    virtual ~DirectXWidget(void);

    void initialize(LPDIRECT3D9 pD3D);

    virtual QPaintEngine* paintEngine() const;

    LPDIRECT3DDEVICE9 getDirect3dDevice();

private:
    LPDIRECT3DDEVICE9 m_pD3DD;

};
