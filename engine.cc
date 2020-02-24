#include "easy_image.h"
#include "ini_configuration.h"
#include "L_System.h"
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <string>
#include <cmath>

img::EasyImage generate_image(const ini::Configuration &configuration)
{
   if (configuration["General"]["type"].as_string_or_die() == "2DLSystem") {
       int size = configuration["General"]["size"].as_int_or_die();
       std::vector<double> background = configuration["General"]["backgroundcolor"].as_double_tuple_or_die();
       std::vector<double> color = configuration["2DLSystem"]["color"].as_double_tuple_or_die();
       std::string input = configuration["2DLSystem"]["inputfile"].as_string_or_die();
       img::EasyImage image(size, size);
       for (int i = 0; i < size; i++) {
           for (int j = 0; j < size; j++) {
               image(i, j).red = (int)round(background[0]*255);
               image(i, j).green = (int)round(background[1]*255);
               image(i, j).blue = (int)round(background[2]*255);
           }
       }
       L_System lSystem = L_System(input, Color(color[0],color[1],color[2]));
       image.draw2Dlines(lSystem.generateLines(), size);
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
