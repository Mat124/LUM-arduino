#include <mcp2515.h>

#define LEDONE 5 //testing LED

MCP2515 mcp2515(10);
struct can_frame frame;

void setup() {
  mcp2515.reset();
  mcp2515.setBitrate(CAN_125KBPS);
  mcp2515.setNormalMode();
  pinMode(LEDONE, OUTPUT); //testing LED stuff
  digitalWrite(LEDONE, LOW);
}

void loop() {
  if (mcp2515.readMessage(&frame) == MCP2515::ERROR_OK) {
    // frame contains received message
    switch (frame.can_id) {
      //TESTING SECTION
      case 1: //frame is from address 1, used for testing
        //turn on/off LED
        if (frame.data[0] == 1) {
          digitalWrite(LEDONE, HIGH);
        }
        else if (frame.data[0] == 0) {
          digitalWrite(LEDONE, LOW);
        }
        break;

      //INVERTER SECTION
      case 0x0A0://Temperatures #1, gain=10
        int16_t mod_A_temp = (int16_t)(frame.data[0] << 8) | frame.data[1]; //convert bytes to signed int16, must do it this way or may become unsigned
        int16_t mod_B_temp = (int16_t)(frame.data[2] << 8) | frame.data[3]; 
        int16_t mod_C_temp = (int16_t)(frame.data[4] << 8) | frame.data[5]; 
        int16_t gate_driver_board_temp = (int16_t)(frame.data[6] << 8) | frame.data[7]; 
        break;
      case 0x0A1://Temperatures #2, gain=10
        int16_t control_board_temp = (int16_t)(frame.data[0] << 8) | frame.data[1]; //convert bytes to signed int16, must do it this way or may become unsigned
        int16_t RTD1_temp = (int16_t)(frame.data[2] << 8) | frame.data[3]; 
        int16_t RTD2_temp = (int16_t)(frame.data[4] << 8) | frame.data[5]; 
        int16_t RTD3_temp = (int16_t)(frame.data[6] << 8) | frame.data[7]; 
        break;
      case 0x0A2://Temperatures #3, gain=10
        int16_t coolant_temp = (int16_t)(frame.data[0] << 8) | frame.data[1]; //convert bytes to signed int16, must do it this way or may become unsigned
        int16_t hotspot_temp = (int16_t)(frame.data[2] << 8) | frame.data[3]; 
        int16_t motor_temp = (int16_t)(frame.data[4] << 8) | frame.data[5]; 
        int16_t torque_shudder = (int16_t)(frame.data[6] << 8) | frame.data[7]; 
        break;
      case 0x0A3://Analog Inputs Voltage, gain=100
        //probably want to double check these, supposedly these are 10-bit signed integers but i think it should be unsigned
        //if unsigned, change definition to uint16_t and remove if, leaving the line in the else
        int16_t analog_1;
        if (frame.data[0] >= 128) { //first bit is 1
          analog_1 = 0b1111110000000000 | (((uint16_t)(frame.data[0])) << 2) | (uint16_t)(frame.data[1] >> 6);
        }
        else {
          analog_1 = (((uint16_t)(frame.data[0])) << 2) | (uint16_t)(frame.data[1] >> 6);
        }
        int16_t analog_2;
        if ((frame.data[1] << 2) >= 128) { //first bit is 1
          analog_2 = 0b1111110000000000 | (((uint16_t)(frame.data[1])) << 4) | (uint16_t)(frame.data[2] >> 4);
        }
        else {
          analog_2 = (((uint16_t)(frame.data[0])) << 2) | (uint16_t)(frame.data[1] >> 6);
        }
        int16_t analog_3;
        if ((frame.data[2] << 4) >= 128) { //first bit is 1
          analog_3 = 0b1111110000000000 | (((uint16_t)(frame.data[2])) << 6) | (uint16_t)(frame.data[3] >> 2);
        }
        else {
          analog_3 = (((uint16_t)(frame.data[0])) << 2) | (uint16_t)(frame.data[1] >> 6);
        }
        int16_t analog_4;
        if (frame.data[0] >= 128) { //first bit is 1
          analog_4 = 0b1111110000000000 | (((uint16_t)(frame.data[4])) << 2) | (uint16_t)(frame.data[5] >> 6);
        }
        else {
          analog_4 = (((uint16_t)(frame.data[0])) << 2) | (uint16_t)(frame.data[1] >> 6);
        }
        int16_t analog_5;
        if ((frame.data[1] << 2) >= 128) { //first bit is 1
          analog_5 = 0b1111110000000000 | (((uint16_t)(frame.data[5])) << 4) | (uint16_t)(frame.data[6] >> 4);
        }
        else {
          analog_5 = (((uint16_t)(frame.data[0])) << 2) | (uint16_t)(frame.data[1] >> 6);
        }
        int16_t analog_6;
        if ((frame.data[2] << 4) >= 128) { //first bit is 1
          analog_6 = 0b1111110000000000 | (((uint16_t)(frame.data[6])) << 6) | (uint16_t)(frame.data[7] >> 2);
        }
        else {
          analog_6 = (((uint16_t)(frame.data[0])) << 2) | (uint16_t)(frame.data[1] >> 6);
        }
      case 0x0A4://Digital Inputs Voltage
      case 0x0A5://Motor Position Information
      case 0x0A6://Current Information
      case 0x0A7://Voltage Information
      case 0x0A8://Flux Informatiuon
      case 0x0A9://Internal Voltages
      case 0x0AA://Internal States
      case 0x0AB://Fault Codes
      case 0x0AC://Torque & Timer Information
      case 0x0AD://Modulation Index & Flux Weakening Output Information
      case 0x0AE://Firmware Information
      case 0x0AF://Diagnostic Data - might be unused now
      case 0x0B0://High Speed Message (transmitted at 3ms, version 2042+)

      //BMS SECTION
      case 0xF004://Relays Status
      case 0xF006://Max Cells Supported Count
      case 0xF007://Populated Cell Count
      case 0xF00A://Pack Charge Current Limit
      case 0xF00B://Pack Discharge Current Limit
      case 0xF00C://Signed Pack Current
      case 0xF015://Unsigned Pack Current
      case 0xF00D://Pack Voltage
      case 0xF00E://Pack Open Voltage
      case 0xF00F://Pack State of Charge
      case 0xF010://Pack Amphours
      case 0xF011://Pack Resistance
      case 0xF012://Pack Depth of Discharge
      case 0xF013://Pack Health
      case 0xF014://Pack Summed Voltage
      case 0xF018://Total Pack Cycles
      case 0xF028://Highest Pack Temperature
      case 0xF029://Lowest Pack Temperature
      case 0xF02A://Avg. Pack Temperature
      case 0xF02D://Heatsink Temperature Sensor
      case 0xF02B://Fan Speed
      case 0xF02C://Requested Fan Speed
      case 0xF032://Low Cell Voltage
      case 0xF03E://Low Cell Voltage ID (Cell Num)
      case 0xF033://High Cell Voltage
      case 0xF03D://High Cell Voltage ID (Cell Num)
      case 0xF034://Avg. Cell Voltage
      case 0xF035://Low Opencell Voltage
      case 0xF040://Low Opencell Voltage ID (Cell Num)
      case 0xF036://High Opencell Voltage
      case 0xF03F://High Opencell Voltage ID (Cell Num)
      case 0xF037://Avg. Opencell Voltage
      case 0xF038://Low Cell Resistance
      case 0xF042://Low Cell Resistance ID (Cell Num)
      case 0xF039://High Cell Resistance
      case 0xF041://High Cell Resistance ID (Cell Num)
      case 0xF03A://Avg. Cell Resistance
      case 0xF046://Input Power Supply Voltage
      case 0xF049://Fan Voltage
      case 0xF100://Cell Voltages (Cells 1-12)
      case 0xF101://Cell Voltages (Cells 13-24)
      case 0xF102://Cell Voltages (Cells 25-36)
      case 0xF103://Cell Voltages (Cells 37-48)
      case 0xF104://Cell Voltages (Cells 49-60)
      case 0xF105://Cell Voltages (Cells 61-72)
      case 0xF106://Cell Voltages (Cells 73-84)
      case 0xF107://Cell Voltages (Cells 85-96)
      case 0xF108://Cell Voltages (Cells 97-108)
      case 0xF109://Cell Voltages (Cells 109-120)
      case 0xF10A://Cell Voltages (Cells 121-132)
      case 0xF10B://Cell Voltages (Cells 133-144)
      case 0xF10C://Cell Voltages (Cells 145-156)
      case 0xF10D://Cell Voltages (Cells 157-168)
      case 0xF10E://Cell Voltages (Cells 169-180)
      case 0xF300://Opencell Voltages (Cells 1-12)
      case 0xF301://Opencell Voltages (Cells 13-24)
      case 0xF302://Opencell Voltages (Cells 25-36)
      case 0xF303://Opencell Voltages (Cells 37-48)
      case 0xF304://Opencell Voltages (Cells 49-60)
      case 0xF305://Opencell Voltages (Cells 61-72)
      case 0xF306://Opencell Voltages (Cells 73-84)
      case 0xF307://Opencell Voltages (Cells 85-96)
      case 0xF308://Opencell Voltages (Cells 97-108)
      case 0xF309://Opencell Voltages (Cells 109-120)
      case 0xF30A://Opencell Voltages (Cells 121-132)
      case 0xF30B://Opencell Voltages (Cells 133-144)
      case 0xF30C://Opencell Voltages (Cells 145-156)
      case 0xF30D://Opencell Voltages (Cells 157-168)
      case 0xF23E://Opencell Voltages (Cells 169-180)
      case 0xF200://Internal Resistances (Cells 1-12)
      case 0xF201://Internal Resistances (Cells 13-24)
      case 0xF202://Internal Resistances (Cells 25-36)
      case 0xF203://Internal Resistances (Cells 37-48)
      case 0xF204://Internal Resistances (Cells 49-60)
      case 0xF205://Internal Resistances (Cells 61-72)
      case 0xF206://Internal Resistances (Cells 73-84)
      case 0xF207://Internal Resistances (Cells 85-96)
      case 0xF208://Internal Resistances (Cells 97-108)
      case 0xF209://Internal Resistances (Cells 109-120)
      case 0xF20A://Internal Resistances (Cells 121-132)
      case 0xF20B://Internal Resistances (Cells 133-144)
      case 0xF20C://Internal Resistances (Cells 145-156)
      case 0xF20D://Internal Resistances (Cells 157-168)
      case 0xF20E://Internal Resistances (Cells 169-180)
        break;
    }
  }
}
