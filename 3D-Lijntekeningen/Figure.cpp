//
// Created by mouni on 12-Mar-20.
//

#include "Figure.h"
#include <cmath>

const double PI = 3.14159265358979323846264338327950288;

Matrix Figure::scaleFigure(const double scale) {
    Matrix scale_matrix;
    scale_matrix(1,1) = scale;
    scale_matrix(2,2) = scale;
    scale_matrix(3,3) = scale;
    scale_matrix(4,4) = 1;
    return scale_matrix;
}

Matrix Figure::rotateX(const double angle) {
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

Matrix Figure::rotateY(const double angle) {
    Matrix Ry;
    double rad_angle = angle*PI/180.0;
    Ry(1,1) = cos(rad_angle);
    Ry(2,2) = 1;
    Ry(1,3) = -sin(rad_angle);
    Ry(3,1) = sin(rad_angle);
    Ry(3,3) = cos(rad_angle);
    Ry(4,4) = 1;
    return Ry;
};

Matrix Figure::rotateZ(const double angle) {
    Matrix Rz;
    double rad_angle = angle*PI/180.0;
    Rz(1,1) = cos(rad_angle);
    Rz(1,2) = sin(rad_angle);
    Rz(3,3) = 1;
    Rz(2,1) = -sin(rad_angle);
    Rz(2,2) = cos(rad_angle);
    Rz(4,4) = 1;
    return Rz;
};

Matrix Figure::translate(const Vector3D &vector) {
    Matrix Mt;
    Mt(1,1) = 1;
    Mt(2,2) = 1;
    Mt(3,3) = 1;
    Mt(4,4) = 1;
    Mt(4,1) = vector.x;
    Mt(4,2) = vector.y;
    Mt(4,3) = vector.z;
    return Mt;
}

void Figure::applyTransformation(const Matrix& matrix) {
    for (auto &x:points) {
        x = x*matrix;
    }
}

Matrix Figure::eyePointTrans(const Vector3D &eyepoint) {
    double theta, phi, r;
    toPolar(eyepoint, theta, phi, r);
    Matrix V;
    V(1,1) = -sin(theta);
    V(1,2) = -cos(theta)*cos(phi);
    V(1,3) = cos(theta)*sin(phi);
    V(2,1) = cos(theta);
    V(2,2) = -sin(theta)*cos(phi);
    V(2,3) = sin(theta)*sin(phi);
    V(3,2) = sin(phi);
    V(3,3) = cos(phi);
    V(4,3) = -r;
    V(4,4) = 1;
    return V;
}

void Figure::toPolar(const Vector3D &point, double &theta, double &phi, double &r) {
    r = sqrt((pow(point.x, 2) + pow(point.z, 2) + pow(point.z, 2)));
    theta = atan2(point.y, point.x);
    phi = acos(point.z/r);
}

Lines2D Figure::doProjection(const Vector3D &eyepoint, const double d) {
    applyTransformation(eyePointTrans(eyepoint));
    Lines2D lines;
    for (auto &face:faces) {
        Point2D p1 = doProjection(d, points[face.point_indexes[0]]);
        Point2D p2 = doProjection(d, points[face.point_indexes[1]]);
        lines.push_back(Line2D(p1,p2,color));
    }
    return lines;
}

Point2D Figure::doProjection(const double d, const Vector3D &point) {
    double x,y;
    x = d * (point.x / -point.z);
    y = d * (point.y / -point.z);
    Point2D point2d(x,y);
    return point2d;
}

void Figure::addPoint(std::vector<double> point) {
    Vector3D newpoint = Vector3D::point(point[0], point[1], point[2]);
    points.push_back(newpoint);
}

void Figure::addFace(std::vector<int> indexes) {
    faces.push_back(Face(indexes));
}
