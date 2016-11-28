#include "Layer.h"
#include "Sprite.h"

#include "GameObject.h"


const float DELTA = 8;

GameObject::GameObject()
:   m_gameLayer(NULL),
    m_shadowLayer(NULL),
    m_main(NULL),
    m_shadow(NULL),
    m_X(0.0f), m_Y(0.0f)
{
}

GameObject::~GameObject(void)
{
    if (m_gameLayer)
        m_gameLayer->removeDrawable(m_main);
    if (m_shadowLayer)
        m_shadowLayer->removeDrawable(m_shadow);
}

void GameObject::setPosition(float x, float y)
{
    m_X = x;
    m_Y = y;

    m_main->setPosition(x + DELTA, y + DELTA);
    m_shadow->setPosition(x + 2*DELTA, y + 2*DELTA);
}

float GameObject::getX()
{
    return m_X;
}

float GameObject::getY()
{
    return m_Y;
}

void GameObject::initialize(Sprite* gameSprite, Sprite* shadowSprite, Layer* gameLayer, Layer* shadowLayer)
{
    m_gameLayer = gameLayer;
    m_shadowLayer = shadowLayer;
    m_main = gameSprite;
    m_shadow = shadowSprite;

    m_gameLayer->addDrawable(m_main);
    m_shadowLayer->addDrawable(m_shadow);
}

void GameObject::setGameSprite(Sprite* gameSprite)
{
    if (m_gameLayer != NULL && m_main != NULL)
    {
        m_gameLayer->removeDrawable(m_main);
    }

    m_main = gameSprite;

    if (m_gameLayer != NULL && m_main != NULL)
    {
        m_gameLayer->addDrawable(m_main);
    }
}

void GameObject::setShadowSprite(Sprite* shadowSprite)
{
    if (m_shadowLayer != NULL && m_shadow != NULL)
    {
        m_shadowLayer->removeDrawable(m_shadow);
    }

    m_shadow = shadowSprite;

    if (m_shadowLayer != NULL && m_shadow != NULL)
    {
        m_shadowLayer->addDrawable(m_shadow);
    }
}

void GameObject::setGameLayer(Layer* gameLayer)
{
    if (m_gameLayer != NULL && m_main != NULL)
    {
        m_gameLayer->removeDrawable(m_main);
    }

    m_gameLayer = gameLayer;

    if (m_gameLayer != NULL && m_main != NULL)
    {
        m_gameLayer->addDrawable(m_main);
    }
}

void GameObject::setShadowLayer(Layer* shadowLayer)
{
    if (m_shadowLayer != NULL && m_shadow != NULL)
    {
        m_shadowLayer->removeDrawable(m_shadow);
    }

    m_shadowLayer = shadowLayer;

    if (m_shadowLayer != NULL && m_shadow != NULL)
    {
        m_shadowLayer->addDrawable(m_shadow);
    }
}

Sprite* GameObject::gameSprite()
{
    return m_main;
}

Sprite* GameObject::shadowSprite()
{
    return m_shadow;
}
