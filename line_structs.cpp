
#include <vector>
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

/*
    if (configuration["General"]["type"].as_string_or_die() == "IntroColorRectangle") {
        int height = configuration["ImageProperties"]["width"].as_int_or_die();
        int width =  configuration["ImageProperties"]["width"].as_int_or_die();
        img::EasyImage image(width, height);
        for (int i = 0; i < width; i++) {
            for (int j = 0; j < height; j++) {
                image(i, j).red = i;
                image(i, j).green = j;
                image(i, j).blue = (i + j) % height;
            }
        }
        return image;
    }
    else if (configuration["General"]["type"].as_string_or_die() == "IntroBlocks") {
        int height = configuration["ImageProperties"]["height"].as_int_or_die();
        int width = configuration["ImageProperties"]["width"].as_int_or_die();
        int nrX = configuration["BlockProperties"]["nrXBlocks"].as_int_or_die();
        int nrY = configuration["BlockProperties"]["nrYBlocks"].as_int_or_die();
        bool invert = configuration["BlockProperties"]["invertColors"].as_bool_or_die();
        img::EasyImage image(width, height);
        double Wb = (double)width / (double) nrX;
        double Wh = (double)height / (double) nrY;
        int Bx;
        int By;
        std::vector<double> white = configuration["BlockProperties"]["colorWhite"].as_double_tuple_or_die();
        std::vector<double> black = configuration["BlockProperties"]["colorBlack"].as_double_tuple_or_die();
        double wred = white[0]*(double)255;
        double wgreen = white[1]*(double)255;
        double wblue = white[2]*(double)255;
        double bred = black[0]*(double)255;
        double bgreen = black[1]*(double)255;
        double bblue = black[2]*(double)255;
        for (int x = 0; x < width; x++) {
            for (int y = 0; y < height; y++) {
                Bx = (int) (x/Wb);
                By = (int) (y/Wh);
                if (((Bx+By)%2 == 0 and !invert) or ((Bx+By)%2 == 1 and invert)) {
                    image(x,y).red = (int) wred;
                    image(x,y).green = (int) wgreen;
                    image(x,y).blue = (int) wblue;
                }
                else {
                    image(x,y).red = (int) bred;
                    image(x,y).green = (int) bgreen;
                    image(x,y).blue = (int) bblue;
                }
            }
        }
        return image;
    }
    else if (configuration["General"]["type"].as_string_or_die() == "IntroLines") {
        int height = configuration["ImageProperties"]["height"].as_int_or_die();
        int width = configuration["ImageProperties"]["width"].as_int_or_die();
        int nrLines = configuration["LineProperties"]["nrLines"].as_int_or_die();
        std::vector<double> background = configuration["LineProperties"]["backgroundcolor"].as_double_tuple_or_die();
        std::vector<double> lineColor = configuration["LineProperties"]["lineColor"].as_double_tuple_or_die();
        std::string type = configuration["LineProperties"]["figure"].as_string_or_die();
        img::EasyImage image(width, height);
        double Hs = (double)height/(double)nrLines;
        double Ws = (double)width/(double)nrLines;

        for (int i = 0; i < width; i++) {
            for (int j = 0; j < height; j++) {
                image(i, j).red = (int)(background[0]*255);
                image(i, j).green = (int)(background[1]*255);
                image(i, j).blue = (int)(background[2]*255);
            }
        }
        if (type == "QuarterCircle") {
            for (int x = 0; x < nrLines; x++) {
                image.draw_line(x * Ws, height - 1, 0, x * Hs,
                                img::Color(lineColor[0] * 255, lineColor[1] * 255, lineColor[2] * 255));
            }
            return image;
        }
        else if (type == "Diamond") {
            for (int x = 0; x < nrLines; x++) {
                image.draw_line((x*Ws)/2+width/2, height/2, width/2, x*Hs/2, img::Color(lineColor[0] * 255, lineColor[1] * 255, lineColor[2] * 255));
                image.draw_line(-(x*Ws)/2+width/2, height/2, width/2, x*Hs/2, img::Color(lineColor[0] * 255, lineColor[1] * 255, lineColor[2] * 255));
                image.draw_line(-(x*Ws)/2+width/2, height/2, width/2, height-1-x*Hs/2, img::Color(lineColor[0] * 255, lineColor[1] * 255, lineColor[2] * 255));
                image.draw_line((x*Ws)/2+width/2, height/2, width/2, height-1-x*Hs/2, img::Color(lineColor[0] * 255, lineColor[1] * 255, lineColor[2] * 255));
            }
            return image;
        }
    }
    */