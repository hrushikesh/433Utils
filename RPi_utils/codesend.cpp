/*

 'codesend' hacked from 'send' by @justy
 
 - The provided rc_switch 'send' command uses the form systemCode, unitCode, command
   which is not suitable for our purposes.  Instead, we call 
   send(code, length); // where length is always 24 and code is simply the code
   we find using the RF_sniffer.ino Arduino sketch.

 Usage: ./codesend decimalcode
 (Use RF_Sniffer.ino to check that RF signals are being produced by the RPi's transmitter)
 */

#include "RCSwitch.h"
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <sstream>

using namespace std;

int PIN = 0;
RCSwitch* mySwitch;


void sendCode(int code) {
  cerr << "Sending: " << code << endl;
  mySwitch->send(code, 24);
}

     
int main(int argc, char *argv[]) {
  if (wiringPiSetup () == -1) return 1;
  RCSwitch mySwitch_ = RCSwitch();
  mySwitch = &mySwitch_;

  mySwitch->enableTransmit(PIN);
  
  if (argc > 1) {
    sendCode(atoi(argv[1]));
    return 0;
  }

  string line;
  while (std::getline(cin, line)) {
    cout << line << endl;
    if (line.find("COMMAND") != string::npos) {
      cerr << "Matched: " << line << endl;
      istringstream in(line);
      string str;
      in >> str;
      in >> str;
      cerr << "Sending str: " << str << endl;
      sendCode(atoi(str.c_str()));
    }
  }
}
