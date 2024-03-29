
#include <fstream>
#include "L_System.h"
#include "../L2D_Parser/l_parser.h"
#include <cmath>

const double pi = 3.14159265358979323846264338327950288;
L_System::L_System(const string input, Color linecolor) : linecolor(linecolor) {
    LParser::LSystem2D l_system;

    std::ifstream input_stream(input);
    input_stream >> l_system;
    input_stream.close();
    Alphabet = l_system.get_alphabet();
    angle = l_system.get_angle();
    initiator = l_system.get_initiator();
    S_angle = l_system.get_starting_angle();

    iterations = l_system.get_nr_iterations();
    for (auto symbol:Alphabet) {
        replacement.insert(make_pair(symbol, l_system.get_replacement(symbol)));
        draw.insert(make_pair(symbol, l_system.draw(symbol)));
    }
}

const Lines2D L_System::generateLines() {
    string finalstring = initiator;
    string helpstring = "";
    double current_angle = S_angle;
    for (unsigned int i=0; i<iterations; i++) {
        for (auto symbol:finalstring) {
            helpstring += replacement.at(symbol);
        }
        finalstring = helpstring;
        helpstring = "";
    }
    pair<double, double> last_change = make_pair(0,0);
    pair<double, double> position = make_pair(0,0);
    Lines2D lines;
    vector<pair<pair<double, double>,double>> stack;
    for (auto symbol:finalstring) {
        if (symbol=='+') {
            if (last_change.first != position.first or last_change.second != position.second) {
            lines.push_back(Line2D(Point2D(last_change.first, last_change.second), Point2D(position.first, position.second), linecolor,0,0));}
            current_angle += angle;
            last_change.first = position.first;
            last_change.second = position.second;
    }
        else if (symbol=='-') {
            if (last_change.first != position.first or last_change.second != position.second) {
            lines.push_back(Line2D(Point2D(last_change.first, last_change.second), Point2D(position.first, position.second), linecolor,0,0)); }
            current_angle -= angle;
            last_change.first = position.first;
            last_change.second = position.second;
        }
        else if (symbol=='(') {
            stack.push_back(make_pair(position, current_angle));
        }
        else if (symbol==')') {
            if (last_change.first != position.first or last_change.second != position.second) {
                lines.push_back(Line2D(Point2D(last_change.first, last_change.second), Point2D(position.first, position.second), linecolor,0,0)); }
            current_angle = stack[stack.size()-1].second;
            position = stack[stack.size()-1].first;
            last_change = position;
            stack.erase(stack.end());
    }
        else {
            if (!draw.at(symbol)) {
                if (last_change.first != position.first or last_change.second != position.second) {
                lines.push_back(Line2D(Point2D(last_change.first, last_change.second), Point2D(position.first, position.second), linecolor,0,0)); }
                last_change = position;
                last_change.first += cos(current_angle*pi/180.0);
                last_change.second += sin(current_angle*pi/180.0);
            }
            position.first += cos(current_angle*(pi/180.0));
            position.second += sin(current_angle*(pi/180.0));
        }
        lines.push_back(Line2D(Point2D(last_change.first, last_change.second), Point2D(position.first, position.second), linecolor,0,0));
    }
    return lines;
}