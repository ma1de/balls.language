using namespace std;
#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <cctype>

enum class TokenType {
    FORWARD,
    BACKWARD,
    FORWARD_HALF,
    BACKWARD_HALF,
    SQUARE,
    SQUARE_ROOT,
    DIGIT,
    DIVIDE,
    SPACE
};

typedef struct {
    TokenType type;
    int value;
} Token;

struct SyntaxToken {
    TokenType type;
    char value;

    SyntaxToken(TokenType init_type, char init_value) {
        type = init_type;
        value = init_value;
    };
};

vector<SyntaxToken> syntaxTokens = {
    SyntaxToken(TokenType::FORWARD, '>'),
    SyntaxToken(TokenType::BACKWARD, '<'),
    SyntaxToken(TokenType::FORWARD_HALF, ';'),
    SyntaxToken(TokenType::BACKWARD_HALF, ':'),
    SyntaxToken(TokenType::SQUARE, '^'),
    SyntaxToken(TokenType::SQUARE_ROOT, '['),
    SyntaxToken(TokenType::DIVIDE, '/'),
    SyntaxToken(TokenType::SPACE, ' ')
};

vector<Token> tokenize(const string& code) {
    vector<Token> tokens;

    for (char i : code) {
        Token token;
        bool noneMatch;

        for (SyntaxToken syntaxToken: syntaxTokens) {
            if (i != syntaxToken.value) {
                noneMatch = true;
                continue;
            }

            token.type = syntaxToken.type;

            tokens.push_back(token);
            noneMatch = false;
            break;
        }

        if (isdigit(i)) {
            token.type = TokenType::DIGIT;
            token.value = i - '0';

            tokens.push_back(token);
            continue;
        }

        if (noneMatch) {
            cerr << "UNEXPECTED SYMBOL AT " << i << " SHUTTING DOWN" << endl;
            exit(404);
        }
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
        case TokenType::DIVIDE:
            return count_if(tokens.begin(), tokens.end(), [](Token a) { return a.type == TokenType::DIVIDE; });
        default:
            return 0;
    }
} 

vector<string> getAllSpaces(string contents) {
    vector<string> spliced;
    string delimiter = " ";
    int pos = 0;
    string token;

    while ((pos = contents.find(delimiter)) != string::npos) {
        token = contents.substr(0, pos);
        spliced.push_back(token);
        contents.erase(0, pos + delimiter.length());
    }

    spliced.push_back(contents);

    vector<string> vectors;

    vectors.reserve(spliced.size());
    for (const string& content : spliced) {
        vectors.push_back(content);
    }

    return vectors;
}

vector<int> getAllDigits(const vector<Token>& tokens) {
    vector<int> digits;

    for (Token token : tokens) {
        if (token.type != TokenType::DIGIT) {
            continue;
        }

        digits.push_back(token.value);
    }

    return digits;
}

double tokens_to_output(const vector<Token>& tokens) {
    double output;

    int forward = count(tokens, TokenType::FORWARD);
    int backward = count(tokens, TokenType::BACKWARD);
    double forwardHalf = count(tokens, TokenType::FORWARD_HALF) / 2.0;
    double backwardHalf = count(tokens, TokenType::BACKWARD_HALF) / 2.0;
    int square = count(tokens, TokenType::SQUARE);
    int squareRoot = count(tokens, TokenType::SQUARE_ROOT);
    int divide = count(tokens, TokenType::DIVIDE);

    output = (forward + forwardHalf) - (backward + backwardHalf);

    if (square > 0) {
        output = pow(output, square);
    }

    if (squareRoot > 0) {
        output = sqrt(output);
    }

    if (!getAllDigits(tokens).empty()) {
        vector<int> digits = getAllDigits(tokens);

        for (int i : digits) {
            output += i;
        }
    }

    if (divide > 0) {
        output /= 2;
    }

    return output;
}

bool hasSuffix(const string& a, const string& suffix) {
    return a.size() >= suffix.size() && a.compare(a.size() - suffix.size(), suffix.size(), suffix) == 0;
}

int main([[maybe_unused]] int argc, char * argv[]) {
    cout << "[]==========================[]" << endl;
    cout << "[  BALLS.LANGUAGE COMPILER   ]" << endl;
    cout << "[]==========================[]" << endl;
    cout << "" << endl;

    string fileName = "code.abd";

    if (argv[1] != nullptr) {
        fileName = argv[1];
    }

    if (!hasSuffix(fileName, ".abd")) {
        cerr << "Only files that have the `.abd` extension are allowed." << endl;
        exit(404);
    }

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

    vector<string> spaces = getAllSpaces(contents);

    if (spaces.size() >= 2) {
        int vectorCount = 0;

        for (const string& str : spaces) {
            vectorCount++;

            cout << "(Part " << vectorCount << ") Output (int): " << tokens_to_output(tokenize(str)) << endl;
        }

        return 0;
    }

    const vector<Token> result = tokenize(contents);

    cout << "Output (int): " << tokens_to_output(result) << endl;

    return 0;
}