#ifndef Globals_h
#define Globals_h

#define FIRMWARE_VERSION    "v1.00.04"

#define DEBUG               true

#define WIFI_SSID           "SSID"
#define WIFI_PASSWORD       "PASSWORD"

//Suported locales: French, German, Spanish.

//#include "def/DEFAULT.h"

//#include "def/Altherma(EBLA-EDLA D series 4-8kW Monobloc).h"
//#include "def/Altherma(EBLA-EDLA D series 9-16kW Monobloc).h"
//#include "def/Altherma(EGSAH-X-EWSAH-X-D series 6-10kW GEO3).h"
//#include "def/Altherma(EGSQH-A series 10kW GEO2).h"
//#include "def/Altherma(EPGA D EAB-EAV-EAVZ D(J) series 11-16kW).h"
//#include "def/Altherma(EPRA D ETSH-X 16P30-50 D series 14-16kW-ECH2O).h"
//#include "def/Altherma(EPRA D ETV16-ETB16-ETVZ16 D series 14-16kW).h"
//#include "def/Altherma(EPRA D_D7 ETSH-X 16P30-50 E_E7 series 14-18kW-ECH2O).h"
//#include "def/Altherma(EPRA D_D7 ETV16-ETB16-ETVZ16 E_E7 series 14-18kW).h"
//#include "def/Altherma(EPRA E ETSH-X 16P30-50 E series 8-12kW-ECH2O).h"
//#include "def/Altherma(EPRA E ETV16-ETB16-ETVZ16 E_EJ series 8-12kW).h"
#include "def/Altherma(ERGA D EHSH-X P30-50 D series 04-08kW-ECH2O).h"
//#include "def/Altherma(ERGA D EHV-EHB-EHVZ DA series 04-08kW).h"
//#include "def/Altherma(ERGA D EHV-EHB-EHVZ DJ series 04-08 kW).h"
//#include "def/Altherma(ERGA E EHSH-X P30-50 E_EF series 04-08kW-ECH2O).h"
//#include "def/Altherma(ERGA E EHV-EHB-EHVZ E_EJ series 04-08kW).h"
//#include "def/Altherma(ERLA D EBSH-X 16P30-50 D SERIES 11-16kW-ECH2O).h"
//#include "def/Altherma(ERLA D EBV-EBB-EBVZ D SERIES 11-16kW).h"
//#include "def/Altherma(ERLA03 D EHFH-EHFZ DJ series 3kW).h"
//#include "def/Altherma(Hybrid).h"
//#include "def/Altherma(LT_CA_CB_04-08kW).h"
//#include "def/Altherma(LT_CA_CB_11-16kW).h"
//#include "def/Altherma(LT_CB_04-08kW Bizone).h"
//#include "def/Altherma(LT_CB_11-16kW Bizone).h"
//#include "def/Altherma(LT_EBLQ-EBLQ-CA series 5-7kW Monobloc).h"
//#include "def/Altherma(LT_EBLQ-EDLQ-CA series 11-16kW Monobloc).h"
//#include "def/Altherma(LT_Multi_DHWHP).h"
//#include "def/Altherma(LT_Multi_Hybrid).h"
//#include "def/Daikin Mini chiller(EWAA-EWYA D series 4-8kW).h"
//#include "def/Daikin Mini chiller(EWAA-EWYA D series 9-16kW).h"
//#include "def/Daikin Mini chiller(EWAQ-EWYQ B series 4-8kW).h"
//#include "def/DEFAULT.h"
//#include "def/EKHWET-BAV3(Multi DHW tank).h"

//#include "def/PROTOCOL_S_ROTEX.h"
//#include "def/PROTOCOL_S.h"

#define PROTOCOL 'I'

unsigned long lastRead = 0;
const unsigned long interval = 5000;  

#endif /*Globals_h*/