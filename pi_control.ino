#include <TimerOne.h>

#define timer 10000//intervalo de tempo da interrupção interna (em micro segundos)

#define encoder_a 3//pino de interrupção 1 
#define encoder_b 1//

#define enable_1 9
#define in1 8
#define in2 7

#define s1 A4

double input_m1, output_m1;
double last_output_m1;

double kp = 0.091039;//0.091039   
double ki = 0.33885;//0.338855

double current_error_m1 = 0, last_error_m1 = 0;

volatile long long int pulses = 0;
volatile long long int last_pulses = 0;

double setpoint_m1 = 0;

double velocity_m1 = 0;

int pwm_out1;

int state_enc_m1;
int last_state_enc_m1;

void setup(){
  pinMode(s1,INPUT_PULLUP);
  pinMode(enable_1,OUTPUT);
  pinMode(in1,OUTPUT);
  pinMode(encoder_a,INPUT);
  pinMode(encoder_b,INPUT);
  
  Timer1.initialize(timer);//cria a interrupção interna e define o intervalo o qual ela ocorre
  Timer1.attachInterrupt(calculate);//cria a função para interrupção de tempo
  attachInterrupt(encoder_b, pulses_count, CHANGE);//cria a interrupção externa do encoder direito
}

void loop(){
}

void calculate()
{
    velocity();
    control();
    motor();
}

void velocity(){
    int pulses_count = pulses - last_pulses;
    velocitym1 = ((2*PI*pulses)/(ratio*(TIMER/1000000.0))));
    last_pulses = pulses;
}

void control(){
  if(velocity_m1<0) input_m1 = -velocity_m1;
  else input_m1 = velocity_m1;

  if(setpoint_m1 - input_m1 < 0) current_error_m1 = -(setpoint_m1 - input_m1);
  else current_error_m1 = setpoint_m1 - input_m1;
  
  output_m1 = last_output_m1 + kp*(current_error_m1) + (ki*(last_error_m1));

  pwm_out1 = (output_m1*255)/55;

  last_output_m1 = output_m1;

  last_error_m1 = current_error_m1;
}

void motor(){
    if(velocity_m1 >= 0.0){
        analogWrite(enable_1, pwm_out1);
        digitalWrite(in1, HIGH);
        digitalWrite(in2, LOW);
    }
    else{
        analogWrite(enable_1, pwm_out1);
        digitalWrite(in1, LOW);
        digitalWrite(in2, HIGH);
    }
}

void pulses_count(){
    state_enc_m1=(digitalRead(encoder_a)<<1) | digitalRead(encoder_b);
    if(state_enc_m1==0 || state_enc_m1==3) pulses++;
    else if (state_enc_m1==1 || state_enc_m1==2) pulses--;
    last_state_enc_m1 = state_enc_m1;
}
