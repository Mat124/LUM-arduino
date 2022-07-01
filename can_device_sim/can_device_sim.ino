#include <mcp2515.h>

MCP2515 mcp2515(10);
struct can_frame frame;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  mcp2515.reset();
  mcp2515.setBitrate(CAN_125KBPS);
  mcp2515.setNormalMode();
  frame.can_id = 1;
  frame.can_dlc = 1;
}

void loop() {
  // put your main code here, to run repeatedly:
  frame.data[0] = 1;
  if (mcp2515.sendMessage(&frame) == MCP2515::ERROR_OK) {
    Serial.println("sent CAN message containing 1 from address 1");
  }
  else {
    Serial.println("failed to send CAN message");
  }
  delay(2000);
  frame.data[0] = 0;
  if (mcp2515.sendMessage(&frame) == MCP2515::ERROR_OK) {
    Serial.println("sent CAN message containing 0 from address 1");
  }
  else {
    Serial.println("failed to send CAN message");
    
  }
  delay(2000);
}  
