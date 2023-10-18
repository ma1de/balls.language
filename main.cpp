using namespace std;
#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <cmath>

#define ULL unsigned long long

typedef enum class TokenType {
    FORWARD,
    BACKWARD,
    FORWARD_HALF,
    BACKWARD_HALF,
    SQUARE,
    SQUARE_ROOT
};

typedef struct {
    TokenType type;
} Token;

vector<Token> tokenize(string code) {
    vector<Token> tokens;

    for (int i = 0; i < code.length(); i++) {
        Token token;

        if (code.at(i) == '>') {
            token.type = TokenType::FORWARD;

            tokens.push_back(token);
            continue;
        }

        if (code.at(i) == '<') {
            token.type = TokenType::BACKWARD;

            tokens.push_back(token);
            continue;
        }

        if (code.at(i) == ';') {
            token.type = TokenType::FORWARD_HALF;

            tokens.push_back(token);
            continue;
        }

        if (code.at(i) == ':') {
            token.type = TokenType::BACKWARD_HALF;

            tokens.push_back(token);
            continue;
        }

        if (code.at(i) == '^') {
            token.type = TokenType::SQUARE;

            tokens.push_back(token);
            continue;
        }

        if (code.at(i) == '[') {
            token.type = TokenType::SQUARE_ROOT;

            tokens.push_back(token);
            continue;
        }

        cerr << "UNEXPECTED SYMBOL AT " << code.at(i) << " SHUTTING DOWN" << endl;
        exit(404);
    }

    return tokens;
}

int count(vector<Token> tokens, TokenType type) {
    switch (type) {
        case TokenType::FORWARD:
            return count_if(tokens.begin(), tokens.end(), [](Token a) { return a.type == TokenType::FORWARD; });
        case TokenType::BACKWARD:
            return count_if(tokens.begin(), tokens.end(), [](Token a) { return a.type == TokenType::BACKWARD; });
        case TokenType::FORWARD_HALF:
            return count_if(tokens.begin(), tokens.end(), [](Token a) { return a.type == TokenType::FORWARD_HALF; });
        case TokenType::BACKWARD_HALF:
            return count_if(tokens.begin(), tokens.end(), [](Token a) { return a.type == TokenType::BACKWARD_HALF; });
        case TokenType::SQUARE:
            return count_if(tokens.begin(), tokens.end(), [](Token a) { return a.type == TokenType::SQUARE; });
        case TokenType::SQUARE_ROOT:
            return count_if(tokens.begin(), tokens.end(), [](Token a) { return a.type == TokenType::SQUARE_ROOT; });
        default:
            return 0;
    }
} 

ULL tokens_to_output(vector<Token> tokens) {
    int output = 0;

    int forward = count(tokens, TokenType::FORWARD);
    int backward = count(tokens, TokenType::BACKWARD);
    int forwardHalf = count(tokens, TokenType::FORWARD_HALF) / 2;
    int backwardHalf = count(tokens, TokenType::BACKWARD_HALF) / 2;
    int square = count(tokens, TokenType::SQUARE);
    int squareRoot = count(tokens, TokenType::SQUARE_ROOT);

    output = (forward + forwardHalf) - (backward + backwardHalf);

    if (square > 0) {
        output = pow(output, square);
    }

    if (squareRoot > 0) {
        output = sqrt(output);
    }

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