

#ifndef ENGINE_LINESTRUCTS
#define ENGINE_LINESTRUCTS
#include <vector>
#include <limits>

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
    Line2D (Point2D P1, Point2D P2, Color c, double Z1, double Z2):p1(P1), p2(P2), color(c), z1(Z1), z2(Z2)  {}
    double z1;
    double z2;
};



struct ZBuffer {
    std::vector<std::vector<double>> buffer;
    ZBuffer(unsigned int width, unsigned int height ) {
        double posInf = std::numeric_limits<double>::infinity();
        std::vector<double> rij;
        for (unsigned int w = 0; w < width; w++) {
            rij.push_back(posInf);
        }
        for (unsigned int h = 0; h < height; h++) {
            buffer.push_back(rij);
        }
    }
};


using Lines2D = std::vector<Line2D>;

#endif



