#define EN_RIGHT 11
#define EN_LEFT 3
#define LEFT_A 6
#define LEFT_B 4
#define RIGHT_A 7
#define RIGHT_B 5


int state;
void setup() {
  pinMode(LEFT_A, OUTPUT);
  pinMode(LEFT_B, OUTPUT);
  pinMode(EN_RIGHT, OUTPUT);
  pinMode(EN_LEFT, OUTPUT);
  pinMode(RIGHT_A, OUTPUT);
  pinMode(RIGHT_B, OUTPUT);
  Serial.begin(9600);
  state = 0;
}

void loop() {
  if(Serial.available()){
    char input = (char) Serial.read();
    fsm(input);
  }
}
void forward(int leftSpeed, int rightSpeed){
  Serial.println("forward");
  
  digitalWrite(LEFT_A, HIGH);
  digitalWrite(RIGHT_A, HIGH);
  digitalWrite(LEFT_B, LOW);
  digitalWrite(RIGHT_B, LOW);
  analogWrite(EN_RIGHT, rightSpeed);
  analogWrite(EN_LEFT, leftSpeed);
}
void backward(int leftSpeed, int rightSpeed){
  Serial.println("backward");

  digitalWrite(LEFT_A, LOW);
  digitalWrite(LEFT_B, HIGH);
  digitalWrite(RIGHT_A, LOW);
  digitalWrite(RIGHT_B, HIGH);
  analogWrite(EN_RIGHT, rightSpeed);
  analogWrite(EN_LEFT, leftSpeed);
}
//原地转向
void turnRight(int leftSpeed, int rightSpeed){
  //左边正向，右边反向
  digitalWrite(LEFT_A, HIGH);
  digitalWrite(LEFT_B, LOW);
  digitalWrite(RIGHT_A, LOW);
  digitalWrite(RIGHT_B, HIGH);
  analogWrite(EN_LEFT, leftSpeed);
  analogWrite(EN_RIGHT, rightSpeed);
}
void turnLeft(int leftSpeed, int rightSpeed){
  //右边正向，左边反向
  digitalWrite(LEFT_A, LOW);
  digitalWrite(LEFT_B, HIGH);
  digitalWrite(RIGHT_A, HIGH);
  digitalWrite(RIGHT_B, LOW);
  analogWrite(EN_LEFT, leftSpeed);
  analogWrite(EN_RIGHT, rightSpeed);
}
void brake(){
  Serial.println("brake");
  digitalWrite(LEFT_A, LOW);
  digitalWrite(LEFT_B, LOW);
  digitalWrite(RIGHT_A, LOW);
  digitalWrite(RIGHT_B, LOW);
  analogWrite(EN_RIGHT, 0);
  analogWrite(EN_LEFT, 0);
}


char command;
String param1;
String param2;
/* state 为指令解析状态机状态
* 0-准备读入命令 f|b|s|r|l -> 1,other -> 5
* 1-准备读入分隔符 "," -> 2 ";" -> 4, other -> 5
* 2-准备读入第一个参数 0-9 -> 2, "," -> 3, other -> 
* 3-准备读入第二个参数 0-9 -> 3, ";" -> 4
* 4-指令读取完成，执行命令然后复位状态
* 5-错误状态，等待复位 ; -> 0
 */
void fsm(char input){
  switch(state){
    case 0:
      param1 = "";
      param2 = "";
      //f - forward, 前进
      //b - backward, 后退
      //s - stop, 停止
      if(input == 'f' || input =='b' || input == 's' 
          || input == 'r' || input == 'l'){
        command = input;
        state = 1;
        Serial.println("state -> 1");
      }else if(input == ';'){
        ;
      }else{
        state = 5;
        Serial.println("state -> 5");
      }
      break;
    case 1:
      if(input == ','){
        state = 2;
        Serial.println("state -> 2");
      }else if(input == ';'){
        executeCommand();
        state = 0;
        Serial.println("state -> 0");
      }else{
        state = 5;
        Serial.println("state -> 5");
      }
      break;
    case 2:
      if(input >= '0' && input <= '9'){
        param1 += input;
      }else if(input == ','){
        state = 3;
        Serial.println("state -> 3");
      }else{
        state = 5;
        Serial.println("state -> 5");
      }
      break;
    case 3:
      if(input >= '0' && input <= '9'){
        param2 += input;
      }else if(input == ';'){
        executeCommand();
        state = 0;
        Serial.println("state -> 0");
      }else{
        state = 5;
        Serial.println("state -> 5");
      }
      break;
    case 5:
      if(input == ';'){
        state = 0;
        Serial.println("state -> 0");
      }
      break;
    default:
      break;
  }
}
void executeCommand(){
  Serial.println("execute");
  int leftSpeed, rightSpeed;
  if(param1.equals("")){
    leftSpeed = 255;
  }else{
    leftSpeed = param1.toInt();
  }
  if(param2.equals("")){
    rightSpeed = 255;
  }else{
    rightSpeed = param2.toInt();
  }
  switch(command){
    case 'f':
      forward(leftSpeed, rightSpeed);
      break;
    case 'b':
      backward(leftSpeed, rightSpeed);
      break;
    case 's':
      brake();
      break;
    case 'r':
      turnRight(leftSpeed, rightSpeed);
      break;
    case 'l':
      turnLeft(leftSpeed, rightSpeed);
      break;
  }
}
