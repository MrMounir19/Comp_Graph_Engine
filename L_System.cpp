//
// Created by mounir on 24.02.20.
//
#include <fstream>
#include "L_System.h"
#include "L2D_Parser/l_parser.h"
#include <cmath>

L_System::L_System(const string input, Color linecolor) : linecolor(linecolor) {
    LParser::LSystem2D l_system;

    std::ifstream input_stream(input);
    input_stream >> l_system;
    input_stream.close();
    Alphabet = l_system.get_alphabet();
    angle = l_system.get_angle();
    initiator = l_system.get_initiator();
    S_angle = l_system.get_starting_angle();
    current_angle = S_angle;
    iterations = l_system.get_nr_iterations();
    for (auto symbol:Alphabet) {
        replacement.insert(make_pair(symbol, l_system.get_replacement(symbol)));
        draw.insert(make_pair(symbol, l_system.draw(symbol)));
    }
}

const Lines2D L_System::generateLines() {
    string finalstring = initiator;
    string helpstring = "";

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
    for (auto symbol:finalstring) {
        if (symbol=='+') {
            if (last_change.first != position.first or last_change.second != position.second) {
            lines.push_back(Line2D(Point2D(last_change.first, last_change.second), Point2D(position.first, position.second), linecolor));}
            current_angle = fmod(current_angle+angle,365);
            last_change.first = position.first;
            last_change.second = position.second;
    }
        else if (symbol=='-') {
            if (last_change.first != position.first or last_change.second != position.second) {
            lines.push_back(Line2D(Point2D(last_change.first, last_change.second), Point2D(position.first, position.second), linecolor)); }
            current_angle = fmod(current_angle-angle, 365);
            last_change.first = position.first;
            last_change.second = position.second;
        }
        else {
            if (!draw.at(symbol)) {
                if (last_change.first != position.first or last_change.second != position.second) {
                lines.push_back(Line2D(Point2D(last_change.first, last_change.second), Point2D(position.first, position.second), linecolor)); }
                last_change.first += cos(current_angle*M_PI/180.0);
                last_change.second += sin(current_angle*M_PI/180.0);
            }
            position.first += cos(angle*M_PI/180.0);
            position.second += sin(angle*M_PI/180.0);
        }
    }
    return lines;
}