#include <iostream>
#include <vector>
#include <cmath>
#include <string>
#include <iomanip>
#include <sstream>
#include <fstream>
#include <algorithm>

using namespace std;


const double EPSILON = 1e-6;


vector<char> operators = {'+', '-', '*', '/'};


bool isCloseTo24(double value) {
    return fabs(value - 24) < EPSILON;
}


struct Expression {
    string expr;
    double value;
    Expression(string e, double v) : expr(e), value(v) {}
};


double calculate(double a, double b, char op, const string& expr1, const string& expr2, string& result_expr) {
    
    result_expr = "(" + expr1 + " " + op + " " + expr2 + ")";
    
    switch (op) {
        case '+': return a + b;
        case '-': return a - b;
        case '*': return a * b;
        case '/': return (b != 0) ? a / b : 1e9;
        default: return 1e9;
    }
}


bool solve24Helper(vector<double>& nums, vector<string>& exprs, string& solution);


bool solve24(vector<double>& nums, vector<string>& exprs, string& solution) {
    
    vector<int> indices(nums.size());
    for (size_t i = 0; i < indices.size(); i++) {
        indices[i] = i;
    }

    do {
        
        vector<double> permuted_nums;
        vector<string> permuted_exprs;
        for (int idx : indices) {
            permuted_nums.push_back(nums[idx]);
            permuted_exprs.push_back(exprs[idx]);
        }

        
        if (solve24Helper(permuted_nums, permuted_exprs, solution)) {
            return true;
        }
    } while (next_permutation(indices.begin(), indices.end()));

    return false;
}


bool solve24Helper(vector<double>& nums, vector<string>& exprs, string& solution) {
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

                if (solve24Helper(rest_nums, rest_exprs, solution)) {
                    return true;
                }

                rest_nums.pop_back();
                rest_exprs.pop_back();
            }
        }
    }
    return false;
}

bool isValidInput(const string& input) {
    vector<string> validInputs = {"A", "1", "2", "3", "4", "5", "6", "7", "8", "9", "10", "J", "Q", "K"};
    return find(validInputs.begin(), validInputs.end(), input) != validInputs.end();
}


bool processLine(const string& line, string& result, string& solution) {
    vector<double> nums(4);
    vector<string> exprs(4);
    stringstream ss(line);
    string input;
    int i = 0;
    
    
    vector<string> inputs;
    while (ss >> input) {
        inputs.push_back(input);
    }
    
    if (inputs.size() != 4) {
        result = "! Invalid input: exactly 4 numbers required";
        return false;
    }
    
    
    for (const string& input : inputs) {
        if (!isValidInput(input)) {
            result = "! Invalid input: " + input + " is not a valid card value";
            return false;
        }
        
        
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


void processConsoleInput() {
    while (true) {
        cout << "Please enter 4 numbers (A, 2-10, J, Q, K), separated by spaces:" << endl;
        string input;
        getline(cin, input);
        
        if (input.empty()) {
            cout << "Empty input. Please try again." << endl;
            continue;
        }
        
        string result, solution;
        bool success = processLine(input, result, solution);
        
        if (!result.empty() && result[0] == '!') {
            cout << result << endl;
            continue;
        }
        
        cout << (success ? "Solution found!\n" : "No solution found.\n");
        if (success) {
            cout << "Expression: " << solution << " = 24" << endl;
        }
        break;
    }
}


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









        


        














    



