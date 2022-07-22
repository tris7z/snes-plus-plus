#include "core/core.hpp"
#include "gui/MainMenu.hpp"

int main(int argc, char *argv[]) {
    if(argc == 1)
    {
        Fl_Window *window = new MainMenu(640, 480, "");
        window->position((Fl::w() - window->w())/2, (Fl::h() - window->h())/2);
        window->show(argc, argv);
        return Fl::run();
    }
    else
    {
        Core system(argv[1]);
    }
}
