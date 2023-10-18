#include <iostream>
#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <ctype.h>

enum class TokenType {
    FORWARD,
    BACKWARD,
    FORWARD_HALF,
    BACKWARD_HALF,
    SQUARE,
    SQUARE_ROOT,
    DIGIT,
    SPACE
};

typedef struct {
    TokenType type;
    int value;
} Token;

struct SyntaxToken {
    TokenType type;
    char value;

    SyntaxToken(){};

    SyntaxToken(TokenType init_type, char init_value) {
        type = init_type;
        value = init_value;
    };
};

std::vector<SyntaxToken> syntaxTokens = {
    SyntaxToken(TokenType::FORWARD, '>'),
    SyntaxToken(TokenType::BACKWARD, '<'),
    SyntaxToken(TokenType::FORWARD_HALF, ';'),
    SyntaxToken(TokenType::BACKWARD_HALF, ':'),
    SyntaxToken(TokenType::SQUARE, '^'),
    SyntaxToken(TokenType::SQUARE_ROOT, '['),
    SyntaxToken(TokenType::SPACE, '|')
};

std::vector<Token> tokenize(std::string code) {
    std::vector<Token> tokens;

    for (int i = 0; i < code.length(); i++) {
        Token token;
        bool noneMatch;

        for (SyntaxToken syntaxToken: syntaxTokens) {
            if (code.at(i) != syntaxToken.value) {
                noneMatch = true;
                continue;
            }

            token.type = syntaxToken.type;

            tokens.push_back(token);
            noneMatch = false;
            break;
        }

        if (isdigit(code.at(i))) {
            token.type = TokenType::DIGIT;
            token.value = code.at(i) - '0';

            tokens.push_back(token);
            continue;
        }

        if (noneMatch) {
            std::cerr << "UNEXPECTED SYMBOL AT " << code.at(i) << " SHUTTING DOWN" << std::endl;
            exit(404);
        }
    }

    return tokens;
}

int count(std::vector<Token> tokens, TokenType type) {
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

std::vector<std::string> getAllSpaces(std::string contents) {
    std::vector<std::string> spliced;
    std::string delimiter = "|";
    int pos = 0;
    std::string token;

    while ((pos = contents.find(delimiter)) != std::string::npos) {
        token = contents.substr(0, pos);
        spliced.push_back(token);
        contents.erase(0, pos + delimiter.length());
    }

    spliced.push_back(contents);

    std::vector<std::string> vectors;

    for (std::string content : spliced) {
        vectors.push_back(content);
    }

    return vectors;
}

std::vector<int> getAllDigits(std::vector<Token> tokens) {
    std::vector<int> digits;

    for (Token token : tokens) {
        if (token.type != TokenType::DIGIT) {
            continue;
        }

        digits.push_back(token.value);
    }

    return digits;
}

double tokens_to_output(std::vector<Token> tokens) {
    double output = 0;

    int forward = count(tokens, TokenType::FORWARD);
    int backward = count(tokens, TokenType::BACKWARD);
    double forwardHalf = count(tokens, TokenType::FORWARD_HALF) / 2.0;
    double backwardHalf = count(tokens, TokenType::BACKWARD_HALF) / 2.0;
    int square = count(tokens, TokenType::SQUARE);
    int squareRoot = count(tokens, TokenType::SQUARE_ROOT);

    output = (forward + forwardHalf) - (backward + backwardHalf);

    if (square > 0) {
        output = pow(output, square);
    }

    if (squareRoot > 0) {
        output = sqrt(output);
    }

    if (getAllDigits(tokens).size() > 0) {
        std::vector<int> digits = getAllDigits(tokens);

        for (int i : digits) {
            output += i;
        }
    }

    return output;
}

int main(int argc, char * argv[]) {
    std::string contents;
    std::ifstream inputStream("code.abd");

    if (!inputStream) {
        std::cerr << "Couldn't open the code.abd file." << std::endl;
        exit(404);
    }

    std::string line;

    while (getline(inputStream, line)) {
        contents += line;
    }

    std::vector<std::string> spaces = getAllSpaces(contents);

    if (spaces.size() >= 2) {
        int vectorCount = 0;

        for (std::string str : spaces) {
            vectorCount++;

            std::cout << "(Part " << vectorCount << ") Output (char): " << (char) tokens_to_output(tokenize(str)) << std::endl;
            std::cout << "(Part " << vectorCount << ") Output (int): " << tokens_to_output(tokenize(str)) << std::endl;
        }

        return 0;
    }

    const std::vector<Token> result = tokenize(contents);

    std::cout << "Output (char): " << (char) tokens_to_output(result) << std::endl;
    std::cout << "Output (int): " << tokens_to_output(result) << std::endl;

    return 0;
}
