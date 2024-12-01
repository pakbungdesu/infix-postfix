
#include <iostream>
#include <string>
using namespace std;

struct nodePos {
    string data;
    nodePos* next;
};

struct nodeIn {
    char oper;
    nodeIn* link;
};

void PushPos(nodePos*& top, string data) {
    nodePos* p = new nodePos;
    p->data = data;
    p->next = top;
    top = p;
}

void PopPos(nodePos*& top, string& data) {
    if (top == NULL) {
        cout << "Stack under flow" << endl;
    }
    else {
        data = top->data;
        nodePos* p = top;
        top = top->next;
        delete p;
    }
}

void postfixToInfix(string postfix, string& infix) {
    nodePos* top = NULL;
    int len = postfix.length();

    for (int i = 0; i < len; i++) {
        char ch = postfix.at(i);
        if (isalnum(ch)) { // Check for alphanumeric characters (operands)
            PushPos(top, string(1, ch)); // Create a single-character string
        }
        else if (ch == '%' || ch == '/' || ch == '*' || ch == '+' || ch == '-') {
            if (top == NULL || top->next == NULL) {
                cout << "Invalid postfix expression (missing operands)" << endl;
                return; // Indicate error
            }

            string operand2, operand1;
            PopPos(top, operand2);
            PopPos(top, operand1);

            // Build infix expression using popped operands and operator
            string infix = "(" + operand1 + ch + operand2 + ")";
            PushPos(top, infix); // Push the entire infix string
        }
        else {
            // Handle invalid characters (optional)
            cout << "Invalid character in postfix expression: " << ch << endl;
            return; // Indicate error
        }
    }

    if (top == NULL || top->next != NULL) {
        cout << "Invalid postfix expression (extra operators or missing operands)" << endl;
        return; // Indicate error
    }

    infix = top->data;
}

void PushIn(nodeIn*& top, char oper1) {
    nodeIn* p;
    p = new nodeIn;
    p->oper = oper1;
    p->link = NULL;

    if (top == NULL) {
        top = p;
    }
    else {
        p->link = top;
        top = p;
    }
}



void PopIn(nodeIn*& top, char& oper1) {
    nodeIn* p;

    if (top == NULL) {
        cout << "Stack under flow";
    }
    else {
        oper1 = top->oper;
        p = top;
        top = top->link;
        delete(p);
    }
}



int ICP(char oper1) {
    if (oper1 == '*' || oper1 == '/' || oper1 == '%') {
        return 3;
    }
    else if (oper1 == '+' || oper1 == '-') {
        return 2;
    }
    else if (oper1 == ')') {
        return 1;
    }
}



int ISP(nodeIn* top) {
    switch (top->oper) {
    case '*':case '/':case '%': return 3;
    case '+':case '-': return 2;
    case '(': return 1;

    }
}


void infixToPostfix(string infix, string& postfix) {
    nodeIn* top = NULL;
    int len = infix.length();
    char oper1;

    for (int i = 0; i < len; i++) {
        if (infix.at(i) >= 'A' && infix.at(i) <= 'Z' || infix.at(i) >= 'a' && infix.at(i) <= 'z') {
            postfix += infix.at(i);
        }
        else if (top == NULL || infix.at(i) == '(' || ICP(infix.at(i)) > ISP(top)) {
            PushIn(top, infix.at(i));
        }
        else if (infix.at(i) == ')') {
            PopIn(top, oper1);
            while (top != NULL && oper1 != '(') {
                postfix += oper1;
                PopIn(top, oper1);
            }
        }
        else {
            while (top != NULL && ICP(infix.at(i)) <= ISP(top)) {
                PopIn(top, oper1);
                postfix += oper1;
            }
            PushIn(top, infix.at(i));
        }
    }

    while (top != NULL) {
        PopIn(top, oper1);
        postfix += oper1;
    }
}


int main() {
    int opt;
    string input, result;
    void (*pf[])(string, string&) = { postfixToInfix, infixToPostfix };
    cout << "1 - Postfix to Infix\n2 - Infix to Postfix\nInsert a number of direction (1-2): ";
    cin >> opt;

    cout << "Insert input: "; cin >> input;
    pf[opt - 1](input, result);
    if (!result.empty()) {
        cout << "Result: " << result << endl;
    }

    return 0;
}
