/* 
 * File:   Protocol.cpp
 * Author: Dimopoulos Elias
 * LinkedIn: https://gr.linkedin.com/in/DimopoulosElias
 * e-mail: Dimopoulos.Elias@gmail.com
 * website: http://www.blackhat-ops.com
 * facebook: https://www.facebook.com/blackhat.operations
 * twitter: DimopoulosElias
 * Created on August 2, 2015, 1:16 AM
 */

#define D(a) cout<<endl<<"--------->"<<a<<"<------------"<<endl
#define MAX_TIMEOUT 600

#include "headers/Protocol.h"
#include <iostream>
using std::cout;
using std::cin;
using std::endl;
#include <cstring> //memset
#include <fcntl.h> //F_SETFL
#include <sys/select.h>//select()
#include <errno.h>//errno
#include <vector>
using std::vector;



Protocol::Protocol(const Target* target, const int sec_timeout, const string socket_type, const bool debug, const int verbose_level)
:
debugger("Protocol", debug, verbose_level),
target(*target),
socket_type(check_socket_type(socket_type)),
sec_timeout(check_sec_timeout(sec_timeout)),
timeout(initialize_timeout()),
target_restrictions_struct(initialize_struct()) {

    if (get_socket_type() == 3) {
        throw ("I think you chose to kill your Protocol, because socket_type is not supported. Exception in Protocol's Constructor");
        delete this;
    }


    debugger.print_constructor();

    if (!try_getaddrinfo(getaddrinfo(target->get_target_c_str(), target->get_port_c_str(), &this->target_restrictions_struct, &this->target_struct))) {
        throw ("getaddrinfo didn't succeed. I can't use any kind of sockets. Killing your Protocol object!");
        delete this;
    }
       

}

const int Protocol::check_socket_type(const string socket_type) const {
    if (socket_type != "SOCK_STREAM" && socket_type != "SOCK_DGRAM") {
        int choice = 0;

        cout << "*MSG from class \"Protocol\"-->function \"initialize_struct\" :" << endl;
        cout << "++Socket type \"" << socket_type << "\"" << "is not supported!++" << endl;
        cout << "++Supported socket types (1)\"SOCK_STREAM\" and (2)\"SOCK_DGRAM\".++" << endl;
        cout << "Choose 1, 2, or 3 to kill this object: ";
        cin>>choice;

        if (choice == 1)
            return 1;
        else if (choice == 2)
            return 2;
        else {
            cout << "\n You chose to kill this object. Make sure to catch the exception, or be ready for a Segmentation fault if you use this object below to your code!" << endl;
            return 3;
        }

    } else if (socket_type == "SOCK_STREAM")
        return 1;
    else if (socket_type == "SOCK_DGRAM")
        return 2;
    else return 3; //perhaps more socket types some day


}

const int Protocol::check_sec_timeout(const int sec_timeout) const {

    if (sec_timeout > MAX_TIMEOUT || sec_timeout<1 ) {
        cout << "*MSG from class \"Protocol\"-->function \"check_sec_timeout\" :" << endl;
        cout << "++You set timeout "<<sec_timeout<<" secs."<<"You must set 0< timeout <="<<MAX_TIMEOUT<<". Changing it to " << MAX_TIMEOUT << " sec++" << endl;

        return MAX_TIMEOUT;
    } else return sec_timeout;

}

const timeval Protocol::initialize_timeout() {
    //this block for member initialization
    timeval tmp;
    tmp.tv_sec = get_sec_timeout();
    tmp.tv_usec = 0;

    //in case where a select() does not succeed 
    timeout.tv_sec = get_sec_timeout();
    timeout.tv_usec = 0;

    return tmp; //use this in member initialization
}

const char* const Protocol::initialize_port(const int port) const {
    string port_str = std::to_string(port);
    return port_str.c_str();
}

const struct addrinfo Protocol::initialize_struct() {

    tmp_target_struct = unique_ptr<addrinfo> (new addrinfo);

    if (get_socket_type() == 2)
        tmp_target_struct->ai_socktype = SOCK_DGRAM;

    else
        tmp_target_struct->ai_socktype = SOCK_STREAM;

    tmp_target_struct->ai_family = AF_UNSPEC; // Allow IPv4 or IPv6 
    tmp_target_struct->ai_flags = 0;
    tmp_target_struct->ai_protocol = 0; // Any protocol 

    return *tmp_target_struct;

}

const int Protocol::get_socket_type() const {
    return this->socket_type;
}

bool Protocol::try_getaddrinfo(int getaddrinfo_error_code) {

    if (getaddrinfo_error_code != 0) {
        debugger.print_msg("Constructor-getaddrinfo", string(gai_strerror(getaddrinfo_error_code), strlen(gai_strerror(getaddrinfo_error_code))));
        return false;
    } else {
        debugger.print_msg("Constructor", "getaddrinfo success! ");
        return true;
    }
}

const int Protocol::get_socket_fd() const {
    return this->socket_file_descriptor;
}

const int Protocol::get_socket_flags() const {
    return this->socket_fd_flags;
}

const int Protocol::get_select_check() const {
    return this->select_check;
}

timeval* const Protocol::get_timeval() {
    return &this->timeout;
}

const addrinfo* const Protocol::get_target_struct() const{
    return this->target_struct;
}

fd_set * const Protocol::get_fdset_read() {
    return &this->read;
}

fd_set * const Protocol::get_fdset_error() {
    return &this->error;
}

fd_set * const Protocol::get_fdset_write() {
    return &this->write;
}

const int Protocol::set_socket_fd(const int socket_file_descriptor) {

    if (socket_file_descriptor == -1) {
        debugger.print_msg("set_socket_fd()", "socket error!\nerrno:\t" + string(strerror(errno)));
        return -1;
    } else {
        this->socket_file_descriptor = socket_file_descriptor;
        return 0;
    }

}

const int Protocol::set_socket_flags(const int socket_flags) {
    this->socket_fd_flags = socket_flags;
    return 0;
}

const int Protocol::set_select_check(const int select_check) {
    this->select_check = select_check;
    return 0;
}

const int Protocol::get_sec_timeout() const {
    return this->sec_timeout;
}


const int Protocol::_connect() {

    if (set_socket_fd(socket(this->target_struct->ai_family, this->target_struct->ai_socktype,
            this->target_struct->ai_protocol)) == -1) {
        return -1;
    } else//everything ok with the socket.
    {

        //SET SOCKET IN O_NONBLOCK MODE.
        //IF YOU WANT IT IN BLOCK MODE:
        //comment the following two lines
        set_socket_flags(fcntl(get_socket_fd(), F_GETFL, 0));
        fcntl(get_socket_fd(), F_SETFL, get_socket_flags() | O_NONBLOCK);
        set_socket_flags(fcntl(get_socket_fd(), F_GETFL, 0));

        //connect with the socket.We use O_NONBLOCK sockets, show this should not happen
        if (connect(get_socket_fd(), this->target_struct->ai_addr, this->target_struct->ai_addrlen) == -1 && errno != EINPROGRESS) {
            debugger.print_msg("_connect", "connect error!\nerrno:\t" + string(strerror(errno)));
            return -1;
        }

        //this should never happen
        if (this->target_struct == NULL) {
            debugger.print_msg("_connect", "Target struct is null!\nerrno:\t" + string(strerror(errno)));
            return -1;
        }

        return 0;
    }

}

const int Protocol::send_data(const string send_data) {

    //SOCKET IS IN O_NONBLOCK MODE, FROM _connect().
    //IF YOU WANT IT IN BLOCK MODE:
    //set_socket_flags(fcntl(get_socket_fd(), F_GETFL, 0) & ~O_NONBLOCK);
    //fcntl(get_socket_fd(), F_SETFL, get_socket_flags());

    socklen_t address_len = sizeof (struct addrinfo);

    //preparation for select() 
    FD_ZERO(&this->write);
    FD_SET(get_socket_fd(), &this->write);

    set_select_check(select(get_socket_fd() + 1, NULL, &this->write, &this->error, &timeout)); //set Protocol timeout with select() 

    if (select_ok("send_data")) {

        if (FD_ISSET(get_socket_fd(), &this->write)) {

            if (sendto(get_socket_fd(), send_data.c_str(), send_data.length(), 0, this->target_struct->ai_addr, address_len) != send_data.length()) {
                debugger.print_msg("send_data", "failed send the data!\nerrno:\t" + string(strerror(errno)));
                return -1;

            } else {//if everything is ok.
                debugger.print_msg("send_data", "Successfully sent the data!\nerrno:\t" + string(strerror(errno)));
                return 0;
            }
        } else //if FD_ISSET goes wrong (this should not happen.)
        {
            debugger.print_msg("send_data", "Something went wrong. Failed send data!\nerrno:\t" + string(strerror(errno)));
            return -1;
        }

    } else //if select_ok() is not ok.
    {
        debugger.print_msg("send_data", "Select_ok() failed! No data sent.\nerrno:\t" + string(strerror(errno)));
        return -1;
    }



}

const string Protocol::receive_data(const long int buffer_size) {

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
        FD_ZERO(&this->read);
        FD_SET(get_socket_fd(), &this->read);

        set_select_check(select(get_socket_fd() + 1, &this->read, NULL, &this->error, &timeout));

        if (select_ok("receive_data")) {



            data_bytes = check_recvfrom(recvfrom(get_socket_fd(), received_data.data(), buffer_size, flags(), this->target_struct->ai_addr, &address_len));



            if (data_bytes < 0) //if recvfrom returns negative, there are two reasons
            {

                if (errno == EINTR || errno == EAGAIN) //your buffer is not ready and you have to try again.
                    continue; //try again

                else //or there is a real problem.
                {
                    // delete received_data;
                    return "Can't receive msg!";
                }
            } else if (data_bytes == 0) { //if recvfrom  returns zero, there are two reasons

                if (received_data_str.size() != 0)//probably you just finished and the server shutdown() 
                {
                    //  delete received_data;
                    return received_data_str;
                } else //or there is no message to receive
                {
                    //  delete received_data;
                    return "No msg received!";
                }
            } else {
                string tmp(received_data.data(), data_bytes);
                received_data_str += tmp;
                memset(received_data.data(), 0, buffer_size);
                tmp.clear();
                // total_bytes_received+=data_bytes;
                // D(total_bytes_received);
            }

        } else //if select_ok() is not ok.
        {
            if (received_data_str.size() > 0) {
                //delete received_data;
                return received_data_str;
            } else {
                //delete received_data;
                return "No msg received!";
            }
        }

    }

}

const int Protocol::flags() const {
    if (get_socket_type() == 1)//tcp
        return MSG_WAITALL;

    else if (get_socket_type() == 1)
        return 0;

    else//maybe other socket types some day
        return 0;
}

const bool Protocol::select_ok(const string calling_function) {

    if (get_select_check() < 0) {
        debugger.print_msg("select_ok() called from " + calling_function, "Error in select().\nerrno:\t" + string(strerror(errno)));
        initialize_timeout();
        return false;
    } else if (get_select_check() == 0) {
        debugger.print_msg("select_ok() called from " + calling_function, "Protocol timed out.\nerrno:\t" + string(strerror(errno)));
        initialize_timeout();
        return false;
    } else {
        return true;
    }


}

const int Protocol::check_recvfrom(const int recv_from) const {

    if (recv_from == 0) {
        debugger.print_msg("check_recvfrom()", "No msg received.\nerrno:\t" + string(strerror(errno)));
        return 0;
    } else if (recv_from > 0) {
        debugger.print_msg("check_recvfrom()", std::to_string(recv_from) + " bytes msg received!\nerrno:\t" + string(strerror(errno)));
        return recv_from;
    } else {
        debugger.print_msg("check_recvfrom()", "Error receiving the message!\nerrno:\t" + string(strerror(errno)));
        return -1;
    }


}

const int Protocol::get_verbose_level() const{
    return debugger.get_verbose_level();
}

const int Protocol::set_verbose_level(const int verbose_level) {
   return debugger.set_verbose_level(verbose_level);    
}

const int Protocol::set_timeout(const int sec_timeout) {
    
   this->sec_timeout = check_sec_timeout(sec_timeout);
   initialize_timeout();
   return get_sec_timeout();

}


Protocol::~Protocol() {
    debugger.print_destructor();
    close(get_socket_fd());
}

