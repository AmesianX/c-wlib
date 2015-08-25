/* 
 * File:   Target.h
 * Author: Dimopoulos Elias
 * LinkedIn: https://gr.linkedin.com/in/DimopoulosElias
 * e-mail: Dimopoulos.Elias@gmail.com
 * website: http://www.blackhat-ops.com
 * facebook: https://www.facebook.com/blackhat.operations
 * twitter: DimopoulosElias
 * Created on August 10, 2015, 9:18 PM
 */

#ifndef TARGET_H
#define	TARGET_H

#include "Debugger.h"
#include <string>
using std::string;

class Target {
public:
    Target(const string target, const int port, const bool debug = true, const int verbose_level=3);
    virtual ~Target();

    const char *const get_port_c_str() const;
    const char *const get_target_c_str() const;

    const string get_target() const;
    const int get_port() const;


private:


    const string target;
    const string port;

    const Debugger debugger;



};

#endif	/* TARGET_H */

