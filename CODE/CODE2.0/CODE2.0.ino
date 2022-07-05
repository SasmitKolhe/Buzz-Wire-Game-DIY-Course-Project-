
int latchPin = 3;      // ST_CP [RCK] on 74HC595
int clockPin = 4;      // SH_CP [SCK] on 74HC595
int dataPin = 2;     // DS [S1] on 74HC595


const int wire = 10;  // The pin of the wire
const int led = 6;   // Pin to be connected to the LED
const int buzzer = 8; // The pin of buzzer
const int fail_threshold = 9; // The Threshold of failing

byte seg_digits[10] = // The function which contains all the segment digits to display each number 
{ 
  B00000011,  // = 0
  B10011111,  // = 1
  B00100101,  // = 2
  B00001101,  // = 3
  B10011001,  // = 4
  B01001001,  // = 5
  B01000001,  // = 6
  B00011111,  // = 7
  B00000001,  // = 8
  B00001001,  // = 9
};

enum Status // To assign a constant to a name variable
{
  STOP = 0,// Stop is assingned number 0
  GO = 1  // Go is assigned number 1
};

void displayDigit(int x)  // Function to display the number on the LED screen
{
   digitalWrite(latchPin, LOW);
   shiftOut(dataPin, clockPin, LSBFIRST, seg_digits[x]);
   digitalWrite(latchPin, HIGH);
}


void displayInitialSetup() // The function consisting of display's inital setup
{
  pinMode(latchPin, OUTPUT);
  pinMode(dataPin, OUTPUT);  
  pinMode(clockPin, OUTPUT);
  displayDigit(0);
}
void beep() // Function to make the buzzer beep
{
  for(int i=0; i<2; i++)
  {
    digitalWrite(buzzer, HIGH);
    delay(50);
    digitalWrite(buzzer, LOW);
    delay(50);
  }
}
void failAlarm()// Function to indicate the touching of wire(Failing)
{
    digitalWrite(led, HIGH);
    beep();
    delay(150);

    digitalWrite(led, LOW);
    digitalWrite(buzzer, LOW);
    delay(500);
}



void gameInitialSetup()// Initializing game setup
{
  
  pinMode(led, OUTPUT);
  pinMode(buzzer, OUTPUT);
  
  pinMode(wire, HIGH);
  digitalWrite(led,LOW);
  digitalWrite(buzzer,LOW);
  digitalWrite(wire, LOW);
}

void setup() {

  Serial.begin(9600);
  displayInitialSetup();
  gameInitialSetup();
}

Status stat = GO; // Setting inital conditions
int failCounter = 0;// The variable to count the number of fails

void loop() {


  while (failCounter > fail_threshold) // To check whether the player has crossed the failing limit
    {
      gameover();
    }

  switch (stat) // The loop of the game till the player fails
  {
    case GO: // Case of not touching the wire(Non fail)
      digitalWrite(led, LOW);
      digitalWrite(buzzer, LOW);
      if (digitalRead(wire) == HIGH)
       {
         stat = STOP;
       }
      break;
      
    case STOP: // Case when the player touches the wire(Fails)
      failCounter++;
      if (failCounter > fail_threshold)
        break;
      displayDigit(failCounter);
      Serial.println(failCounter);
      failAlarm();
      stat = GO;
      break;
  }

}
void gameover() // To make a significant difference from beeping when the game is over
{
  for (int i=0; i<2; i++)
  {
    digitalWrite(led,HIGH);
    digitalWrite(buzzer, HIGH);
    delay(5);
    digitalWrite(buzzer, LOW);
    digitalWrite(led,LOW);
    delay(50);
  }
}
