using namespace std;
#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>

typedef enum class TokenType {
    FORWARD,
    BACKWARD
};

typedef struct {
    TokenType type;
} Token;

vector<Token> tokenize(string code) {
    vector<Token> tokens;

    for (int i = 0; i < code.length(); i++) {
        if (code.at(i) == '>') {
            Token token;
            token.type = TokenType::FORWARD;

            tokens.push_back(token);
            continue;
        }

        if (code.at(i) == '<') {
            Token token;
            token.type = TokenType::BACKWARD;

            tokens.push_back(token);
            continue;
        }

        cerr << "UNEXPECTED SYMBOL AT " << code.at(i) << " SHUTTING DOWN" << endl;
        exit(404);
    }

    return tokens;
}

int tokens_to_output(vector<Token> tokens) {
    int output = 0;

    int forward = count_if(tokens.begin(), tokens.end(), [](Token a) { return a.type == TokenType::FORWARD; });
    int backward = count_if(tokens.begin(), tokens.end(), [](Token a) { return a.type == TokenType::BACKWARD; });

    output += forward;
    output -= backward;

    return output;
}

int main(void) {
    string contents;
    ifstream inputStream("code.abd");

    if (!inputStream) {
        cerr << "Couldn't open the code.abd file." << endl;
        exit(404);
    }

    string line;

    while (getline(inputStream, line)) {
        contents += line;
    }

    const vector<Token> result = tokenize(contents);

    cout << "Output (char): " << (char) tokens_to_output(result) << endl;
    cout << "Output (int): " << tokens_to_output(result) << endl;

    return 0;
}