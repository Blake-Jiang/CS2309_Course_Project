#ifndef GAME_WINDOW_H
#define GAME_WINDOW_H

#include <FL/Fl.H>
#include <FL/Fl_Double_Window.H>
#include <FL/Fl_Button.H>
#include <FL/Fl_Box.H>
#include <FL/Fl_Output.H>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include "solve24.h"

class GameWindow : public Fl_Double_Window {
private:
    // Card display boxes
    std::vector<Fl_Box*> cardBoxes;
    
    // Buttons
    Fl_Button* dealButton;
    Fl_Button* solveButton;
    Fl_Button* resetButton;
    Fl_Button* fileButton;
    
    // Result display
    Fl_Output* resultOutput;
    
    // Game state
    std::vector<int> currentCards;
    
    static void cb_deal(Fl_Widget*, void*);
    static void cb_solve(Fl_Widget*, void*);
    static void cb_reset(Fl_Widget*, void*);
    static void cb_file(Fl_Widget*, void*);
    
    void deal();
    void solve();
    void reset();
    void processFile();
    bool processLine(const std::string& line, std::string& result, std::string& solution);

public:
    GameWindow(int w, int h, const char* title);
    ~GameWindow();
};

#endif 