#include <vector>
#include <list>
#include "vector3d.cc"
#include "vector3d.h"
#include <cmath>

struct Color {
    double red;
    double green;
    double blue;
    Color (double R, double G, double B):red(R), green(G), blue(B) {}
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

struct Face {
    //De indexen refereren naar
    //punten in de ‘points’ vector
    //van de Figure-klasse
    std::vector<int> point_indexes;
};

struct Figure {
    std::vector<Vector3D> points;
    std::vector<Face> faces;
    Color color;
};

typedef std::list<Figure> Figures3D;

Matrix scaleFigure(const double scale) {
    Matrix scale_matrix;
    scale_matrix(1,1) = scale;
    scale_matrix(2,2) = scale;
    scale_matrix(3,3) = scale;
    scale_matrix(4,4) = 1;
    return scale_matrix;
}

Matrix rotateX(const double angle) {
    const double PI = 3.14159265358979323846264338327950288;
    Matrix Rx;
    double rad_angle = angle*PI/180.0;
    Rx(1,1) = 1;
    Rx(2,2) = cos(rad_angle);
    Rx(2,3) = sin(rad_angle);
    Rx(3,2) = -sin(rad_angle);
    Rx(3,3) = cos(rad_angle);
    Rx(4,4) = 1;
    return Rx;
};

Matrix rotateY(const double angle) {
    const double PI = 3.14159265358979323846264338327950288;
    Matrix Ry;
    double rad_angle = angle*PI/180.0;
    Ry(1,1) = 1;
    Ry(2,2) = cos(rad_angle);
    Ry(2,3) = sin(rad_angle);
    Ry(3,2) = -sin(rad_angle);
    Ry(3,3) = cos(rad_angle);
    Ry(4,4) = 1;
    return Ry;
};

Matrix rotateZ(const double angle) {
    const double PI = 3.14159265358979323846264338327950288;
    Matrix Rz;
    double rad_angle = angle*PI/180.0;
    Rz(1,1) = cos(rad_angle);;
    Rz(3,1) = sin(rad_angle);
    Rz(2,2) = 1;
    Rz(1,3) = -sin(rad_angle);
    Rz(3,3) = cos(rad_angle);
    Rz(4,4) = 1;
    return Rz;
};

Matrix translate(const Vector3D &vector) {
    Matrix Mt;
    Mt(1,1) = 1;
    Mt(2,2) = 1;
    Mt(3,3) = 1;
    Mt(4,4) = 1;
    Mt(4,1) = vector[0];
    Mt(4,2) = vector[1];
    Mt(4,3) = vector[2];
    return Mt;
}

void applyTransformation(Figure& figure, const Matrix& matrix) {
    for (auto x:figure.points) {
        x = x*matrix;
    }
}


