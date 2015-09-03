/* 
 * File:   HTTP.h
 * Author: Dimopoulos Elias
 * LinkedIn: https://gr.linkedin.com/in/DimopoulosElias
 * e-mail: Dimopoulos.Elias@gmail.com
 * website: http://www.blackhat-ops.com
 * facebook: https://www.facebook.com/blackhat.operations
 * twitter: DimopoulosElias
 * Created on August 12, 2015, 12:20 AM
 */

#ifndef HTTP_H
#define	HTTP_H

#include "TCP.h"

class HTTP : public TCP {
public:
    HTTP(const Target* target, const int sec_timeout, const bool debug = true, const int verbose_level=3);
    virtual ~HTTP();

    //in case you want to use Protocol::send_data instead of http methods
    virtual const string receive_data(const long int buffer_size);
    
    virtual const int set_verbose_level(const int verbose_level);
    virtual const int get_verbose_level() const;

    //++++++++++pure virtual++++++++++

    virtual const bool check_relativeURL(const string relativeURI) const;

    //getters

    virtual const int get_received_data_flag() const;

    virtual const string get_Accept() const;

    virtual const string get_Accept_Encoding() const;

    virtual const string get_Accept_Language() const;

    virtual const string get_User_Agent() const;

    virtual const string get_Cookie() const;

    virtual const string get_Content_Length() const;

    virtual const string get_Content_Type() const;

    virtual const string get_Host() const;


    //setters
    virtual const int set_Accept(const string Accept);

    virtual const int set_Accept_Encoding(const string Accept_Encoding);

    virtual const int set_Accept_Language(const string Accept_Language);

    virtual const int set_User_Agent(const string User_Agent);

    virtual const int set_Cookie(const string Cookie);

    virtual const int set_Content_Length(const string Content_Length);

    virtual const int set_Content_Type(const string Content_Type);

    virtual const int set_Host(const string Host);


    //methods
    virtual const string POST(const string relativeURL, const string data);

    virtual const string GET(const string relativeURL);

    virtual const string OPTIONS(const string relativeURL);

    virtual const string HEAD(const string relativeURL);

private:
    //0 for unspecified
    //negative if the the length is not the expected one from the Content-Length
    //positive if the length is ok.
    int received_data_flag = 0;

    //header fields
    string User_Agent = "User-Agent: Mozilla/5.0 (X11; Linux x86_64; rv:38.0) Gecko/20100101 Firefox/38.0 Iceweasel/38.2.0";
    string Accept;
    string Accept_Language;
    string Accept_Encoding;
    string Content_Length;
    string Content_Type;
    string Cookie;
    string Host;

    string http_request;
    string relativeURL;

    //getters
    virtual const string get_relativeURL() const;

    virtual const string get_http_request() const;

    //setters
    const int set_received_data_flag(const string received_data);

    virtual const int set_relativeURL(const string relativeURL);
    

    virtual const int create_request(const string method);

    //Composition classes
    Debugger debugger;


};

#endif	/* HTTP_H */

