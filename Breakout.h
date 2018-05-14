#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QRect>
#include <QKeyEvent>

class Breakout : public QWidget
{
    Q_OBJECT

public:
    Breakout(QWidget *parent = 0);
    ~Breakout() = default;

protected:
    void paintEvent(QPaintEvent *);
    void timerEvent(QTimerEvent *);
    void keyPressEvent(QKeyEvent *);

private:
    enum Direction {
        UpLeft,
        Up,
        UpRight,
        DownLeft,
        Down,
        DownRight
    };

    enum PaddleDirection {
        None,
        Left,
        Right
    };

private:
    void game_step();
    void game_finished(const QString & message);

    void move_ball();
    void move_paddle();
    void switch_direction(const QRect & rect);
    void bounce_top();
    void bounce_right();
    void bounce_left();
    void check_boundaries();

private:
    static const int WIDTH = 600;
    static const int HEIGHT = 400;
    static const int SPEED = 15;
    static const int DELAY = 70;

private:
    bool m_in_game;
    bool m_won;
    Direction m_direction;
    PaddleDirection m_paddle_direction;

    QRect m_paddle;
    std::vector<QRect> m_bricks;
    int m_ball_x;
    int m_ball_y;
    int m_ball_radius;

    int m_timer_id;
};

#endif // WIDGET_H
