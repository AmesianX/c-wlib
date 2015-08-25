/* 
 * File:   Debugger.h
 * Author: Dimopoulos Elias
 * LinkedIn: https://gr.linkedin.com/in/DimopoulosElias
 * e-mail: Dimopoulos.Elias@gmail.com
 * website: http://www.blackhat-ops.com
 * facebook: https://www.facebook.com/blackhat.operations
 * twitter: DimopoulosElias
 * Created on August 3, 2015, 8:39 PM
 */

#ifndef DEBUGGER_H
#define	DEBUGGER_H

#include <string>
using std::string;


class Debugger {
public:
    Debugger(const string class_name, const bool debug=true, const int verbose_level=3);
    virtual ~Debugger();
    const int print_msg(const string member_function, const string msg, const int verbose_level=3) const;
    const int print_constructor() const;
    const int print_destructor() const;
    
    const int set_verbose_level(const int verbose_level, const bool constructor=false);
    const int get_verbose_level() const;

private:
    const string class_name;
    const bool debug;
    
    const bool debug_state() const;
    const string get_class() const;
    
    int verbose_level; 

};

#endif	/* DEBUGGER_H */

