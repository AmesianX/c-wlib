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
#include "sources/headers/HTTPS.h"
#include <iostream>

using namespace std;

/*
 * 
 */
int main(int argc, char** argv) {


    string target = "www.blackhat-ops.com";

    string received_buffer;
    bool debug = true;
    bool Protocol_error = false;

    Target *target1;
    target1 = new Target(target, 80, false);

    Protocol *Protocol1;

    try {
        Protocol1 = new HTTP(target1, 10, true, 3);

    } catch (char const *error) {
        cout << "(main) ERROR!\n" << "+++" << error << "+++" << endl;
        Protocol_error = true;
    }

    if (!Protocol_error) {
        Protocol1->_connect();
        Protocol1->set_User_Agent("Mozilla/5.0 (X11; Linux x86_64; rv:38.0) Gecko/20100101 Firefox/38.0 Iceweasel/38.2.0");
        received_buffer = Protocol1->GET("/");

        cout << received_buffer << endl;
        delete Protocol1;

    }

    return 0;
}

