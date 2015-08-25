/* 
 * File:   HTTP_Connection.cpp
 * Author: Dimopoulos Elias
 * LinkedIn: https://gr.linkedin.com/in/DimopoulosElias
 * e-mail: Dimopoulos.Elias@gmail.com
 * website: http://www.blackhat-ops.com
 * facebook: https://www.facebook.com/blackhat.operations
 * twitter: DimopoulosElias
 * Created on August 12, 2015, 12:20 AM
 */

#define D(a) cout<<endl<<"--------->\n"<<a<<"\n<------------"<<endl

#include "HTTP_Connection.h"
#include <vector>
using std::vector;
#include <cstring> //memset
#include <iostream>
using std::cout;
using std::cin;
using std::endl;

HTTP_Connection::HTTP_Connection(const Target* target, const int sec_timeout, const bool debug, const int verbose_level)
:
TCP_Connection(target, sec_timeout, debug, verbose_level),
debugger("HTTP_Connection", debug, verbose_level) {
    debugger.print_constructor();
    set_Host(target->get_target());
}

const string HTTP_Connection::POST(const string relativeURL, const string data) {

    string http_post_request;
    size_t pos=0;
    string tmp;
    
    if (check_relativeURL(relativeURL) == true) {
       
        set_Content_Length(std::to_string(data.size()));
        tmp="\r\n"+data+"\r\n\r\n";
        
        set_relativeURL(relativeURL);
        create_request("POST");
        http_post_request=get_http_request();
        pos=http_post_request.find("\r\n\r\n");
        
        if(pos!= std::string::npos)
        http_post_request.replace(http_post_request.end()-2,http_post_request.end(),tmp.begin(),tmp.end());
        
        debugger.print_msg("POST",http_post_request,2);
        send_data(http_post_request);
        
        return receive_data(100);
        
    } else
        return "I didn't find a \"/\" in your relativeURL.\nrelativeURL example: \n\t1. /members/login.php\n\t2. /members/login.php?user=admin";

}


const string HTTP_Connection::GET(const string relativeURL) {

    if (check_relativeURL(relativeURL) == true) {
        
        set_relativeURL(relativeURL);
        create_request("GET");
        
        debugger.print_msg("GET",get_http_request(),2);
        send_data(get_http_request());
        
        return receive_data(100);
        
    } else
        return "I didn't find a \"/\" in your relativeURL.\nrelativeURL example: \n\t1. /members/login.php\n\t2. /members/login.php?user=admin";


}

const string HTTP_Connection::OPTIONS(const string relativeURL) {

    if (check_relativeURL(relativeURL) == true) {
        
        set_relativeURL(relativeURL);
        create_request("OPTIONS");
        
        debugger.print_msg("OPTIONS",get_http_request(),2);
        send_data(get_http_request());
        
        return receive_data(100);
        
    } else
        return "I didn't find a \"/\" in your relativeURL.\nrelativeURL example: \n\t1. /members/login.php\n\t2. /members/login.php?user=admin";

}

const string HTTP_Connection::HEAD(const string relativeURL) {

    if (check_relativeURL(relativeURL) == true) {
        
        set_relativeURL(relativeURL);
        create_request("HEAD");
        
        debugger.print_msg("HEAD",get_http_request(),2);
        send_data(get_http_request());
        
        return receive_data(100);
        
    } else
        return "I didn't find a \"/\" in your relativeURL.\nrelativeURL example: \n\t1. /members/login.php\n\t2. /members/login.php?user=admin";

}

const string HTTP_Connection::receive_data(const long int buffer_size) {
    //SOCKET IS IN O_NONBLOCK MODE, FROM _connect().
    //IF YOU WANT IT IN BLOCK MODE:
    //set_socket_flags(fcntl(get_socket_fd(), F_GETFL, 0) & ~O_NONBLOCK);
    //fcntl(get_socket_fd(), F_SETFL, get_socket_flags());

    vector<char> received_data(buffer_size);
    memset(received_data.data(), 0, buffer_size);
    socklen_t address_len = sizeof (struct addrinfo);
    string received_data_str;
    int data_bytes = 0;
    //int total_bytes_received=0;

    for (;;) {//loop because socket in O_NONBLOCK mode.

        //preparation for select() 
        FD_ZERO(get_fdset_read());
        FD_SET(get_socket_fd(), get_fdset_read());

        set_select_check(select(get_socket_fd() + 1, get_fdset_read(), NULL, get_fdset_error(), get_timeval()));

        if (select_ok("receive_data")) {



            data_bytes = check_recvfrom(recvfrom(get_socket_fd(), received_data.data(), buffer_size, flags(), get_target_struct()->ai_addr, &address_len));



            if (data_bytes < 0) //if recvfrom returns negative, there are two reasons
            {

                if (errno == EINTR || errno == EAGAIN) //your buffer is not ready and you have to try again.
                    continue; //try again

                else //or there is a real problem.
                {
                    return "Can't receive msg!";
                }
            } else if (data_bytes == 0) { //if recvfrom  returns zero, there are two reasons

                if (received_data_str.size() != 0)//probably you just finished and the server shutdown() 
                {
                    break;
                } else //or there is no message to receive
                {
                    return "No msg received!";
                }
            } else {
                string tmp(received_data.data(), data_bytes);
                received_data_str += tmp;
                memset(received_data.data(), 0, buffer_size);
                tmp.clear();
                // total_bytes_received+=data_bytes;
            }

        } else //if select_ok() is not ok.
        {
            if (received_data_str.size() > 0) {
                break;
            } else {
                return "No msg received!";
            }
        }

    }

    set_received_data_flag(received_data_str);

    return received_data_str;
}

const int HTTP_Connection::set_received_data_flag(const string received_data) {
    size_t pos1 = 0;
    size_t pos2 = 0;
    size_t pos3 = 0;

    string header;
    string body;
    string Content_Length = "Content-Length:";
    int Length = 0;

    pos1 = received_data.find("\n\r"); //header finishes here
    header = received_data.substr(0, pos1); //get header
    body = received_data.substr(pos1 + 3); //get body
    pos2 = header.find(Content_Length); //search for the content length header field
    Content_Length = header.substr(pos2 + Content_Length.size()); //get the value of content length+the remaining fields
    pos3 = Content_Length.find("\r\n"); //go to the end of the content length field
    Content_Length = Content_Length.substr(0, pos3); //get the value of content-length

    //check if we found the content-length field
    if (pos2 == std::string::npos) {
        debugger.print_msg("check_received_data()", "Sorry i can't find Content-Length. This is not necessary a problem. Just make sure you have the expected response from the server.");
        this->received_data_flag = 0;
        return get_received_data_flag();
    } else {
        //Try to convert to integer.
        try {
            Length = std::stoi(Content_Length); //convert to integer
        }//If we have unexpected rubbish, catch the exception from stoi.
        catch (...) {
            debugger.print_msg("check_received_data()", "Sorry i can't convert Content-Length to integer. I am trying to convert:\n\n" + Content_Length + "\n\n to integer. Perhaps you can continue without a problem. Just make sure you have the expected response from the server.");
            this->received_data_flag = 0;
            return get_received_data_flag();
        }

    }

    //if everything is ok and we have our Content-Length, check if the response is ok.
    if (body.size() == Length && body.find("</html>") != std::string::npos) {
        debugger.print_msg("check_received_data()", "Body Length seems ok and i can see a </html>. Everything seems cool...");
        this->received_data_flag = 2;

    } else if (body.size() == Length && body.find("</html>") == std::string::npos) {
        debugger.print_msg("check_received_data()", "Body Length seems ok but i CAN'T see a </html>. I believe that we are ok...");
        this->received_data_flag = 1;
    } else if (body.size() != Length && body.find("</html>") == std::string::npos) {
        debugger.print_msg("check_received_data()", "Body Length is:\t" + std::to_string(body.size()) + "\tand expected length is:\t" + std::to_string(Length) + "\t. Furthermore i CAN'T see a </html>. I think i wasn't able to get the expected response from the server!");
        this->received_data_flag = -2;
    } else if (body.size() != Length && body.find("</html>") != std::string::npos) {
        debugger.print_msg("check_received_data()", "Body Length is:\t" + std::to_string(body.size()) + "\tand expected length is:\t" + std::to_string(Length) + "\t. I can see a </html>, but i believe i wasn't able to get the expected response from the server!");
        this->received_data_flag = -1;
    } else {
        debugger.print_msg("check_received_data()", "Unexpected behavior. Body Length is:\t" + std::to_string(body.size()) + "\tand expected length is:\t" + std::to_string(Length) + "\t. Now it's up to you to check if you have the expected response!");
        this->received_data_flag = 0;
    }

    return get_received_data_flag();
}

const int HTTP_Connection::get_received_data_flag() const {
    return this->received_data_flag;
}

const string HTTP_Connection::get_Accept() const {
    return this->Accept;
}

const string HTTP_Connection::get_Accept_Encoding() const {
    return this->Accept_Encoding;
}

const string HTTP_Connection::get_Accept_Language() const {
    return this->Accept_Language;
}

const string HTTP_Connection::get_User_Agent() const {
    return this->User_Agent;
}

const string HTTP_Connection::get_relativeURL() const {
    return this->relativeURL;
}

const string HTTP_Connection::get_http_request() const {
    return this->http_request;
}

const string HTTP_Connection::get_Cookie() const {
    return this->Cookie;
}

const string HTTP_Connection::get_Content_Length() const {
    return this->Content_Length;
}

const string HTTP_Connection::get_Content_Type() const {
    return this->Content_Type;
}

const string HTTP_Connection::get_Host() const {
    return this->Host;
}

const int HTTP_Connection::get_verbose_level() const{
    return debugger.get_verbose_level();
}

const int HTTP_Connection::set_Accept(const string Accept) {
    this->Accept = "Accept: "+Accept;
    return 0;
}

const int HTTP_Connection::set_Accept_Encoding(const string Accept_Encoding) {
    this->Accept_Encoding = "Accept-Encoding: "+Accept_Encoding;
    return 0;
}

const int HTTP_Connection::set_Accept_Language(const string Accept_Language) {
    this->Accept_Language = "Accept-Language: "+Accept_Language;
    return 0;
}

const int HTTP_Connection::set_User_Agent(const string User_Agent) {
    this->User_Agent = "User-Agent: "+User_Agent;
    return 0;
}

const int HTTP_Connection::set_Content_Length(const string Content_Length) {
    this->Content_Length= "Content-Length: "+Content_Length;
    return 0;
}

const int HTTP_Connection::set_Content_Type(const string Content_Type) {
    this->Content_Type= "Content-Type: "+Content_Type;
    return 0;
}

const int HTTP_Connection::set_Cookie(const string Cookie) {
    this->Cookie="Cookie: "+Cookie;
    return 0;
}

const int HTTP_Connection::set_Host(const string Host) {
    this->Host="Host: "+Host;
    return 0;
}

const int HTTP_Connection::set_relativeURL(const string relativeURL) {
    this->relativeURL = relativeURL;
    return 0;
}

const int HTTP_Connection::set_verbose_level(const int verbose_level) {
   TCP_Connection::set_verbose_level(verbose_level);
   return debugger.set_verbose_level(verbose_level);    
}




const bool HTTP_Connection::check_relativeURL(const string relativeURL) const {
    if (relativeURL.find("/") != std::string::npos)
        return true;
    else {
        debugger.print_msg("POST", "I didn't find a \"/\" in your relativeURL.\nrelativeURL example: \n\t1. /members/login.php\n\t2. /members/login.php?user=admin");
        return false;
    }

}

const int HTTP_Connection::create_request(const string method) {

    this->http_request.clear();

    this->http_request = method + " " + get_relativeURL() + " HTTP/1.1";

    //check if the user has set the header fields and add them to the http request.
    if (get_Host().size() != 0)
        this->http_request = this->http_request + "\r\n" + get_Host();
    
    if (get_Accept().size() != 0)
        this->http_request = this->http_request + "\r\n" + get_Accept();

    if (get_Accept_Encoding().size() != 0)
        this->http_request = this->http_request + "\r\n" + get_Accept_Encoding();

    if (get_Accept_Language().size() != 0)
        this->http_request = this->http_request + "\r\n" + get_Accept_Language();

    if (get_User_Agent().size() != 0)
        this->http_request = this->http_request + "\r\n" + get_User_Agent();
    
    if (get_Cookie().size() != 0)
        this->http_request = this->http_request + "\r\n" + get_Cookie();
    
    if (get_Content_Length().size() != 0)
        this->http_request = this->http_request + "\r\n" + get_Content_Length();
    
    if (get_Content_Type().size() != 0)
        this->http_request = this->http_request + "\r\n" + get_Content_Type();

    this->http_request = this->http_request + "\r\n\r\n";

    return 0;

}

HTTP_Connection::~HTTP_Connection() {
    debugger.print_destructor();
}

