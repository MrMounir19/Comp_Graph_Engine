

#ifndef ENGINE_LINESTRUCTS
#define ENGINE_LINESTRUCTS
#include <vector>

struct Color {
    double red;
    double green;
    double blue;
    Color (double R, double G, double B):red(R), green(G), blue(B) {}
    Color (std::vector<double> color) {
        red = color[0];
        green = color[1];
        blue = color[2];
    }
};

struct Point2D {
    double x;
    double y;
    Point2D (double X, double Y):x(X), y(Y) {}
};

struct Line2D {
    Point2D p1;
    Point2D p2;
    Color color;
    Line2D (Point2D P1, Point2D P2, Color c):p1(P1), p2(P2), color(c) {}
};

using Lines2D = std::vector<Line2D>;

#endif



