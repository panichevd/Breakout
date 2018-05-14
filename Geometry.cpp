#include <cmath>

#include "Geometry.h"

/*
 * We are searching for points present both on the ring and line segment.
 * If P (Px, Py) is such point then 3 following equalities are true
 * 1) Px = Ax + t * (Bx - Ax)
 * 2) Py = Ay + t * (By - Ay)
 * 3) (Px - Ox)^2 + (Py - Oy)^2 = r^2
 *
 * Where t is some parameter, 0 <= t <= 1. After inserting Px and Py
 * from 1) and 2) into 3) we get a quadratic equasion over t and we
 * need to solve it. First we find discriminant. If D < 0 means there is
 * no solutions. In other cases we find values for t.
 *
 * If t is not in [0, 1] then it means that the line has intersection
 * point(s), but they do not lie on this line segment.
 *
 * For simplicity lets calculate dx = Bx - Ax and dy = By - Ay
 *
 */
bool line_segment_intersects_ring(
        double Ax, double Ay,
        double Bx, double By,
        double Ox, double Oy, double r)
{
    double dx = Bx - Ax;
    double dy = By - Ay;

    double a = std::pow(dx, 2) + std::pow(dy, 2);
    double b = 2 * ((Ax - Ox) * dx + (Ay - Oy) * dy);
    double c = std::pow(Ax - Ox, 2) + std::pow(Ay - Oy, 2) - std::pow(r, 2);

    double D = std::pow(b, 2) - 4 * a * c;

    if (D < 0) {
        return false;
    }

    if (D == 0) {
        double t = -b / (2 *a);
        return (t >= 0) && (t <= 1);
    }

    double t1 = (-b + std::sqrt(D)) / (2 * a);
    if ((t1 >= 0) && (t1 <= 1)) {
        return true;
    }

    double t2 = (-b - std::sqrt(D)) / (2 * a);
    return (t2 >= 0) && (t2 <= 1);
}

/*
 * In our breakout game all the points and radiuses are integer,
 * so we can just check all the rings with integer r in [0, r0]
 */
bool line_segment_intersects_circle(
        double Ax, double Ay,
        double Bx, double By,
        double Ox, double Oy,
        double r0)
{
    for (double r = r0; r >=0; --r) {
        if (line_segment_intersects_ring(Ax, Ay, Bx, By, Ox, Oy, r))
            return true;
    }

    return false;
}

double calculate_triangle_area(
        double Ax, double Ay,
        double Bx, double By,
        double Cx, double Cy)
{
    return std::abs(Ax * (By - Cy) + Bx * (Cy - Ay) + Cx * (Ay - By)) / 2;
}

bool point_is_inside_rectangle(
        double Ax, double Ay,
        double Bx, double By,
        double Cx, double Cy,
        double Dx, double Dy,
        double Ox, double Oy)
{
    double rect_area =
            std::sqrt(std::pow(Bx - Ax, 2) +std::pow(By - Ay, 2)) *
            std::sqrt(std::pow(Dx - Ax, 2) + std::pow(Dy - Ay, 2));

    double triangles_area =
        calculate_triangle_area(Ax, Ay, Bx, By, Ox, Oy) +
        calculate_triangle_area(Ax, Ay, Dx, Dy, Ox, Oy) +
        calculate_triangle_area(Bx, By, Cx, Cy, Ox, Oy) +
        calculate_triangle_area(Cx, Cy, Dx, Dy, Ox, Oy);

    return rect_area == triangles_area;
}

/*
 * Rectangle intersects cirlce in 2 cases:
 * 1) Circle center lies inside of the rectangle
 * 2) One of the rectangle sides intersects with the circle
 */
bool rectangle_intersects_circle(
        double Ax, double Ay,
        double Bx, double By,
        double Cx, double Cy,
        double Dx, double Dy,
        double Ox, double Oy, double r)
{
    if (point_is_inside_rectangle(Ax, Ay, Bx, By, Cx, Cy, Dx, Dy, Ox, Oy)) {
        return true;
    }

    return
            line_segment_intersects_circle(Ax, Ay, Bx, By, Ox, Oy, r) ||
            line_segment_intersects_circle(Ax, Ay, Dx, Dy, Ox, Oy, r) ||
            line_segment_intersects_circle(Bx, By, Cx, Cy, Ox, Oy, r) ||
            line_segment_intersects_circle(Dx, Dy, Cx, Cy, Ox, Oy, r);
}

bool rectangle_intersects_circle(
        const QRect & rect,
        double Ox, double Oy, double r)
{
    return rectangle_intersects_circle(
                rect.x(),                rect.y(),
                rect.x(),                rect.y() + rect.height(),
                rect.x() + rect.width(), rect.y() + rect.height(),
                rect.x() + rect.width(), rect.y(),
                Ox, Oy, r);
}
