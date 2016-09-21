#include <iostream>
#include <string>
#include <exception>
#include "lexer.hpp"

using namespace std;

int main(int argc, char * argv[]) {
    if (argc != 2) {
        cerr << "Input only one file argument." << endl;
        return 1;
    }

    ifstream file;
    file.open(argv[1]);
    if (!file.good()) {
        cerr << "File could not be opened." << endl;
        return 1;
    }

    auto lex = Lexer(file);
    while (!file.eof()) {
        try {
            auto curr_token = lex.get_token();
            cout << curr_token << " ";
        } catch (exception& e) {
            cerr << endl << "Lexer Error: " << e.what() << endl;
            exit(1);
        }
    }

    file.close();
    return 0;
}
