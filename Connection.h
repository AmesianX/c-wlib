/* 
 * File:   Connection.h
 * Author: Dimopoulos Elias
 * LinkedIn: https://gr.linkedin.com/in/DimopoulosElias
 * e-mail: Dimopoulos.Elias@gmail.com
 * website: http://www.blackhat-ops.com
 * facebook: https://www.facebook.com/blackhat.operations
 * twitter: DimopoulosElias
 * Created on August 2, 2015, 1:16 AM
 */

#ifndef CONNECTION_H
#define	CONNECTION_H

//#define MAX_TIMEOUT 600
#include "Debugger.h"
#include "Target.h"
#include <string>
using std::string;
#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include <unistd.h>
#include <sys/time.h> //timeval
#include <memory> 
using std::unique_ptr;

class Connection {
public:
    Connection(const Target* target, const int sec_timeout = 5, const string socket_type = "SOCK_STREAM", const bool debug = true, const int verbose_level=3);
    virtual ~Connection();

    virtual const int send_data(const string send_data);
    virtual const string receive_data(const long int buffer_size);
    virtual const int _connect();
    virtual const int get_received_data_flag() const=0;
    virtual const int get_verbose_level() const;
    virtual const int set_verbose_level(const int verbose_level);
    virtual const int set_timeout(const int sec_timeout);

    
    //++++++++++http connections++++++++++
    virtual const bool check_relativeURL(const string relativeURL) const=0;

    //header:
    //getters
    virtual const string get_Accept() const=0;  
    virtual const string get_User_Agent() const=0;    
    virtual const string get_Accept_Language() const=0;
    virtual const string get_Accept_Encoding() const=0;
    virtual const string get_Cookie() const=0;
    virtual const string get_Content_Length() const=0;
    virtual const string get_Content_Type() const=0;
    virtual const string get_Host() const=0;
      
    //setters
    virtual const int set_User_Agent(const string User_Agent)=0;
    virtual const int set_Accept(const string Accept)=0;
    virtual const int set_Accept_Language(const string Accept_Language)=0;
    virtual const int set_Accept_Encoding(const string Accept_Encoding)=0;
    virtual const int set_Cookie(const string Cookie)=0;
    virtual const int set_Content_Length(const string Content_Length)=0;
    virtual const int set_Content_Type(const string Content_Type)=0;
    virtual const int set_Host(const string Host)=0;
    
    
    //supported methods
    virtual const string POST(const string relativeURL, const string data)=0;
    virtual const string GET(const string relativeURL)=0;
    virtual const string OPTIONS(const string relativeURL)=0;
    virtual const string HEAD(const string relativeURL)=0;

protected:
    //++++++++++functions++++++++++
    //check functions
    bool try_getaddrinfo(int getaddrinfo_error_code);
    const int check_socket_type(const string socket_type) const;
    const int check_sec_timeout(const int sec_timeout) const;
    const bool select_ok(const string calling_function = "select_ok()");
    const int flags() const;
    const int check_recvfrom(const int recv_from) const;

    //getters

    const int get_socket_fd() const;
    const int get_socket_flags() const;
    const int get_select_check() const;
    const int get_socket_type() const;
    const int get_sec_timeout() const;
    
    timeval*const get_timeval();
    const struct addrinfo*const get_target_struct() const;
    fd_set *const get_fdset_read();
    fd_set *const get_fdset_error();
    fd_set *const get_fdset_write();

    //setters
    const int set_socket_fd(const int socket_file_descriptor);
    const int set_socket_flags(const int socket_flags);
    const int set_select_check(const int select_check);
      
    const Target target;
    
    //++++++++++http connections++++++++++
    virtual const int create_request(const string method)=0;
    virtual const int set_relativeURL(const string relativeURL)=0;
    virtual const string get_relativeURL() const=0;
    virtual const string get_http_request() const=0;


private:
    //++++++++++functions++++++++++
    //help initializers
    const struct addrinfo initialize_struct();
    const char* const initialize_port(const int port) const; //
    const timeval initialize_timeout();

   

    //++++++++++variables++++++++++
    const struct addrinfo target_restrictions_struct;
    unique_ptr<addrinfo> tmp_target_struct;

    const int socket_type;
    int sec_timeout;

    struct addrinfo* target_struct;
    int socket_file_descriptor = 0;
    int select_check = -1;
    int socket_fd_flags = 0;
    
    timeval timeout;
    fd_set read, write, error;
    //const int max_timeout = MAX_TIMEOUT;
    //Composition classes
    Debugger debugger;
    


};

#endif	/* CONNECTION_H */

