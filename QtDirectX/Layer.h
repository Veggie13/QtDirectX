#pragma once
#include <QList>

#include "drawable.h"


class Layer : public Drawable
{
public:
    Layer(void);
    virtual ~Layer(void);

    virtual void draw();

    void addDrawable(Drawable* item);
    void removeDrawable(Drawable* item);

private:
    QList<Drawable*> m_contents;

};
