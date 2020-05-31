
#ifndef ENGINE_FIGURE_H
#define ENGINE_FIGURE_H
#include <list>
#include "../vector3d/vector3d.h"
#include "../2D_Lines/line_structs.cpp"
#include "../easy_image/easy_image.h"

struct Face {
    //De indexen refereren naar
    //punten in de ‘points’ vector
    //van de Figure-klasse
    std::vector<int> point_indexes;
    Face(std::vector<int> indexes):point_indexes(indexes) {}
};

class Figure {
private:
    std::vector<Vector3D> points;
    std::vector<Face> faces;
    Color color;
public:
    const Color &getColor() const;

public:
    Figure(Color c) : color(c) {}
    void addPoint(std::vector<double> point);
    void addFace(std::vector<int> indexes);
    void addPoint(Vector3D point);
    void setPoints(const std::vector<Vector3D> points);
    void setFaces(const std::vector<Face> faces);
    void triangulate();
    std::vector<Vector3D> &getPoints();
    std::vector<Face> &getFaces();
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
    void draw_zbuf_triag(ZBuffer& buf, img::EasyImage& image, double d, double dx, double dy);
};

Figure createCube(Color c);

Figure createTetrahedron(Color c);

Figure createOctahedron(Color c);

Figure createIcosahedron(Color c);

Figure createDodecahedron(Color c);

Figure createSphere(Color c, const int n);

Figure createPlatonicSolid(std::string type, Color c);

Figure createCone(Color c, const int n, const double h);

Figure createCylinder(Color c, const int n, const double h);

Figure createTorus(Color c, const double r, const double R, const int n, const int m, bool triangulate);

Vector3D createTorusPoint(double r, double R, const double u, const double v);

double getMin(double a, double b, double c);

double getMax(double a, double b, double c);

typedef std::vector<Figure> Figures3D;

Figures3D generateFractal(Figure& fig, const int nr_iterations, const double scale);

Figures3D generateThickFigures(Figure& fig, double r, int n, int m, bool triangulatie, bool lijntekening);



#endif //ENGINE_FIGURE_H
