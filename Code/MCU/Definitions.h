// Fill ssid and password with your network credentials
const char* ssid = "SS";
const char* password = "A0*********b!cd"; 
const char* host = "script.google.com";
// Replace with your own script id to make server side changes
//const char *GScriptId = "AKfycbxlakK3J_h6KU6gyuV9qkqr8WIcTEE49_*******";
const char *GScriptId = "AKfycbwM_WVTc24x6JcUmoAefVfXrrZNH0sftz-*******";
int actuatorNum = 0;; // Number of actuators
int stepDurationActuator = 0; // Time between actuator's states in ms
int loopCycleTime = 0;
int loopSteps = 0;

int settingsArray[5] = {};
int motorsInputArray[7] = {};
int motorPin = 0;
String strFromCell;
String firstThreeCharacters;
const int httpsPort = 443;

// echo | openssl s_client -connect script.google.com:443 |& openssl x509 -fingerprint -noout
const char* fingerprint = "";
//const uint8_t fingerprint[20] = {};

// Write to Google Spreadsheet
String url = String("/macros/s/") + GScriptId + "/exec?value=HelloA";
// Fetch Google Calendar events for 1 week ahead
String url2 = String("/macros/s/") + GScriptId + "/exec?cal";
// Read from Google Spreadsheet
String url3 = String("/macros/s/") + GScriptId + "/exec?read";

String payload_base =  "{\"command\": \"appendRow\", \
                    \"sheet_name\": \"Sheet1\", \
                    \"values\": ";
String payload = "";

#define SETTINGS_PARAMETERS_NUM 4
#define BYTES_MASK 32-1
#define BYTES_TO_USE 10 // With the current setup we can use up to 10 states per loop. Each state is 0/1 so we have 10 bits   

unsigned int free_heap_before = 0;
unsigned int free_stack_before = 0;

static int connect_count = 0;
static int error_count = 0;
