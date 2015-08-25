/* 
 * File:   Target.cpp
 * Author: Dimopoulos Elias
 * LinkedIn: https://gr.linkedin.com/in/DimopoulosElias
 * e-mail: Dimopoulos.Elias@gmail.com
 * website: http://www.blackhat-ops.com
 * facebook: https://www.facebook.com/blackhat.operations
 * twitter: DimopoulosElias
 * Created on August 10, 2015, 9:18 PM
 */

#define D(a) cout<<endl<<"--------->"<<a<<"<------------"<<endl
#include "Target.h"
#include <cstdlib>
#include <iostream>
using std::cout;
using std::endl;

Target::Target(const string target, const int port, const bool debug, const int verbose_level)
:
debugger("Target", debug, verbose_level),
target(target.c_str()),
port(std::to_string(port)) 

{
    debugger.print_constructor();
}

const string Target::get_target() const {
    return this->target;
}

const int Target::get_port() const {
    return std::stoi(this->port);
}

const char* const Target::get_port_c_str() const {
   return this->port.c_str();
}

const char* const Target::get_target_c_str() const {
    return this->target.c_str();
}

Target::~Target() {
    debugger.print_destructor();
}

