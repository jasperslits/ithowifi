
#include "IthoSystem.h"
#include "hardware.h"
#include "i2c_esp32.h"
#include "notifyClients.h"

uint8_t ithoDeviceID = 0;
uint8_t itho_fwversion = 0;
volatile uint16_t ithoCurrentVal = 0;
uint8_t id0 = 0;
uint8_t id1 = 0;
uint8_t id2 = 0;
int8_t swap = 0;
struct ihtoDeviceType* ithoSettingsptr = getSettingsPtr(ithoDeviceID, itho_fwversion);
int ithoSettingsLength = getSettingsLength(ithoDeviceID, itho_fwversion);

const uint8_t itho_14v134[]   {0, 1, 2, 3, 4, 5, 6, 7, 0, 0, 0, 0, 0, 0, 0, 102, 103, 255};
const uint8_t itho_14v2[]     {0, 1, 2, 3, 4, 5, 6, 7, 102, 103, 255};
const uint8_t itho_14v5[]     {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 26, 48, 49, 50, 51, 96, 52, 53, 97, 98, 99, 100, 101, 102, 103, 255};
const uint8_t itho_14v6[]     {0, 1, 2, 3, 4, 5, 6, 7, 26, 48, 49, 50, 51, 96, 52, 53, 97, 98, 99, 100, 101, 102, 103, 255};


const uint8_t itho_1Bv6[]     {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 26, 48, 49, 50, 51, 96, 52, 53, 97, 98, 99, 100, 101, 102, 103, 255};
const uint8_t itho_1Bv7[]     {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 26, 48, 49, 50, 51, 96, 52, 53, 97, 98, 99, 55, 56, 57, 58, 59, 61, 62, 63, 64, 65, 66, 255};
const uint8_t itho_1Bv8[]     {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 104, 17, 18, 19, 26, 48, 49, 50, 51, 96, 52, 53, 97, 98, 99, 55, 56, 57, 58, 59, 61, 62, 63, 64, 65, 66, 255};
const uint8_t itho_1Bv9[]     {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 104, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 48, 49, 50, 51, 96, 52, 53, 97, 98, 99, 55, 56, 57, 58, 59, 61, 62, 63, 64, 65, 66, 255};
const uint8_t itho_1Bv10[]    {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 104, 17, 18, 19, 20, 21, 22, 23, 24, 25, 48, 49, 50, 51, 96, 52, 53, 97, 98, 99, 55, 56, 57, 58, 59, 61, 62, 63, 64, 105, 65, 66, 255};
const uint8_t itho_1Bv11[]    {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 104, 17, 18, 19, 20, 21, 22, 23, 24, 25, 105, 48, 49, 50, 51, 96, 52, 53, 97, 98, 99, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 65, 66, 255};
const uint8_t itho_1Bv17[]    {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 106, 30, 31, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 65, 66, 255};
const uint8_t itho_1Bv1820[]  {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 65, 66, 255};
const uint8_t itho_1Bv21[]    {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 65, 66, 255};
const uint8_t itho_1Bv22[]    {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 80, 81, 82, 83, 84, 85, 86, 87, 88, 89, 90, 91, 92, 93, 94, 95, 65, 66, 255};
const uint8_t itho_1Bv24[]    {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78, 79, 255};
const uint8_t itho_1Bv25[]    {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78, 79, 255};
const uint8_t itho_1Bv2627[]  {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78, 79, 255};


const uint8_t *ihto14Versions[] = { itho_14v134, itho_14v2, itho_14v134, itho_14v134, itho_14v5, itho_14v6 };
const uint8_t *ihto1BVersions[] = { nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, itho_1Bv6, itho_1Bv7, itho_1Bv8, itho_1Bv9, itho_1Bv10, itho_1Bv11, nullptr, nullptr, nullptr, nullptr, nullptr, itho_1Bv17, itho_1Bv1820, nullptr, itho_1Bv1820, itho_1Bv21, itho_1Bv22, nullptr, itho_1Bv24, itho_1Bv25, itho_1Bv2627, itho_1Bv2627 };


struct ihtoDeviceType {
  uint8_t ID;
  const char *name;
  const uint8_t **versions;
  uint8_t versionsLen;

};


struct ihtoDeviceType ithoDevices[] {
  { 0x01, "Air curtain",        nullptr, 0 },
  { 0x03, "HRU ECO-fan",        nullptr, 0 },
  { 0x08, "LoadBoiler",         nullptr, 0 },
  { 0x0A, "GGBB",               nullptr, 0 },
  { 0x0B, "DemandFlow",         nullptr, 0 },
  { 0x0C, "CO2 relay",          nullptr, 0 },
  { 0x0D, "Heatpump",           nullptr, 0 },
  { 0x0E, "OLB Single",         nullptr, 0 },
  { 0x0F, "AutoTemp",           nullptr, 0 },
  { 0x10, "OLB Double",         nullptr, 0 },
  { 0x11, "RF+",                nullptr, 0 },
  { 0x14, "CVE",                ihto14Versions, sizeof(ihto14Versions) / sizeof(ihto14Versions[0]) },
  { 0x15, "Extended",           nullptr, 0 },
  { 0x16, "Extended Plus",      nullptr, 0 },
  { 0x1A, "AreaFlow",           nullptr, 0 },
  { 0x1B, "CVE-Silent",         ihto1BVersions, sizeof(ihto1BVersions) / sizeof(ihto1BVersions[0]) },
  { 0x1C, "CVE-SilentExt",      nullptr, 0 },
  { 0x1D, "CVE-SilentExtPlus",  nullptr, 0 },
  { 0x20, "RF_CO2",             nullptr, 0 },
  { 0x2B, "HRU 350",            nullptr, 0 }
};


//struct ihtoDeviceType* ithoDevicesptr = ithoDevices;
//struct ihtoDeviceType* ithoDevicesendPtr = ithoDevices + sizeof(ithoDevices)/sizeof(ithoDevices[0]);
//while ( ithoDevicesptr < ithoDevicesendPtr ){
//   // do your thing with the ptr
//   ithoDevicesptr++;
//}

const __FlashStringHelper *ihtoCVESettingsLabels[] =
{
  F("oemNumber"),
  F("Unit version"),
  F("minAdjustMinimumSpeed"),
  F("maxAdjustMinimumSpeed"),
  F("minAdjustMaximumSpeed"),
  F("maxAdjustMaximumSpeed"),
  F("RFEnabled"),
  F("I2cIdle"),
  F("Constant_Ca2"),
  F("Constant_Ca1"),
  F("Constant_Ca0"),
  F("Constant_Cb2"),
  F("Constant_Cb1"),
  F("Constant_Cb0"),
  F("Constant_Cc2"),
  F("Constant_Cc1"),
  F("Constant_Cc0"),
  F("Away wait time"),
  F("Max delayed wait time"),
  F("Delayed away level"),
  F("PIR_Level1"),
  F("PIR_Level2"),
  F("PIR_Level1_waittime"),
  F("PIR_Level2_waittime"),
  F("PIR_RunOn_Level1"),
  F("PIR_RunOn_Level2"),
  F("CO2MinVentilation"),
  F("maxTimeHighSpeed"),
  F("maxTimeOtherSpeeds"),
  F("fallbackPreviousSpeed"),
  F("poorCo2Quality"),
  F("goodCo2Quality"),
  F("Amount of floors"),
  F("Inhabitants"),
  F("nightOnePersonOneFloorOptima1"),
  F("nightTwoPersonsOneFloorOptima1"),
  F("nightMultiPersonsOneFloorOptima1"),
  F("nightOnePersonMultiFloorOptima1"),
  F("nightTwoPersonsMultiFloorOptima1"),
  F("nightMultiPersonsMultiFloorOptima1"),
  F("nightOnePersonOneFloorOptima2"),
  F("nightTwoPersonsOneFloorOptima2"),
  F("nightMultiPersonsOneFloorOptima2"),
  F("nightOnePersonMultiFloorOptima2"),
  F("nightTwoPersonsMultiFloorOptima2"),
  F("nightMultiPersonsMultiFloorOptima2"),
  F("autoVentilationOneFloor"),
  F("autoVentilationMultiFloor"),
  F("CO2personsAbsent"),
  F("CO2personsPresent"),
  F("MinFanSetpValveLowCO2"),
  F("MinFanSetpValveHighCO2"),
  F("CO2Fan100ValveLow"),
  F("CO2Fan100ValveHigh"),
  F("CO2MaxFanSlope"),
  F("CO2PeriodTime"),
  F("CO2RiseLevel"),
  F("CO2DropLevel"),
  F("DeltaNormalAdjustment"),
  F("BlockingTimeFan"),
  F("BlockingTimeLimiter"),
  F("MinFanSpeedStopBlocking"),
  F("MaxFanSlopeBlocking"),
  F("CO2RiseStartBlocking"),
  F("MaxFanSpeedBlocking"),
  F("ManualControl"),
  F("ManualFanSpeed"),
  F("dewPointStartHeater"),
  F("dewPointStopHeater"),
  F("minHeaterOnTime"),
  F("minHeaterOffTime"),
  F("RHsensorDetected"),
  F("RHventHighDemand"),
  F("RHminOnTime"),
  F("OverruleInternCtrl"),
  F("SampleTimeInternCtrl"),
  F("RVOffInternCtrl"),
  F("XOnInternCtrl"),
  F("XOffInternCtrl"),
  F("MaxTimeOnInternCtrl"),
  F("UseWiredRh"),
  F("RhSensorInterval"),
  F("StoreInterval"),
  F("RHventHighDemand"),
  F("RHmin"),
  F("RHmax"),
  F("RHstart"),
  F("RHstop"),
  F("RHIncrease"),
  F("RHminOnTime"),
  F("RHdiff"),
  F("RHconstantDiff"),
  F("RHconstantTime"),
  F("RHlow"),
  F("RHselectOperationMode"),
  F("RHsensitivityIncrease"),
  F("CO2MinFanSetpValveHigh_PIR"),
  F("CO2fan100valveHighPIR"),
  F("CO2MaxFanSetpChMin"),
  F("PersonPresentTime"),
  F("Sample cyclustijd interval"),
  F("Stabilize periode"),
  F("ManualControl"),
  F("ManualFanSpeed"),
  F("Away speed"),
  F("MinVentilationCO2Baseflow"),
  F("heatrae")
};

const __FlashStringHelper *ihtoCVESettingsDescriptions[] =
{
  F("OEM number"),
  F("Print version (1 is high performance)"),
  F("Min setting of potentiometer low"),
  F("Max setting of potentiometer low"),
  F("Min setting of potentiometer high"),
  F("Max setting of potentiometer high"),
  F("RF enable"),
  F("I2C mode"),
  F("Fan constant Ca2"),
  F("Fan constant Ca1"),
  F("Fan constant Ca0"),
  F("Fan constant Cb2"),
  F("Fan constant Cb1"),
  F("Fan constant Cb0"),
  F("Fan constant cc2"),
  F("Fan constant cc1"),
  F("Fan constant Cc0"),
  F("Block time auto. reset absence"),
  F("Max. time deferred absence"),
  F("Min. time deferred absence"),
  F("PIR level 1"),
  F("PIR level 2"),
  F("PIR level 1 waiting time"),
  F("PIR level 2 waiting time"),
  F("PIR level 1 running time"),
  F("PIR level 2 running time"),
  F("Minimum ventilation level in Auto without CO2 sensor"),
  F("Max time in high"),
  F("Max time in low or middle"),
  F("Mode after maximum time high"),
  F("CO2 value air quality moderate, Display in spIDer"),
  F("CO2 value air quality good, Display in spIDer"),
  F("Type of dwelling, 1 = apartment; 2 = family home"),
  F("Number of occupants, 3 and more occupants = 3"),
  F("Minimum ventilation speed during Auto Night mode"),
  F("Minimum ventilation speed during Auto Night mode"),
  F("Minimum ventilation speed during Auto Night mode"),
  F("Minimum ventilation speed during Auto Night mode"),
  F("Minimum ventilation speed during Auto Night mode"),
  F("Minimum ventilation speed during Auto Night mode"),
  F("Minimum ventilation speed during Auto Night mode"),
  F("Minimum ventilation speed during Auto Night mode"),
  F("Minimum ventilation speed during Auto Night mode"),
  F("Minimum ventilation speed during Auto Night mode"),
  F("Minimum ventilation speed during Auto Night mode"),
  F("Minimum ventilation speed during Auto Night mode"),
  F("Minimum ventilation speed during Auto mode"),
  F("Minimum ventilation speed during Auto mode"),
  F("CO2 value absent"),
  F("CO2 value present"),
  F("Min valve speed low AreaFlow"),
  F("Min. valve speed high AreaFlow"),
  F("CO2 value vent. 100% valve low"),
  F("CO2 value Vent. 100% valve high"),
  F("Max speed change CO2 control"),
  F("CO2 period time"),
  F("CO2 change rising"),
  F("CO2 change decreasing CO2"),
  F("CO2 change constant CO2"),
  F("Blocking time fan speed"),
  F("Blocking time maximum duration"),
  F("Stop blocking time at low demand"),
  F("Max. speed change CO2 reg. Blocking time"),
  F("CO2 increase start of blocking time"),
  F("Max speed up during disabling time"),
  F("Manual operation"),
  F("Manual operation speed"),
  F("Dew point at which RV-IC heater should start"),
  F("Dew point at which the RV-IC heater should stop"),
  F("Minimum time the RV-IC heater is on"),
  F("Maximum time to run the RV-IC heater"),
  F("RH sensor found/available"),
  F("Fan level to which it will go when the RH control switches on"),
  F("Minimum time for fan to go high in RH control"),
  F("Remote RH sensor overrides internal or not"),
  F("Sampling time of the RH and temperature measurement"),
  F("Hysteresis RH before going back to normal operation"),
  F("Rise in absolute humidity (X mg/kg) at which it goes high"),
  F("Hysteresis X before returning to normal operation"),
  F("Maximum time to remain in high by RH control"),
  F("0 = ventilation request wired sensor is ignored, 1 = ventilation request wired sensor is honored"),
  F("Measurement interval wired RH sensor"),
  F("Measurement storage interval to determine delta’s"),
  F("Maximum ventilation demand"),
  F("Minimum level to start wet room control"),
  F("Maximum level wet room control"),
  F("Always start ventilation above this value"),
  F("Stop ventilation below this value"),
  F("Detection level fast increase of humidity"),
  F("Minimum time demand is send"),
  F("Absolute RH percentage"),
  F("RH assumed constant when variation is less than this value"),
  F("Time to detect constant RH"),
  F("Stop ventilation below this value"),
  F("Configuration setting 1: 0 = no control performed, 1 = bathroom, 2 = wet room."),
  F("User setting 1: 1 = interval => 1 * number, 2 = 2 * etc."),
  F("Min. Fan setpoint at valve high and PIR"),
  F("CO2 concentration at 100% valve high and PIR"),
  F("Max speed change per minute"),
  F("Expiration time PIR present"),
  F("Period time"),
  F("Stabilization period"),
  F("Manual control"),
  F("Speed with manual operation"),
  F("Speed when absence"),
  F("Min. Fan setpoint BaseFlow available"),
  F("heatrae")
};

char* getIthoType(uint8_t deviceID) {
  static char ithoDeviceType[32] = "Unkown device type";

  struct ihtoDeviceType* ithoDevicesptr = ithoDevices;
  struct ihtoDeviceType* ithoDevicesendPtr = ithoDevices + sizeof(ithoDevices) / sizeof(ithoDevices[0]);
  while ( ithoDevicesptr < ithoDevicesendPtr ) {
    if (ithoDevicesptr->ID == deviceID) {
      strcpy(ithoDeviceType, ithoDevicesptr->name);
    }
    ithoDevicesptr++;
  }
  return ithoDeviceType;
}

int getSettingsLength(uint8_t deviceID, uint8_t version) {

  struct ihtoDeviceType* ithoDevicesptr = ithoDevices;
  struct ihtoDeviceType* ithoDevicesendPtr = ithoDevices + sizeof(ithoDevices) / sizeof(ithoDevices[0]);
  while ( ithoDevicesptr < ithoDevicesendPtr ) {
    if (ithoDevicesptr->ID == deviceID) {
      if (ithoDevicesptr->versions == nullptr) {
        return -2; //Settings not available for this device
      }
      if (*(ithoDevicesptr->versions + version) == nullptr) {
        return -3; //Settings not available for this version
      }

      for (int i = 0; i < 255; i++) {
        if ((int) * (*(ithoDevicesptr->versions + version) + i) == 255) {
          //end of array
          return i;
        }
      }
    }
    ithoDevicesptr++;
  }
  return -1;
}

void getSetting(uint8_t i, bool updateState, bool loop) {
  getSetting(i, updateState, loop, ithoSettingsptr, itho_fwversion);
}

void getSetting(uint8_t i, bool updateState, bool loop, struct ihtoDeviceType* getSettingsPtr, uint8_t version) {
  if (getSettingsPtr == nullptr) {
    return;
  }

  char fStringBuf[128];
  strcpy_P(fStringBuf, (PGM_P)ihtoCVESettingsLabels[(int) * (*(getSettingsPtr->versions + version) + i)]);
  
  char fDescBuf[128];
  strcpy_P(fDescBuf, (PGM_P)ihtoCVESettingsDescriptions[(int) * (*(getSettingsPtr->versions + version) + i)]);
    
  StaticJsonDocument<512> doc;
  JsonObject root = doc.to<JsonObject>();


  root["Index"] = i;
  //root["Setting"] = fStringBuf;
  root["Description"] = fDescBuf;

  if (updateState) {
    root["update"] = true;
    root["loop"] = loop;
    int32_t *valuePtr = sendQuery2410(i, false);
    root["Current"] = *(valuePtr + 0);
    root["Minimum"] = *(valuePtr + 1);
    root["Maximum"] = *(valuePtr + 2);
  }
  else {
    root["update"] = false;
    root["loop"] = loop;
    root["Current"] = nullptr;
    root["Minimum"] = nullptr;
    root["Maximum"] = nullptr;
  }


  jsonLogMessage(root, ITHOSETTINGS);
}

void updateSetting(uint8_t i, int32_t value) {
  setSetting2410(i, value);
}

struct ihtoDeviceType* getSettingsPtr(uint8_t deviceID, uint8_t version) {
  int settingsLen = getSettingsLength(deviceID, version);
  if (settingsLen < 0) {
    return nullptr;
  }
  struct ihtoDeviceType* ithoDevicesptr = ithoDevices;
  struct ihtoDeviceType* ithoDevicesendPtr = ithoDevices + sizeof(ithoDevices) / sizeof(ithoDevices[0]);
  while ( ithoDevicesptr < ithoDevicesendPtr ) {
    if (ithoDevicesptr->ID == deviceID) {
      return ithoDevicesptr;
    }
    ithoDevicesptr++;
  }
  return nullptr;
}



uint8_t checksum(const uint8_t* buf, size_t buflen) {
  uint8_t sum = 0;
  while (buflen--) {
    sum += *buf++;
  }
  return -sum;
}



uint8_t cmdCounter = 0;

void sendButton(uint8_t number) {

  uint8_t command[] = { 0x82, 0x60, 0xC1, 0x01, 0x01, 0x11, 0x00, 0x00, 0x00, 0x00, 0x16, 0xFF, 0xFF, 0xFF, 0xFF, 0x22, 0xF1, 0x03, 0x00, 0x01, 0x04, 0x00, 0x00, 0xFF };

  command[11] = id0;
  command[12] = id1;
  command[13] = id2;

  command[14] = cmdCounter;
  cmdCounter++;

  if (swap) {
    if (number == 1) {
      number = 3;
    }
    else if (number == 3) {
      number = 1;
    }
  }

  command[19] += number;

  command[sizeof(command) - 1] = checksum(command, sizeof(command) - 1);

  while (digitalRead(SCLPIN) == LOW ) {
    yield();
    delay(1);
  }

#if defined (__HW_VERSION_ONE__)
  Wire.beginTransmission(byte(0x41));
  for (uint8_t i = 1; i < sizeof(command); i++) {
    Wire.write(command[i]);
  }
  Wire.endTransmission(true);
#elif defined (__HW_VERSION_TWO__)
  i2c_sendBytes(command, sizeof(command));
#endif

}

void sendJoinI2C() {

  uint8_t command[] = { 0x82, 0x60, 0xC1, 0x01, 0x01, 0x1A, 0x00, 0x00, 0x00, 0x00, 0x16, 0xFF, 0xFF, 0xFF, 0xFF, 0x1F, 0xC9, 0x0C, 0x00, 0x22, 0xF1, 0xFF, 0xFF, 0xFF, 0x01, 0x10, 0xE0, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0xFF };

  command[11] = id0;
  command[12] = id1;
  command[13] = id2;

  command[14] = cmdCounter;
  cmdCounter++;

  command[21] = id0;
  command[22] = id1;
  command[23] = id2;

  command[27] = id0;
  command[28] = id1;
  command[29] = id2;

  command[sizeof(command) - 1] = checksum(command, sizeof(command) - 1);

  while (digitalRead(SCLPIN) == LOW ) {
    yield();
    delay(1);
  }

#if defined (__HW_VERSION_ONE__)
  Wire.beginTransmission(byte(0x41));
  for (uint8_t i = 1; i < sizeof(command); i++) {
    Wire.write(command[i]);
  }
  Wire.endTransmission(true);
#elif defined (__HW_VERSION_TWO__)
  i2c_sendBytes(command, sizeof(command));
#endif


}

void sendLeaveI2C() {

  uint8_t command[] = { 0x82, 0x60, 0xC1, 0x01, 0x01, 0x14, 0x00, 0x00, 0x00, 0x00, 0x16, 0xFF, 0xFF, 0xFF, 0xFF, 0x1F, 0xC9, 0x06, 0x00, 0x1F, 0xC9, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0xFF };


  command[11] = id0;
  command[12] = id1;
  command[13] = id2;

  command[14] = cmdCounter;
  cmdCounter++;

  command[21] = id0;
  command[22] = id1;
  command[23] = id2;

  command[sizeof(command) - 1] = checksum(command, sizeof(command) - 1);

  while (digitalRead(SCLPIN) == LOW ) {
    yield();
    delay(1);
  }

#if defined (__HW_VERSION_ONE__)
  Wire.beginTransmission(byte(0x41));
  for (uint8_t i = 1; i < sizeof(command); i++) {
    Wire.write(command[i]);
  }
  Wire.endTransmission(true);
#elif defined (__HW_VERSION_TWO__)
  i2c_sendBytes(command, sizeof(command));
#endif

}

void sendQueryDevicetype() {

  uint8_t command[] = { 0x82, 0x80, 0x90, 0xE0, 0x04, 0x00, 0x8A };

  Serial.println(1);
  
  while (digitalRead(SCLPIN) == LOW ) {
    Serial.println(2);
    yield();
    delay(1);
  }

#if defined (__HW_VERSION_ONE__)
  Wire.beginTransmission(byte(0x41));
  for (uint8_t i = 1; i < sizeof(command); i++) {
    Wire.write(command[i]);
  }
  Wire.endTransmission(true);

  unsigned long timeoutmillis = millis() + 1000;

  while (!callback_called && millis() < timeoutmillis) {
    if (callback_called) {
      callback_called = false;
      jsonSysmessage("ithotype", i2c_slave_buf);
      ithoDeviceID = i2c_slave_data[9];
      itho_fwversion = i2c_slave_data[11];
      ithoSettingsptr = getSettingsPtr(ithoDeviceID, itho_fwversion);
      ithoSettingsLength = getSettingsLength(ithoDeviceID, itho_fwversion);
    }
    else {
      jsonSysmessage("ithotype", "failed");
    }
  }

#endif
#if defined (__HW_VERSION_TWO__)
  i2c_sendBytes(command, sizeof(command));

  i2c_slaveInit();

  if (callback_called) {
    callback_called = false;
    jsonSysmessage("ithotype", i2c_slave_buf);
    ithoDeviceID = i2c_slave_data[9];
    itho_fwversion = i2c_slave_data[11];
    ithoSettingsptr = getSettingsPtr(ithoDeviceID, itho_fwversion);
    ithoSettingsLength = getSettingsLength(ithoDeviceID, itho_fwversion);
  }
  else {
    jsonSysmessage("ithotype", "failed");
  }
  i2c_slave_deinit();

#endif

}

void sendQueryStatusFormat() {

  uint8_t command[] = { 0x82, 0x80, 0xA4, 0x00, 0x04, 0x00, 0x56 };

  while (digitalRead(SCLPIN) == LOW ) {
    yield();
    delay(1);
  }


#if defined (__HW_VERSION_ONE__)
  Wire.beginTransmission(byte(0x41));
  for (uint8_t i = 1; i < sizeof(command); i++) {
    Wire.write(command[i]);
  }
  Wire.endTransmission(true);

  unsigned long timeoutmillis = millis() + 1000;

  while (!callback_called && millis() < timeoutmillis) {
    if (callback_called) {
      callback_called = false;
      jsonSysmessage("ithostatusformat", i2c_slave_buf);

    }
    else {
      jsonSysmessage("ithostatusformat", "failed");
    }
  }
#endif

#if defined (__HW_VERSION_TWO__)
  i2c_sendBytes(command, sizeof(command));

  i2c_slaveInit();

  if (callback_called) {
    callback_called = false;
    jsonSysmessage("ithostatusformat", i2c_slave_buf);

  }
  else {
    jsonSysmessage("ithostatusformat", "failed");
  }
  i2c_slave_deinit();

#endif


}

void sendQueryStatus() {

  uint8_t command[] = { 0x82, 0x80, 0xA4, 0x01, 0x04, 0x00, 0x55 };

  while (digitalRead(SCLPIN) == LOW ) {
    yield();
    delay(1);
  }


#if defined (__HW_VERSION_ONE__)
  Wire.beginTransmission(byte(0x41));
  for (uint8_t i = 1; i < sizeof(command); i++) {
    Wire.write(command[i]);
  }
  Wire.endTransmission(true);

  unsigned long timeoutmillis = millis() + 1000;

  while (!callback_called && millis() < timeoutmillis) {
    if (callback_called) {
      callback_called = false;
      jsonSysmessage("ithostatus", i2c_slave_buf);

    }
    else {
      jsonSysmessage("ithostatus", "failed");
    }
  }

#endif

#if defined (__HW_VERSION_TWO__)
  i2c_sendBytes(command, sizeof(command));

  i2c_slaveInit();

  if (callback_called) {
    callback_called = false;
    jsonSysmessage("ithostatus", i2c_slave_buf);

  }
  else {
    jsonSysmessage("ithostatus", "failed");
  }
  i2c_slave_deinit();

#endif


}

void sendQuery2400() {

  uint8_t command[] = { 0x82, 0x80, 0x24, 0x00, 0x04, 0x00, 0xD6 };

  while (digitalRead(SCLPIN) == LOW ) {
    yield();
    delay(1);
  }

#if defined (__HW_VERSION_ONE__)
  Wire.beginTransmission(byte(0x41));
  for (uint8_t i = 1; i < sizeof(command); i++) {
    Wire.write(command[i]);
  }
  Wire.endTransmission(true);

  unsigned long timeoutmillis = millis() + 1000;

  while (!callback_called && millis() < timeoutmillis) {
    if (callback_called) {
      callback_called = false;
      jsonSysmessage("itho2400", i2c_slave_buf);

    }
    else {
      jsonSysmessage("itho2400", "failed");
    }
  }

#endif
#if defined (__HW_VERSION_TWO__)
  i2c_sendBytes(command, sizeof(command));

  i2c_slaveInit();

  if (callback_called) {
    callback_called = false;
    jsonSysmessage("itho2400", i2c_slave_buf);

  }
  else {
    jsonSysmessage("itho2400", "failed");
  }
  i2c_slave_deinit();

#endif

}

void sendQuery2401() {

  uint8_t command[] = { 0x82, 0x80, 0x24, 0x01, 0x04, 0x00, 0xD5 };

  while (digitalRead(SCLPIN) == LOW ) {
    yield();
    delay(1);
  }

#if defined (__HW_VERSION_ONE__)
  Wire.beginTransmission(byte(0x41));
  for (uint8_t i = 1; i < sizeof(command); i++) {
    Wire.write(command[i]);
  }
  Wire.endTransmission(true);

  unsigned long timeoutmillis = millis() + 1000;

  while (!callback_called && millis() < timeoutmillis) {
    if (callback_called) {
      callback_called = false;
      jsonSysmessage("itho2401", i2c_slave_buf);

    }
    else {
      jsonSysmessage("itho2401", "failed");
    }
  }

#endif
#if defined (__HW_VERSION_TWO__)
  i2c_sendBytes(command, sizeof(command));

  i2c_slaveInit();

  if (callback_called) {
    callback_called = false;
    jsonSysmessage("itho2401", i2c_slave_buf);

  }
  else {
    jsonSysmessage("itho2401", "failed");
  }
  i2c_slave_deinit();

#endif

}

void sendQuery31DA() {

  uint8_t command[] = { 0x82, 0x80, 0x31, 0xDA, 0x04, 0x00, 0xEF };

  while (digitalRead(SCLPIN) == LOW ) {
    yield();
    delay(1);
  }

#if defined (__HW_VERSION_ONE__)
  Wire.beginTransmission(byte(0x41));
  for (uint8_t i = 1; i < sizeof(command); i++) {
    Wire.write(command[i]);
  }
  Wire.endTransmission(true);

  unsigned long timeoutmillis = millis() + 1000;

  while (!callback_called && millis() < timeoutmillis) {
    if (callback_called) {
      callback_called = false;
      jsonSysmessage("itho31DA", i2c_slave_buf);

    }
    else {
      jsonSysmessage("itho31DA", "failed");
    }
  }

#endif

#if defined (__HW_VERSION_TWO__)
  i2c_sendBytes(command, sizeof(command));

  i2c_slaveInit();

  if (callback_called) {
    callback_called = false;
    jsonSysmessage("itho31DA", i2c_slave_buf);

  }
  else {
    jsonSysmessage("itho31DA", "failed");
  }
  i2c_slave_deinit();

#endif

}

void sendQuery31D9() {

  uint8_t command[] = { 0x82, 0x80, 0x31, 0xD9, 0x04, 0x00, 0xF0 };

  while (digitalRead(SCLPIN) == LOW ) {
    yield();
    delay(1);
  }

#if defined (__HW_VERSION_ONE__)
  Wire.beginTransmission(byte(0x41));
  for (uint8_t i = 1; i < sizeof(command); i++) {
    Wire.write(command[i]);
  }
  Wire.endTransmission(true);

  unsigned long timeoutmillis = millis() + 1000;

  while (!callback_called && millis() < timeoutmillis) {
    if (callback_called) {
      callback_called = false;
      jsonSysmessage("itho31D9", i2c_slave_buf);

    }
    else {
      jsonSysmessage("itho31D9", "failed");
    }
  }

#endif

#if defined (__HW_VERSION_TWO__)
  i2c_sendBytes(command, sizeof(command));

  i2c_slaveInit();

  if (callback_called) {
    callback_called = false;
    jsonSysmessage("itho31D9", i2c_slave_buf);

  }
  else {
    jsonSysmessage("itho31D9", "failed");
  }
  i2c_slave_deinit();

#endif

}

int32_t * sendQuery2410(uint8_t settingID, bool webUpdate) {

  static int32_t values[3];
  values[0] = 0;
  values[1] = 0;
  values[2] = 0;

  uint8_t command[] = { 0x82, 0x80, 0x24, 0x10, 0x04, 0x13, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0x00, 0xFF };

  command[23] = settingID;
  command[sizeof(command) - 1] = checksum(command, sizeof(command) - 1);

  while (digitalRead(SCLPIN) == LOW ) {
    yield();
    delay(1);
  }

#if defined (__HW_VERSION_ONE__)
  Wire.beginTransmission(byte(0x41));
  for (uint8_t i = 1; i < sizeof(command); i++) {
    Wire.write(command[i]);
  }
  Wire.endTransmission(true);

  unsigned long timeoutmillis = millis() + 1000;

  while (!callback_called && millis() < timeoutmillis) {
    if (callback_called) {
      callback_called = false;
      if (webUpdate) {
        jsonSysmessage("itho2410", i2c_slave_buf);
      }
    }
    else {
      if (webUpdate) {
        jsonSysmessage("itho2410", "failed");
      }
    }
  }

#endif

#if defined (__HW_VERSION_TWO__)
  i2c_sendBytes(command, sizeof(command));

  i2c_slaveInit();

  if (callback_called) {
    callback_called = false;
    if (webUpdate) {
      jsonSysmessage("itho2410", i2c_slave_buf);
    }

    //current value
    values[0] = i2c_slave_data[6] << 24; //6,7,8,9
    values[0] |= i2c_slave_data[7] << 16;
    values[0] |= i2c_slave_data[8] << 8;
    values[0] |= i2c_slave_data[9];
    //min value
    values[1] = i2c_slave_data[10] << 24; //10,11,12,13
    values[1] |= i2c_slave_data[11] << 16;
    values[1] |= i2c_slave_data[12] << 8;
    values[1] |= i2c_slave_data[13];
    //max value
    values[2] = i2c_slave_data[14] << 24; //14,15,16,17
    values[2] |= i2c_slave_data[15] << 16;
    values[2] |= i2c_slave_data[16] << 8;
    values[2] |= i2c_slave_data[17];

    if (webUpdate) {
      char tempbuffer[256];

      sprintf(tempbuffer, "%ld", values[0]);
      jsonSysmessage("itho2410cur", tempbuffer);
      sprintf(tempbuffer, "%ld", values[1]);
      jsonSysmessage("itho2410min", tempbuffer);
      sprintf(tempbuffer, "%ld", values[2]);
      jsonSysmessage("itho2410max", tempbuffer);
    }


  }
  else {
    if (webUpdate) {
      jsonSysmessage("itho2410", "failed");
    }

  }
  i2c_slave_deinit();

#endif

  return values;

}

void setSetting2410(uint8_t settingID, int32_t value) {

  if (settingID == 7 && value == 1) {
    jsonLogMessage("<br>!!Warning!! Command ignored!<br>Setting index 7 to value 1 will switch off I2C!", WEBINTERFACE);
    return;
  }

  uint8_t command[] = { 0x82, 0x80, 0x24, 0x10, 0x06, 0x13, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0x00, 0xFF };

  command[23] = settingID;

  command[9] = value & 0xFF;
  if (value > 255) {
    command[8] = (value >> 8) & 0xFF;
  }
  if (value > 65535) {
    command[6] = (value >> 24) & 0xFF;
    command[7] = (value >> 16) & 0xFF;
  }

  command[sizeof(command) - 1] = checksum(command, sizeof(command) - 1);

  std::string s;
  s.reserve(sizeof(command) * 3 + 2);
  for (size_t i = 0; i < sizeof(command); ++i) {
    if (i)
      s += ' ';
    s += toHex(command[i] >> 4);
    s += toHex(command[i] & 0xF);
  }
  char tempbuffer[256];
  strlcpy(tempbuffer, s.c_str(), sizeof(tempbuffer));

  jsonSysmessage("itho2410set", tempbuffer);


  while (digitalRead(SCLPIN) == LOW ) {
    yield();
    delay(1);
  }

#if defined (__HW_VERSION_ONE__)
  Wire.beginTransmission(byte(0x41));
  for (uint8_t i = 1; i < sizeof(command); i++) {
    Wire.write(command[i]);
  }
  Wire.endTransmission(true);

  unsigned long timeoutmillis = millis() + 1000;

  while (!callback_called && millis() < timeoutmillis) {
    if (callback_called) {
      callback_called = false;
      jsonSysmessage("itho2410setres", i2c_slave_buf);

    }
    else {
      jsonSysmessage("itho2410setres", "failed");
    }
  }

#endif

#if defined (__HW_VERSION_TWO__)
  i2c_sendBytes(command, sizeof(command));

  i2c_slaveInit();

  if (callback_called) {
    callback_called = false;
    if (command[6] == i2c_slave_data[6] && command[7] == i2c_slave_data[7] && command[8] == i2c_slave_data[8] && command[9] == i2c_slave_data[9] && command[23] == i2c_slave_data[23]) {
      jsonSysmessage("itho2410setres", "confirmed");
    }
    else {
      jsonSysmessage("itho2410setres", "confirmation failed");
    }
  }
  else {
    jsonSysmessage("itho2410setres", "failed");
  }
  i2c_slave_deinit();

#endif

}
