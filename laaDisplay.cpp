struct PinStruct {
  int id, value;
};

class laaDisplay {
 private:
  struct Pins {
    struct Control {
      PinStruct rs, en;
    } control;
    struct Data {
      PinStruct d0, d1, d2, d3, d4, d5, d6, d7;
    } data;
  } pins;
  const int DELAY = 1;  // 300ns -> 0.0003ms -> 1ms

  void updatePinState(PinStruct &_pin, int _value) {
    _pin.value = _value;
    digitalWrite(_pin.id, _pin.value);
  };

  void sendCommand() {
    this->updatePinState(this->pins.control.en, LOW);
    this->updatePinState(this->pins.control.en, HIGH);
    delay(this->DELAY);
    this->updatePinState(this->pins.control.en, LOW);
  };

  void init() {
    this->pins.control.rs.value = LOW;
    this->pins.control.en.value = LOW;
    this->pins.data.d0.value = LOW;
    this->pins.data.d1.value = LOW;
    this->pins.data.d2.value = LOW;
    this->pins.data.d3.value = LOW;
    this->pins.data.d4.value = LOW;
    this->pins.data.d5.value = LOW;
    this->pins.data.d6.value = LOW;
    this->pins.data.d7.value = LOW;
  };

  /*
    RS:
    - LOW   = CONFIG MODE
    - HIGH  = WRITING MODE
  */
  void setWritingMode() { this->updatePinState(this->pins.control.rs, HIGH); };

  struct Configurations {
    Configurations() { this->configurations.setConfigMode(); };
    /*
      RS:
      - LOW   = CONFIG MODE
      - HIGH  = WRITING MODE
    */
    void setConfigMode() {
      this->updatePinState(this->pins.control.rs, LOW);
    };
    struct FunctionSet {
      /*
        D5:
        - LOW  = OFF FUNCTION SET
        - HIGH = ON  FUNCTION SET
      */
      void setFunctionSetMode(bool _isOn) {
        this->updatePinState(this->pins.data.d5, _isOn);
      };
      /*
        D4:
        - LOW  = 4BIT MODE
        - HIGH = 8BIT MODE
      */
      void setBitMode(bool _is8BitMode) {
        this->updatePinState(this->pins.data.d4, _is8BitMode);
      };
      /*
        D3:
        - LOW  = 1 LINE DISPLAY
        - HIGH = 2 LINE DISPLAY
      */
      void setNumberOfLines(bool _is2lines) {
        this->updatePinState(this->pins.data.d3, _is2lines);
      };
      /*
        D2:
        - LOW  = 5x8 DOTS MODE
        - HIGH = 5x11 DOTS MODE
      */
      void setDotMode(bool _is5x11) {
        this->updatePinState(this->pins.data.d2, _is5x11);
      };
    } functionSet;
    struct Methods {
      Methods() { this->init(); }
      void clear() {
        this->updatePinState(this->pins.data.d0, HIGH);
        this->sendCommand();
      };
      void home() {
        this->updatePinState(this->pins.data.d1, HIGH);
        this->sendCommand();
      };
      /*
        D2:
        - LOW   = LCD SPENTO
        - HIGH  = LCD ACCESSO

        D1:
        - LOW   = CURSOR OFF
        - HIGH  = CURSOR ON

        D0:
        - LOW   = CURSOR_BLINK OFF
        - HIGH  = CURSOR_BLINK ON
      */
      void backlightCursorRegister(bool _isBacklightOn, bool _isCursorOn = false, bool _isCursorBlinking = false) {
        this->updatePinState(this->pins.data.d3, HIGH);
        this->updatePinState(this->pins.data.d2, _isBacklightOn);
        this->updatePinState(this->pins.data.d1, _isCursorOn);
        this->updatePinState(this->pins.data.d0, _isCursorBlinking);
      };
    } methods;
  } configurations;

 public:
  laaDisplay(int rs, int en, int d0, int d1, int d2, int d3, int d4, int d5, int d6, int d7) {
    this->pins.control.rs.id = rs;
    this->pins.control.en.id = en;
    this->pins.data.d0.id = d0;
    this->pins.data.d1.id = d1;
    this->pins.data.d2.id = d2;
    this->pins.data.d3.id = d3;
    this->pins.data.d4.id = d4;
    this->pins.data.d5.id = d5;
    this->pins.data.d6.id = d6;
    this->pins.data.d7.id = d7;

    this->init();
    // this->configurations.setConfigMode();
    this->configurations.setFunctionSetMode(true);
    this->configurations.functionSet.setBitMode(true);
    this->configurations.functionSet.setNumberOfLines(false);
    this->configurations.functionSet.setDotMode(false);
    this->sendCommand();
    this->configurations.methods.clear();
    this->configurations.methods.home();
    this->configurations.methods.backlightCursorRegister(true, false, false);
  };

  void print(String _myString) {
    this->setWritingMode();
    for (int i = 0; i < _myString.length(); i++) {
      const char thisChar = _myString[i];
      const int asciiCode = (int)thisChar;

      updatePinState(pins.data.d0, asciiCode & 1);
      updatePinState(pins.data.d1, (asciiCode >> 1) & 1);
      updatePinState(pins.data.d2, (asciiCode >> 2) & 1);
      updatePinState(pins.data.d3, (asciiCode >> 3) & 1);
      updatePinState(pins.data.d4, (asciiCode >> 4) & 1);
      updatePinState(pins.data.d5, (asciiCode >> 5) & 1);
      updatePinState(pins.data.d6, (asciiCode >> 6) & 1);
      updatePinState(pins.data.d7, (asciiCode >> 7) & 1);

      sendCommand();
    };
  };
};

laaDisplay myDisplay(2, 3, 4, 5, 6, 7, 8, 9, 10, 11);

void setup() { myDisplay.print("hello"); };

void loop() {};