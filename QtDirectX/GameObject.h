#pragma once

class Layer;
class Sprite;

class GameObject
{
public:
    GameObject();
    virtual ~GameObject(void);

    void setPosition(float x, float y);
    float getX();
    float getY();

protected:
    void initialize(Sprite* gameSprite, Sprite* shadowSprite, Layer* gameLayer, Layer* shadowLayer);
    
    void setGameSprite(Sprite* gameSprite);
    void setShadowSprite(Sprite* shadowSprite);
    
    void setGameLayer(Layer* gameLayer);
    void setShadowLayer(Layer* shadowLayer);

    Sprite* gameSprite();
    Sprite* shadowSprite();

private:
    Layer*  m_gameLayer;
    Layer*  m_shadowLayer;

    Sprite* m_main;
    Sprite* m_shadow;

    float m_X, m_Y;

};
