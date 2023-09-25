//Ultrasonic Sensor 1
const int trigPin2 = A0, echoPin2 = A1;
//Ultrasonic Sensor 2
const int trigPin1 = 7, echoPin1 = 6;
//Ultrasonic Sensor 3
const int trigPin3 = A2, echoPin3 = A3;
const int squareSide = 30;
int wallType = 0; //Type of wall found

//L298N Motor Driver
// Motor A connections
int enA = 9, in1 = 11, in2 = 12;//Right
// Motor B connections
int enB = 10, in3 = 4, in4 = 5;//Left
int orient = 0;
int steps[]={0,3,2};
//Encoder
#define CLK1 1 //Left
#define CLK2 2 //Right
int stateCLK1=0, stateCLK2=0,counter=0;
float duration1,duration2,duration3,distance1,distance2,distance3;
const float pi = 3.14; // Pi value 
const int R = 0.03, wtw=22;
const int N = 40; //no of steps for one rotation
float distance = 0;

//SETUP HERE
void setup() {
  Serial.begin(9600);
  ultrasonics();
  motorControl();
  encoders();
}

void ultrasonics() {
  pinMode(trigPin1, OUTPUT);
  pinMode(echoPin1, INPUT);
  pinMode(trigPin2, OUTPUT);
  pinMode(echoPin2, INPUT);
  pinMode(trigPin3, OUTPUT);
  pinMode(echoPin3, INPUT);
}

void motorControl() {
// Motor control pins as outputs
	pinMode(enA, OUTPUT);
	pinMode(enB, OUTPUT);
	pinMode(in1, OUTPUT);
	pinMode(in2, OUTPUT);
	pinMode(in3, OUTPUT);
	pinMode(in4, OUTPUT);
	
	// // Turn off motors - Initial state
	// digitalWrite(in1, LOW);
	// digitalWrite(in2, LOW);
	// digitalWrite(in3, LOW);
	// digitalWrite(in4, LOW);
}

void encoders() {
  pinMode(CLK1,INPUT);
  pinMode(CLK2,INPUT);
  stateCLK1 = digitalRead(CLK1);  
  stateCLK2 = digitalRead(CLK2);    
}

//LOOP STARTS HERE
void loop() {
  // wallType = ultrasoundDistance();
  for(int i=0;i<sizeof(steps);i++){
    movementControl(orient);
  }
	// speedControl();//Will see later
}

int ultrasoundDistance() {
  digitalWrite(trigPin1, HIGH);
  digitalWrite(trigPin2, HIGH);
  digitalWrite(trigPin3, HIGH);

  duration1 = pulseIn(echoPin1, HIGH);
  duration2 = pulseIn(echoPin2, HIGH);
  duration3 = pulseIn(echoPin3, HIGH);

  distance1 = (duration1*.0343)/2;// in cm
  distance2 = (duration2*.0343)/2;
  distance3 = (duration3*.0343)/2;
  // Serial.print("Distance: ");
  // Serial.println(distance);
  wallType = 0;
  if (distance1 <=2 || distance2 <=2 || distance3 <=2){
    return wallType = -1;
    delay(500);
  }
  else if (distance1>2 && distance1<squareSide/2){
    return wallType = 1;
    delay(500);
  }
  else if (distance2>2 && distance2<squareSide/2){
    return wallType = 2;
     delay(500);
  }
  else if (distance3>2 && distance3<squareSide/2){
    return wallType = 3;
    delay(500);
  }
  else if (distance1>2 && distance1<squareSide/2 && distance2>2 && distance2<squareSide/2){
    return wallType = 4;
    delay(500);
  }
  else if (distance1>2 && distance1<squareSide/2 && distance3>2 && distance3<squareSide/2){
    return wallType = 5;
    delay(500);
  }
  else if (distance2>2 && distance2<squareSide/2 && distance3>2 && distance3<squareSide/2){
    return wallType = 6;
    delay(500);
  }
  else if (distance1>2 && distance1<squareSide/2 && distance3>2 && distance3<squareSide/2 && distance3>2 && distance3<squareSide/2){
    return wallType = 7;
    delay(500);
  }
}

void movementControl(int orient) {
  analogWrite(enA, 120);
	analogWrite(enB, 120);

  if(orient==0){//Front
  digitalWrite(in1, HIGH);
	digitalWrite(in2, LOW);
	digitalWrite(in3, HIGH);
	digitalWrite(in4, LOW);
	delay(500);
  move(squareSide);
  }
  else if(orient==1){//Back
  digitalWrite(in1, LOW);
	digitalWrite(in2, HIGH);
	digitalWrite(in3, LOW);
	digitalWrite(in4, HIGH);
	delay(500);
  move(squareSide);
  }
  else if(orient==2){//Left
  digitalWrite(in1, HIGH);
	digitalWrite(in2, LOW);
	digitalWrite(in3, LOW);
	digitalWrite(in4, HIGH);
	delay(1000);
  turn(orient,wtw);
  }
  else if(orient==3){//Right
  digitalWrite(in1, LOW);
	digitalWrite(in2, HIGH);
	digitalWrite(in3, HIGH);
	digitalWrite(in4, LOW);
	delay(1000);
  turn(orient,wtw);
  }
  orient = 0;
}

void move(int squareSide) {
  int dist=0;
  while(dist == squareSide){
    if(stateCLK1==1){
    counter++;
  }
    else{
    counter=counter;
  }
  dist = ((2*pi*R)/N) * counter ;
  }
  digitalWrite(in1, LOW);
	digitalWrite(in2, LOW);
	digitalWrite(in3, LOW);
	digitalWrite(in4, LOW);
}
void turn(int orient, int wtw){
  int dist=0;
  while(dist == wtw){
  if(orient==2){
    if(stateCLK2==1){
    counter++;
  }
    else{
    counter=counter;
  }
  }
  else if(orient==3){
    if(stateCLK1==1){
    counter++;
  }
    else{
    counter=counter;
  }
  }
  dist = ((2*pi*R)/N) * counter ;
  }
  move(squareSide);
  digitalWrite(in1, LOW);
	digitalWrite(in2, LOW);
	digitalWrite(in3, LOW);
	digitalWrite(in4, LOW);
}
// void speedControl() {
// 	// Turn on motors
// 	digitalWrite(in1, LOW);
// 	digitalWrite(in2, HIGH);
// 	digitalWrite(in3, LOW);
// 	digitalWrite(in4, HIGH);
	
// 	// Accelerate from zero to maximum speed
// 	for (int i = 0; i < 256; i++) {
// 		analogWrite(enA, i);
// 		analogWrite(enB, i);
// 		delay(20);
// 	}
	
// 	// Decelerate from maximum speed to zero
// 	for (int i = 255; i >= 0; --i) {
// 		analogWrite(enA, i);
// 		analogWrite(enB, i);
// 		delay(20);
// 	}
	
// 	// Now turn off motors
// 	digitalWrite(in1, LOW);
// 	digitalWrite(in2, LOW);
// 	digitalWrite(in3, LOW);
// 	digitalWrite(in4, LOW);
// }


