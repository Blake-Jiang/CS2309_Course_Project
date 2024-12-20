#include <iostream>
#include <vector>
#include <cmath>
#include <string>
#include <iomanip>
#include <sstream>

using namespace std;

// 常量：用于判断浮点数的误差
const double EPSILON = 1e-6;

// 定义运算符
vector<char> operators = {'+', '-', '*', '/'};

// 检查是否接近 24
bool isCloseTo24(double value) {
    return fabs(value - 24) < EPSILON;
}

// 添加表达式结构体
struct Expression {
    string expr;
    double value;
    Expression(string e, double v) : expr(e), value(v) {}
};

// 修改calculate函数，接收完整的表达式而不是数字
double calculate(double a, double b, char op, const string& expr1, const string& expr2, string& result_expr) {
    // 构建新表达式，组合两个子表达式
    result_expr = "(" + expr1 + " " + op + " " + expr2 + ")";
    
    switch (op) {
        case '+': return a + b;
        case '-': return a - b;
        case '*': return a * b;
        case '/': return (b != 0) ? a / b : 1e9;
        default: return 1e9;
    }
}

// 修改solve24函数
bool solve24(vector<double>& nums, vector<string>& exprs, string& solution) {
    if (nums.size() == 1) {
        if (isCloseTo24(nums[0])) {
            solution = exprs[0];
            return true;
        }
        return false;
    }

    for (size_t i = 0; i < nums.size(); ++i) {
        for (size_t j = i + 1; j < nums.size(); ++j) {
            double num1 = nums[i];
            double num2 = nums[j];
            string expr1 = exprs[i];
            string expr2 = exprs[j];

            vector<double> rest_nums;
            vector<string> rest_exprs;
            for (size_t k = 0; k < nums.size(); ++k) {
                if (k != i && k != j) {
                    rest_nums.push_back(nums[k]);
                    rest_exprs.push_back(exprs[k]);
                }
            }

            for (char op : operators) {
                string new_expr;
                double result = calculate(num1, num2, op, expr1, expr2, new_expr);
                
                rest_nums.push_back(result);
                rest_exprs.push_back(new_expr);

                if (solve24(rest_nums, rest_exprs, solution)) {
                    return true;
                }

                rest_nums.pop_back();
                rest_exprs.pop_back();
            }
        }
    }
    return false;
}

// 修改main函数，支持扑克牌输入
int main() {
    vector<double> nums(4);
    vector<string> exprs(4);
    string input;
    
    cout << "Enter 4 cards (A,2-10,J,Q,K): ";
    for (int i = 0; i < 4; ++i) {
        cin >> input;
        // 转换扑克牌输入
        if (input == "A") {
            nums[i] = 1;
        } else if (input == "J") {
            nums[i] = 11;
        } else if (input == "Q") {
            nums[i] = 12;
        } else if (input == "K") {
            nums[i] = 13;
        } else {
            nums[i] = stod(input);
        }
        exprs[i] = input;
    }

    string solution;
    if (solve24(nums, exprs, solution)) {
        cout << "Yes, it is possible to get 24." << endl;
        cout << "Solution: " << solution << " = 24" << endl;
    } else {
        cout << "No, it is not possible to get 24." << endl;
    }

    return 0;
}
