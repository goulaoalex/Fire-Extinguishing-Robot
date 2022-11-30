// ----------------- Alexandre Goulão -----------------
// ------------- Fire Extinguishing Robot -------------
//
// The robot will run through a determined course, by following the wall to it's right, using an ultrassonic sensor
// The objective is to find a lit candle and extinguish it's flame

//Motor speed set between 0 and 255

int enA = 2;                      // Velocity Motor A (PIN 2)                                      
int Motor_Left_Backwards = 3;     // Motor A (left) backwards motion -> IN1 (PIN 3)               
int Motor_Left_Forwards = 4;      // Motor A (left) forwards motion -> IN2 (PIN 4)              

int enB = 5;                      // Velocity Motor B (PIN 5)                
int Motor_Right_Forwards = 6;     // Motor B (right) forwards motion -> IN3 (PIN 6)              
int Motor_Right_Backwards = 7;    // Motor B (right) backwards motion -> IN4 (PIN 7)                   

int i = 1;

long distanceRight;               // Distance for the ultrasonic sensor on the right
long distanceMiddle;              // Distance for the ultrasonic sensor on the front (middle)
long time1;                       // Time it takes to transmit the ultrasonic signal in the sensor place on the right
long time2;                       // Time it takes to transmit the ultrasonic signal in the sensor place on the front (middle)                
int trigRight = 9;                // The trig PIN of the sensor to the right is connected to PIN 9 on the arduino
int echoRight = 8;                // The echo PIN of the sensor to the right is connected to PIN 8 on the arduino
int trigMiddle = 11;              // The trig PIN of the middle sensor is connected to PIN 11 on the arduino
int echoMiddle = 10;              // The echo PIN of the middle sensor is connected to PIN 10 on the arduino

int INA_FAN = 53;                 // Connects the INA pin of the FAN to PIN 31 of the board (moving clockwise)                
int INB_FAN = 51;                 // Connects the INB pin of the FAN to PIN 30 of the board (moving counter-clockwise)           

int flame_value = analogRead(A14);                   // Initialize the flame value of the sensor by connecting it to PIN analog 6


void setup() {
  Serial.begin(9600);                               // Establish serial communication between arduino board and the user device 
  pinMode(enA, OUTPUT);                             // Set enA has output
  pinMode(enB, OUTPUT);                             // Set enB has output
  pinMode(Motor_Left_Backwards, OUTPUT);            // Set Motor_Left_Backwards has output
  pinMode(Motor_Left_Forwards, OUTPUT);             // Set Motor_Left_Forwards has output
  pinMode(Motor_Right_Forwards, OUTPUT);            // Set Motor_Right_Forwards has output
  pinMode(Motor_Right_Backwards, OUTPUT);           // Set Motor_Right_Backwards has output
  
  pinMode(trigRight, OUTPUT);                       // Set trigRight has output
  pinMode(echoRight, INPUT);                        // Set echoRight has input
  pinMode(trigMiddle, OUTPUT);                      // Set trigMiddle has output
  pinMode(echoMiddle, INPUT);                       // Set echoMiddle has input
  
  pinMode(A6, INPUT);                               // Set A6 has input
  pinMode(INA_FAN, OUTPUT);                         // Set INA_FAN has output
  pinMode(INB_FAN, OUTPUT);                         // Set INB_FAN has output
}

// The difference in speeds when both motors are going in the same directions, happens because of the weigth distribituion 
//on the chassi and because of the physical properties of both motors.


// Both motors go FORWARD
void FORWARD()                     
{
  digitalWrite(Motor_Left_Forwards, HIGH);
  digitalWrite(Motor_Left_Backwards, LOW); 
  analogWrite(enA, 100);
  digitalWrite(Motor_Right_Forwards, HIGH); 
  digitalWrite(Motor_Right_Backwards, LOW);
  analogWrite(enB, 76);
}

// Move FORWARD to the RIGHT, by only turning on the LEFT motor
void FORWARD_RIGHT()                
{
  digitalWrite(Motor_Left_Backwards, LOW); 
  digitalWrite(Motor_Left_Forwards, HIGH);
  analogWrite(enA, 85);
  digitalWrite(Motor_Right_Forwards, LOW);
  digitalWrite(Motor_Right_Backwards, LOW);
}

// Move FORWARD to the LEFT, by only turning on the RIGHT motor
void FORWARD_LEFT()               
{
  digitalWrite(Motor_Left_Backwards, LOW);
  digitalWrite(Motor_Left_Forwards, LOW);
  digitalWrite(Motor_Right_Forwards, HIGH); 
  digitalWrite(Motor_Right_Backwards, LOW);
  analogWrite(enB, 84);
}

// Move BACKWARD to the RIGHT, by only turning on the LEFT motor
void BACKWARD_RIGHT()
{
  digitalWrite(Motor_Left_Backwards, HIGH);
  digitalWrite(Motor_Left_Forwards, LOW);
  analogWrite(enA, 115);
  digitalWrite(Motor_Right_Forwards, LOW);
  digitalWrite(Motor_Right_Backwards, LOW);
}

// Move BACKWARD to the LEFT, by only turning on the RIGHT motor
void BACKWARD_LEFT()
{
  digitalWrite(Motor_Left_Backwards, LOW);
  digitalWrite(Motor_Left_Forwards, LOW);
  digitalWrite(Motor_Right_Forwards, LOW);
  digitalWrite(Motor_Right_Backwards, HIGH);
   analogWrite(enB, 84);
}

// Both motors go BACKWARD
void BACKWARD() {
  digitalWrite(Motor_Left_Backwards, HIGH);
  digitalWrite(Motor_Left_Forwards, LOW);
  analogWrite(enA, 110);
  digitalWrite(Motor_Right_Forwards, LOW);
  digitalWrite(Motor_Right_Backwards, HIGH);
  analogWrite(enB, 100);
}

// The robot moves SLOWLY in a curve to the RIGHT
void HARD_TURN_RIGHT() {
  digitalWrite(Motor_Left_Backwards, HIGH);
  digitalWrite(Motor_Left_Forwards, LOW);
  analogWrite(enA, 115);
  digitalWrite(Motor_Right_Forwards, HIGH);
  digitalWrite(Motor_Right_Backwards, LOW);
  analogWrite(enB, 105);
}

// The robot moves SLOWLY in a curve to the LEFT
void HARD_TURN_LEFT() {
  digitalWrite(Motor_Left_Backwards, LOW);
  digitalWrite(Motor_Left_Forwards, HIGH);
  analogWrite(enA, 120);
  digitalWrite(Motor_Right_Forwards, LOW);
  digitalWrite(Motor_Right_Backwards, HIGH);
  analogWrite(enB, 110);
}

// The robot moves RAPIDLY in a curve to the RIGHT
void SOFT_TURN_RIGHT() {
  digitalWrite(Motor_Left_Backwards, LOW);
  digitalWrite(Motor_Left_Forwards, HIGH);
  analogWrite(enA, 95);
  digitalWrite(Motor_Right_Forwards, HIGH);
  digitalWrite(Motor_Right_Backwards, LOW);
  analogWrite(enB, 135);
}

// The robot moves RAPIDLY in a curve to the LEFT
void SOFT_TURN_LEFT() {
  digitalWrite(Motor_Left_Backwards, LOW);
  digitalWrite(Motor_Left_Forwards, HIGH);
  analogWrite(enA, 135);
  digitalWrite(Motor_Right_Forwards, HIGH);
  digitalWrite(Motor_Right_Backwards, LOW);
  analogWrite(enB, 95);
}

// Stops the right motor
void STOP_RIGHT() {               
  digitalWrite(Motor_Left_Backwards, LOW);
  digitalWrite(Motor_Left_Forwards, LOW);
}

// Stops the left motor
void STOP_LEFT() {               
  digitalWrite(Motor_Right_Forwards, LOW);
  digitalWrite(Motor_Right_Backwards, LOW);  
}

void loop() {
  
  // Applies a 1 microsecond pulse to make sure the RIGHT trig PIN is clear 
  digitalWrite(trigRight,LOW);           
  delayMicroseconds(1);
 
  // Applies a 10 microsecond pulse to generate the ultrasound wave for the RIGHT sensor
  digitalWrite(trigRight, HIGH);
  delayMicroseconds(10);
 
  //pulseIn function reads the travel time and puts that value into the variable “distanceRight”
  time1 = pulseIn(echoRight, HIGH);
 
  //Gives the exact distance between the obstacle and the sensor on the right in cm
  distanceRight= int(0.017*time1);

  // Applies a 1 microsecond pulse to make sure the LEFT trig PIN is clear 
  digitalWrite(trigMiddle,LOW);        
  delayMicroseconds(1);
 
  // Applies a 10 microsecond pulse to generate the ultrasound wave for the LEFT sensor
  digitalWrite(trigMiddle, HIGH);
  delayMicroseconds(10);
 
  //pulseIn function reads the travel time and puts that value into the variable “distanceLeft”
  time2 = pulseIn(echoMiddle, HIGH);
 
  //Gives the exact distance between the obstacle and the sensor on the right in cm
  distanceMiddle= int(0.017*time2);

  // If the middle sensors doesn't find the wall, and the right sensor can find the wall, it will simply move forward
  if (distanceMiddle > 20 && distanceRight < 20)
  {
    FORWARD();
  }

  // If the middle sensor finds the wall, and the right sensor can find the wall, it means it has to make a slow turn to the left to compensate
  else if (distanceMiddle < 20 && distanceRight < 20)
  {
    HARD_TURN_LEFT();
  }

  // If neither sensor can find the wall too close, it means the robot has reached an intersection and has to make a fast turn to the left
  else if (distanceMiddle > 20 && distanceRight > 20)
  {
   FORWARD_LEFT();
  }

  // If the middle sensor get's too close to the wall, it will move backwards
  else if (distanceMiddle < 4 && distanceRight > 10)
  {
   BACKWARD();
  }

  // If the flame value is less or equal to 300, it means the sensor found a flame
  if (flame_value <= 300)
  {
    while(i == 1)
    {  
      int flame_value = analogRead(A14);                 // Continuously read the value of the flame

      // If the flame value is greater than 80 it means the flame is still too far away and the robot has to move forward
      if(flame_value > 80)       
      {
        FORWARD();
      }

      // If the flame value is less than 80 it means the flame is close enough for the fan to put it away
      else if (flame_value <= 80)
      {
        delay(100);
        STOP_LEFT();                                    // Starts off my shutting down the left motor
        STOP_RIGHT();                                   // Turns off the right motor
        digitalWrite(31, HIGH);                         // Turns ON the fan
        delay(4000);                                    // Fan is ON for 4 seconds to put out the flame
        digitalWrite(31, LOW);                          // Turns the fan OFF
      }

      // In case neither condition is met, it will stop for 1 second and resume searching for the flame
      else 
      {
        STOP_LEFT();
        STOP_RIGHT();
        delay(1000); 
      }
    }
  }

  // Prints the current value of the flame
  Serial.print("Value of the flame: ");
  Serial.println(flame_value);
}
