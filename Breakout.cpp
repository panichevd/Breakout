#include <QPainter>

#include "Breakout.h"
#include "Geometry.h"

Breakout::Breakout(QWidget *parent) :
    QWidget(parent),
    m_in_game(true),
    m_won(false),
    m_direction(Up),
    m_paddle_direction(None),
    m_paddle(250, 370, 100, 20),
    m_ball_x(290),
    m_ball_y(350),
    m_ball_radius(20)
{
    setStyleSheet("background-color:black;");
    resize(WIDTH, HEIGHT);

    for (int j = 30; j <= 150; j += 30) {
        for (int i = 60; i <= 460; i+= 100) {
            m_bricks.push_back(QRect(i, j, 80, 20));
        }
    }

    m_timer_id = startTimer(DELAY);
}

void Breakout::paintEvent(QPaintEvent *e)
{
    Q_UNUSED(e);

    if (m_in_game) {
        game_step();
    } else if (m_won) {
        game_finished("You won!");
    } else {
        game_finished("Game over!");
    }
}

void Breakout::game_step()
{
    QPainter painter(this);

    painter.fillRect(m_paddle, Qt::darkCyan);

    painter.setBrush(Qt::red);
    painter.drawEllipse(m_ball_x, m_ball_y, m_ball_radius, m_ball_radius);

    painter.setBrush(Qt::darkGreen);
    for (const auto & brick : m_bricks) {
        painter.drawRect(brick);
    }
}

void Breakout::game_finished(const QString & message)
{
    QPainter painter(this);

    painter.setPen(Qt::darkGreen);

    QFont font("Courier", 15, QFont::DemiBold);
    QFontMetrics fm(font);
    int textWidth = fm.width(message);

    painter.setFont(font);
    int h = height();
    int w = width();

    painter.translate(QPoint(w/2, h/2));
    painter.drawText(-textWidth/2, 0, message);
}

void Breakout::move_ball()
{
    if (m_direction == UpLeft) {
        m_ball_y -= SPEED / 2;
        m_ball_x -= SPEED / 2;
    } else if (m_direction == Up) {
        m_ball_y -= SPEED;
    } else if (m_direction == UpRight) {
        m_ball_y -= SPEED / 2;
        m_ball_x += SPEED / 2;
    } else if (m_direction == DownLeft) {
        m_ball_y += SPEED / 2;
        m_ball_x -= SPEED / 2;
    } else if (m_direction == Down) {
        m_ball_y += SPEED;
    } else {
        m_ball_y += SPEED / 2;
        m_ball_x += SPEED / 2;
    }
}

void Breakout::move_paddle()
{
    if (m_paddle_direction == Left) {
        m_paddle.moveLeft(m_paddle.x() - SPEED);
    } else if (m_paddle_direction == Right) {
        m_paddle.moveLeft(m_paddle.x() + SPEED);
    }
    m_paddle_direction = None;
}

void Breakout::switch_direction(const QRect & rect)
{
    QRect left_third(rect.x(), rect.y(), rect.width()/3, rect.height());
    if (rectangle_intersects_circle(left_third, m_ball_x, m_ball_y, m_ball_radius)) {
        if (m_direction == Up || m_direction == UpLeft || m_direction == UpRight) {
            m_direction = DownLeft;
        } else if (m_direction == Down || m_direction == DownLeft || m_direction == DownRight) {
            m_direction = UpLeft;
        }
        return;
    }

    QRect middle_third(rect.x() + rect.width()/3, rect.y(), rect.width()/3, rect.height());
    if (rectangle_intersects_circle(middle_third, m_ball_x, m_ball_y, m_ball_radius)) {
        if (m_direction == Up || m_direction == UpLeft || m_direction == UpRight) {
            m_direction = Down;
        } else if (m_direction == Down || m_direction == DownLeft || m_direction == DownRight) {
            m_direction = Up;
        }
        return;
    }

    QRect right_third(rect.x() + (2*rect.width())/3, rect.y(), rect.width()/3, rect.height());
    if (rectangle_intersects_circle(right_third, m_ball_x, m_ball_y, m_ball_radius)) {
        if (m_direction == Up || m_direction == UpLeft || m_direction == UpRight) {
            m_direction = DownRight;
        } else if (m_direction == Down || m_direction == DownLeft || m_direction == DownRight) {
            m_direction = UpRight;
        }
        return;
    }
}

void Breakout::bounce_top()
{
    if (m_ball_x < WIDTH/3) {
        m_direction = DownRight;
    } else if (m_ball_x < 2*(WIDTH/3)) {
        if (m_direction == UpLeft) {
            m_direction = DownLeft;
        } else if (m_direction == Up) {
            m_direction = Down;
        } else {
            m_direction = DownRight;
        }
    } else {
        m_direction = DownLeft;
    }
}

void Breakout::bounce_right()
{
    if (m_direction == UpRight) {
        m_direction = UpLeft;
    } else if (m_direction == DownRight) {
        m_direction = DownLeft;
    } else {
        Q_ASSERT(false);
    }
}

void Breakout::bounce_left()
{
    if (m_direction == UpLeft) {
        m_direction = UpRight;
    } else if (m_direction == DownLeft) {
        m_direction = DownRight;
    } else {
        Q_ASSERT(false);
    }
}

void Breakout::check_boundaries()
{
    for (auto it = m_bricks.begin(); it != m_bricks.end();) {
        if (rectangle_intersects_circle(*it, m_ball_x, m_ball_y, m_ball_radius))
        {
            switch_direction(*it);
            it = m_bricks.erase(it);
            return;
        } else {
            ++it;
        }
    }

    if (rectangle_intersects_circle(m_paddle, m_ball_x, m_ball_y, m_ball_radius)) {
        switch_direction(m_paddle);
        return;
    }

    if (m_ball_y >= HEIGHT) {
        m_in_game = false;
        killTimer(m_timer_id);
        return;
    } else if (m_ball_y <= 0) {
        bounce_top();
    } else if (m_ball_x >= WIDTH) {
        bounce_right();
    } else if (m_ball_x <= 0) {
        bounce_left();
    }
}

void Breakout::timerEvent(QTimerEvent *e)
{
    Q_UNUSED(e);

    if (m_in_game) {
        move_paddle();
        move_ball();
        check_boundaries();

        if (m_bricks.empty()) {
            m_in_game = false;
            killTimer(m_timer_id);
            m_won = true;
        }
    }

    repaint();
}

void Breakout::keyPressEvent(QKeyEvent *e)
{
    int key = e->key();

    if (key == Qt::Key_Left) {
        m_paddle_direction = Left;
    } else if (key == Qt::Key_Right) {
        m_paddle_direction = Right;
    }

    QWidget::keyPressEvent(e);
}
