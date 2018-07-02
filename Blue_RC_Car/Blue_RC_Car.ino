#define MOTOR_A_a 3     //모터A의 +출력핀은 3번핀입니다
#define MOTOR_A_b 11    //모터A의 -출력핀은 11번핀입니다
#define MOTOR_B_a 5     //모터B의 +출력핀은 5번핀입니다
#define MOTOR_B_b 6     //모터B의 -출력핀은 6번핀입니다
#define MOTOR_SPEED 150 //모터의 기준속력입니다(0~255)

unsigned char m_a_spd = 0, m_b_spd = 0; //모터의 속력을 저장하는 전역변수
boolean m_a_dir = 0, m_b_dir = 0;       //모터의 방향을 결정하는 전역변수

void setup()
{
  //모터 제어 핀들을 출력으로 설정합니다.
  pinMode(MOTOR_A_a, OUTPUT);
  pinMode(MOTOR_A_b, OUTPUT);
  pinMode(MOTOR_B_a, OUTPUT);
  pinMode(MOTOR_B_b, OUTPUT);

  Serial.begin(9600);       //시리얼 통신 초기화
  Serial.println("Hello!"); //터미널 작동 확인용 문자열
}

void loop()
{
  unsigned char bt_cmd = 0;   //명령어 저장용 문자형 변수

  if (Serial.available())   //데이터가 입력되었을 때
  {
    bt_cmd = Serial.read(); //변수에 입력된 데이터 저장
    rc_ctrl_val(bt_cmd);     //입력된 데이터에 따라 모터에 입력될 변수를 조정하는 함수
  }
  motor_drive();             //모터를 구동하는 함수
}

void rc_ctrl_val(unsigned char cmd) //입력된 데이터에 따라 모터에 입력될 변수를 조정하는 함수
{
  if(cmd == 'w')  //'w'가 입력되었을 때, 전진
  {
    m_a_dir = 0;  //모터A 정방향
    m_b_dir = 0;  //모터B 정방향
    m_a_spd = MOTOR_SPEED;  //모터A의 속력값 조정
    m_b_spd = MOTOR_SPEED;  //모터B의 속력값 조정
  }
  else if(cmd == 'a')  //'a'가 입력되었을 때, 제자리 좌회전
  {
    m_a_dir = 1;  //모터A 역방향
    m_b_dir = 0;  //모터B 정방향
    m_a_spd = MOTOR_SPEED;  //모터A의 속력값 조정
    m_b_spd = MOTOR_SPEED;  //모터B의 속력값 조정
  }
  else if(cmd == 'd')  //'d'가 입력되었을 때, 제자리 우회전
  {
    m_a_dir = 0;  //모터A 정방향
    m_b_dir = 1;  //모터B 역방향
    m_a_spd = MOTOR_SPEED;  //모터A의 속력값 조정
    m_b_spd = MOTOR_SPEED;  //모터B의 속력값 조정
  }
  else if(cmd == 's')  //'s'가 입력되었을 때, 후진
  {
    m_a_dir = 1;  //모터A 역방향
    m_b_dir = 1;  //모터B 역방향
    m_a_spd = MOTOR_SPEED;  //모터A의 속력값 조정
    m_b_spd = MOTOR_SPEED;  //모터B의 속력값 조정
  }
  else if(cmd == 'x')
  {
    m_a_dir = 0;  //모터A 정방향
    m_b_dir = 0;  //모터B 정방향
    m_a_spd = 0;  //모터A의 정지
    m_b_spd = 0;  //모터B의 정지
  }
}

void motor_drive()  //모터를 구동하는 함수
{
  if(m_a_dir == 0)
  {
    digitalWrite(MOTOR_A_a, LOW);     //모터A+ LOW
    analogWrite(MOTOR_A_b, m_a_spd);  //모터A-의 속력을 PWM 출력
  }
  else
  {
    analogWrite(MOTOR_A_a, m_a_spd);  //모터A+의 속력을 PWM 출력
    digitalWrite(MOTOR_A_b, LOW);     //모터A- LOW
  }
  if(m_b_dir == 1)
  {
    digitalWrite(MOTOR_B_a, LOW);     //모터B+ LOW
    analogWrite(MOTOR_B_b, m_b_spd);  //모터B-의 속력을 PWM 출력
  }
  else
  {
    analogWrite(MOTOR_B_a, m_b_spd);  //모터B+의 속력을 PWM 출력
    digitalWrite(MOTOR_B_b, LOW);     //모터B- LOW
  }
}
