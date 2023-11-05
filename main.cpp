#include "ProjectAED/script.h"
#include <filesystem>
using namespace std;
bool color_mode = false;

int main() {
    cout << "1- Import original dataset 2- Use current dataset\n";
    string option;
    cin >> option;
    while(option != "1" && option != "2"){
        cout << "Invalid Input, please try again\n";
        cin >> option;
    }
    Script s = Script(option == "2");
    s.run();
    return 0;
}

