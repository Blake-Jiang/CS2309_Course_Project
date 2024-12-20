#include <iostream>
#include <vector>
#include <cmath>
#include <string>
#include <iomanip>

using namespace std;

// 常量：用于判断浮点数的误差
const double EPSILON = 1e-6;

// 定义运算符
vector<char> operators = {'+', '-', '*', '/'};

// 检查是否接近 24
bool isCloseTo24(double value) {
    return fabs(value - 24) < EPSILON;
}

// 计算 a op b 的结果
double calculate(double a, double b, char op) {
    switch (op) {
        case '+': return a + b;
        case '-': return a - b;
        case '*': return a * b;
        case '/': return (b != 0) ? a / b : 1e9; // 避免除零
        default: return 1e9; // 异常情况返回无效值
    }
}

// 回溯法解决 24 点问题
bool solve24(vector<double> nums) {
    // 如果只有一个数字，检查是否接近 24
    if (nums.size() == 1) {
        return isCloseTo24(nums[0]);
    }

    // 两两配对进行计算
    for (size_t i = 0; i < nums.size(); ++i) {
        for (size_t j = 0; j < nums.size(); ++j) {
            if (i == j) continue; // 不能使用同一个数字

            // 剩余数字
            vector<double> rest;
            for (size_t k = 0; k < nums.size(); ++k) {
                if (k != i && k != j) {
                    rest.push_back(nums[k]);
                }
            }

            // 遍历所有运算符
            for (char op : operators) {
                // 计算结果
                double result = calculate(nums[i], nums[j], op);
                rest.push_back(result);

                // 递归判断
                if (solve24(rest)) {
                    return true;
                }

                // 回溯
                rest.pop_back();
            }
        }
    }

    return false;
}

int main() {
    // 输入 4 个数字
    vector<double> nums(4);
    cout << "Enter 4 numbers: ";
    for (int i = 0; i < 4; ++i) {
        cin >> nums[i];
    }

    // 计算并输出结果
    if (solve24(nums)) {
        cout << "Yes, it is possible to get 24." << endl;
    } else {
        cout << "No, it is not possible to get 24." << endl;
    }

    return 0;
}
