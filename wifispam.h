// ===== Settings ===== //
const uint8_t channels[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11}; // used Wi-Fi channels (available: 1-14)
const bool wpa2 = true; // WPA2 networks
int spamtype = 1; // 1 = funny, 2 = rickroll, maybe more later

/*
  SSIDs:
  - don't forget the \n at the end of each SSID!
  - max. 32 characters per SSID
  - don't add duplicates! You have to change one character at least
*/
char ssids[]={};
uint8_t mac[6];

const char funnyssids[] PROGMEM = {

 "Wi-Fi infectada\n"
 "Minha Internet é Horrível\n"
 "Rede Aberta\n"
 "Tente outra vez...\n"
 "Isto não é uma rede Wi-Fi\n"
 "Mãe, acesse essa daqui\n"
 "Tenta a de baixo\n"
 "Procurando rede...\n"
 "Carregando...\n"
 "Obtendo seu IP...\n"
 "Pede para a minha mãe\n"
 "Em manutenção\n"
 "A senha é 1234\n"
 "Conexão com o além\n"
 "Eu sei onde você mora\n"
 "Não entre, hacker brabo\n"
 "Nem veloz, nem furiosa\n"
 "Acesso ao inferno\n"
 "Digita a senha baixo...\n"
 "O pai tá ON, você tá OFF\n"
 "Corpo de Bombeiros\n"
 "virus.exe\n"
 "VIVOFIBRA-00FL1N3\n"
 "WiFi da Prisão\n"
 "Internet do Motel\n"
 "Vírus grátis\n"
 "McDonald's WiFi\n"
 "CLARO-QU3N40\n"
 "Servidor de Minecraft\n"
 "Polícia Federal\n"
 "Exército Brasileiro\n"
 "Conecte-se com Deus\n"
 "Geme baixo vizinha\n"
 "Sua música está muito alta\n"
 "Você tem péssimo gosto musical\n"
 "Satan 66G\n"
 "Hotel clientes\n"
 "Interdimentional Net\n"
 "Rede Oculta\n"
 "Acesso não gratuito\n"
 "Conecte aqui e use seu cartão\n"
 "Apenas para uso pornô\n"
 "Ibis Guest\n"
 "Conecte-se e morra\n"
 "Digite sua senha\n"
 "🖕🖕🖕🖕🖕🖕🖕🖕🖕🖕🖕🖕\n"
 "Eu sei tudo sobre você\n"
 "Divulgarei seu histórico\n"
};

const char rickrollssids[] PROGMEM = {
  "01 Never gonna give you up\n"
  "02 Never gonna let you down\n"
  "03 Never gonna run around\n"
  "04 and desert you\n"
  "05 Never gonna make you cry\n"
  "06 Never gonna say goodbye\n"
  "07 Never gonna tell a lie\n"
  "08 and hurt you\n"
};

// run-time variables
char emptySSID[32];
char beaconSSID[32];
char randomName[32];
uint8_t channelIndex = 0;
uint8_t macAddr[6];
uint8_t wifi_channel = 1;
uint32_t packetSize = 0;
uint32_t packetCounter = 0;
uint32_t packetRateTime = 0;

#include <WiFi.h>

extern "C" {
#include "esp_wifi.h"
  esp_err_t esp_wifi_set_channel(uint8_t primary, wifi_second_chan_t second);
  esp_err_t esp_wifi_80211_tx(wifi_interface_t ifx, const void *buffer, int len, bool en_sys_seq);
}

const char* generateRandomName() {
  const char* charset = "!@?<>-_=+!@#$%&*()[]{}1234567890abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
  int len = rand() % 10 + 1; // Generate a random length between 1 and 10
  char* randomName = (char*)malloc((len + 1) * sizeof(char)); // Allocate memory for the random name
  for (int i = 0; i < len; ++i) {
    randomName[i] = charset[rand() % strlen(charset)]; // Select random characters from the charset
  }
  randomName[len] = '\0'; // Null-terminate the string
  return randomName;
}

char* randomSSID() {
  const char* charset = "!@?<>-_=+!@#$%&*()[]{}1234567890abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
  int len = rand() % 22 + 7; // Generate a random length between 1 and 10
  for (int i = 0; i < len; ++i) {
    randomName[i] = charset[rand() % strlen(charset)]; // S elect random characters from the charset
  }
  randomName[len] = '\0'; // Null-terminate the string
  return randomName;
}


uint8_t packet[128] = { 0x80, 0x00, 0x00, 0x00, //Frame Control, Duration
                /*4*/   0xff, 0xff, 0xff, 0xff, 0xff, 0xff, //Destination address
                /*10*/  0x01, 0x02, 0x03, 0x04, 0x05, 0x06, //Source address - overwritten later
                /*16*/  0x01, 0x02, 0x03, 0x04, 0x05, 0x06, //BSSID - overwritten to the same as the source address
                /*22*/  0xc0, 0x6c, //Seq-ctl
                /*24*/  0x83, 0x51, 0xf7, 0x8f, 0x0f, 0x00, 0x00, 0x00, //timestamp - the number of microseconds the AP has been active
                /*32*/  0x64, 0x00, //Beacon interval
                /*34*/  0x01, 0x04, //Capability info
                /* SSID */
                /*36*/  0x00
                };

// beacon frame definition
uint8_t beaconPacket[109] = {
  /*  0 - 3  */ 0x80, 0x00, 0x00, 0x00, // Type/Subtype: managment beacon frame
  /*  4 - 9  */ 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, // Destination: broadcast
  /* 10 - 15 */ 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, // Source
  /* 16 - 21 */ 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, // Source

  // Fixed parameters
  /* 22 - 23 */ 0x00, 0x00, // Fragment & sequence number (will be done by the SDK)
  /* 24 - 31 */ 0x83, 0x51, 0xf7, 0x8f, 0x0f, 0x00, 0x00, 0x00, // Timestamp
  /* 32 - 33 */ 0xe8, 0x03, // Interval: 0x64, 0x00 => every 100ms - 0xe8, 0x03 => every 1s
  /* 34 - 35 */ 0x31, 0x00, // capabilities Tnformation

  // Tagged parameters

  // SSID parameters
  /* 36 - 37 */ 0x00, 0x20, // Tag: Set SSID length, Tag length: 32
  /* 38 - 69 */ 0x20, 0x20, 0x20, 0x20,
  0x20, 0x20, 0x20, 0x20,
  0x20, 0x20, 0x20, 0x20,
  0x20, 0x20, 0x20, 0x20,
  0x20, 0x20, 0x20, 0x20,
  0x20, 0x20, 0x20, 0x20,
  0x20, 0x20, 0x20, 0x20,
  0x20, 0x20, 0x20, 0x20, // SSID

  // Supported Rates
  /* 70 - 71 */ 0x01, 0x08, // Tag: Supported Rates, Tag length: 8
  /* 72 */ 0x82, // 1(B)
  /* 73 */ 0x84, // 2(B)
  /* 74 */ 0x8b, // 5.5(B)
  /* 75 */ 0x96, // 11(B)
  /* 76 */ 0x24, // 18
  /* 77 */ 0x30, // 24
  /* 78 */ 0x48, // 36
  /* 79 */ 0x6c, // 54

  // Current Channel
  /* 80 - 81 */ 0x03, 0x01, // Channel set, length
  /* 82 */      0x01,       // Current Channel

  // RSN information
  /*  83 -  84 */ 0x30, 0x18,
  /*  85 -  86 */ 0x01, 0x00,
  /*  87 -  90 */ 0x00, 0x0f, 0xac, 0x02,
  /*  91 -  92 */ 0x02, 0x00,
  /*  93 - 100 */ 0x00, 0x0f, 0xac, 0x04, 0x00, 0x0f, 0xac, 0x04, /*Fix: changed 0x02(TKIP) to 0x04(CCMP) is default. WPA2 with TKIP not supported by many devices*/
  /* 101 - 102 */ 0x01, 0x00,
  /* 103 - 106 */ 0x00, 0x0f, 0xac, 0x02,
  /* 107 - 108 */ 0x00, 0x00
};

// goes to next channel
void nextChannel() {
  if (sizeof(channels) > 1) {
    uint8_t ch = channels[channelIndex];
    channelIndex++;
    if (channelIndex > sizeof(channels)) channelIndex = 0;

    if (ch != wifi_channel && ch >= 1 && ch <= 14) {
      wifi_channel = ch;
      //wifi_set_channel(wifi_channel);
      esp_wifi_set_channel(wifi_channel, WIFI_SECOND_CHAN_NONE);
    }
  }
}

void beaconSpamList(const char list[]){
  // temp variables
  int i = 0;
  int j = 0;
  int ssidNum = 1;
  char tmp;
  int ssidsLen = strlen_P(list);
  bool sent = false;

  // go to next channel
  nextChannel();

  while (i < ssidsLen) {
    // read out next SSID
    j = 0;
    do {
      tmp = pgm_read_byte(list + i + j);
      j++;
    } while (tmp != '\n' && j <= 32 && i + j < ssidsLen);

    uint8_t ssidLen = j - 1;

    // set MAC address
    macAddr[5] = ssidNum;
    ssidNum++;

    // write MAC address into beacon frame
    memcpy(&beaconPacket[10], macAddr, 6);
    memcpy(&beaconPacket[16], macAddr, 6);

    // reset SSID
    memcpy(&beaconPacket[38], emptySSID, 32);

    // write new SSID into beacon frame
    memcpy_P(&beaconPacket[38], &list[i], ssidLen);

    // set channel for beacon frame
    beaconPacket[82] = wifi_channel;

    // send packet
    for (int k = 0; k < 3; k++) {
      packetCounter += esp_wifi_80211_tx(WIFI_IF_STA, beaconPacket, packetSize, 0) == 0;
      delay(1);
    }
    i += j;
  }
}
