//
// pages.ino
//
// Code to emit the various html pages for the IF switch
//
// Nov 25, 2024   Bob Bownes
//
//


#include "if_switch.h"


///////////////////////////////////////////////////////// DashboardPage /////////////////////////////////////
// Display dashboard page with on/off buttons for relays, Clear, Save/Recall, and Configure buttons
void dashboardPage(EthernetClient &client) {
  int li, yl = 0;
  int x;

  client.println(F("HTTP/1.1 200 OK"));
  client.println(F("Content-Type: text/html"));
  client.println(F("Connection: close"));
  client.println("");  // Blank line separating headers from the body
  client.println(F("<!DOCTYPE HTML><html><head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\"></head><body> \
  <center><h3>W2SZ/MGEF RF Matrix Switch by KI2L <a href=\"/\">Refresh</a></h3>"));  // Document header is stored as a constant to save variable space.
  client.println(F("<center><img src=\"https://w2sz.org/images/W2SZ_small.gif\"><p>"));
  client.print(F("<style>table {border-collapse: collapse;margin: 20px;}td, th {border: 1px solid #ddd;padding: 10px;text-align: center;}\
        button { \
            width: 100%;\
            height: 100%;\
            font-size: 14px;\
        }\
        .header {\
            background-color: "));
  client.print(header_color);       // Change the header color depending on if the state is saved. Green - Good, Red - Bad
  client.println(F(";}</style>"));
  client.println(F("<table>"));


  client.print(F("<tr class=\"header\"><th></th><th>")); client.print(cols[0]);
  client.print(F("</th><th>")); client.print(cols[1]);
  client.print(F("</th><th>")); client.print(cols[2]);
  client.print(F("</th><th>")); client.print(cols[3]); client.println(F("</th></tr>"));

  client.println(F("<tr><th>")); client.print(rows[yl]); client.println(F("</th>"));



  for (li = 0; li < MATRIX1LEN; li++) {

    if (matrix1state[li] == OFF) {
      client.print("<td><a href=\"/m1st_"); client.print(toascii(li)); client.print(F("_OFF\"><button style=\"background-color:red\">OFF</button></a>")); client.println("</td>");
    }
    // If relay is on, it shows the button to turn the output off
    else if (matrix1state[li] == ON) {
      client.print("<td><a href=\"/m1st_"); client.print(toascii(li)); client.print(F("_ON\"><button style=\"background-color:green\">ON</button></a>")); client.println("</td>");
    }
    if (li == 3 || li == 7 || li == 11 || li == 15) { // groups of four
      yl++;
      client.println(F("</tr>"));
      client.print(F("<tr><th>")); client.print(rows[yl]); client.println(F("</th>"));
    }

  }

  //// Second relay group ///////

  for (li = 0; li < MATRIX2LEN; li++) {  // If the relay is off, button is red and is labeled 'OFF'
    if (matrix2state[li] == OFF) {
      client.print(F("<td><a href=\"/m2st_")); client.print(toascii(li)); client.print(F("_OFF\"><button style=\"background-color:red\">OFF</button></a>")); client.println("</td>");
    }

    // If relay is on, button is green and is labeled 'ON'

    else if (matrix2state[li] == ON) {
      client.print(F("<td><a href=\"/m2st_")); client.print(toascii(li)); client.print(F("_ON\"><button style=\"background-color:green\">ON</button></a>")); client.println("</td>");
    }

    if (li == 3) {     // groups of four, terminate the row
      yl++;
      client.println(F("</tr>"));
      client.print(F("<th>")); client.print(rows[yl]); client.println(F("</th>"));
    }
    else if (li == 7) { // all over. terminate the last row
      client.println(F("</tr>"));
    }
  }
  client.println(F("</tr></table>"));

  client.print(F("<a href=\"/ee_clear\"><button style=\"background-color:#aaaaaa\">Clear Relays</button></a>"));
  client.print(F("<a href=\"/ee_save\"><button style=\"background-color:#3377ff\">Save State</button></a>"));
  client.print(F("<a href=\"/ee_restore\"><button style=\"background-color:#FFCE33\">Restore State</button></a>"));
  client.println(F("<a href=\"/config\"><button style=\"background-color:#66CE33\">Configure System</button></a>"));

  client.println(F("<p><hr></body></html>"));

}


////////////////////////////////////////// Begin error_404 /////////////////////

void error_404(EthernetClient &client) {

#ifdef DEBUG
  Serial.println(F("404"));
#endif
  //    pageheader(client);
  client.println(F("ErrorDocument 404 /"));
  client.println(F(""));
}
////////////////////////////////////////// End error_404 /////////////////////

////////////////////////////////// EEPROM Manipulation /////////////////////////

void eeprom_clear() {
#ifdef DEBUG
  Serial.println(F("Clear"));
#endif

  memset(matrix1state, 0, MATRIX1LEN);
  memset(matrix2state, 0, MATRIX2LEN);
  //  EEPROM.put(0, matrix1state);
  //  EEPROM.put(MATRIX1LEN + 1, matrix2state);
  updaterelays();
}


void eeprom_save() {
#ifdef DEBUG
  Serial.println(F("EEProm Save"));
#endif
  EEPROM.put(0, matrix1state);
  EEPROM.put(MATRIX1LEN + 1, matrix2state);
  EEPROM.put(MATRIX1LEN + MATRIX2LEN + 2, rows);
  EEPROM.put(MATRIX1LEN + MATRIX2LEN + 3 + sizeof(rows), cols);
  header_color = RESTORED;
}

void eeprom_restore() {
#ifdef DEBUG
  Serial.println(F("EEPROM Restore"));
#endif
  EEPROM.get(0, matrix1state);
  EEPROM.get(MATRIX1LEN + 1, matrix2state);
  EEPROM.get(MATRIX1LEN + MATRIX2LEN + 2, rows);
  EEPROM.get(MATRIX1LEN + MATRIX2LEN + 3 + sizeof(rows), cols);
  updaterelays();
  header_color = RESTORED;
}

////////////////////////////////////////// End eeprom_save/restore /////////////////////

//////////////////////////////////////////////// Config page /////////////////////////////////////
void configPage(EthernetClient &client) {
#ifdef DEBUG
  Serial.println(F("Config"));
#endif

  client.println(F("HTTP/1.1 200 OK"));
  client.println(F("Content-Type: text/html"));
  client.println(F("Connection: close"));
  client.println();  // Blank line separating headers from the body

  // HTML content for the second page
  client.println(F("<!DOCTYPE html>"));
  client.println(F("<html>"));
  client.println(F("<head><title>configuration</title></head>"));
  client.println(F("<body>"));
  client.println(F("<h1>Configure IF switch</h1>"));

  client.println(F("<h1>Please enter labels</h1>"));
  client.println(F("<form action=\"/\" method=\"GET\">"));
  client.println(F("Row 1: <input type=\"text\" name=\"row1\"><br>"));
  client.println(F("Row 2: <input type=\"text\" name=\"row2\"><br>"));
  client.println(F("Row 3: <input type=\"text\" name=\"row3\"><br>"));
  client.println(F("Row 4: <input type=\"text\" name=\"row4\"><br>"));
  client.println(F("Row 5: <input type=\"text\" name=\"row5\"><br>"));
  client.println(F("Row 6: <input type=\"text\" name=\"row6\"><br>"));
  client.println(F("Col 1: <input type=\"text\" name=\"col1\"><br>"));
  client.println(F("Col 2: <input type=\"text\" name=\"col2\"><br>"));
  client.println(F("Col 3: <input type=\"text\" name=\"col3\"><br>"));
  client.println(F("Col 4: <input type=\"text\" name=\"col4\"><br>"));
  client.println(F("<input type=\"submit\" value=\"Submit\">"));
  client.println(F("<br><a href='/'>Back to First Page</a>"));
  client.println(F("</body>"));
  client.println(F("</html>"));
  header_color = UNSAVED;

}

////////////////////////////////////////////// Parse the HTTP query and put the labels in the right places //////////////
void parse_labels(char *line) {

  char query_copy[MAXLINELEN];
  char *token;
  int count = 0, rowcount = 0, colcount = 0;

  // Copy query to avoid modifying original
  strncpy(query_copy, line, MAXLINELEN - 1);
  query_copy[MAXLINELEN - 1] = '\0';

  // Tokenize query string using '&' as the delimiter

  token = strtok(query_copy, "?");

  token = strtok(NULL, "&");

  while (token != NULL && count < (ROWCOUNT + COLCOUNT)) {

    char *eq_pos = strchr(token, '=');

    //Serial.print("eq_pos1=");Serial.println((char *)eq_pos);

    if (eq_pos != NULL) {

      eq_pos++;

      char *key = token;
      char *value = eq_pos;

      if (strstr(key, "row") > 0 ) {

        if (strlen(value) != 0) {
          memset(rows[rowcount], 0, MAXNAME + 1);
          strncpy(rows[rowcount], value, MAXNAME);
        }
        rowcount++;
      }
      else if (strstr(key, "col") > 0) {
        for (int c = 0; c <= MAXNAME; c++) {
          if (value[c] == ' ')
            value[c] = NULL;
        }
        if (strlen(value) != 0) {
          memset(cols[colcount], 0, MAXNAME + 1);
          strncpy(cols[colcount], value, MAXNAME);
        }

        colcount++;
      }
      count++;
    } // if (eq_pos != NULL)

    token = strtok(NULL, "&");

  } // while (token != NULL && count < (ROWCOUNT + COLCOUNT))

} // end parse_labels


////////////////////////////////////////// Begin Network config /////////////////////

void network_config_page(EthernetClient &client) {
#ifdef DEBUG
  Serial.println(F("Network Config"));
#endif
  client.println(F("<html>"));
  client.println(F("<head><title>Enter Networking Configuration</title></head>"));
  client.println(F("<body>"));
  client.println(F("<h1>Please enter four names</h1>"));
  client.println(F("<form action=\"/\" method=\"GET\">"));
  client.println(F("Mac Address: <input type=\"text\" name=\"MACADDR\"><br>"));
  client.println(F("IP Address: <input type=\"text\" name=\"IP\"><br>"));
  client.println(F("Netmask: <input type=\"text\" name=\"NETMASK\"><br>"));
  client.println(F("Gateway: <input type=\"text\" name=\"GW\"><br>"));
  client.println(F("DNS: <input type=\"text\" name=\"DNS\"><br>"));
  client.println(F("<p>NOTE: The system WILL reboot when you click 'Submit'</p>"));
  client.println(F("<input type=\"submit\" value=\"Submit\">"));
  client.println(F("</form>"));
  client.println(F("</body>"));
  client.println(F("</html>"));

}

////////////////////////////////////////// end config /////////////////////
