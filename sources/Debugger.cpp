/* 
 * File:   Debugger.cpp
 * Author: Dimopoulos Elias
 * LinkedIn: https://gr.linkedin.com/in/DimopoulosElias
 * e-mail: Dimopoulos.Elias@gmail.com
 * website: http://www.blackhat-ops.com
 * facebook: https://www.facebook.com/blackhat.operations
 * twitter: DimopoulosElias
 * Created on August 3, 2015, 8:39 PM
 */

#include "headers/Debugger.h"
#include <iostream>
using std::endl;
using std::cout;

Debugger::Debugger(const string class_name, const bool debug, const int verbose_level)
:
class_name(class_name),
debug(debug),
verbose_level(set_verbose_level(verbose_level,true)) {



    if (get_verbose_level() >= 3)
        cout << "Debugger is ON, for Class \"" << get_class() << "\"" << " with verbose level " << get_verbose_level() <<"\n"<< endl;


    /*cout<<"Debugger is OFF, for Class \""<<get_class()<<"\""<<endl*/;
}

const bool Debugger::debug_state() const {
    return this->debug;
}

const string Debugger::get_class() const {
    return this->class_name;
}

const int Debugger::get_verbose_level() const {
    return this->verbose_level;
}

const int Debugger::set_verbose_level(const int verbose_level, const bool constructor) {

    if (debug_state()) {

        if (verbose_level >= 0 && verbose_level <= 3) {

            this->verbose_level = verbose_level;
            return get_verbose_level();
        }
        else {
            
            cout << "\n\nChoose verbose level 0<=verbose_level<=3. Default is 3.\n" << endl;
            return 3;
        }

    } else
    {
        if(!constructor)
        cout << "\n\nSorry debug is off.\n" << endl;
        return -1;
    }
}

const int Debugger::print_msg(const string member_function, const string msg, const int verbose_level) const {
    if (debug_state() && get_verbose_level() >= verbose_level) {

        cout << "[*]MSG from class \"" << get_class() << "\"-->function \"" << member_function << "\" :" << endl;
        cout << "\n++++++++++\n" << msg << "\n++++++++++\n" << endl;

        return 0;
    } else
        return -1;
}

const int Debugger::print_constructor() const {
    if (get_verbose_level() >= 3) {
        cout << "[+]MSG from class \"" << get_class() << "\"-->function \"Constructor\" :" << endl;
        cout << "++Object created!++\n" << endl;
        return 0;
    } else
        return -1;
}

const int Debugger::print_destructor() const {


    if (get_verbose_level() >= 3) {
        cout << "[!]MSG from class \"" << get_class() << "\"-->function \"Destructor\" :" << endl;
        cout << "--Object is dead--\n" << endl;
        return 0;
    }

    else
        return -1;
}

Debugger::~Debugger() {

    if (get_verbose_level() >= 3)
        cout << "End of debugging, for Class \"" << get_class() << "\"\n" << endl;
}