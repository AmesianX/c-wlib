/* 
 * File:   TCP.h
 * Author: Dimopoulos Elias
 * LinkedIn: https://gr.linkedin.com/in/DimopoulosElias
 * e-mail: Dimopoulos.Elias@gmail.com
 * website: http://www.blackhat-ops.com
 * facebook: https://www.facebook.com/blackhat.operations
 * twitter: DimopoulosElias
 * Created on August 11, 2015, 11:27 PM
 */

#ifndef TCP_H
#define	TCP_H

#include "Protocol.h"

class TCP : public Protocol {
public:
    TCP(const Target* target, const int sec_timeout = 5, const bool debug = true, const int verbose_level=3);
    virtual ~TCP();
    
    virtual const int set_verbose_level(const int verbose_level);
    virtual const int get_verbose_level() const;

private:
    //Composition classes
    Debugger debugger;

};


#endif	/* TCP_H */

