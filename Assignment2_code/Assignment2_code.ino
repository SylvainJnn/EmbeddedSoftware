#include <Ticker.h>


/* Prototypes of the functions */
Ticker myTicker;
void task1();//prototype of the tasks
void task2();
void task3();
void task4();
void task5();
void task6();
void task7();
void task8();
void task9();
//Period of each tasks
const int TimeTask1 = 9; // time to wait signal B
const int TimeTask2 = 200;
const int TimeTask3 = 1000;
const int TimeTask4 = 42;
const int TimeTask5 = 42;
const int TimeTask6 = 100;
const int TimeTask7 = 333;
const int TimeTask8 = 333;
const int TimeTask9 = 5000;

int Counter = 0;  // Counter use to know wich task launch or not


// ============= task 1 =============
int const task1_input = 12;    //task 1 output
 

// ============= task 2 =============
int const task2_input = 14;    //pin where the task2 button is connected

int task2_state;             //state of the button is task2
// ============= task 3 =============
int const task3_input = 27;

unsigned long current_time = 1; // init time
unsigned long previous_time = 0;
unsigned long period = 1;
float signal_frequency = 0;
// ============= task 4 & 5 =============
int const task4_input_analog = 26;  //pin where the potentiometer is connected
int current_analog_value;
float pot_value_digital;

float task_period = 1;

//initliase the analog_value variables to avoid bug at the begining
float analog_value1 = 0;
float analog_value2 = 0;
float analog_value3 = 0;
float analog_value4 = 0;
float average_analogue_in = 0;

// ============= task 7 and 8 =============
int error_code = 0;   //variable use in task 7 and 8, we set this value to avoid any issue at the begining

int const task8_output = 25; //set pin used for task 8

// === Variable use to check the frenquency of the tasks ===
unsigned long t1_task4 = 0;//task 4 check time
unsigned long t2_task4 = micros();



// ====================================
// ============= Function =============
// ====================================
long check_time(long t1, long t2)//Function that returns the period between two activations of a task.
{
    unsigned long  frequency_signal;
    unsigned long period_signal;
    period_signal = t2-t1;
    frequency_signal = (unsigned long)((1000000)/ period_signal);
    return(period_signal);
    
}


// ============= task 1 =============
// Check the sate of the input button "task2_input"


void task1()
{
  digitalWrite(task1_input, HIGH);
  delayMicroseconds(50); //wait 50 µS
  digitalWrite(task1_input, LOW);
}



// ============= task 2 =============
// Check the sate of the input button "task2_input"
void task2()
{
  task2_state = digitalRead(task2_input);
}


// ============= task 3 =============
// compute the frequency of an input signal 
void ReadFrequency()// To compute the period of the input signal we set the previous time with the current_time variable and then read the current time with micros(). this function is called every rising edge
{
  previous_time = current_time;                   //set the previous time
  current_time = micros();                        //Read the current time
  period = (current_time - previous_time);        //Calcultae the period
}


void task3()
{
  signal_frequency = (float)((1000000/ period));//the period is in µs -> we passe the frenquency in Hz, this variable comes from the function "ReadFrequency"
}


// ============= task 4 =============
//Read the potentiometer
void task4()
{
  current_analog_value = analogRead(task4_input_analog);             // reads the value of the potentiometer (value between 0 and 1023)
  
  t1_task4 = t2_task4;
  t2_task4 = micros();
  task_period = check_time(t1_task4, t2_task4);
}

// ============= task 5 =============
// Do the average of the last 4 analog inputs
void task5()
{
  analog_value4 = analog_value3;                          //update variables   
  analog_value3 = analog_value2;
  analog_value2 = analog_value1;
  analog_value1 = current_analog_value;                   //Set the current analogue value

  average_analogue_in = (analog_value1 + analog_value2 + analog_value3 + analog_value4) / 4;  //Do the average
}

// ============= task 6 =============
// launch 1000 times 1 instruction
void task6()
{
  int i;
  for (i = 0; i < 1000; i++)
  {
    __asm__ __volatile__ ("nop");
  }
}
// ============= task 7 =============
// Change the state fo "error_code" based on the input of the potentiometer of task 4
void task7()
{
  float half_of_maximum_range = 3.3 / 2;
  float half_of_maximum_range_digital = 2048; // =2^^12/2, number of bit for the ADC divided by two.
  if(average_analogue_in > half_of_maximum_range_digital)
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
  if(error_code == 1) // check the state of error_code
  {
    digitalWrite(task8_output, HIGH);// turn on the LED
  }
  else
  {
    digitalWrite(task8_output, LOW); // turn off the LED
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

  Serial.print("Task 5 analog input average :");
  
  //float analogue_in_print =  (average_analogue_in*3,3)/4096; 
  
  Serial.print(average_analogue_in);
  Serial.print("\n");
  Serial.println(error_code);
  Serial.println(task_period);
}


void my_function()//My cycle executive, it actualises very 1ms, to check if the the timing for one task, we do the modulo between the counter the its period-> if the rest is 0 it is the good timing
{
  Counter++;
  
  if(Counter % TimeTask1 == 0) task1();
  if(Counter % TimeTask2 == 0) task2();
  if(Counter % TimeTask3 == 0) task3();
  if(Counter % TimeTask4 == 0) task4();
  if(Counter % TimeTask5 == 0) task5();
  if(Counter % TimeTask6 == 0) task6();
  if(Counter % TimeTask7 == 0) task7();
  if(Counter % TimeTask8 == 0) task8();
  if(Counter % TimeTask9 == 0) task9();
}


/* ==================================== */
/*    Definition of INPUT and OUTPUT    */
/* ==================================== */

void setup() {
  Serial.begin(115200);

  pinMode(task1_input, OUTPUT); 
  pinMode(task2_input, INPUT);
  attachInterrupt(digitalPinToInterrupt(task3_input), ReadFrequency, RISING);
  pinMode(task4_input_analog, INPUT);
  pinMode(task8_output, OUTPUT);

  myTicker.attach_ms(1, my_function);
}

/* ====================================== */
/*    Call and Scheduling of the tasks    */
/* ====================================== */

void loop() 
{
  
}
