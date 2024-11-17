/*  GND -> GND 
    5V  -> 5V
    VRx -> A0
    VRy -> A1
    SW  -> ...
*/

struct XY {
  int x;
  int y;
};

struct JoystickBase {
  XY pin;
  XY value;
};

JoystickBase myJoystick = { 
  .pin = {A0, A1},
};

void setup() {
  Serial.begin(9600);
  
  pinMode(myJoystick.pin.x, INPUT);
  pinMode(myJoystick.pin.y, INPUT);
}

void loop() {
  myJoystick.value.x = analogRead(myJoystick.pin.x);
  myJoystick.value.y = analogRead(myJoystick.pin.y);

  Serial.println(
       "X: " + String(myJoystick.value.x) + 
    "\t Y: " + String(myJoystick.value.y)
  );
};