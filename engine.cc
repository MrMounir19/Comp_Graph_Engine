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

img::EasyImage generate_image(const ini::Configuration &configuration) {
   bool check = false;
   Lines2D lines;
    //--------------------------------------------------------------------------------------
   if (configuration["General"]["type"].as_string_or_die() == "2DLSystem") {
       check = true;
       vector<double> color = configuration["2DLSystem"]["color"].as_double_tuple_or_die();
       string input = configuration["2DLSystem"]["inputfile"].as_string_or_die();
       L_System lSystem = L_System(input, Color(color[0],color[1],color[2]));
       lines = lSystem.generateLines();
   }
   //--------------------------------------------------------------------------------------
   else if (configuration["General"]["type"].as_string_or_die() == "Wireframe") {
       check = true;
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
           if (type == "Cube" or type == "Tetrahedron" or type == "Octahedron" or type == "Icosahedron" or type == "Dodecahedron") {
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
                                       configuration[figure]["n"].as_int_or_die(), configuration[figure]["m"].as_int_or_die());
           }
           //---------------------------
           else if (type == "3DLSystem") {
               string input = configuration[figure]["inputfile"].as_string_or_die();
               L_System_3D lsystem3D(input, c);
               newfigure = lsystem3D.generateFigure();
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
   }
   //--------------------------------------------------------------------------------------

   if (check) {
       int size = configuration["General"]["size"].as_int_or_die();
       vector<double> background = configuration["General"]["backgroundcolor"].as_double_tuple_or_die();
       vector<double> max = img::getMax(lines);
       img::EasyImage image((int) round(max[4] * size), (int) round(max[5] * size),
               img::Color((int) round(background[0] * 255), (int) round(background[1] * 255),
                                       (int) round(background[2] * 255)));
       image.draw2Dlines(lines, size, max[0], max[1], max[2], max[3]);
       return image;
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
