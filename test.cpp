using namespace std;
#include <string>
#include <cstdlib>
int main() {
	for (int i=1; i <= 9; i++) {
	string call = "Comp_Graph_Engine/engine wireframes00" + to_string(i) + ".ini";
	system(call.c_str()); 	
        }
	for (int i=10; i <= 94; i++) {
	string call = "Comp_Graph_Engine/engine wireframes0" + to_string(i) + ".ini";
	system(call.c_str());	
        }
	return 0;
}
