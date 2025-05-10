// this is the .h file for the IF switch code
//
// Nov 24,2024
//



// Enter a MAC address and IP address for your controller below.
// The IP address will be dependent on your local network:

#ifndef IF_SWITCH

#define DEBUG


#define MACADDRESS 0xBE,0xEF,0x0B,0xDE,0xAD,0x05
#define MYIPADDR 192,168,1,60
#define MYIPMASK 255,255,255,0
#define MYDNS 192,168,1,1
#define MYGW 192,168,1,1
#define LISTENPORT 80
#define UARTBAUD 115200

//  Set up the output pins
#define D0 18
#define D1 3
#define D2 4
#define D3 5
#define D4 6
#define D5 7
#define D6 8
#define D7 9

#define OE 17
#define SELECT1 16
#define SELECT2 15
#define SELECT3 14

// Some constants
#define OFF 0
#define ON 1

const uint8_t mac[6] = {MACADDRESS};
const uint8_t myIP[4] = {MYIPADDR};
const uint8_t myMASK[4] = {MYIPMASK};
const uint8_t myDNS[4] = {MYDNS};
const uint8_t myGW[4] = {MYGW};

// Initialize the Ethernet server library
// with the IP address and port you want to use 
// (port 80 is default for HTTP):

//const char doc_head[]="<!DOCTYPE HTML><html><head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\"></head><body> \                                                            
  <center><h3>W2SZ/MGEF RF Matrix Switch by KI2L <a href=\"/\">Refresh</a></h3>";

//const char doc_tail[]="<p><hr></body></html></a>";

//const char green_button[]="_ON\"><button style=\"background-color:green\">ON</button></a>";
//const char red_button[]="_OFF\"><button style=\"background-color:red\">OFF</button></a>";

// Variables to store the user input
#define MAXNAME 10
#define ROWCOUNT 6
#define COLCOUNT 4

char rows[ROWCOUNT][MAXNAME + 1];
char cols[COLCOUNT][MAXNAME + 1];

EthernetServer server = EthernetServer(LISTENPORT);
EthernetUDP udpClient;

#ifdef SYSLOG_ENABLE
IPAddress syslogServer(192, 168, 1, 125);
const int syslogPort = 514;

#define SYSLOG_SERVER "dev01.web9.com"
#define SYSLOG_PORT 514

#endif

// This device info
#define DEVICE_HOSTNAME "MGEF IF Switch"
#define APP_NAME "standalone-app"

#ifdef SYSLOG_ENABLE
Syslog syslog(udpClient, SYSLOG_SERVER, SYSLOG_PORT, DEVICE_HOSTNAME, APP_NAME, LOG_KERN);
#endif

// Relay state and pin

#define MATRIX1LEN  16
#define MATRIX2LEN 8

bool matrix1state[MATRIX1LEN];
bool matrix2state[MATRIX2LEN];

// Client variables 

#define MAXLINELEN 107
char linebuf[MAXLINELEN];
int charcount=0;
char count=0;

#define RESTORED "#CCFFCC";     // Header colors to indicate saved state of the config. Green is good
#define UNSAVED "#ffcccc";

char *header_color;

#define IF_SWITCH
#else     // we've already been here, set all the variables to externs

extern const uint8_t mac[];
extern const uint8_t myIP[];
extern const uint8_t myMASK[];
extern const uint8_t myDNS[];
extern const uint8_t myGW[];

extern const char doc_head[];

extern const char doc_tail[];

extern const char green_button[];
extern const char red_button[];

// EthernetServer server = EthernetServer(LISTENPORT);

// Relay state and pin
extern const int matrix1len;
extern const int matrix2len;
extern bool matrix1state[];
extern bool matrix2state[];

// Client variables 

extern char linebuf[];
extern int charcount;
extern char count;

#endif
