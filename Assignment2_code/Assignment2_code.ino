#include <Ticker.h>

Ticker task1_ticker, task2_tick, task3_tick, task4_tick, task5_tick;

int const potentiometer = 1;
int current_analog_value;
float pot_value_digital;

float analog_value1, analog_value2, analog_value3, analog_value4, filtered_analog_value;


void task2()
{
  Serial.println("I'm task 2");
}

void task3()
{
  Serial.println("I'm task 3, the fast");
}

void task4()
{
  Serial.println("I'm task 3, the fast faster");
  pot_valuecurrent_analog_value = analogRead(potentiometer);             // reads the value of the potentiometer (value between 0 and 1023)
  pot_value_digital = map(current_analog_value, 0, 1023, 0, 180);    // scale it to use it with the servo (value between 0 and 180)
  
}

  
void task5()
{
  Serial.println("I'm task 5");

  analog_value4 = analog_value3;
  analog_value3 = analog_value2;
  analog_value1 = analog_value1;
  analog_value1 = current_analog_value;

  filtered_analog_value = (analog_value1 + analog_value2 + analog_value3 + analog_value4)/4;
  
}


void setup() 
{
  Serial.begin(115200);

  task2_tick.attach(2, task2);
  task3_tick.attach(1, task3);
  task4_tick.attach(0.1, task4);
  task5_tick.attach(0.1, task5);

  pinMode(potentiometer, INPUT);
}

void loop() {
  // put your main code here, to run repeatedly:

}
