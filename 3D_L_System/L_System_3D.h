

#ifndef ENGINE_L_SYSTEM_3D_H
#define ENGINE_L_SYSTEM_3D_H

#include "../2D_Lines/line_structs.cpp"
#include <set>
#include <string>
#include <map>
#include "../Figure/Figure.h"
#include <fstream>
#include "../L2D_Parser/l_parser.h"
#include <cmath>

using namespace std;

class L_System_3D {
private:
    set<char> Alphabet;
    map<char, string> replacement = {make_pair('+',"+"), make_pair('-', "-"), make_pair('(',"("), make_pair(')',")"),
                                     make_pair('^',"^"), make_pair('&', "&"), make_pair('\\', "\\"), make_pair('/',"/"), make_pair('|',"|")};
    map<char, bool> draw;
    double angleD;
    string initiator;
    unsigned int iterations;
    Color linecolor;
public:
    L_System_3D(const string input, Color linecolor);
    Figure generateFigure();
    void addLine(vector<Vector3D> &points, vector<Face>& faces, Vector3D last_position, Vector3D position, int& index);
};


#endif //ENGINE_L_SYSTEM_3D_H
