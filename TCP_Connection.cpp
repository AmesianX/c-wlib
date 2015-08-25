/* 
 * File:   TCP_Connection.cpp
 * Author: Dimopoulos Elias
 * LinkedIn: https://gr.linkedin.com/in/DimopoulosElias
 * e-mail: Dimopoulos.Elias@gmail.com
 * website: http://www.blackhat-ops.com
 * facebook: https://www.facebook.com/blackhat.operations
 * twitter: DimopoulosElias
 * Created on August 11, 2015, 11:27 PM
 */

#include "TCP_Connection.h"

TCP_Connection::TCP_Connection(const Target* target, const int sec_timeout, const bool debug, const int verbose_level) 
:
Connection(target, sec_timeout, "SOCK_STREAM",debug, verbose_level),
debugger("TCP_Connection", debug, verbose_level)

{
debugger.print_constructor();
}

const int TCP_Connection::get_verbose_level() const{
    return debugger.get_verbose_level();
}

const int TCP_Connection::set_verbose_level(const int verbose_level) {
   Connection::set_verbose_level(verbose_level);
   return debugger.set_verbose_level(verbose_level);    
}

TCP_Connection::~TCP_Connection() {
    debugger.print_destructor();
}

