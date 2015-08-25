/* 
 * File:   main.cpp
 * Author: Dimopoulos Elias
 * LinkedIn: https://gr.linkedin.com/in/DimopoulosElias
 * e-mail: Dimopoulos.Elias@gmail.com
 * website: http://www.blackhat-ops.com
 * facebook: https://www.facebook.com/blackhat.operations
 * twitter: DimopoulosElias
 * Created on August 2, 2015, 1:12 AM
 */

//C++ 11
//#include <limits>
#include <cstdlib>
#include <string>
#include "sources/headers/HTTP_Connection.h"
//#include "Connection.h"
//#include "Debugger.h"
//#include "Target.h"
//#include "TCP_Connection.h"
#include <iostream>

using namespace std;


/*
 * 
 */
int main(int argc, char** argv) {
    size_t pos=0;
    size_t pos2=0;
    string header;
    string body;
    string Content_Length="Content-Length:";
    int Length=0;

    string target="postcatcher.in";
    //string target="www.blackhat-ops.com";
   
    string received_buffer;
    bool debug = true;
    bool Connection_error=false;

    const int buffersize=10000;
    
    Target *target1;
    target1=new Target(target, 80,false);
  
   // cout<<"===>"<<target1->get_port()<<endl;
   
    Connection *connection1;
            
    try {
        connection1=new HTTP_Connection(target1, 10,true,3);

    } catch (char const *error) {
        cout << "(main)ERROR!\n"<<"++"<<error<<"++"<<endl;
        Connection_error=true;     
    }
    
    if(!Connection_error)
    {
   connection1->_connect();
   //connection1->send_data("OPTIONS /fsdgedsrtfer.html HTTP/1.1\nHost: www.blackhat-ops.com\n\n");   
   
  //received_buffer=connection1->GET("/");
  //connection1->set_verbose_level(3);
 // cout<<connection1->get_verbose_level();
//connection1->set_timeout(25);
   connection1->set_Accept("*/*");
   connection1->set_Content_Type("application/x-www-form-urlencoded");
   connection1->set_User_Agent("Mozilla/5.0 (X11; Linux x86_64; rv:38.0) Gecko/20100101 Firefox/38.0 Iceweasel/38.2.0");
   received_buffer=connection1->POST("/catchers/55d8e4628190a703000062b5","user: test6");
   
   
   //target1->send_data("list");
   //received_buffer=connection1->receive_data(buffersize);
   //cout<<"----+++=========>"<<connection1->get_received_data_flag()<<endl;
   cout<<received_buffer<<endl;
  //pos=received_buffer.find("Content-Length:");
   //substr=received_buffer.substr(pos);
   //cout<<received_buffer.size()<<"\t"<<substr.size()<<endl;
  // pos=received_buffer.find("\n\r");
  // header=received_buffer.substr(0,pos);
  // body=received_buffer.substr(pos+3);
  // cout<<"----------->\n"<<header<<"\n<----------------\n";
   //pos=header.find(Content_Length);
  // Content_Length=header.substr(pos+Content_Length.size());
  // pos2=Content_Length.find("\n");
   //Content_Length=Content_Length.substr(0,pos2);
   //cout<<"====>"<<Content_Length.at(0)<<"<===="<<endl;
   //Length=std::stoi(Content_Length);
   //cout<<"----------->\n"<<Length<<"\n<----------------\n";
  // cout<<"----------->\n"<<body<<"\n<----------------\n";
  // cout<<body.size();
  // received_buffer.clear();
   //target1->send_data("help\n");
  // received_buffer=target1->receive_data(buffersize);
  // cout<<received_buffer<<endl;
   // target1->send_data("GET /sdfsdf.html HTTP/1.1\nHost: www.blackhat-ops.com\n\n");
 //  received_buffer=target1->receive_data(buffersize);
   //target1->send_data("GET /sdfsdf.html HTTP/1.1\nHost: www.blackhat-ops.com\n\n");
   //received_buffer=target1->receive_data(buffersize);
   //cout<<received_buffer<<endl;
       delete connection1;

    }
    //target1->_connect();

    // Debugger debug1("class 1",true);
    //  debug1.error("test","test");
 
    return 0;
}

