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
            lines.push_back(Line2D(p1,p2,color, points[face.point_indexes[0]].z, points[face.point_indexes[1]].z));
        }
        else {
            int size = face.point_indexes.size();
            for (int i=0; i < size-1; i++) {
                Point2D p1 = doProjection(d, points[face.point_indexes[i]]);
                Point2D p2 = doProjection(d, points[face.point_indexes[i+1]]);
                lines.push_back(Line2D(p1,p2, color, points[face.point_indexes[i]].z, points[face.point_indexes[i+1]].z));
            }
            lines.push_back(Line2D(doProjection(d, points[face.point_indexes[0]]), doProjection(d, points[face.point_indexes[size-1]]), color,
                    points[face.point_indexes[0]].z, points[face.point_indexes[size-1]].z));
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

void Figure::triangulate() {
    std::vector<Face> newFaces;
    while (!faces.empty()) {
        if (faces[0].point_indexes.size() > 3) {
            for (int i = 1; i < (int) faces[0].point_indexes.size() - 1; i++) {
                newFaces.push_back(Face({faces[0].point_indexes[0], faces[0].point_indexes[i], faces[0].point_indexes[i + 1]}));
            }
        }
        else {
            newFaces.push_back(faces[0]);
        }
        faces.erase(faces.begin());
    }
    faces = newFaces;
}

void Figure::draw_zbuf_triag(ZBuffer &buf, img::EasyImage &image, double d, double dx, double dy) {
    for (auto face:faces) {
        Vector3D A = points[face.point_indexes[0]];
        Vector3D B = points[face.point_indexes[1]];
        Vector3D C = points[face.point_indexes[2]];
        Point2D a(((d*A.x) / -A.z) + dx, ((d*A.y) / -A.z) + dy);
        Point2D b(((d*B.x) / -B.z) + dx, ((d*B.y) / -B.z) + dy);
        Point2D c(((d*C.x) / -C.z) + dx, ((d*C.y) / -C.z) + dy);
        Point2D g((a.x + b.x + c.x)/3, (a.y + b.y + c.y)/3);
        double InvZg = 1/(3*A.z) + 1/(3*B.z)  + 1/(3*C.z);
        Vector3D u = Vector3D::vector(B.x-A.x, B.y-A.y, B.z-A.z);
        Vector3D v = Vector3D::vector(C.x-A.x, C.y-A.y, C.z-A.z);
        Vector3D w = Vector3D::vector(u.y*v.z-u.z*v.y, u.z*v.x-u.x*v.z, u.x*v.y-u.y*v.x);
        double k = w.x*A.x + w.y*A.y + w.z*A.z;
        double dzdx = w.x/(-d*k);
        double dzdy = w.y/(-d*k);
        int Ymin = (int) round(getMin(a.y, b.y, c.y));
        int Ymax = (int) round(getMax(a.y, b.y, c.y));

        for (int y = Ymin; y <= Ymax; y++) {
            //AB, AC, BC
            std::vector<double> xLs = {std::numeric_limits<double>::infinity(),std::numeric_limits<double>::infinity(),std::numeric_limits<double>::infinity()};
            std::vector<double> xRs = {-std::numeric_limits<double>::infinity(),-std::numeric_limits<double>::infinity(),-std::numeric_limits<double>::infinity()};
            //AB
            if ((y-round(a.y))*(y-round(b.y)) <= 0.0 and round(a.y) != round(b.y)) {
                xLs[0] = round(b.x) + (round(a.x)-round(b.x))*(y-round(b.y))/(round(a.y)-round(b.y));
                xRs[0] = round(b.x) + (round(a.x)-round(b.x))*(y-round(b.y))/(round(a.y)-round(b.y));
            }
            //AC
            if ((y-round(a.y))*(y-round(c.y)) <= 0.0 and round(a.y) != round(c.y)) {
                xLs[1] = round(c.x) + (round(a.x)-round(c.x))*(y-round(c.y))/(round(a.y)-round(c.y));
                xRs[1] = round(c.x) + (round(a.x)-round(c.x))*(y-round(c.y))/(round(a.y)-round(c.y));
            }
            //BC
            if ((y-round(b.y))*(y-round(c.y)) <= 0.0 and round(b.y) != round(c.y)) {
                xLs[2] = round(c.x) + (round(b.x)-round(c.x))*(y-round(c.y))/(round(b.y)-round(c.y));
                xRs[2] = round(c.x) + (round(b.x)-round(c.x))*(y-round(c.y))/(round(b.y)-round(c.y));
            }
            int xL = (int)round(getMin(xLs[0], xLs[1], xLs[2]));
            int xR = (int)round(getMax(xRs[0], xRs[1], xRs[2]));
            for (int x=xL; x < xR; x++) {
                double invZ = 1.0001*InvZg + (x-g.x)*dzdx + (y-g.y)*dzdy;
                if (invZ < buf.buffer[y][x]) {
                    image(x, y) = img::Color(color.red * 255, color.green * 255, color.blue * 255);
                    buf.buffer[y][x] = invZ;
                }
            }
        }
    }
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
    std::vector<int> lastFace;
    for (int i=0; i<n; i++) {
        newFigure.addPoint(Vector3D::point(cos(2*i*PI/n),sin(2*i*PI/n),0));
        newFigure.addFace({i, (i+1)%n , n});
        lastFace.push_back(i);
    }
    newFigure.addPoint(Vector3D::point(0,0,h));
    newFigure.addFace(lastFace);


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

    std::vector<int> upface;
    std::vector<int> downface;

    for (int i=0; i < 2*n; i+=2) {
        upface.push_back(i);
        downface.push_back(i+1);
    }
    newFigure.addFace(upface);
    newFigure.addFace(downface);
    return newFigure;
}

Figure createTorus(Color c, const double r, const double R, const int n, const int m, bool triangulate) {
    Figure newFigure(c);
    for (int i=0; i < n; i++) {
        for (int j=0; j<m; j++) {
            double u = 2*i*PI/n;
            double v = 2*j*PI/m;
            newFigure.addPoint(createTorusPoint(r, R, u, v));
            if (triangulate) {
                newFigure.addFace({(i * m + j), (((i + 1) % n) * m + j) % (n * m), (((i + 1) % n) * m + ((j + 1) % m)) % (n * m)});
                newFigure.addFace({(i * m + j), (((i + 1) % n) * m + ((j + 1) % m)) % (n * m), (i * m + ((j + 1) % m))});
            }
            else {
                newFigure.addFace({(i * m + j), (((i + 1) % n) * m + j) % (n * m), (((i + 1) % n) * m + ((j + 1) % m)) % (n * m),(i * m + ((j + 1) % m))});
            }
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

double getMin(double a, double b, double c) {
    double minimum = std::min(a,b);
    return std::min(minimum, c);
}

double getMax(double a, double b, double c) {
    double maximum = std::max(a,b);
    return std::max(maximum, c);
}

Figures3D generateFractal(Figure& fig, const int nr_iterations, const double scale) {
    Figures3D figures = {fig};
    for (int i=0; i < nr_iterations; i++) {
        Figures3D newFigures;
        for (auto figure:figures) {
            for (unsigned int j = 0; j < figure.getPoints().size(); j++) {
                Figure newFigure = figure;
                newFigure.applyTransformation(newFigure.scaleFigure(1/scale));
                newFigure.applyTransformation(newFigure.translate(figure.getPoints()[j]-newFigure.getPoints()[j]));
                newFigures.push_back(newFigure);
            }
        }
        figures = newFigures;
    }
    return figures;
}
