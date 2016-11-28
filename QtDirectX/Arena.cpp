#include "Ball.h"
#include "Block.h"
#include "Layer.h"
#include "Paddle.h"

#include "Arena.h"


const float F_DELTA = 1e-5;
const float PADDLE_Y = 140.0f;

Arena::Arena(const Spec& arenaSpec, LPDIRECT3DDEVICE9 d3dd)
:   m_gameLayer(NULL),
    m_shadowLayer(NULL),
    m_ball(NULL),
    m_paddle(NULL),
    m_blockSize(arenaSpec.blockSize),
    m_ballStartPosition(arenaSpec.ballStartPosition),
    m_ballStartVelocity(arenaSpec.ballStartVelocity)
{
    m_gameLayer = new Layer();
    m_shadowLayer = new Layer();

    m_ball = new Ball(m_gameLayer, m_shadowLayer, d3dd);

    m_paddle = new Paddle(m_gameLayer, m_shadowLayer, d3dd);
    m_paddle->setPosition(50, PADDLE_Y);

    for (unsigned int x = 0; x < COLS; x++)
    {
        for (unsigned int y = 0; y < ROWS; y++)
        {
            float fX = (float)(x * arenaSpec.blockSize.width());
            float fY = (float)(y * arenaSpec.blockSize.height());
            unsigned int blockType = arenaSpec.blockMap[x][y];
            if (blockType >= arenaSpec.blockTypes.count())
            {
                m_blocks[x][y] = NULL;
                continue;
            }

            m_blocks[x][y] = new Block( arenaSpec.blockTypes.at(blockType),
                                        arenaSpec.blockSize,
                                        m_gameLayer, m_shadowLayer,
                                        d3dd );
            m_blocks[x][y]->setPosition(fX, fY);
        }
        m_blocks[x][ROWS] = NULL;
    }

    restart();
}

Arena::~Arena(void)
{
    delete m_ball;
    delete m_paddle;

    for (unsigned int x = 0; x < COLS; x++)
    for (unsigned int y = 0; y < ROWS; y++)
    {
        if (m_blocks[x][y] != NULL)
            delete m_blocks[x][y];
    }

    delete m_gameLayer;
    delete m_shadowLayer;
}

void Arena::restart()
{
    m_ball->setPosition(m_ballStartPosition.first, m_ballStartPosition.second);
    m_ball->setVelocity(m_ballStartVelocity.first, m_ballStartVelocity.second);
}

void Arena::setPaddlePosition(float x)
{
    x -= 8.0f;
    QRect paddleRect = m_paddle->getRectangle();
    float paddleWidth = paddleRect.width();
    if (x < paddleWidth / 2.0f)
        x = paddleWidth / 2.0f;
    else if ((float)COLS * m_blockSize.width() - x < paddleWidth / 2.0f)
        x = (float)COLS * m_blockSize.width() - paddleWidth / 2.0f;

    m_paddle->setPosition(x, PADDLE_Y);
}

bool Arena::recalc(float dt)
{
    float x1 = m_ball->getX();
    float y1 = m_ball->getY();
    float x2 = m_ball->getTargetX(dt);
    float y2 = m_ball->getTargetY(dt);
    float dX = m_ball->getXVelocity();
    float dY = m_ball->getYVelocity();

    bool horizontalReflection;
    bool verticalReflection;
    static bool that = false;

    do
    {
        horizontalReflection = false;
        verticalReflection = false;

        QList<QPair<int, int> > blocks =
            getCollisionCandidates( x1, y1, x2, y2,
                                    m_blockSize.width(),
                                    m_blockSize.height(),
                                    Ball::WIDTH,
                                    Ball::HEIGHT );

        QPair<int, int> coord, coordLast;
        bool match = false;
        int count = 0;
        for ( QList<QPair<int, int> >::iterator it = blocks.begin();
              it != blocks.end();
              it++ )
        {
            coordLast = coord;
            coord = (*it);
            count++;
            if ( m_blocks[coord.first][coord.second] != NULL )
            {
                if ( m_blocks[coord.first][coord.second]->impact() )
                {
                    delete m_blocks[coord.first][coord.second];
                    m_blocks[coord.first][coord.second] = NULL;
                }
                match = true;
                break;
            }
        }

        float hRAxis, vRAxis;
        if (!match)
        {
            if (x2 < 0.0f)
            {
                horizontalReflection = true;
                hRAxis = 0.0f;
            }
            else if (x2 > COLS * m_blockSize.width() - Ball::WIDTH)
            {
                horizontalReflection = true;
                hRAxis = COLS * m_blockSize.width() - Ball::WIDTH;
            }
            if (y2 < 0.0f)
            {
                verticalReflection = true;
                vRAxis = 0.0f;
            }
            else if (
                QRectF( m_paddle->getRectangle() )
                    .intersects( QRectF(
                        (qreal)x2,
                        (qreal)y2,
                        (qreal)Ball::WIDTH,
                        (qreal)Ball::HEIGHT
                        ) )
                    )
            {
                QRect paddleRect = m_paddle->getRectangle();
                float x1i = x1 + (dX > 0 ? Ball::WIDTH : 0);
                float x2i = x2 + (dX > 0 ? Ball::WIDTH : 0);
                x2i = x1i + (x2i - x1i) * (PADDLE_Y - y1 - Ball::HEIGHT) / (y2 - y1);
                if (x2i > paddleRect.left() && x2i < paddleRect.right())
                {
                    verticalReflection = true;
                    vRAxis = PADDLE_Y - Ball::HEIGHT;
                    if (that)
                        that = false;
                }
                else
                {
                    x2i = x2 + (dX > 0 ? Ball::WIDTH : 0);
                    if (x2i > paddleRect.left() && x2i < paddleRect.right())
                    {
                        horizontalReflection = true;
                        hRAxis = (dX > 0) ?
                            ( paddleRect.left() - Ball::WIDTH ) :
                            ( paddleRect.right() );
                        that = true;
                    }
                }
            }
        }
        else
        {
            if (coordLast.first < coord.first)
            {
                horizontalReflection = true;
                hRAxis = coord.first * m_blockSize.width() - Ball::WIDTH;
            }
            else if (coordLast.first > coord.first)
            {
                horizontalReflection = true;
                hRAxis = (coord.first + 1) * m_blockSize.width();
            }
            if (coordLast.second < coord.second)
            {
                verticalReflection = true;
                vRAxis = coord.second * m_blockSize.height() - Ball::HEIGHT;
            }
            else if (coordLast.second > coord.second)
            {
                verticalReflection = true;
                vRAxis = (coord.second + 1) * m_blockSize.height();
            }
        }

        if (verticalReflection)
        {
            y1 = 2.0f * vRAxis - y1;
            y2 = 2.0f * vRAxis - y2;

            x1 = x1 + (x2 - x1) * (vRAxis - y2) / (y1 - y2);
            y1 = vRAxis;

            dY *= -1.0f;
        }
        if (horizontalReflection)
        {
            x1 = 2.0f * hRAxis - x1;
            x2 = 2.0f * hRAxis - x2;

            y1 = y1 + (y2 - y1) * (hRAxis - x2) / (x1 - x2);
            x1 = hRAxis;

            dX *= -1.0f;
        }
    } while (verticalReflection || horizontalReflection);

    m_ball->setPosition(x2, y2);
    m_ball->setVelocity(dX, dY);

    return !(y2 > 180);
}

QList<QPair<int, int> >
Arena::getCollisionCandidates(  float x1, float y1, float x2, float y2,
                                unsigned int blockWidth, unsigned int blockHeight,
                                unsigned int ballWidth,  unsigned int ballHeight )
{
    float beginCoord_X;
    float beginCoord_Y;
    float endCoord_X;
    float endCoord_Y;
    if (x2 > x1)
    {
        beginCoord_X = (x1 + (float)ballWidth) / (float)blockWidth;
        endCoord_X =   (x2 + (float)ballWidth) / (float)blockWidth;
    }
    else
    {
        beginCoord_X = x1 / (float)blockWidth;
        endCoord_X =   x2 / (float)blockWidth;
    }
    if (y2 > y1)
    {
        beginCoord_Y = (y1 + (float)ballHeight) / (float)blockHeight;
        endCoord_Y =   (y2 + (float)ballHeight) / (float)blockHeight;
    }
    else
    {
        beginCoord_Y = y1 / (float)blockHeight;
        endCoord_Y =   y2 / (float)blockHeight;
    }

    if (endCoord_X < 0.0f)
    {
        endCoord_Y =
            beginCoord_Y + beginCoord_X * (endCoord_Y - beginCoord_Y) / (beginCoord_X - endCoord_X);
        endCoord_X = 0.0f;
    }
    else if ((int)endCoord_X >= COLS)
    {
        float colCoord = (float)COLS - F_DELTA;
        endCoord_Y =
            beginCoord_Y + (colCoord - beginCoord_X) * (endCoord_Y - beginCoord_Y) / (beginCoord_X - endCoord_X);
        endCoord_X = colCoord;
    }

    if (endCoord_Y < 0.0f)
    {
        endCoord_X =
            beginCoord_X + beginCoord_Y * (endCoord_X - beginCoord_X) / (beginCoord_Y - endCoord_Y);
        endCoord_Y = 0.0f;
    }

    QList<QPair<int, int> > result;
    if ( (int)beginCoord_Y >= ROWS && (int)endCoord_Y >= ROWS )
        return result;

    if ((int)beginCoord_X == (int)endCoord_X)
    {
        int dY = (endCoord_Y > beginCoord_Y)?(1):(-1);
        for (int y = beginCoord_Y; y != (int)endCoord_Y + dY; y += dY)
        {
            result.append(QPair<int, int>((int)beginCoord_X, y));
        }
    }
    else if ((int)beginCoord_Y == (int)endCoord_Y)
    {
        int dX = (endCoord_X > beginCoord_X)?(1):(-1);
        for (int x = beginCoord_X; x != (int)endCoord_X + dX; x += dX)
        {
            result.append(QPair<int, int>(x, (int)beginCoord_Y));
        }
    }
    else
    {
        int dX = (endCoord_X > beginCoord_X)?(1):(-1);
        int dY = (endCoord_Y > beginCoord_Y)?(1):(-1);

        int x = beginCoord_X;
        for (int y = beginCoord_Y; y != (int)endCoord_Y; y += dY)
        {
            float switchX = endCoord_X
                          + (beginCoord_X - endCoord_X)
                          * (y + 1 - endCoord_Y)
                          / (beginCoord_Y - endCoord_Y);
            for (; (x < dX*switchX) && (x != (int)endCoord_X + dX); x += dX)
            {
                result.append(QPair<int, int>(x, y));
            }
            x -= dX;
        }
    }

    return result;
}
