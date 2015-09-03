/* 
 * File:   TCP_Connection.h
 * Author: Dimopoulos Elias
 * LinkedIn: https://gr.linkedin.com/in/DimopoulosElias
 * e-mail: Dimopoulos.Elias@gmail.com
 * website: http://www.blackhat-ops.com
 * facebook: https://www.facebook.com/blackhat.operations
 * twitter: DimopoulosElias
 * Created on August 11, 2015, 11:27 PM
 */

#ifndef TCP_CONNECTION_H
#define	TCP_CONNECTION_H

#include "Connection.h"

class TCP_Connection : public Connection {
public:
    TCP_Connection(const Target* target, const int sec_timeout = 5, const bool debug = true, const int verbose_level=3);
    virtual ~TCP_Connection();
    
    virtual const int set_verbose_level(const int verbose_level);
    virtual const int get_verbose_level() const;

private:
    //Composition classes
    Debugger debugger;

};


#endif	/* TCP_CONNECTION_H */

