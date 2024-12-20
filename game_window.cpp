#include "game_window.h"
#include <FL/fl_draw.H>
#include <cstdlib>
#include <ctime>
#include "solve24.h"
#include <fstream>
#include <string>
#include <sstream>
#include <stack>
#include <cctype>
#include <algorithm>

GameWindow::GameWindow(int w, int h, const char* title) 
    : Fl_Double_Window(w, h, title) {
    
    begin();
    
    
    currentScore = 0;
    highScore = 0;
    comboCount = 0;
    isTimerRunning = false;
    selectedTime = 60;  
    remainingTime = selectedTime;
    isFirstTry = true;
    
    
    Fl_Box* logo = new Fl_Box(w/2-150 , h/2-50, 300, 30, "24 CHALLENGE");
    logo->labelsize(48);
    logo->labelfont(FL_BOLD);
    logo->labelcolor(FL_DARK_BLUE);
    
    
    int cardWidth = 80;
    int cardHeight = 120;
    int startX = (w - 4 * cardWidth - 3 * 10) / 2;
    int startY = 60;
    
    for (int i = 0; i < 4; i++) {
        Fl_Box* card = new Fl_Box(startX + i * (cardWidth + 10), startY, 
                                 cardWidth, cardHeight);
        card->box(FL_UP_BOX);
        card->color(FL_WHITE);
        cardBoxes.push_back(card);
    }
    
    
    timerDisplay = new Fl_Box(30, 10, 100, 30, "");
    timerDisplay->box(FL_FLAT_BOX);
    timerDisplay->labelsize(16);
    updateTimerDisplay();  
    
    scoreDisplay = new Fl_Box(120, 10, 150, 30, "Score: 0");
    scoreDisplay->box(FL_FLAT_BOX);
    scoreDisplay->labelsize(16);
    
    highScoreDisplay = new Fl_Box(280, 10, 150, 30, "High Score: 0");
    highScoreDisplay->box(FL_FLAT_BOX);
    highScoreDisplay->labelsize(16);
    
    comboDisplay = new Fl_Box(440, 10, 100, 30, "Combo: 0");
    comboDisplay->box(FL_FLAT_BOX);
    comboDisplay->labelsize(16);
    
    
    timeChoice = new Fl_Choice(w - 120, 10, 100, 30, "Time:");
    timeChoice->add("30 sec");
    timeChoice->add("60 sec");
    timeChoice->add("120 sec");
    timeChoice->value(1);  
    timeChoice->callback(cb_time_changed, this);
    
    
    int buttonWidth = 70;
    int buttonSpacing = 20;
    int totalWidth = 4 * buttonWidth + 3 * buttonSpacing;
    int buttonStartX = w/2 - totalWidth/2;
    
    startButton = new Fl_Button(buttonStartX, h - 70, buttonWidth, 30, "Start");
    startButton->callback(cb_start, this);
    
    checkButton = new Fl_Button(buttonStartX + buttonWidth + buttonSpacing, h - 70, buttonWidth, 30, "Check");
    checkButton->callback(cb_check, this);
    
    resetButton = new Fl_Button(buttonStartX + 2 * (buttonWidth + buttonSpacing), h - 70, buttonWidth, 30, "Reset");
    resetButton->callback(cb_reset, this);
    
    fileButton = new Fl_Button(buttonStartX + 3 * (buttonWidth + buttonSpacing), h - 70, buttonWidth, 30, "File");
    fileButton->callback(cb_file, this);
    
    
    answerInput = new Fl_Input(100, h - 120, w - 200, 30, "Answer:");
    answerInput->align(FL_ALIGN_LEFT);
    
    
    Fl_Box* formatHint = new Fl_Box(100, h - 150, w - 200, 30, 
        "Format: Use numbers and operators (+,-,*,/). Example: (3+5)*(7-2)");
    formatHint->labelsize(12);
    formatHint->align(FL_ALIGN_LEFT|FL_ALIGN_INSIDE);
    
    resultOutput = new Fl_Output(100, h - 190, w - 200, 30, "Result:");
    resultOutput->align(FL_ALIGN_LEFT);
    
    end();
    
    
    std::srand(std::time(nullptr));
    
    
    loadGameData();
}

void GameWindow::cb_start(Fl_Widget*, void* v) {
    ((GameWindow*)v)->start();
}

void GameWindow::cb_check(Fl_Widget*, void* v) {
    ((GameWindow*)v)->check();
}

void GameWindow::cb_reset(Fl_Widget*, void* v) {
    ((GameWindow*)v)->reset();
}

void GameWindow::cb_file(Fl_Widget*, void* v) {
    ((GameWindow*)v)->processFile();
}

void GameWindow::cb_timer(void* v) {
    GameWindow* window = (GameWindow*)v;
    window->updateTimer();
}

void GameWindow::startTimer() {
    isTimerRunning = true;
    Fl::add_timeout(1.0, cb_timer, this);  
}

void GameWindow::stopTimer() {
    isTimerRunning = false;
    Fl::remove_timeout(cb_timer, this);
}

void GameWindow::resetTimer() {
    stopTimer();
    remainingTime = selectedTime;
    isFirstTry = true;
    updateTimerDisplay();  
}

void GameWindow::updateTimer() {
    if (isTimerRunning) {
        remainingTime--;
        if (remainingTime <= 0) {
            stopTimer();
            
            
            std::vector<double> nums(currentCards.begin(), currentCards.end());
            std::vector<std::string> exprs;
            for (int num : currentCards) {
                exprs.push_back(std::to_string(num));
            }
            std::string solution;
            if (solve24(nums, exprs, solution)) {
                std::string message = "Time's up! A solution is: " + solution + " = 24";
                resultOutput->value(message.c_str());
            } else {
                resultOutput->value("Time's up!");
            }
            
            
            checkButton->deactivate();
            startButton->activate();
            
            
            updateTimerDisplay();
            return;
        }
        
        
        updateTimerDisplay();
        
        
        if (remainingTime <= 10) {
            timerDisplay->labelcolor(FL_RED);
        }
        
        Fl::add_timeout(1.0, cb_timer, this);
    }
}

void GameWindow::calculateScore() {
    int baseScore = 100;
    double timeMultiplier;
    
    
    if (remainingTime >= 45) timeMultiplier = 1.5;
    else if (remainingTime >= 30) timeMultiplier = 1.2;
    else if (remainingTime >= 15) timeMultiplier = 1.0;
    else timeMultiplier = 0.8;
    
    
    int finalScore = baseScore * timeMultiplier;
    
    
    if (isFirstTry) {
        finalScore += 50;
    }
    
    
    finalScore += comboCount * 10;
    
    
    currentScore += finalScore;
    
    
    if (currentScore > highScore) {
        highScore = currentScore;
        saveGameData();
    }
}

void GameWindow::updateDisplays() {
    
    static char scoreStr[32];
    static char highScoreStr[32];
    static char comboStr[32];
    
    
    snprintf(scoreStr, sizeof(scoreStr), "Score: %d", currentScore);
    snprintf(highScoreStr, sizeof(highScoreStr), "High Score: %d", highScore);
    snprintf(comboStr, sizeof(comboStr), "Combo: %d", comboCount);
    
    
    scoreDisplay->copy_label(scoreStr);
    highScoreDisplay->copy_label(highScoreStr);
    comboDisplay->copy_label(comboStr);
    
    scoreDisplay->redraw();
    highScoreDisplay->redraw();
    comboDisplay->redraw();
}

void GameWindow::saveGameData() {
    std::ofstream file("game_data.txt");
    if (file.is_open()) {
        file << highScore << std::endl;
        file.close();
    }
}

void GameWindow::loadGameData() {
    std::ifstream file("game_data.txt");
    if (file.is_open()) {
        file >> highScore;
        file.close();
        updateDisplays();
    }
}

void GameWindow::start() {
    
    if (!isTimerRunning) {
        currentScore = 0;
        comboCount = 0;
        updateDisplays();
    }
    
    resetTimer();
    currentCards.clear();
    
    std::vector<int> tempCards;
    bool hasSolution = false;
    
    while (!hasSolution) {
        tempCards.clear();
        for (int i = 0; i < 4; i++) {
            int card = rand() % 13 + 1;
            tempCards.push_back(card);
        }
        
        std::vector<double> nums(tempCards.begin(), tempCards.end());
        std::vector<std::string> exprs;
        for (int num : tempCards) {
            exprs.push_back(std::to_string(num));
        }
        
        std::string solution;
        if (solve24(nums, exprs, solution)) {
            hasSolution = true;
            currentCards = tempCards;
        }
    }
    
    
    for (int i = 0; i < 4; i++) {
        std::string cardText;
        int card = currentCards[i];
        if (card == 1) cardText = "A";
        else if (card == 11) cardText = "J";
        else if (card == 12) cardText = "Q";
        else if (card == 13) cardText = "K";
        else cardText = std::to_string(card);
        
        cardBoxes[i]->copy_label(cardText.c_str());
    }
    
    
    answerInput->value("");
    resultOutput->value("");
    checkButton->activate();  
    startButton->deactivate();  
    timerDisplay->labelcolor(FL_BLACK);  
    
    startTimer();
    redraw();
}

bool GameWindow::validateAnswer(const std::string& answer) {
    
    std::vector<int> usedNumbers;
    std::string num;
    for (size_t i = 0; i < answer.length(); i++) {
        if (isdigit(answer[i])) {
            num += answer[i];
            
            if (i + 1 == answer.length() || !isdigit(answer[i + 1])) {
                usedNumbers.push_back(std::stoi(num));
                num.clear();
            }
        }
    }
    
    
    if (usedNumbers.size() != currentCards.size()) {
        return false;
    }
    
    
    std::vector<int> availableCards = currentCards;
    
    
    for (int num : usedNumbers) {
        auto it = std::find(availableCards.begin(), availableCards.end(), num);
        if (it == availableCards.end()) {
            return false;  
        }
        
        availableCards.erase(it);
    }
    
    
    try {
        double result = evaluateExpression(answer);
        return fabs(result - 24.0) < 1e-6;  
    } catch (const std::runtime_error& e) {
        return false;
    }
}

double GameWindow::evaluateExpression(const std::string& expr) {
    std::stack<double> values;
    std::stack<char> ops;
    
    for (size_t i = 0; i < expr.length(); i++) {
        if (expr[i] == ' ')
            continue;
            
        if (isdigit(expr[i])) {
            double val = 0;
            while (i < expr.length() && isdigit(expr[i])) {
                val = val * 10 + (expr[i] - '0');
                i++;
            }
            i--;
            values.push(val);
        }
        else if (expr[i] == '(') {
            ops.push(expr[i]);
        }
        else if (expr[i] == ')') {
            while (!ops.empty() && ops.top() != '(') {
                double val2 = values.top(); values.pop();
                double val1 = values.top(); values.pop();
                char op = ops.top(); ops.pop();
                values.push(applyOp(val1, val2, op));
            }
            if (!ops.empty())
                ops.pop();  
        }
        else if (expr[i] == '+' || expr[i] == '-' || expr[i] == '*' || expr[i] == '/') {
            while (!ops.empty() && precedence(ops.top()) >= precedence(expr[i])) {
                double val2 = values.top(); values.pop();
                double val1 = values.top(); values.pop();
                char op = ops.top(); ops.pop();
                values.push(applyOp(val1, val2, op));
            }
            ops.push(expr[i]);
        }
    }
    
    while (!ops.empty()) {
        double val2 = values.top(); values.pop();
        double val1 = values.top(); values.pop();
        char op = ops.top(); ops.pop();
        values.push(applyOp(val1, val2, op));
    }
    
    return values.top();
}

int GameWindow::precedence(char op) {
    if (op == '+' || op == '-')
        return 1;
    if (op == '*' || op == '/')
        return 2;
    return 0;
}

double GameWindow::applyOp(double a, double b, char op) {
    switch (op) {
        case '+': return a + b;
        case '-': return a - b;
        case '*': return a * b;
        case '/': return b != 0 ? a / b : throw std::runtime_error("Division by zero");
        default: throw std::runtime_error("Invalid operator");
    }
}

void GameWindow::check() {
    if (!isTimerRunning) {
        resultOutput->value("Please start the game first or time's up!");
        return;
    }
    
    if (currentCards.empty()) {
        resultOutput->value("Please start the game first!");
        return;
    }
    
    std::string playerAnswer = answerInput->value();
    if (playerAnswer.empty()) {
        resultOutput->value("Please enter your answer!");
        return;
    }
    
    try {
        if (validateAnswer(playerAnswer)) {
            resultOutput->value("Correct!");
            calculateScore();
            comboCount++;
            updateDisplays();
            
            
            Fl::add_timeout(1.0, [](void* v) {
                GameWindow* window = (GameWindow*)v;
                window->start();
            }, this);
        } else {
            resultOutput->value("Wrong answer! Try again.");
            comboCount = 0;
            
            currentScore = std::max(0, currentScore - 50);
            isFirstTry = false;
            updateDisplays();
        }
    } catch (const std::runtime_error& e) {
        resultOutput->value(e.what());  
        comboCount = 0;
        
        currentScore = std::max(0, currentScore - 50);
        isFirstTry = false;
        updateDisplays();
    }
}

void GameWindow::reset() {
    stopTimer();
    currentCards.clear();
    for (auto box : cardBoxes) {
        box->label("");
    }
    answerInput->value("");  
    resultOutput->value("");
    currentScore = 0;
    comboCount = 0;
    remainingTime = selectedTime;
    isFirstTry = true;
    
    
    startButton->activate();
    checkButton->deactivate();
    
    
    timerDisplay->labelcolor(FL_BLACK);
    updateTimerDisplay();
    updateDisplays();
    redraw();
}

void GameWindow::processFile() {
    std::ifstream inFile("test.txt");
    std::ofstream outFile("test_result.txt");
    
    if (!inFile.is_open()) {
        resultOutput->value("Cannot open input file!");
        return;
    }

    std::string line;
    int totalCount = 0;
    int successCount = 0;
    
    while (std::getline(inFile, line)) {
        if (line.empty()) continue;
        
        std::string result, solution;
        bool success = processLine(line, result, solution);
        
        if (success) successCount++;
        totalCount++;
        
        outFile << result << std::endl;
    }
    
    outFile << successCount << "/" << totalCount << std::endl;
    
    inFile.close();
    outFile.close();
    
    resultOutput->value("Results have been saved to test_result.txt");
}

bool GameWindow::processLine(const std::string& line, std::string& result, std::string& solution) {
    std::vector<double> nums(4);
    std::vector<std::string> exprs(4);
    std::stringstream ss(line);
    std::string input;
    int i = 0;
    
    
    std::vector<std::string> inputs;
    while (ss >> input) {
        inputs.push_back(input);
    }
    
    if (inputs.size() != 4) {
        result = "! Invalid input: exactly 4 numbers required";
        return false;
    }
    
    
    for (const std::string& input : inputs) {
        
        if (input == "A" || input == "1") {
            nums[i] = 1;
        } else if (input == "J") {
            nums[i] = 11;
        } else if (input == "Q") {
            nums[i] = 12;
        } else if (input == "K") {
            nums[i] = 13;
        } else {
            try {
                nums[i] = std::stod(input);
            } catch (...) {
                result = "! Invalid input: " + input + " is not a valid card value";
                return false;
            }
        }
        exprs[i] = input;
        i++;
    }

    bool success = solve24(nums, exprs, solution);
    result = (success ? "+ " : "- ") + line;
    return success;
}

GameWindow::~GameWindow() {
    stopTimer();
    saveGameData();
    
    for (auto box : cardBoxes) {
        delete box;
    }
    delete startButton;
    delete checkButton;
    delete resetButton;
    delete resultOutput;
    delete fileButton;
    delete timerDisplay;
    delete scoreDisplay;
    delete highScoreDisplay;
    delete comboDisplay;
    delete timeChoice;
    delete answerInput;
}

void GameWindow::updateTimerDisplay() {
    static char timeStr[32];
    snprintf(timeStr, sizeof(timeStr), "Time: %d", remainingTime);
    timerDisplay->copy_label(timeStr);
    timerDisplay->redraw();
}

void GameWindow::cb_time_changed(Fl_Widget* w, void* v) {
    GameWindow* window = (GameWindow*)v;
    Fl_Choice* choice = (Fl_Choice*)w;
    int selected = choice->value();
    
    switch(selected) {
        case 0: window->selectedTime = 30; break;
        case 1: window->selectedTime = 60; break;
        case 2: window->selectedTime = 120; break;
    }
    window->remainingTime = window->selectedTime;
    window->updateTimerDisplay();
} 