#include <Ticker.h>

Ticker task2_tick;
Ticker task3_tick;

void task2()
{
  Serial.println("I'm task 2");
}

void task3()
{
  Serial.println("I'm task 3, the fast");
}

void setup() 
{
  Serial.begin(115200);

  task2_tick.attach(2, task2);
  task3_tick.attach(1, task3);
}

void loop() {
  // put your main code here, to run repeatedly:

}
