#include "game_window.h"
#include <FL/fl_draw.H>
#include <cstdlib>
#include <ctime>
#include "solve24.h"

GameWindow::GameWindow(int w, int h, const char* title) 
    : Fl_Double_Window(w, h, title) {
    
    begin();
    
    // Create card display boxes
    int cardWidth = 80;
    int cardHeight = 120;
    int startX = (w - 4 * cardWidth - 3 * 10) / 2;
    int startY = 50;
    
    for (int i = 0; i < 4; i++) {
        Fl_Box* card = new Fl_Box(startX + i * (cardWidth + 10), startY, 
                                 cardWidth, cardHeight);
        card->box(FL_UP_BOX);
        card->color(FL_WHITE);
        cardBoxes.push_back(card);
    }
    
    // Create buttons
    dealButton = new Fl_Button(w/2 - 150, h - 70, 70, 30, "Deal");
    dealButton->callback(cb_deal, this);
    
    solveButton = new Fl_Button(w/2 - 50, h - 70, 70, 30, "Solve");
    solveButton->callback(cb_solve, this);
    
    resetButton = new Fl_Button(w/2 + 50, h - 70, 70, 30, "Reset");
    resetButton->callback(cb_reset, this);
    
    // Create result display
    resultOutput = new Fl_Output(50, h - 120, w - 100, 30);
    resultOutput->align(FL_ALIGN_CENTER);
    
    end();
    
    // Initialize random seed
    std::srand(std::time(nullptr));
}

void GameWindow::cb_deal(Fl_Widget*, void* v) {
    ((GameWindow*)v)->deal();
}

void GameWindow::cb_solve(Fl_Widget*, void* v) {
    ((GameWindow*)v)->solve();
}

void GameWindow::cb_reset(Fl_Widget*, void* v) {
    ((GameWindow*)v)->reset();
}

void GameWindow::deal() {
    currentCards.clear();
    std::vector<int> tempCards;
    bool hasSolution = false;
    
    // Keep generating numbers until we find a solvable combination
    while (!hasSolution) {
        tempCards.clear();
        // Generate 4 random numbers
        for (int i = 0; i < 4; i++) {
            int card = rand() % 13 + 1;
            tempCards.push_back(card);
        }
        
        // Check if this combination has a solution
        std::vector<double> nums(tempCards.begin(), tempCards.end());
        std::vector<std::string> exprs;
        for (int num : tempCards) {
            exprs.push_back(std::to_string(num));
        }
        
        std::string solution;
        if (solve24(nums, exprs, solution)) {
            hasSolution = true;
            currentCards = tempCards;  // Save the valid combination
        }
    }
    
    // Update display
    for (int i = 0; i < 4; i++) {
        std::string cardText;
        int card = currentCards[i];
        if (card == 1) cardText = "A";
        else if (card == 11) cardText = "J";
        else if (card == 12) cardText = "Q";
        else if (card == 13) cardText = "K";
        else cardText = std::to_string(card);
        
        cardBoxes[i]->label(strdup(cardText.c_str()));
    }
    
    resultOutput->value("");  // Clear previous solution
    redraw();
}

void GameWindow::solve() {
    if (currentCards.empty()) {
        resultOutput->value("Please deal cards first!");
        return;
    }
    
    // 使用现有的solve24函数求解
    std::vector<double> nums(currentCards.begin(), currentCards.end());
    std::vector<std::string> exprs;
    for (int num : currentCards) {
        exprs.push_back(std::to_string(num));
    }
    
    std::string solution;
    if (solve24(nums, exprs, solution)) {
        resultOutput->value(("Solution: " + solution + " = 24").c_str());
    } else {
        resultOutput->value("No solution found!");
    }
}

void GameWindow::reset() {
    currentCards.clear();
    for (auto box : cardBoxes) {
        box->label("");
    }
    resultOutput->value("");
    redraw();
}

GameWindow::~GameWindow() {
    for (auto box : cardBoxes) {
        delete box;
    }
    delete dealButton;
    delete solveButton;
    delete resetButton;
    delete resultOutput;
} 