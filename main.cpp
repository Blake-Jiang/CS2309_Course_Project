#include "game_window.h"

int main() {
    GameWindow* window = new GameWindow(400, 300, "24 Points Game");
    window->show();
    return Fl::run();
}
