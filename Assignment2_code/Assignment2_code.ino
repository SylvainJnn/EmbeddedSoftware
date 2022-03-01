#include <Ticker.h>
// =======================================
// =========== Initialisation ============
// =======================================


Ticker task1_ticker, task2_tick, task3_tick, task4_tick, task5_tick, task6_tick, task7_tick, task8_tick, task9_tick;

Ticker myTicker;
int counter =0;

// ============= task 2 =============
int const task2_input = 2;    //pin where the task2 button is connected

int task2_state;             //state of the button is task2
// ============= task 3 =============
int const task3_input = 3;

unsigned long current_time = micros(); // init time
unsigned long previous_time = micros(); 
float signal_frequency = 0;
// ============= task 4 & 5 =============
int const potentiometer = 1;  //pin where the potentiometer is connected
int current_analog_value;
float pot_value_digital;

//initliase the analog_value variables to avoid bug at the begining
float analog_value1 = 0;
float analog_value2 = 0; 
float analog_value3 = 0;
float analog_value4 = 0;
float average_analogue_in = 0;

// ============= task 7 and 8 =============
int error_code = 0;   //variable use in task 7 and 8, we set this value to avoid any issue at the begining

int const task8_output = 8; //set pin used for task 8



// ====================================
// ============= Function =============
// ====================================




// ============= task 2 =============
// Check the sate of the input button "task2_input"
void task2()
{
  Serial.println("I'm task 2");
  task2_state = digitalRead(task2_input);
}


// ============= task 3 =============
// no clue bro
void ReadFrequency()
{
  previous_time = current_time;
  current_time = micros();
}


void task3()
{
  Serial.println("I'm task 3, the fast");
  signal_frequency = 1/(current_time - previous_time);
}


// ============= task 4 =============
//Read the potentiometer
void task4()
{
  Serial.println("I'm task 4, need five seconds ");
  current_analog_value = analogRead(potentiometer);             // reads the value of the potentiometer (value between 0 and 1023)
  pot_value_digital = map(current_analog_value, 0, 1023, 0, 180);    // scale it to use it with the servo (value between 0 and 180)
  
}

// ============= task 5 =============
// Do the average of the last 4 analog inputs  
void task5()
{
  Serial.println("I'm task 5");

  analog_value4 = analog_value3;
  analog_value3 = analog_value2;
  analog_value1 = analog_value1;
  analog_value1 = current_analog_value;

  average_analogue_in = (analog_value1 + analog_value2 + analog_value3 + analog_value4)/4;
  
}

// ============= task 6 =============
// launch 1000 times 1 instruction
void task6()
{ 
 int i;
 for(i = 0; i < 1000; i++)
 {
    __asm__ __volatile__ ("nop");
 }
}
// ============= task 7 =============
// Change the state fo "error_code" based on the input of the potentiometer of task 4
void task7()
{
  float half_of_maximum_range = 3.3/2;
  if(average_analogue_in > half_of_maximum_range)
  {
    error_code = 1;
  }
  else
  {
    error_code = 0;
  }
}

// ============= task 8 =============
// Turn on or off an LED by reading "error_code"
void task8()
{
  if(error_code == 1)// check the state of error_code
  {
    digitalWrite(task8_output, HIGH);// turn on the LED
  }
  else
  {
    digitalWrite(task8_output, LOW); // turn on the LED
  }
}

// ============= task 9 =============
// print : task 2 button's state, task 3 signals' frequency, task5 analog input average 
void task9()
{
  Serial.print("Task 2 switch's state :");  
  Serial.print(task2_state);
  Serial.print(", ");

  Serial.print("Task 3 Frequency :");
  Serial.print(signal_frequency);
  Serial.print(", ");

  Serial.print("Task 5 analog input average");
  Serial.print(average_analogue_in);
  Serial.print("\n");
}


void my_function()
{

  if(counter % 2 == 0)
  {
    task2();
  }

  if(counter % 1 == 0)
  {
    task3();
  }
  
  if(counter % 5 == 0)
  {
    task4();
  }
  
  counter++;
}


// =====================================
// =========== Setup & loop ============
// =====================================
void setup() 
{
  Serial.begin(115200); 

  //task2_tick.attach(2, task2);
  //task3_tick.attach(1, task3);
  /*task4_tick.attach(0.1, task4);
  task5_tick.attach(0.1, task5);
  task6_tick.attach(0.1, task6);
  task7_tick.attach(0.1, task7);
  task8_tick.attach(0.1, task8);
  task9_tick.attach(0.1, task9);*/
  myTicker.attach(1, my_function);


  pinMode(task2_input, INPUT);

  attachInterrupt(digitalPinToInterrupt(task3_input), ReadFrequency, RISING);

  pinMode(potentiometer, INPUT);

  pinMode(task8_output, OUTPUT);
}

void loop() 
{

    
  
}
