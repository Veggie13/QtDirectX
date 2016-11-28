#pragma once

#include <QList>
#include <QPair>
#include <QSize>
#include <QString>

#include <d3dx9.h>


class Ball;
class Block;
class Layer;
class Paddle;

class Arena
{
public:
    static const unsigned int COLS = 11;
    static const unsigned int ROWS = 8;

    struct Spec
    {
        struct BlockType
        {
            QString name;
            QString source;
            QString shadowSource;
            unsigned int strength;
            unsigned int points;
        };

        QSize arenaSize;
        QSize blockSize;
        QList<BlockType> blockTypes;
        unsigned int blockMap[COLS][ROWS];

        QPair<float, float> ballStartPosition;
        QPair<float, float> ballStartVelocity;
    };

    Arena(const Spec& arenaSpec, LPDIRECT3DDEVICE9 d3dd);
    ~Arena(void);

    bool recalc(float dt);
    void restart();

    void setPaddlePosition(float x);

    Layer* m_gameLayer;
    Layer* m_shadowLayer;

private:
    static QList<QPair<int, int> >
        getCollisionCandidates( float x1, float y1, float x2, float y2,
                                unsigned int blockWidth, unsigned int blockHeight,
                                unsigned int ballWidth,  unsigned int ballHeight );

    Block*  m_blocks[COLS][ROWS+1];
    Ball*   m_ball;
    Paddle* m_paddle;

    QSize m_blockSize;

    QPair<float, float> m_ballStartPosition;
    QPair<float, float> m_ballStartVelocity;
    
};
