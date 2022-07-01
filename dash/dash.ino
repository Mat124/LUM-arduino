#include <mcp2515.h>

#define LEDONE 5

MCP2515 mcp2515(10);
struct can_frame frame;

void setup() {
  mcp2515.reset();
  mcp2515.setBitrate(CAN_125KBPS);
  mcp2515.setNormalMode();
  pinMode(LEDONE, OUTPUT);
  digitalWrite(LEDONE, LOW);
}

void loop() {
  digitalWrite(LEDONE, LOW);
  if (mcp2515.readMessage(&frame) == MCP2515::ERROR_OK) {
    // frame contains received message
    digitalWrite(LEDONE, HIGH);
    delay(1000);
    /*
    switch (frame.can_id) {
      case 1: //frame is from address 1
        //turn on/off LED
        if (frame.data[0] == 1) {
          digitalWrite(LEDONE, HIGH);
        }
        else if (frame.data[0] == 0) {
          digitalWrite(LEDONE, LOW);
        }
        break;
    }
    */
    /*
    if (frame.data[0] == 1) {
      digitalWrite(LEDONE, HIGH);
    }
    else if (frame.data[0] == 0) {
      digitalWrite(LEDONE, LOW);
    }
    */
  }
}
