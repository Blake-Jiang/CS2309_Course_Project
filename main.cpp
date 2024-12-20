#include "game_window.h"

int main() {
    GameWindow* window = new GameWindow(800, 600, "24 Points Game");
    window->show();
    return Fl::run();
}
