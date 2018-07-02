#include <Servo.h>     //서보모터 제어함수용 헤더

#define MOTOR_A_a 3     //모터A의 +출력핀은 3번핀입니다
#define MOTOR_A_b 11    //모터A의 -출력핀은 11번핀입니다
#define MOTOR_B_a 5     //모터B의 +출력핀은 5번핀입니다
#define MOTOR_B_b 6     //모터B의 -출력핀은 6번핀입니다
#define MOTOR_SPEED 200 //모터의 기준속력입니다(0~255) 기준속력 변경시 제자리 회전 시간 변경이 필요합니다.
#define ULTRASENS_T 13  //초음파센서 트리거 출력핀은 13번핀입니다.
#define ULTRASENS_E 12  //초음파센서 에코 입력핀은 12번핀입니다.
#define SERVO_PIN 9     //서보모터 제어용 출력핀은 9번핀 입니다.
#define 90TURN  360     //제자리 90도 회전 Delay

Servo servo;           //서보 제어용 변수 선언

unsigned char m_a_spd = 0, m_b_spd = 0; //모터의 속력을 저장하는 전역변수
boolean m_a_dir = 0, m_b_dir = 0; //모터의 방향을 결정하는 전역변수

void setup()  
{
  //모터 제어 핀들을 출력으로 설정합니다.
  pinMode(MOTOR_A_a, OUTPUT);
  pinMode(MOTOR_A_b, OUTPUT);
  pinMode(MOTOR_B_a, OUTPUT);
  pinMode(MOTOR_B_b, OUTPUT);

  servo.attach(SERVO_PIN);      //서보 제어핀 선언

  pinMode(ULTRASENS_T, OUTPUT);   //초음파센서 트리거핀을 출력모드로 설정합니다.
  pinMode(ULTRASENS_E, INPUT);    //초음파센서 에코핀을 입력모드로 설정합니다.
  digitalWrite(ULTRASENS_T, LOW); //초음파센서 트리거핀 LOW로 초기화

  servo_drive(90);              //서보모터 초기 각도를 90도로 설정합니다.
  
  Serial.begin(9600); //시리얼 통신 초기화
  Serial.println("Hello!"); //터미널 작동 확인용 문자열
  delay(1000);        //1초간 작동대기
}

void loop()
{
  dodgewall_val();  //입력된 데이터에 따라 모터에 입력될 변수를 조정하는 함수
  motor_drive();  //모터를 구동하는 함수
}

void dodgewall_val() //입력된 데이터에 따라 모터에 입력될 변수를 조정하는 함수
{
  long distance=0, distance_r=0, distance_l=0;  //전방, 좌/우의 거리 변수 선언
  distance = howclose();  //전방 거리측정
  if(distance > 60)       //전방 거리가 60cm 초과일때 빠르게 전진
  {
    m_a_dir = 0;  //모터A 정방향
    m_b_dir = 0;  //모터B 정방향
    m_a_spd = constrain(MOTOR_SPEED+50, 0, 255);  //모터A의 속력값 빠르게 조정
    m_b_spd = constrain(MOTOR_SPEED+50, 0, 255);  //모터B의 속력값 빠르게 조정
  }
  else if(distance > 30)  //전방 거리가 30cm 초과일때 보통 속도로 전진
  {
    m_a_dir = 0;  //모터A 정방향
    m_b_dir = 0;  //모터B 정방향
    m_a_spd = MOTOR_SPEED;  //모터A의 속력값 조정
    m_b_spd = MOTOR_SPEED;  //모터B의 속력값 조정    
  }
  else  //전방 거리가 30cm 이하일때 장애물 회피
  {
    m_a_dir = 0;      //모터A 정방향
    m_b_dir = 0;      //모터B 정방향
    m_a_spd = 0;      //모터A 정지
    m_b_spd = 0;      //모터B 정지
    motor_drive();    //모터 정지상태 반영
    servo_drive(180); //서보모터를 왼쪽으로 회전
    delay(500);       //서보 회전시간 대기
    distance_l = howclose();  //왼쪽 거리측정
    servo_drive(0);   //서보모터를 오른쪽으로 회전
    delay(1000);      //서보 회전시간 대기
    distance_r = howclose();  //오른쪽 거리측정
    servo_drive(90);  //서보모터를 전방으로 회전
    delay(500);       //서보 회전시간 대기
    if(distance_r < 20 && distance_l < 20) //좌우 거리가 20cm미만일 경우 우회전으로 뒤로 돌기
    {
      m_a_dir = 0;    //모터A 정방향
      m_b_dir = 1;    //모터B 역방향
      m_a_spd = MOTOR_SPEED;  //모터A의 속력값 조정
      m_b_spd = MOTOR_SPEED;  //모터B의 속력값 조정
      motor_drive();    //모터 회전상태 반영
      delay(90TURN*2);  //뒤로 도는 시간 
    }
    else if(distance_r >= distance_l)  //오른쪽 거리가 왼쪽 거리 이상일 때 우회전
    {
      m_a_dir = 0;    //모터A 정방향
      m_b_dir = 1;    //모터B 역방향
      m_a_spd = MOTOR_SPEED;  //모터A의 속력값 조정
      m_b_spd = MOTOR_SPEED;  //모터B의 속력값 조정
      motor_drive();  //모터 회전상태 반영
      delay(90TURN);  //90도 우회전 시간
    }
    else  //왼쪽 거리가 오른쪽 거리 초과일 때 좌회전
    {
      m_a_dir = 1;  //모터A 역방향
      m_b_dir = 0;  //모터B 정방향
      m_a_spd = MOTOR_SPEED;  //모터A의 속력값 조정
      m_b_spd = MOTOR_SPEED;  //모터B의 속력값 조정
      motor_drive();  //모터 회전상태 반영
      delay(90TURN);  //90도 좌회전 시간
    }
    m_a_dir = 0;    //모터A 역방향
    m_b_dir = 0;    //모터B 정방향
    m_a_spd = 0;    //모터A의 속력값 조정
    m_b_spd = 0;    //모터B의 속력값 조정
    motor_drive();  //모터 정지상태 반영
    delay(1000);    //정지 상태로 잠시 대기
  }
}

long howclose() //초음파 센서 거리측정 함수
{
  long duration=0, distance=0;      //echo펄스 도달시간 변수와 거리 변수 선언
  digitalWrite(ULTRASENS_T, HIGH);  //Trig 핀 HIGH로 변경하여 신호 발생
  delayMicroseconds(10);            //10us 유지
  digitalWrite(ULTRASENS_T, LOW);   //Trig 핀 LOW로 초기화
  duration = pulseIn(ULTRASENS_E, HIGH);  //pulseIn함수가 호출되고 펄스가 입력될 때까지의 시간. us단위로 값을 리턴.
  distance = duration/29/2;         //센치미터로 환산
  Serial.print(distance);          //시리얼모니터에 거리값 출력
  Serial.print("cm");              //단위 출력
  Serial.println();                //줄바꿈
  return distance;                  //측정 거리값 리턴
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

void servo_drive(unsigned char deg) //서보모터를 구동하는 함수
{
  deg = constrain(deg, 0, 180); //구동범위를 0도에서 180도로 제한
  servo.write(deg);             //제한된 범위로 서보모터 구동
}
