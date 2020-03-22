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
    r = sqrt((pow(point.x, 2) + pow(point.y, 2) + pow(point.z, 2)));
    theta = atan2(point.y, point.x);
    phi = acos(point.z/r);
}

Lines2D Figure::doProjection(const Vector3D &eyepoint, const double d) {
    applyTransformation(eyePointTrans(eyepoint));
    Lines2D lines;
    for (auto &face:faces) {
        if (face.point_indexes.size() == 2) {
            Point2D p1 = doProjection(d, points[face.point_indexes[0]]);
            Point2D p2 = doProjection(d, points[face.point_indexes[1]]);
            lines.push_back(Line2D(p1,p2,color));
        }
        else {
            int size = face.point_indexes.size();
            for (int i=0; i < size-1; i++) {
                Point2D p1 = doProjection(d, points[face.point_indexes[i]]);
                Point2D p2 = doProjection(d, points[face.point_indexes[i+1]]);
                lines.push_back(Line2D(p1,p2,color));
            }
            lines.push_back(Line2D(doProjection(d, points[face.point_indexes[0]]), doProjection(d, points[face.point_indexes[size-1]]), color));
        }
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

void Figure::addPoint(Vector3D point) {
    points.push_back(point);
}

std::vector<Vector3D> &Figure::getPoints() {
    return points;
}

std::vector<Face> &Figure::getFaces() {
    return faces;
}

void Figure::setPoints(std::vector<Vector3D> points) {
    Figure::points = points;
}

void Figure::setFaces(std::vector<Face> faces) {
    Figure::faces = faces;
}


//Works
Figure createCube(Color c) {
    Figure newFigure(c);
    newFigure.addPoint(Vector3D::point(1,-1,-1));
    newFigure.addPoint(Vector3D::point(-1,1,-1));
    newFigure.addPoint(Vector3D::point(1,1,1));
    newFigure.addPoint(Vector3D::point(-1,-1,1));
    newFigure.addPoint(Vector3D::point(1,1,-1));
    newFigure.addPoint(Vector3D::point(-1,-1,-1));
    newFigure.addPoint(Vector3D::point(1,-1,1));
    newFigure.addPoint(Vector3D::point(-1,1,1));
    newFigure.addFace({0,4,2,6});
    newFigure.addFace({4,1,7,2});
    newFigure.addFace({1,5,3,7});
    newFigure.addFace({5,0,6,3});
    newFigure.addFace({6,2,7,3});
    newFigure.addFace({0,5,1,4});
    return newFigure;
}

//Works
Figure createTetrahedron(Color c) {
    Figure newFigure(c);
    newFigure.addPoint(Vector3D::point(1,-1,-1));
    newFigure.addPoint(Vector3D::point(-1,1,-1));
    newFigure.addPoint(Vector3D::point(1,1,1));
    newFigure.addPoint(Vector3D::point(-1,-1,1));
    newFigure.addFace({0,1,2});
    newFigure.addFace({1,3,2});
    newFigure.addFace({0,3,1});
    newFigure.addFace({0,2,3});
    return newFigure;
}

//Works
Figure createOctahedron(Color c) {
    Figure newFigure(c);
    newFigure.addPoint(Vector3D::point(1,0,0));
    newFigure.addPoint(Vector3D::point(0,1,0));
    newFigure.addPoint(Vector3D::point(-1,0,0));
    newFigure.addPoint(Vector3D::point(0,-1,0));
    newFigure.addPoint(Vector3D::point(0,0,-1));
    newFigure.addPoint(Vector3D::point(0,0,1));
    newFigure.addFace({0,1,5});
    newFigure.addFace({1,2,5});
    newFigure.addFace({2,3,5});
    newFigure.addFace({3,0,5});
    newFigure.addFace({1,0,4});
    newFigure.addFace({2,1,4});
    newFigure.addFace({3,2,4});
    newFigure.addFace({0,3,4});
    return newFigure;
}

//Works
Figure createIcosahedron(Color c) {
    Figure newFigure(c);
    newFigure.addPoint(Vector3D::point(0,0,sqrt(5)/2));
    for (int i=2; i <=11; i++) {
        double x;
        double y;
        double z;
        if (i<=6) {
            x = cos((i-2)*2*PI/5);
            y = sin((i-2)*2*PI/5);
            z = 0.5;
        }
        else {
            x = cos(PI/5+(i-7)*2*PI/5);
            y = sin(PI/5+(i-7)*2*PI/5);
            z = -0.5;
        }
        newFigure.addPoint(Vector3D::point(x,y,z));
    }
    newFigure.addPoint(Vector3D::point(0,0,-sqrt(5)/2));

    newFigure.addFace({0,1,2});
    newFigure.addFace({0,2,3});
    newFigure.addFace({0,3,4});
    newFigure.addFace({0,4,5});
    newFigure.addFace({0,5,1});
    newFigure.addFace({1,6,2});
    newFigure.addFace({2,6,7});
    newFigure.addFace({2,7,3});
    newFigure.addFace({3,7,8});
    newFigure.addFace({3,8,4});
    newFigure.addFace({4,8,9});
    newFigure.addFace({4,9,5});
    newFigure.addFace({5,9,10});
    newFigure.addFace({5,10,1});
    newFigure.addFace({1,10,6});
    newFigure.addFace({11,7,6});
    newFigure.addFace({11,8,7});
    newFigure.addFace({11,9,8});
    newFigure.addFace({11,10,9});
    newFigure.addFace({11,6,10});

    return newFigure;
}

//Works
Figure createDodecahedron(Color c) {
    Figure newFigure = createIcosahedron(c);
    std::vector<Vector3D> dodecaPoints;
    std::vector<Face> dodecaFaces;

    for (auto face:newFigure.getFaces()) {
        std::cout << face.point_indexes[0]+1 << " " << face.point_indexes[1]+1 << " " << face.point_indexes[2]+1 << std::endl;
        Vector3D p1 = newFigure.getPoints()[face.point_indexes[0]];
        Vector3D p2 = newFigure.getPoints()[face.point_indexes[1]];
        Vector3D p3 = newFigure.getPoints()[face.point_indexes[2]];
        dodecaPoints.push_back((p1+p2+p3)/3);
    }
    dodecaFaces.push_back(Face({0,1,2,3,4}));
    dodecaFaces.push_back(Face({0,5,6,7,1}));
    dodecaFaces.push_back(Face({1,7,8,9,2}));
    dodecaFaces.push_back(Face({2,9,10,11,3}));
    dodecaFaces.push_back(Face({3,11,12,13,4}));
    dodecaFaces.push_back(Face({4,13,14,5,0}));
    dodecaFaces.push_back(Face({19,18,17,16,15}));
    dodecaFaces.push_back(Face({19,14,13,12,18}));
    dodecaFaces.push_back(Face({18,12,11,10,17}));
    dodecaFaces.push_back(Face({17,10,9,8,16}));
    dodecaFaces.push_back(Face({16,8,7,6,15}));
    dodecaFaces.push_back(Face({15,6,5,14,19}));

    newFigure.setPoints(dodecaPoints);
    newFigure.setFaces(dodecaFaces);

    return newFigure;
}

//Works
Figure createSphere(Color c, const int n) {
    Figure newFigure = createIcosahedron(c);

    std::vector<Vector3D> newPoints;
    std::vector<Face> newFaces;

    for (int i=0; i<n; i++) {
        int index = 0;
        std::vector<Vector3D> points = newFigure.getPoints();
        std::vector<Face> faces = newFigure.getFaces();
        for (auto& face:faces) {
            Vector3D pointD = (points[face.point_indexes[0]] + points[face.point_indexes[1]])/2;
            Vector3D pointE = (points[face.point_indexes[0]] + points[face.point_indexes[2]])/2;
            Vector3D pointF = (points[face.point_indexes[1]] + points[face.point_indexes[2]])/2;
            newPoints.push_back(points[face.point_indexes[0]]);
            newPoints.push_back(points[face.point_indexes[1]]);
            newPoints.push_back(points[face.point_indexes[2]]);
            newPoints.push_back(pointD);
            newPoints.push_back(pointE);
            newPoints.push_back(pointF);
            newFaces.push_back(Face({index, index+3, index+4}));
            newFaces.push_back(Face({index+1, index+3, index+5}));
            newFaces.push_back(Face({index+2, index+4, index+5}));
            newFaces.push_back(Face({index+3, index+4, index+5}));
            index += 6;
        }
        newFigure.setPoints(newPoints);
        newFigure.setFaces(newFaces);
        newPoints.clear();
        newFaces.clear();
    }

    for (auto &point:newFigure.getPoints()) {
        double r = sqrt((pow(point.x, 2) + pow(point.y, 2) + pow(point.z, 2)));
        point = point/r;
    }


    return newFigure;
}

Figure createPlatonicSolid(std::string type, Color c) {
    if (type == "Cube") {
        return createCube(c);
    }
    else if (type == "Tetrahedron") {
        return createTetrahedron(c);
    }
    else if (type == "Octahedron") {
        return createOctahedron(c);
    }
    else if (type == "Icosahedron") {
        return createIcosahedron(c);
    }
    else if (type == "Dodecahedron") {
        return createDodecahedron(c);
    }
    return Figure(c);
}

//Works
Figure createCone(Color c, const int n, const double h) {
    Figure newFigure(c);
    for (int i=0; i<n; i++) {
        newFigure.addPoint(Vector3D::point(cos(2*i*PI/n),sin(2*i*PI/n),0));
        newFigure.addFace({i, (i+1)%n , n});
    }
    newFigure.addPoint(Vector3D::point(0,0,h));

    return newFigure;
}

//Works
Figure createCylinder(Color c, const int n, const double h) {
    Figure newFigure(c);

    int index = 0;
    for (int i=0; i < n; i++) {
        newFigure.addPoint(Vector3D::point(cos(2*i*PI/n), sin(2*i*PI/n),0));
        newFigure.addPoint(Vector3D::point(cos(2*i*PI/n), sin(2*i*PI/n),h));
        newFigure.addFace({index, (index+2)%(2*n), (index+3)%(2*n), index+1});
        index += 2;
    }

    return newFigure;
}

Figure createTorus(Color c, const double r, const double R, const int n, const int m) {
    Figure newFigure(c);
    for (int i=0; i < n; i++) {
        for (int j=0; j<m; j++) {
            double u = 2*i*PI/n;
            double v = 2*j*PI/m;
            newFigure.addPoint(createTorusPoint(r, R, u, v));
            newFigure.addFace({(i*m+j) , (((i+1)%n)*m+j) % (n*m) , (((i+1)%n)*m+((j+1)%m))%(n*m) , (i*m+((j+1)%m))});
        }
    }
    return newFigure;
}

Vector3D createTorusPoint(double r, double R, const double u, const double v) {
    double x = (R+r*cos(v))*cos(u);
    double y = (R+r*cos(v))*sin(u);
    double z = r*sin(v);
    return Vector3D::point(x,y,z);
}



