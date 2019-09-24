#include <TimerOne.h>

#define timer 10000//intervalo de tempo da interrupção interna (em micro segundos)

#define encoder_a 3//pino de interrupção 1 
#define encoder_b 11//

#define enable_1 9
#define in1 8
#define in2 7

#define s1 A4

float ratio = 6*51.45;

float input_m1, output_m1;
float last_output_m1;

float kp = 0.97/1;//0.97   
float ki = 2085012.05/1000000;//0.338855
float kd = -0.00615/1;  

float current_error_m1 = 0, last_error_m1 = 0, last_last_error_m1 = 0;

volatile long int pulses = 0;
volatile long int last_pulses = 0;

float setpoint_m1 = 0;

float position_m1 = 0;

int pwm_out1;

int state_enc_m1;
int last_state_enc_m1;

long a;

void setup(){
  pinMode(s1,INPUT_PULLUP);
  pinMode(enable_1,OUTPUT);
  pinMode(in1,OUTPUT);
  pinMode(encoder_a,INPUT);
  pinMode(encoder_b,INPUT);
  
  Serial.begin(38400);
  
  Timer1.initialize(timer);//cria a interrupção interna e define o intervalo o qual ela ocorre
  Timer1.attachInterrupt(calculate);//cria a função para interrupção de tempo
  attachInterrupt(1, pulses_count, CHANGE);//cria a interrupção externa do encoder direito
  
  a = millis();
}

void loop()
{
    if(millis() - a >= 4000) {
      setpoint_m1 = 1;
    }
    
    Serial.println("---");
    Serial.print("SP: ");
    Serial.println(setpoint_m1);
    Serial.print("V1: ");
    Serial.println(position_m1);
    Serial.print("T1: ");
    Serial.println(output_m1);
    Serial.println("---");
}

void calculate()
{
    pos();
    control();
    motor();
}

void pos()
{
    int pc = pulses - last_pulses;
    position_m1 += ((2.0*PI*pc)/ratio);
    last_pulses = pulses;
}

void control()
{
  //if(velocity_m1<0) input_m1 = -velocity_m1;
  //else input_m1 = velocity_m1;
  input_m1 = position_m1;

  //if(setpoint_m1 - input_m1 < 0) current_error_m1 = -(setpoint_m1 - input_m1);
  //else current_error_m1 = setpoint_m1 - input_m1;
  current_error_m1 = setpoint_m1 - input_m1;
  
  output_m1 = last_output_m1 + (kp + kd/0.01)*current_error_m1 - (kp - ki*0.01 + (2*kd)/0.01)*last_error_m1 + (kd/0.01)*last_last_error_m1;
  
  pwm_out1 = (output_m1*255)/6.0;
  //pwm_out1 = 125;
  last_output_m1 = output_m1;

  last_last_error_m1 = last_error_m1;

  last_error_m1 = current_error_m1;
}

void motor()
{
    if(pwm_out1 >= 0){
        analogWrite(enable_1, pwm_out1);
        digitalWrite(in1, HIGH);
        digitalWrite(in2, LOW);
    }
    else{
        analogWrite(enable_1, -pwm_out1);
        digitalWrite(in1, LOW);
        digitalWrite(in2, HIGH);
    }
}

void pulses_count()
{
    state_enc_m1=(digitalRead(encoder_a)<<1) | digitalRead(encoder_b);
    if(state_enc_m1==0 || state_enc_m1==3) pulses--;
    else if (state_enc_m1==1 || state_enc_m1==2) pulses++;
    last_state_enc_m1 = state_enc_m1;
}
