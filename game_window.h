#ifndef GAME_WINDOW_H
#define GAME_WINDOW_H

#include <FL/Fl.H>
#include <FL/Fl_Double_Window.H>
#include <FL/Fl_Button.H>
#include <FL/Fl_Box.H>
#include <FL/Fl_Output.H>
#include <FL/Fl_Input.H>
#include <FL/Fl_Choice.H>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <cmath>
#include "solve24.h"

class GameWindow : public Fl_Double_Window {
private:
    
    std::vector<Fl_Box*> cardBoxes;
    
    
    Fl_Button* startButton;
    Fl_Button* checkButton;
    Fl_Button* resetButton;
    Fl_Button* fileButton;
    
    
    Fl_Input* answerInput;
    Fl_Output* resultOutput;
    
    
    Fl_Box* timerDisplay;
    Fl_Box* scoreDisplay;
    Fl_Box* highScoreDisplay;
    Fl_Box* comboDisplay;
    Fl_Choice* timeChoice;
    
    
    std::vector<int> currentCards;
    
    
    int remainingTime;
    int selectedTime;
    bool isTimerRunning;
    int currentScore;
    int highScore;
    int comboCount;
    bool isFirstTry;
    
    
    static void cb_start(Fl_Widget*, void*);
    static void cb_check(Fl_Widget*, void*);
    static void cb_reset(Fl_Widget*, void*);
    static void cb_file(Fl_Widget*, void*);
    static void cb_timer(void* v);
    static void cb_time_changed(Fl_Widget*, void*);
    
    
    void start();
    void check();
    void reset();
    void processFile();
    bool processLine(const std::string& line, std::string& result, std::string& solution);
    
    
    void startTimer();
    void stopTimer();
    void resetTimer();
    void updateTimer();
    void updateTimerDisplay();
    void calculateScore();
    void updateDisplays();
    void saveGameData();
    void loadGameData();
    
    
    bool validateAnswer(const std::string& answer);
    double evaluateExpression(const std::string& expr);
    int precedence(char op);
    double applyOp(double a, double b, char op);

public:
    GameWindow(int w, int h, const char* title);
    ~GameWindow();
};

#endif 