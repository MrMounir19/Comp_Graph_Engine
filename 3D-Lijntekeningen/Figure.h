//
// Created by mouni on 12-Mar-20.
//

#ifndef ENGINE_FIGURE_H
#define ENGINE_FIGURE_H
#include <list>
#include "../vector3d.h"
#include "../line_structs.cpp"

struct Face {
    //De indexen refereren naar
    //punten in de ‘points’ vector
    //van de Figure-klasse
    std::vector<int> point_indexes;
    Face(std::vector<int> indexes):point_indexes(indexes) {}
};

class Figure {
    std::vector<Vector3D> points;
    std::vector<Face> faces;
    Color color;
public:
    Figure(Color c) : color(c) {}
    void addPoint(std::vector<double> point);
    void addFace(std::vector<int> indexes);
    Matrix scaleFigure(const double scale);
    Matrix rotateX(const double angle);
    Matrix rotateY(const double angle);
    Matrix rotateZ(const double angle);
    Matrix translate(const Vector3D& vector);
    void applyTransformation(const Matrix& matrix);
    Matrix eyePointTrans(const Vector3D& eyepoint);
    void toPolar(const Vector3D &point, double &theta, double &phi, double &r);
    Lines2D doProjection(const Vector3D& eyepoint, const double d);
    Point2D doProjection(const double d, const Vector3D& point);
};



typedef std::list<Figure> Figures3D;

#endif //ENGINE_FIGURE_H
