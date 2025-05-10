

// The connection_data struct needs to be defined in an external file.
#include <SPI.h>
#include <UIPEthernet.h>
#ifdef SYSLOG_ENABLE
#include <Syslog.h>
#endif

#include <EEPROM.h>
#include <string.h>
#include "if_switch.h"

//#include <UIPServer.h>
//#include <UIPClient.h>


// 4x6 relay controller for MGEF VanZed
//
// By Bob Bownes KI2L
// Copyright 2024
//
// Revision History
// V2.0 Nov18,2024
// V3.0 Dec 12,2024
// V3.1 Jan 7,2025 - copied from working test. 
//

void loop() { /////////////////////////////// Main Loop ///////////////////////

  boolean currentLineIsBlank = true;
  int relay_num;
  char state[4];


  // listen for incoming clients
  EthernetClient client = server.available();
#ifdef LOOP_DEBUG
  //  Serial.println(F("client connected"));
#endif

  if (client) {

    //#ifdef LOOP_DEBUG
    Serial.print(F("new client "));
    //#endif
    Serial.println(client.remoteIP());

    memset(linebuf, 0, sizeof(linebuf));
    charcount = 0;

    // an http request ends with a blank line

    currentLineIsBlank = true;


    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        //read char by char HTTP request
        linebuf[charcount] = c;
        if (charcount < sizeof(linebuf) - 1) charcount++;

        // if you've gotten to the end of the line (received a newline
        // character) and the line is blank, the http request has ended,
        // so you can send a reply
        //        if (c == '\n' && currentLineIsBlank || (charcount - 1) >= MAXLINELEN) {

        if (c == '\n' && currentLineIsBlank ) {
          dashboardPage(client);
          break;
        }

        if (c == '\n') {
#ifdef LOOP_DEBUG
          Serial.println(F("linebuf =")); Serial.println(linebuf);
#endif
          if (strstr(linebuf, "GET /m1st") > 0) {

            *(strstr(linebuf, "_")) = ' ';
            *(strstr(linebuf, "_")) = ' ';

            sscanf(linebuf, "%*s %*s %d %s", &relay_num, state);
#ifdef LOOP_DEBUG
            Serial.print(F("relay_num=")); Serial.println(toascii(relay_num)); Serial.println(state);
#endif
            if (strcmp(state, "ON"))
              matrix1state[relay_num] = ON;
            else
              matrix1state[relay_num] = OFF;

            updaterelays();
            header_color=UNSAVED;
          }
          else if (strstr(linebuf, "GET /m2st") > 0) {
#ifdef LOOP_DEBUG
            Serial.println(linebuf);
#endif
            *(strstr(linebuf, "_")) = ' ';
            *(strstr(linebuf, "_")) = ' ';

            sscanf(linebuf, "%*s %*s %d %s", &relay_num, state);
#ifdef LOOP_DEBUG
            Serial.print(F("relay_num=")); Serial.println(toascii(relay_num)); Serial.println(state);
#endif
            if (strcmp(state, "ON"))
              matrix2state[relay_num] = ON;
            else
              matrix2state[relay_num] = OFF;

            updaterelays();
            header_color=UNSAVED;
          }
          else if (strstr(linebuf, "GET /ee_save") > 0) {
            eeprom_save();
            dashboardPage(client);
            break;
          }
          else if (strstr(linebuf, "GET /ee_restore") > 0) {
            eeprom_restore();
            dashboardPage(client);
            break;
          }
          else if (strstr(linebuf, "GET /ee_clear") > 0) {
            eeprom_clear();
            dashboardPage(client);
            break;
          }
          else if (strstr(linebuf, "GET /config") > 0) {
            configPage(client);
            break;
          }
          else if (strstr(linebuf, "?row1=") > 0) {
            parse_labels(linebuf);
          }

          else if (strstr(linebuf, "GET /cylon") > 0) {
#ifdef LOOP_DEBUG
            Serial.println(client.remoteIP());
#endif
            cylon(1, 1);
            updaterelays();
            //break;
          }

          // you're starting a new line
          currentLineIsBlank = true;
          memset(linebuf, 0, sizeof(linebuf));
          charcount = 0;
        }
        else if (c != '\r') {
          // you've gotten a character on the current line
          currentLineIsBlank = false;
        }
      }
    }


    // give the web browser time to receive the data
    delay(3);

    // close the connection:
    client.stop();
#ifdef DEBUG
    Serial.println(F("client disconnected"));
#endif
  }
}
