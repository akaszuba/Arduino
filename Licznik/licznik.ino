#include <CountDown.h>


#define pinLED PC13
#define button1 PB9
#define button2 PB8
#define button3 PB7
#define button4 PB6


int blink = 0;
int blink_count = 1;

//setup buttons
const int BUTTON_COUNT = 4;
int buttons[BUTTON_COUNT] = { PB9, PB8, PB7, PB6 };
int buttonState[BUTTON_COUNT] = { LOW, LOW, LOW, LOW };

enum State { DEMO,
             SETUP_UP,
             SETUP_DOWN,
             PAUSED,
             COUNTING };
State CurrentState;

unsigned long lastUpdateMills = 0;
uint refreshRate = 500;
unsigned long demoLength = 5000;

//display variables
String upColor = "RED";
String downColor = "GREEN";
String pauseColor = "YELLOW";
bool isBlinking = false;


class DisplayState{
  private:
  int _previousDisplayValue;
  String _prevoidDisplayColor;
  bool _previousIsEnabled;
  public: 
    int displayValue;
    String displayColor;
    bool isEnabled;
    void reset()
    {
      _previousDisplayValue = displayValue;
      _prevoidDisplayColor = displayColor;
      _previousIsEnabled = isEnabled;
    }
    bool hasChanged(){
       return _previousDisplayValue != displayValue || _prevoidDisplayColor != displayColor || _previousIsEnabled != isEnabled;
    }
};

DisplayState displayState = DisplayState();

//counter variables
unsigned long upTime = 20000;
unsigned long downTime = 10000;
CountDown timer(CountDown::MILLIS);
bool isCountingUp = false;


void setup() {
  Serial.begin(9600);
  pinMode(pinLED, OUTPUT);

  //setup button inputs
  for (int i = 0; i < sizeof(buttons); i++) {
    pinMode(buttons[i], INPUT_PULLUP);
  }


  CurrentState = DEMO;
}

void loop() {
  delay(10);
  handleButtons();
  switch (CurrentState) {
    case DEMO:
      if (demoLength > millis()) {

      } else {
        CurrentState = SETUP_UP;
      }
      break;
    case SETUP_UP:
      displayState.displayColor = upColor;
      displayMillis(upTime);
      isBlinking = true;
      break;
    case SETUP_DOWN:
      displayState.displayColor = downColor;
      displayMillis(downTime);
      isBlinking = true;
      break;
    case PAUSED:
      isBlinking = false;
      displayState.displayColor = pauseColor;
      break;
    case COUNTING:
      if(timer.remaining() == 0){
        //timer got down to 0
        if(isCountingUp){
          timer.start(downTime);
          isCountingUp = false;
        }else{
          timer.start(upTime);
          isCountingUp = true;
        }
      }
      if(isCountingUp){
       displayState.displayColor = upColor;
      }else{
       displayState.displayColor = downColor;
      }
      displayMillis(timer.remaining());
      break;
  }

  RefreshView();
}

void displayMillis(unsigned long milis){
   displayState.displayValue = milis/1000 + !!(milis%1000);
}

void RefreshView() {
  if (lastUpdateMills + refreshRate < millis()
      || lastUpdateMills > millis()) {
    lastUpdateMills = millis();

    if(displayState.hasChanged()){
 Serial.print("State: ");
    Serial.print(CurrentState);
    Serial.print(", Display: ");
    Serial.print(displayState.displayValue);
    Serial.print(", Color ");
    Serial.println(displayState.displayColor);
    displayState.reset();
    }

   
  }
}

void handleButtons() {
  if (handleButtonAction(0)) {
    Serial.println("Button UP pressed...");
    switch (CurrentState) {
      case SETUP_UP:
        if (upTime < 95000) {
          upTime = upTime + 5000;
        }
        break;
      case SETUP_DOWN:
        if (downTime < 95000) {
          downTime = downTime + 5000;
        }
        break;
      case DEMO:
      case PAUSED:
      case COUNTING:
        break;
    }
  }
  if (handleButtonAction(1)) {
    Serial.println("Button DOWN pressed...");
    switch (CurrentState) {
      case SETUP_UP:
        if (upTime > 5000) {
          upTime = upTime - 5000;
        }
        break;
      case SETUP_DOWN:
        if (downTime > 5000) {
          downTime = downTime - 5000;
        }
        break;
      case DEMO:
      case PAUSED:
      case COUNTING:
        break;
    }
  }
  if (handleButtonAction(2)) {
       Serial.println("Button START pressed...");

    switch (CurrentState) {
      case SETUP_UP:
        CurrentState = SETUP_DOWN;
        break;
      case SETUP_DOWN:
        CurrentState = PAUSED;
        isCountingUp = false;
        timer.start(downTime);
        timer.stop();
        break;
      case PAUSED:
        CurrentState = COUNTING;
        timer.cont();
        break;
      case COUNTING:
        Serial.println("pause");
        CurrentState = PAUSED;
        timer.stop();
        break;
      case DEMO:
        break;
    }
  }
  if (handleButtonAction(3)) {
    Serial.println("Button RESET pressed...");
    timer.stop();
    CurrentState = DEMO;
  }
}

bool handleButtonAction(int buttonIndex) {
  if (buttonIndex < 0 || buttonIndex >= BUTTON_COUNT) {
    // Invalid button index
    return false;
  }

  int buttonPin = buttons[buttonIndex];

  // Read the button state
  int reading = digitalRead(buttonPin);

  // Check for debouncing
  if (reading != buttonState[buttonIndex]) {
    delay(5);                          // Adjust the delay based on your requirements
    reading = digitalRead(buttonPin);  // Read again

    // If still different, update the state
    if (reading != buttonState[buttonIndex]) {
      buttonState[buttonIndex] = reading;
      return (buttonState[buttonIndex] == HIGH);
    }
  }

  return false;
}


void blinkLed() {
  if (blink > 5) {
    digitalWrite(pinLED, HIGH);
    blink_count = -blink_count;
  }

  if (blink < 0) {
    digitalWrite(pinLED, LOW);
    blink_count = -blink_count;
  }

  blink = blink + blink_count;
}
