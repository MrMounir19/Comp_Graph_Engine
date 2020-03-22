
#include "L_System_3D.h"

const double pi = 3.14159265358979323846264338327950288;


struct turtleSituation {
    Vector3D position;
    Vector3D H;
    Vector3D L;
    Vector3D U;
    turtleSituation(Vector3D pos, Vector3D h, Vector3D l, Vector3D u):position(pos), H(h), L(l), U(u) {}
};

L_System_3D::L_System_3D(const string input, Color linecolor) : linecolor(linecolor) {
    LParser::LSystem3D l_system;

    std::ifstream input_stream(input);
    input_stream >> l_system;
    input_stream.close();
    Alphabet = l_system.get_alphabet();
    angleD = l_system.get_angle();
    initiator = l_system.get_initiator();

    iterations = l_system.get_nr_iterations();
    for (auto symbol:Alphabet) {
        replacement.insert(make_pair(symbol, l_system.get_replacement(symbol)));
        draw.insert(make_pair(symbol, l_system.draw(symbol)));
    }
}

Figure L_System_3D::generateFigure() {
    string finalstring = initiator;
    string helpstring = "";
    for (unsigned int i=0; i<iterations; i++) {
        for (auto symbol:finalstring) {
            helpstring += replacement.at(symbol);
        }
        finalstring = helpstring;
        helpstring = "";
    }

    double angle = angleD*pi/180;
    vector<Vector3D> Points;
    vector<Face> Faces;

    int index = 0;
    Vector3D last_position = Vector3D::point(0,0,0);
    Vector3D position = Vector3D::point(0,0,0);
    vector<turtleSituation> stack;

    Vector3D H = Vector3D::vector(1,0,0);
    Vector3D L = Vector3D::vector(0,1,0);
    Vector3D U = Vector3D::vector(0,0,1);

    for (char symbol:finalstring) {
        Vector3D copyH = H;
        Vector3D copyL = L;
        if (symbol == '+') {
            addLine(Points, Faces, last_position, position, index);
            last_position = position;
            H = H*cos(angle) + L*sin(angle);
            L = -copyH*sin(angle) + L*cos(angle);
        }
        else if (symbol == '-') {
            addLine(Points, Faces, last_position, position, index);
            last_position = position;
            H = H*cos(-angle) + L*sin(-angle);
            L = -copyH*sin(-angle) + L*cos(-angle);
        }
        else if (symbol == '^') {
            addLine(Points, Faces, last_position, position, index);
            last_position = position;
            H = H*cos(angle) + U*sin(angle);
            U = -copyH*sin(angle) + U*cos(angle);
        }
        else if (symbol == '&') {
            addLine(Points, Faces, last_position, position, index);
            last_position = position;
            H = H*cos(-angle) + U*sin(-angle);
            U = -copyH*sin(-angle) + U*cos(-angle);
        }
        else if (symbol == '\\') {
            L = L*cos(angle) - U*sin(angle);
            U = copyL*sin(angle) + U*cos(angle);
        }
        else if (symbol == '/') {
            L = L*cos(-angle) - U*sin(-angle);
            U = copyL*sin(-angle) + U*cos(-angle);
        }
        else if (symbol == '|') {
            addLine(Points, Faces, last_position, position, index);
            last_position = position;
            H = -H;
            L = -L;
        }
        else if (symbol == '(') {
            stack.push_back(turtleSituation(position, H, L, U));
        }
        else if (symbol == ')') {
            addLine(Points, Faces, last_position, position, index);
            position = stack[stack.size()-1].position;
            last_position = stack[stack.size()-1].position;
            H = stack[stack.size()-1].H;
            L = stack[stack.size()-1].L;
            U = stack[stack.size()-1].U;
            stack.erase(stack.end());
        }
        else {
            if (!draw.at(symbol)) {
                addLine(Points, Faces, last_position, position, index);
                last_position = position + H;
            }
            position += H;
        }
    }
    addLine(Points, Faces, last_position, position, index);


    Figure newFigure(linecolor);
    newFigure.setPoints(Points);
    newFigure.setFaces(Faces);
    return newFigure;
}

void L_System_3D::addLine(vector<Vector3D> &points, vector<Face> &faces, Vector3D last_position, Vector3D position, int &index) {
    if (last_position.x != position.x or last_position.y != position.y or last_position.z != position.z) {
        points.push_back(last_position);
        points.push_back(position);
        faces.push_back(Face({index, index+1}));
        index+=2;
    }
}
