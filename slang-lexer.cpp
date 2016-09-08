#include <iostream>
#include <string>
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
        cerr << "File could not be open." << endl;
        return 1;
    }

    auto lex = Lexer(file);
    while (!file.eof()) {
        auto curr_token = lex.get_token();
        cout << curr_token.as_string() << " ";
    }

    file.close();
    return 0;
}
