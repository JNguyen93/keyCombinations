#define arr_len( x )  ( sizeof( x ) / sizeof( *x ) )

const byte left = 2;
const byte right = 3;
const byte leftKey = 4;
const byte midKey = 5;
const byte rightKey = 6;
const byte maglockValve = 11;
const byte maglockBreak = 12;
int solCounter = 0;
bool leftFlag = false;
bool rightFlag = false;
int sol[10] = { -1, -1, 1, -1, -1, -1, 1, 1, -1, 1};
int solLen = arr_len(sol);
bool correct = false;
bool leftKeyFlag = false;
bool midKeyFlag = false;
bool rightKeyFlag = false;
bool penalty = false;
bool penalty0 = false;
bool penalty1 = false;
int resetCounter = 0;

void setup() {
  Serial.begin(9600);
  pinMode(left, INPUT_PULLUP);
  pinMode(right, INPUT_PULLUP);
  pinMode(leftKey, INPUT_PULLUP);
  pinMode(midKey, INPUT_PULLUP);
  pinMode(rightKey, INPUT_PULLUP);
  pinMode(maglockBreak, OUTPUT);
  pinMode(maglockValve, OUTPUT);
  digitalWrite(maglockValve, HIGH);
  digitalWrite(maglockBreak, LOW);
}

void loop() {
  /*Serial.print("Left Key: ");
    Serial.println(!digitalRead(leftKey));
    Serial.print("Right Key: ");
    Serial.println(!digitalRead(rightKey));
    Serial.print("Mid Key: ");
    Serial.println(!digitalRead(midKey));
    Serial.print("Penalty: ");
    Serial.println(penalty);
    Serial.print("Penalty0: ");
    Serial.println(penalty0);
    Serial.print("Penalty1: ");
    Serial.println(penalty1);*/
  if (penalty0 || penalty1) {
    penalty = true;
  }
  else {
    penalty = false;
  }
  if (!digitalRead(leftKey) && !penalty) {
    leftKeyFlag = true;
  }
  else {
    leftKeyFlag = false;
  }

  if (!digitalRead(rightKey) && leftKeyFlag && !penalty) {
    rightKeyFlag = true;
  }
  else if (!digitalRead(rightKey) && !leftKeyFlag) {
    penalty0 = true;
  }
  else {
    rightKeyFlag = false;
    penalty0 = false;
  }

  if (!digitalRead(midKey) && rightKeyFlag && !penalty) {
    digitalWrite(maglockValve, LOW);
  }
  else if (!digitalRead(midKey) && !rightKeyFlag) {
    penalty1 = true;
  }
  else {
    penalty1 = false;
    digitalWrite(maglockValve, HIGH);
  }

  Serial.print("Counter: ");
  Serial.println(solCounter);
  if (!digitalRead(left)) {
    if (!leftFlag) {
      if (sol[solCounter] == -1) {
        solCounter++;
      }
      else {
        solCounter = 0;
      }
      leftFlag = true;
    }
  }
  else {
    leftFlag = false;
  }

  if (!digitalRead(right)) {
    if (!rightFlag) {
      if(correct){
        resetCounter++;
        if(resetCounter >= 2){
          correct = false;
          resetCounter = 0;
        }
      }
      else if (sol[solCounter] == 1) {
        if (solCounter == 9) {
          correct = true;
        }
        else {
          solCounter++;
        }
      }
      else {
        solCounter = 0;
      }
      rightFlag = true;
    }
  }
  else {
    rightFlag = false;
  }

  if (correct) {
    digitalWrite(maglockBreak, HIGH);
    //delay(500);
    solCounter = 0;
  }
  else {
    digitalWrite(maglockBreak, LOW);
  }
}
