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

#include <string>
#include "sources/headers/HTTP_Connection.h"
#include <iostream>

using namespace std;

/*
 * 
 */
int main(int argc, char** argv) {


    string target = "www.blackhat-ops.com";

    string received_buffer;
    bool debug = true;
    bool Connection_error = false;

    const int buffersize = 10000;

    Target *target1;
    target1 = new Target(target, 80, false);

    Connection *connection1;

    try {
        connection1 = new HTTP_Connection(target1, 10, true, 3);

    } catch (char const *error) {
        cout << "(main)ERROR!\n" << "++" << error << "++" << endl;
        Connection_error = true;
    }

    if (!Connection_error) {
        connection1->_connect();
        connection1->set_User_Agent("Mozilla/5.0 (X11; Linux x86_64; rv:38.0) Gecko/20100101 Firefox/38.0 Iceweasel/38.2.0");
        received_buffer = connection1->GET("/");

        cout << received_buffer << endl;
        delete connection1;

    }

    return 0;
}

