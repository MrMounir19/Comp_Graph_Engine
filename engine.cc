#include "easy_image/easy_image.h"
#include "ini_configuration/ini_configuration.h"
#include "2D_L_System/L_System.h"
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <string>
#include <cmath>
#include "Figure/Figure.h"
#include "3D_L_System/L_System_3D.h"

img::EasyImage generate_2DLsystem(const ini::Configuration &configuration) {
    Lines2D lines;
    vector<double> color = configuration["2DLSystem"]["color"].as_double_tuple_or_die();
    string input = configuration["2DLSystem"]["inputfile"].as_string_or_die();
    L_System lSystem = L_System(input, Color(color[0],color[1],color[2]));
    lines = lSystem.generateLines();
    int size = configuration["General"]["size"].as_int_or_die();
    vector<double> background = configuration["General"]["backgroundcolor"].as_double_tuple_or_die();
    vector<double> max = img::getMax(lines);
    img::EasyImage image((int) round(max[4] * size), (int) round(max[5] * size),
                         img::Color((int) round(background[0] * 255), (int) round(background[1] * 255),
                                    (int) round(background[2] * 255)));

    image.draw2Dlines(lines, size, max[0], max[1], max[2], max[3], false);
    return image;
}

img::EasyImage generate_Wireframe(const ini::Configuration &configuration) {        //Ook zbuffered
    Lines2D lines;
    string imagetype = configuration["General"]["type"].as_string_or_die();
    vector<double> eye = configuration["General"]["eye"].as_double_tuple_or_die();
    int nrFigures = configuration["General"]["nrFigures"].as_int_or_die();
    for (int i = 0; i<nrFigures; i++) {
        string figure = "Figure" + to_string(i);
        string type = configuration[figure]["type"].as_string_or_die();
        double scale = configuration[figure]["scale"].as_double_or_die();
        double rX = configuration[figure]["rotateX"].as_double_or_die();
        double rY = configuration[figure]["rotateY"].as_double_or_die();
        double rZ = configuration[figure]["rotateZ"].as_double_or_die();
        vector<double> center = configuration[figure]["center"].as_double_tuple_or_die();
        vector<double> color = configuration[figure]["color"].as_double_tuple_or_die();
        Color c = Color(color[0], color[1], color[2]);
        Figure newfigure = Figure(color);
        //---------------------------
        if (type == "LineDrawing") {
            int nrPoints = configuration[figure]["nrPoints"].as_int_or_die();
            int nrLines = configuration[figure]["nrLines"].as_int_or_die();
            for (int p = 0; p < nrPoints; p++) {
                newfigure.addPoint(configuration[figure]["point" + to_string(p)].as_double_tuple_or_die());
            }
            for (int l = 0; l < nrLines; l++) {
                newfigure.addFace(configuration[figure]["line" + to_string(l)].as_int_tuple_or_die());
            }
        }
        //---------------------------
        else if (type == "Cube" or type == "Tetrahedron" or type == "Octahedron" or type == "Icosahedron" or type == "Dodecahedron") {
            newfigure = createPlatonicSolid(type, c);
        }
            //---------------------------
        else if (type == "Sphere") {
            newfigure = createSphere(c, configuration[figure]["n"].as_int_or_die());
        }
            //---------------------------
        else if (type == "Cone") {
            newfigure = createCone(c, configuration[figure]["n"].as_int_or_die(), configuration[figure]["height"].as_double_or_die());
        }
            //---------------------------
        else if (type == "Cylinder") {
            newfigure = createCylinder(c, configuration[figure]["n"].as_int_or_die(), configuration[figure]["height"].as_double_or_die());
        }
            //---------------------------
        else if (type == "Torus") {
            newfigure = createTorus(c, configuration[figure]["r"].as_double_or_die(), configuration[figure]["R"].as_double_or_die(),
                                    configuration[figure]["n"].as_int_or_die(), configuration[figure]["m"].as_int_or_die(), false);
        }
            //---------------------------
        else if (type == "3DLSystem") {
            string input = configuration[figure]["inputfile"].as_string_or_die();
            L_System_3D lsystem3D(input, c);
            newfigure = lsystem3D.generateFigure();
        }
        else if (type == "FractalCube" or type == "FractalTetrahedron" or type == "FractalOctahedron" or type == "FractalIcosahedron" or type == "FractalDodecahedron") {
            newfigure = createPlatonicSolid(type.substr(7,type.size()-7), c);
            newfigure.applyTransformation(newfigure.scaleFigure(scale));
            newfigure.applyTransformation(newfigure.rotateX(rX));
            newfigure.applyTransformation(newfigure.rotateY(rY));
            newfigure.applyTransformation(newfigure.rotateZ(rZ));
            newfigure.applyTransformation(newfigure.translate(Vector3D::vector(center[0], center[1], center[2])));
            int nrIterations = configuration[figure]["nrIterations"].as_int_or_die();
            double fractalScale = configuration[figure]["fractalScale"].as_double_or_die();
            Figures3D newFigures = generateFractal(newfigure, nrIterations, fractalScale);
            for (auto fig:newFigures) {
                Lines2D newlines = fig.doProjection(Vector3D::point(eye[0], eye[1], eye[2]), 1);
                lines.insert(lines.end(), newlines.begin(), newlines.end());
            }
            continue;
        }
        else if (type == "ThickCube" or type == "ThickTetrahedron" or type == "ThickOctahedron" or type == "ThickIcosahedron" or type == "ThickDodecahedron") {
            newfigure = createPlatonicSolid(type.substr(5,type.size()-5), c);
            newfigure.applyTransformation(newfigure.scaleFigure(scale));
            newfigure.applyTransformation(newfigure.rotateX(rX));
            newfigure.applyTransformation(newfigure.rotateY(rY));
            newfigure.applyTransformation(newfigure.rotateZ(rZ));
            double r = configuration[figure]["radius"].as_double_or_die();
            int n = configuration[figure]["n"].as_int_or_die();
            int m = configuration[figure]["m"].as_int_or_die();
            Figures3D newFigures = generateThickFigures(newfigure, r, n, m, false, false);
            for (auto fig:newFigures) {
                Lines2D newlines = fig.doProjection(Vector3D::point(eye[0], eye[1], eye[2]), 1);
                lines.insert(lines.end(), newlines.begin(), newlines.end());
            }
            continue;
        }
        else if (type == "ThickLineDrawing") {
            int nrPoints = configuration[figure]["nrPoints"].as_int_or_die();
            int nrLines = configuration[figure]["nrLines"].as_int_or_die();
            for (int p = 0; p < nrPoints; p++) {
                newfigure.addPoint(configuration[figure]["point" + to_string(p)].as_double_tuple_or_die());
            }
            for (int l = 0; l < nrLines; l++) {
                newfigure.addFace(configuration[figure]["line" + to_string(l)].as_int_tuple_or_die());
            }
            newfigure.applyTransformation(newfigure.scaleFigure(scale));
            newfigure.applyTransformation(newfigure.rotateX(rX));
            newfigure.applyTransformation(newfigure.rotateY(rY));
            newfigure.applyTransformation(newfigure.rotateZ(rZ));
            newfigure.applyTransformation(newfigure.translate(Vector3D::vector(center[0], center[1], center[2])));
            double r = configuration[figure]["radius"].as_double_or_die();
            int n = configuration[figure]["n"].as_int_or_die();
            int m = configuration[figure]["m"].as_int_or_die();
            Figures3D newFigures = generateThickFigures(newfigure, r, n, m, false, true);
            for (auto fig:newFigures) {
                Lines2D newlines = fig.doProjection(Vector3D::point(eye[0], eye[1], eye[2]), 1);
                lines.insert(lines.end(), newlines.begin(), newlines.end());
            }
            continue;
        }
        else if (type == "Thick3DLSystem") {
            string input = configuration[figure]["inputfile"].as_string_or_die();
            L_System_3D lsystem3D(input, c);
            newfigure = lsystem3D.generateFigure();
            newfigure.applyTransformation(newfigure.scaleFigure(scale));
            newfigure.applyTransformation(newfigure.rotateX(rX));
            newfigure.applyTransformation(newfigure.rotateY(rY));
            newfigure.applyTransformation(newfigure.rotateZ(rZ));
            newfigure.applyTransformation(newfigure.translate(Vector3D::vector(center[0], center[1], center[2])));
            double r = configuration[figure]["radius"].as_double_or_die();
            int n = configuration[figure]["n"].as_int_or_die();
            int m = configuration[figure]["m"].as_int_or_die();
            Figures3D newFigures = generateThickFigures(newfigure, r, n, m, false, true);
            for (auto fig:newFigures) {
                Lines2D newlines = fig.doProjection(Vector3D::point(eye[0], eye[1], eye[2]), 1);
                lines.insert(lines.end(), newlines.begin(), newlines.end());
            }
            continue;
        }

        else {
            return img::EasyImage();
        }
        //---------------------------

        newfigure.applyTransformation(newfigure.scaleFigure(scale));
        newfigure.applyTransformation(newfigure.rotateX(rX));
        newfigure.applyTransformation(newfigure.rotateY(rY));
        newfigure.applyTransformation(newfigure.rotateZ(rZ));
        newfigure.applyTransformation(newfigure.translate(Vector3D::vector(center[0], center[1], center[2])));
        Lines2D newlines = newfigure.doProjection(Vector3D::point(eye[0], eye[1], eye[2]), 1);
        lines.insert(lines.end(), newlines.begin(), newlines.end());
    }

    int size = configuration["General"]["size"].as_int_or_die();
    vector<double> background = configuration["General"]["backgroundcolor"].as_double_tuple_or_die();
    vector<double> max = img::getMax(lines);
    img::EasyImage image((int) round(max[4] * size), (int) round(max[5] * size),
                         img::Color((int) round(background[0] * 255), (int) round(background[1] * 255),
                                    (int) round(background[2] * 255)));

    image.draw2Dlines(lines, size, max[0], max[1], max[2], max[3], imagetype == "ZBufferedWireframe");
    return image;
}

img::EasyImage generate_ZBufferingDriehoeken(const ini::Configuration &configuration) {
    Figures3D figures3D;
    Lines2D lines;
    string imagetype = configuration["General"]["type"].as_string_or_die();
    vector<double> eye = configuration["General"]["eye"].as_double_tuple_or_die();
    int nrFigures = configuration["General"]["nrFigures"].as_int_or_die();
    for (int i = 0; i<nrFigures; i++) {
        string figure = "Figure" + to_string(i);
        string type = configuration[figure]["type"].as_string_or_die();
        double scale = configuration[figure]["scale"].as_double_or_die();
        double rX = configuration[figure]["rotateX"].as_double_or_die();
        double rY = configuration[figure]["rotateY"].as_double_or_die();
        double rZ = configuration[figure]["rotateZ"].as_double_or_die();
        vector<double> center = configuration[figure]["center"].as_double_tuple_or_die();
        vector<double> color = configuration[figure]["color"].as_double_tuple_or_die();
        Color c = Color(color[0], color[1], color[2]);
        Figure newfigure = Figure(color);
        //---------------------------
        if (type == "Cube" or type == "Tetrahedron" or type == "Octahedron" or type == "Icosahedron" or type == "Dodecahedron") {
            newfigure = createPlatonicSolid(type, c);
            if (type == "Cube" or type == "Dodecahedron") {
                newfigure.triangulate();
            }
        }
        //---------------------------
        else if (type == "Sphere") {
            newfigure = createSphere(c, configuration[figure]["n"].as_int_or_die());
        }
        //---------------------------
        else if (type == "Cone") {
            newfigure = createCone(c, configuration[figure]["n"].as_int_or_die(), configuration[figure]["height"].as_double_or_die());
            newfigure.triangulate();
        }
        //---------------------------
        else if (type == "Cylinder") {
            newfigure = createCylinder(c, configuration[figure]["n"].as_int_or_die(), configuration[figure]["height"].as_double_or_die());
            newfigure.triangulate();
        }
        //---------------------------
        else if (type == "Torus") {
            newfigure = createTorus(c, configuration[figure]["r"].as_double_or_die(), configuration[figure]["R"].as_double_or_die(),
                                    configuration[figure]["n"].as_int_or_die(), configuration[figure]["m"].as_int_or_die(), true);
        }
        //---------------------------
        else if (type == "FractalCube" or type == "FractalTetrahedron" or type == "FractalOctahedron" or type == "FractalIcosahedron" or type == "FractalDodecahedron") {
            newfigure = createPlatonicSolid(type.substr(7,type.size()-7), c);
            if (type == "FractalCube" or type == "FractalDodecahedron") {
                newfigure.triangulate();
            }
            newfigure.applyTransformation(newfigure.scaleFigure(scale));
            newfigure.applyTransformation(newfigure.rotateX(rX));
            newfigure.applyTransformation(newfigure.rotateY(rY));
            newfigure.applyTransformation(newfigure.rotateZ(rZ));
            newfigure.applyTransformation(newfigure.translate(Vector3D::vector(center[0], center[1], center[2])));
            int nrIterations = configuration[figure]["nrIterations"].as_int_or_die();
            double fractalScale = configuration[figure]["fractalScale"].as_double_or_die();
            Figures3D newFigures = generateFractal(newfigure, nrIterations, fractalScale);
            for (auto fig:newFigures) {
                Lines2D newlines = fig.doProjection(Vector3D::point(eye[0], eye[1], eye[2]), 1);
                lines.insert(lines.end(), newlines.begin(), newlines.end());
                figures3D.push_back(fig);
            }
            continue;
        }
        else if (type == "ThickCube" or type == "ThickTetrahedron" or type == "ThickOctahedron" or type == "ThickIcosahedron" or type == "ThickDodecahedron") {
            newfigure = createPlatonicSolid(type.substr(5,type.size()-5), c);
            newfigure.applyTransformation(newfigure.scaleFigure(scale));
            newfigure.applyTransformation(newfigure.rotateX(rX));
            newfigure.applyTransformation(newfigure.rotateY(rY));
            newfigure.applyTransformation(newfigure.rotateZ(rZ));
            double r = configuration[figure]["radius"].as_double_or_die();
            int n = configuration[figure]["n"].as_int_or_die();
            int m = configuration[figure]["m"].as_int_or_die();
            Figures3D newFigures = generateThickFigures(newfigure, r, n, m, true, false);
            for (auto fig:newFigures) {
                Lines2D newlines = fig.doProjection(Vector3D::point(eye[0], eye[1], eye[2]), 1);
                lines.insert(lines.end(), newlines.begin(), newlines.end());
                figures3D.push_back(fig);
            }
            continue;
        }
        else if (type == "ThickLineDrawing") {
            int nrPoints = configuration[figure]["nrPoints"].as_int_or_die();
            int nrLines = configuration[figure]["nrLines"].as_int_or_die();
            for (int p = 0; p < nrPoints; p++) {
                newfigure.addPoint(configuration[figure]["point" + to_string(p)].as_double_tuple_or_die());
            }
            for (int l = 0; l < nrLines; l++) {
                newfigure.addFace(configuration[figure]["line" + to_string(l)].as_int_tuple_or_die());
            }
            newfigure.applyTransformation(newfigure.scaleFigure(scale));
            newfigure.applyTransformation(newfigure.rotateX(rX));
            newfigure.applyTransformation(newfigure.rotateY(rY));
            newfigure.applyTransformation(newfigure.rotateZ(rZ));
            newfigure.applyTransformation(newfigure.translate(Vector3D::vector(center[0], center[1], center[2])));
            double r = configuration[figure]["radius"].as_double_or_die();
            int n = configuration[figure]["n"].as_int_or_die();
            int m = configuration[figure]["m"].as_int_or_die();
            Figures3D newFigures = generateThickFigures(newfigure, r, n, m, true, true);
            for (auto fig:newFigures) {
                Lines2D newlines = fig.doProjection(Vector3D::point(eye[0], eye[1], eye[2]), 1);
                lines.insert(lines.end(), newlines.begin(), newlines.end());
                figures3D.push_back(fig);
            }
            continue;
        }
        else if (type == "Thick3DLSystem") {
            string input = configuration[figure]["inputfile"].as_string_or_die();
            L_System_3D lsystem3D(input, c);
            newfigure = lsystem3D.generateFigure();
            newfigure.applyTransformation(newfigure.scaleFigure(scale));
            newfigure.applyTransformation(newfigure.rotateX(rX));
            newfigure.applyTransformation(newfigure.rotateY(rY));
            newfigure.applyTransformation(newfigure.rotateZ(rZ));
            newfigure.applyTransformation(newfigure.translate(Vector3D::vector(center[0], center[1], center[2])));
            double r = configuration[figure]["radius"].as_double_or_die();
            int n = configuration[figure]["n"].as_int_or_die();
            int m = configuration[figure]["m"].as_int_or_die();
            Figures3D newFigures = generateThickFigures(newfigure, r, n, m, true, true);
            for (auto fig:newFigures) {
                Lines2D newlines = fig.doProjection(Vector3D::point(eye[0], eye[1], eye[2]), 1);
                lines.insert(lines.end(), newlines.begin(), newlines.end());
                figures3D.push_back(fig);
            }
            continue;
        }
        else {
            return img::EasyImage();
        }

        newfigure.applyTransformation(newfigure.scaleFigure(scale));
        newfigure.applyTransformation(newfigure.rotateX(rX));
        newfigure.applyTransformation(newfigure.rotateY(rY));
        newfigure.applyTransformation(newfigure.rotateZ(rZ));
        newfigure.applyTransformation(newfigure.translate(Vector3D::vector(center[0], center[1], center[2])));
        Lines2D newlines = newfigure.doProjection(Vector3D::point(eye[0], eye[1], eye[2]), 1);
        figures3D.push_back(newfigure);
        lines.insert(lines.end(), newlines.begin(), newlines.end());
    }

    int size = configuration["General"]["size"].as_int_or_die();
    vector<double> max = img::getMax(lines);
    lines.clear();
    vector<double> background = configuration["General"]["backgroundcolor"].as_double_tuple_or_die();

    img::EasyImage image((int) round(max[4] * size), (int) round(max[5] * size),
                         img::Color((int) round(background[0] * 255), (int) round(background[1] * 255),
                                    (int) round(background[2] * 255)));
    ZBuffer buf = ZBuffer(image.get_width(), image.get_height());

    double Xmax = max[1];
    double Xmin = max[0];
    double Ymax = max[3];
    double Ymin = max[2];
    double Xratio = max[4];
    double Yratio = max[5];
    double ImageX = size * Xratio;
    double ImageY = size * Yratio;
    double d = 0.95*(ImageX/(Xmax-Xmin));
    double DCx = d*(Xmin+Xmax)/2;
    double DCy = d*(Ymin+Ymax)/2;
    double dx = ImageX/2 - DCx;
    double dy = ImageY/2 - DCy;
    for (auto& figure:figures3D) {
        figure.draw_zbuf_triag(buf, image, d, dx, dy);
    }

    return image;
}

img::EasyImage generate_image(const ini::Configuration &configuration) {
   string imagetype = configuration["General"]["type"].as_string_or_die();
   if (imagetype == "2DLSystem") {
       return generate_2DLsystem(configuration);
   }
   else if (imagetype == "Wireframe" or imagetype == "ZBufferedWireframe") {
       return generate_Wireframe(configuration);
   }
   else if (imagetype == "ZBuffering") {
       return generate_ZBufferingDriehoeken(configuration);
   }
   return img::EasyImage();
}



int main(int argc, char const* argv[])

{
        int retVal = 0;
        try
        {
                for(int i = 1; i < argc; ++i)
                {
                        ini::Configuration conf;
                        try
                        {
                                std::ifstream fin(argv[i]);
                                fin >> conf;
                                fin.close();
                        }
                        catch(ini::ParseException& ex)
                        {
                                std::cerr << "Error parsing file: " << argv[i] << ": " << ex.what() << std::endl;
                                retVal = 1;
                                continue;
                        }

                        img::EasyImage image = generate_image(conf);
                        if(image.get_height() > 0 && image.get_width() > 0)
                        {
                                std::string fileName(argv[i]);
                                std::string::size_type pos = fileName.rfind('.');
                                if(pos == std::string::npos)
                                {
                                        //filename does not contain a '.' --> append a '.bmp' suffix
                                        fileName += ".bmp";
                                }
                                else
                                {
                                        fileName = fileName.substr(0,pos) + ".bmp";
                                }
                                try
                                {
                                        std::ofstream f_out(fileName.c_str(),std::ios::trunc | std::ios::out | std::ios::binary);
                                        f_out << image;

                                }
                                catch(std::exception& ex)
                                {
                                        std::cerr << "Failed to write image to file: " << ex.what() << std::endl;
                                        retVal = 1;
                                }
                        }
                        else
                        {
                                std::cout << "Could not generate image for " << argv[i] << std::endl;
                        }
                }
        }
        catch(const std::bad_alloc &exception)
        {
    		//When you run out of memory this exception is thrown. When this happens the return value of the program MUST be '100'.
    		//Basically this return value tells our automated test scripts to run your engine on a pc with more memory.
    		//(Unless of course you are already consuming the maximum allowed amount of memory)
    		//If your engine does NOT adhere to this requirement you risk losing points because then our scripts will
		//mark the test as failed while in reality it just needed a bit more memory
                std::cerr << "Error: insufficient memory" << std::endl;
                retVal = 100;
        }
        return retVal;
}
