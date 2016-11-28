#include "Layer.h"

Layer::Layer(void)
{ }

Layer::~Layer(void)
{ }

void Layer::draw()
{
    for ( QList<Drawable*>::iterator it = m_contents.begin();
          it != m_contents.end();
          it++ )
    {
        (*it)->draw();
    }
}

void Layer::addDrawable(Drawable* item)
{
    m_contents.append(item);
}

void Layer::removeDrawable(Drawable* item)
{
    int index = m_contents.lastIndexOf(item);
    if (index != -1)
        m_contents.removeAt(index);
}
