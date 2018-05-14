#pragma once

#include <QRect>

bool rectangle_intersects_circle(
        double Ax, double Ay,
        double Bx, double By,
        double Cx, double Cy,
        double Dx, double Dy,
        double Ox, double Oy, double r);

bool rectangle_intersects_circle(
        const QRect & rect,
        double Ox, double Oy, double r);
