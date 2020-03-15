//
// Created by mounir on 24.02.20.
//

using namespace std;
#ifndef ENGINE_L_SYSTEM_H
#define ENGINE_L_SYSTEM_H
#include "../line_structs.cpp"
#include <set>
#include <string>
#include <map>

class L_System {
    set<char> Alphabet;
    map<char, string> replacement = {make_pair('+',"+"), make_pair('-', "-"), make_pair('(',"("), make_pair(')',")")};
    map<char, bool> draw;
    double angle;
    double S_angle; //start angle
    string initiator;
    unsigned int iterations;
    Color linecolor;
public:
    L_System(const string input, Color linecolor);
    const Lines2D generateLines();

};

#endif //ENGINE_L_SYSTEM_H
