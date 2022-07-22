#include <string>

#include <FL/Fl.H>
#include <FL/Fl_Double_Window.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Input.H>
#include <FL/Fl_Button.H>
#include <FL/Fl_Return_Button.H>
#include <FL/Fl_Text_Editor.H>
#include <FL/Fl_Menu.H>
#include <FL/Fl_Menu_Item.H>
#include <FL/Fl_Menu_Bar.H>
#include <FL/Fl_File_Chooser.H>
//include <FL/Fl_.H>

#include "../core/core.hpp"

class MainMenu : public Fl_Double_Window {
    public:
        MainMenu(int w, int h, const char* t);
        ~MainMenu() = default;

        Fl_Menu_Bar* menu_bar;    

        static void openROM(Fl_Widget*, void*);
        static void dump_ram(Fl_Widget*, void*);
        static void exit_cb(Fl_Widget*, void*);

};