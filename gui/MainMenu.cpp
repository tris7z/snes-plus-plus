#include "MainMenu.hpp"

MainMenu::MainMenu(int w, int h, const char* t) : Fl_Double_Window(w, h, t) {
    Fl_Menu_Item menuitems[] = {
        { "File",              0, 0, 0, FL_SUBMENU },
            { "&Open ROM...",      FL_COMMAND + 'o', (Fl_Callback *)openROM },
            { "E&xit",              FL_COMMAND + 'q', (Fl_Callback *)exit_cb, 0 },
            { 0 },
        { "Debug",             0, 0, 0, FL_SUBMENU },
            { "Dump RAM",          0, (Fl_Callback *)dump_ram},
            { 0 },
        { 0 }
    };

    this->menu_bar = new Fl_Menu_Bar(0, 0, 640, 30);
    this->menu_bar->copy(menuitems); 
    this->label("SNES++");
}

void MainMenu::openROM(Fl_Widget* w, void* v) {
    char *newfile = fl_file_chooser("Open ROM?", "*.sfc", "");
    if (newfile != NULL)
        Core system(newfile); // does this need to be destroyed after return?
}
void MainMenu::dump_ram(Fl_Widget* w, void* v) {
    //
}
void MainMenu::exit_cb(Fl_Widget* w, void* v) {
    exit(0);
}