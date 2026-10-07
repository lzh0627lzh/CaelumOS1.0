#include <cmath>
#include <iostream>
#include <windows.h>
#include <sstream>
#include <stack>
#include <string>
#include <vector>

int get_priority(const std::string &op) {
    if (op == "(")
        return 0;
    if (op == "+" || op == "-")
        return 1;
    if (op == "*" || op == "/")
        return 2;
    return -1;
}

bool is_operator(const std::string &s) {
    return s == "+" || s == "-" || s == "*" || s == "/";
}

bool is_bracket(const std::string &s) {
    return s == "(" || s == ")";
}

bool infix_to_rpn(const std::vector<std::string> &infix, std::vector<std::string> &rpn) {
    std::stack<std::string> st;
    for (const auto &token : infix) {
        if (!is_operator(token) && !is_bracket(token)) {
            rpn.push_back(token);
        } else if (token == "(") {
            st.push(token);
        } else if (token == ")") {
            while (!st.empty() && st.top() != "(") {
                rpn.push_back(st.top());
                st.pop();
            }
            if (st.empty()) {
                std::cerr << "[错误] 括号不匹配!\n";
                return false;
            }
            st.pop();
        } else if (is_operator(token)) {
            while (!st.empty() && is_operator(st.top()) && get_priority(st.top()) >= get_priority(token)) {
                rpn.push_back(st.top());
                st.pop();
            }
            st.push(token);
        }
    }
    while (!st.empty()) {
        if (st.top() == "(") {
            std::cerr << "[错误] 括号不匹配!\n";
            return false;
        }
        rpn.push_back(st.top());
        st.pop();
    }
    return true;
}

double eval_rpn(const std::vector<std::string> &rpn) {
    std::stack<double> st;
    for (auto &t : rpn) {
        if (!is_operator(t)) {
            double val;
            std::istringstream iss(t);
            iss >> val;
            st.push(val);
        } else {
            if (st.size() < 2) {
                std::cerr << "[错误] 表达式语法错误\n";
                return NAN;
            }
            double b = st.top();
            st.pop();
            double a = st.top();
            st.pop();
            double res;
            if (t == "+")
                res = a + b;
            else if (t == "-")
                res = a - b;
            else if (t == "*")
                res = a * b;
            else if (t == "/") {
                if (fabs(b) < 1e-12) {
                    std::cerr << "[错误] 除零错误!\n";
                    return NAN;
                }
                res = a / b;
            } else
                return NAN;
            st.push(res);
        }
    }
    if (st.size() != 1) {
        std::cerr << "[错误] 无效表达式\n";
        return NAN;
    }
    return st.top();
}

bool math_evaluate(const std::vector<std::string> &tokens, double &out_result) {
    std::vector<std::string> rpn;
    if (!infix_to_rpn(tokens, rpn))
        return false;
    double ans = eval_rpn(rpn);
    if (std::isnan(ans))
        return false;
    out_result = ans;
    return true;
}

int main() {
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
    std::cout << "==== Caelum 计算器 ====\n";
    std::cout << "输入数学算式，输入 exit 或 quit 退出程序。\n";
    std::cout << "提示：括号两侧必须加空格！示例：( 1 + 2 ) * 5\n\n";

    std::string line;
    while (true) {
        std::cout << "calc> ";
        std::getline(std::cin, line);

        if (line == "quit" || line == "exit") {
            std::cout << "正在退出计算器。\n";
            break;
        }

        std::vector<std::string> tokens;
        std::istringstream iss(line);
        std::string temp;
        while (iss >> temp) {
            tokens.push_back(temp);
        }
        if (tokens.empty())
            continue;

        double result;
        if (math_evaluate(tokens, result)) {
            std::cout << "= " << result << "\n";
        }
    }
    return 0;
}
