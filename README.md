# c-wlib

c-wlib is intended primarily for building penetration testing tools in C++, without having to deal with sockets, but it can also be used for a vast variaty o clients programms.

No need for "c stytle" programming with char pointers. The library's functions are using strings and integers, for sending and receiving datas. 
You create your object, based on the communication you want to accomplish. You can create a more generic object, with TCP class, or a more specific with HTTP object. 

Example member functions, from HTTP class:

 HTTP(const Target* target, const int sec_timeout, const bool debug = true, const int verbose_level=3);

    //setters
 
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

Target is a class for creating your targets. Constructor for Target is:

Target(const string target, const int port, const bool debug = true, const int verbose_level=3);
