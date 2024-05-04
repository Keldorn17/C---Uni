#include <iostream>
#include <stack>
#include <string>
#include <cctype>

using namespace std;
int is_error;

bool isOperator(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '%' || c == '=';
}

bool isOperand(char c) {
    return isalnum(c);
}

int precedence(char op) {
    if (op == '+' || op == '-')
        return 1;
    if (op == '*' || op == '/' || op == '%')
        return 2;
    if (op == '=')
        return 3;
    return 0;
}

string infixToPostfix(const string& infix) {
    stack<char> s;
    string postfix;
    const char* errorOpening = "Hiba: Nyitó zárójel hiányzik!";
    const char* errorOperator = "Hiba: Érvénytelen operátor vagy operandus!";
    const char* errorClosing = "Hiba: Bezáró zárójel hiányzik!";

    for (char c : infix) {
        if (c == ' ')
            continue;
        if (isOperand(c)) {
            postfix += c;
        } else if (c == '(') {
            s.push(c);
        } else if (c == ')') {
            while (!s.empty() && s.top() != '(') {
                postfix += s.top();
                s.pop();
            }
            if (s.empty() || s.top() != '(') {
                is_error = 1;
                printf("%s", errorOpening);
                return postfix;
            }
            s.pop(); // Kiveszi a nyitó zárójelet a veremből
        } else if (isOperator(c)) {
            while (!s.empty() && precedence(c) <= precedence(s.top())) {
                postfix += s.top();
                s.pop();
            }
            s.push(c);
        } else {
            is_error = 1;
            printf("%s", errorOperator);
            return postfix;
        }
    }

    while (!s.empty()) {
        if (s.top() == '(') {
            is_error = 1;
            printf("%s", errorClosing);
            return postfix;
        }
        postfix += s.top();
        s.pop();
    }
    is_error = 0;
    return postfix;
}

int main() {
    setlocale(LC_ALL, "Hun");
    string infix;
    cout << "Kérem, adja meg az infix kifejezést: ";
    getline(cin, infix);

    string postfix = infixToPostfix(infix);

    if (!is_error)
        cout << "Postfix kifejezés: " << postfix;

    return 0;
}
