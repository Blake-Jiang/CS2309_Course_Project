#include <iostream>
#include <vector>
#include <cmath>
#include <string>
#include <iomanip>
#include <sstream>
#include <fstream>

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

// 添加新的函数：处理单行数据
bool processLine(const string& line, string& result, string& solution) {
    vector<double> nums(4);
    vector<string> exprs(4);
    stringstream ss(line);
    string input;
    int i = 0;
    
    while (ss >> input && i < 4) {
        // 转换扑克牌输入
        if (input == "A" || input == "1") {
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
        i++;
    }

    bool success = solve24(nums, exprs, solution);
    result = (success ? "+ " : "- ") + line;
    return success;
}

// 添加新函数：处理控制台输入
void processConsoleInput() {
    string input;
    cout << "Please enter 4 numbers (A, 2-10, J, Q, K), separated by spaces:" << endl;
    getline(cin, input);
    
    string result, solution;
    bool success = processLine(input, result, solution);
    
    cout << (success ? "Solution found!\n" : "No solution found.\n");
    if (success) {
        cout << "Expression: " << solution << " = 24" << endl;
    }
}

// 添加新函数：处理文件输入
void processFileInput() {
    ifstream inFile("test.txt");
    ofstream outFile("test_result.txt");
    
    if (!inFile.is_open()) {
        cout << "Cannot open input file!" << endl;
        return;
    }

    string line;
    int totalCount = 0;
    int successCount = 0;
    
    while (getline(inFile, line)) {
        if (line.empty()) continue;
        
        string result, solution;
        bool success = processLine(line, result, solution);
        
        if (success) successCount++;
        totalCount++;
        
        outFile << result << endl;
    }
    
    outFile << successCount << "/" << totalCount << endl;
    
    inFile.close();
    outFile.close();
    
    cout << "Processing complete! Results saved to test_result.txt" << endl;
}

// 修改 main 函数
int main() {
    while (true) {
        cout << "\n24 Point Calculator" << endl;
        cout << "1. Console Input" << endl;
        cout << "2. File Input" << endl;
        cout << "3. Exit" << endl;
        cout << "Please choose an option (1-3): ";
        
        string choice;
        getline(cin, choice);
        
        if (choice == "1") {
            processConsoleInput();
        }
        else if (choice == "2") {
            processFileInput();
        }
        else if (choice == "3") {
            cout << "Program terminated." << endl;
            break;
        }
        else {
            cout << "Invalid choice, please try again." << endl;
        }
    }
    
    return 0;
}
