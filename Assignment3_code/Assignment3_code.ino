
// define two tasks for Blink & AnalogRead
void Task00( void *pvParameters );
void Task01( void *pvParameters );

void task1(void *pvParameters);//prototype of the tasks
void task2(void *pvParameters);
void task3(void *pvParameters);
void task4(void *pvParameters);
void task5(void *pvParameters);
void task6(void *pvParameters);
void task7(void *pvParameters);
void task8(void *pvParameters);
void task9(void *pvParameters);

//Period of each tasks
const int TimeTask1 = 9; 
const int TimeTask2 = 200; 
const int TimeTask3 = 1000;
const int TimeTask4 = 42;
const int TimeTask5 = 42;
const int TimeTask6 = 100;
const int TimeTask7 = 333;
const int TimeTask8 = 333;
const int TimeTask9 = 5000;
const int TimeTask10 = 1000;

SemaphoreHandle_t mySemaphore;
QueueHandle_t myQueue;

typedef struct my_struct my_struct;
struct my_struct
{
  bool state;
  float frequency;
  float average; 
};

my_struct la_struct;

// ============= task 1 =============
int const task1_output = 12;    //task 1 output
 

// ============= task 2 =============
int const task2_input = 14;    //pin where the task2 button is connected

int task2_state;             //state of the button is task2
// ============= task 3 =============
int const task3_input = 34;

unsigned long current_time = 1; // init time
unsigned long previous_time = 0;
unsigned long period = 1;
float signal_frequency = 0;

unsigned long actualTime = micros();
unsigned long passedTime = micros();

// ============= task 4 & 5 =============
int const task4_input_analog = 26;  //pin where the potentiometer is connected
        
float pot_value_digital;

float task_period = 1;              // time between 2 call of the task

//initliase the analog_value variables to avoid bug at the begining

float average_analogue_in = 0;

// ============= task 7 and 8 =============
int error_code = 0;   //variable use in task 7 and 8, we set this value to avoid any issue at the begining

int const task8_output = 25; //set pin used for task 8

// === Variable use to check the frenquency of the tasks ===
unsigned long t1_task4 = 0;//task 4 check time
unsigned long t2_task4 = micros();



/*--------------------------------------------------*/
/*---------------------- Tasks ---------------------*/
/*--------------------------------------------------*/

void Task00(void *pvParameters)  // This is a task.
{
  (void) pvParameters;
  for (;;) // A Task shall never return or exit.
  {
    
    Serial.println("\nTask 00 toutes les secondes\n");
    vTaskDelay(1000 );
  }
}

void Task01(void *pvParameters)  // This is a task.
{
  (void) pvParameters;
  // initialize serial communication at 9600 bits per second:
  //Serial.begin(9600);

  for (;;)
  {
    Serial.println("Task 01 toutes les deux secondes");
    vTaskDelay(2000);
  }
}

// ====================================
// ============= Function =============
// ====================================
long check_time(long t1, long t2)//Function that returns the period between two activations of a task.
{
    unsigned long  frequency_signal;  //Local variable that indicates the ...
    unsigned long period_signal;
    period_signal = t2-t1;
    frequency_signal = (unsigned long)((1000000)/ period_signal);
    return(period_signal);
    
}


// ============= task 1 =============
// Check the sate of the input button "task2_input"
void task1(void *pvParameters)
{
  (void) pvParameters;
  for (;;)
  {
    digitalWrite(task1_output, HIGH);
    delayMicroseconds(50); //wait 50 µS//or vTaskDelay(0.050);
    digitalWrite(task1_output, LOW);
    vTaskDelay(TimeTask1);
  }
}


// ============= task 2 =============
// Check the sate of the input button "task2_input"
void task2(void *pvParameters)
{
  (void) pvParameters;
  for(;;)
  {
      xSemaphoreTake(mySemaphore, portMAX_DELAY); // or     if ( xSemaphoreTake( xSerialSemaphore, ( TickType_t ) 5 ) == pdTRUE ), condition to check i don't know why and we can use tick instead of max_delay because i dunno what it is
      task2_state = digitalRead(task2_input);
      la_struct.state = task2_state; //digitalRead(task2_input); direcrectly 
      xSemaphoreGive(( mySemaphore)); 
      vTaskDelay(TimeTask2);

  }
}
// ============= task 3 =============
// compute the frequency of an input signal 
void ReadFrequency()// To compute the period of the input signal we set the previous time with the current_time variable and then read the current time with micros(). this function is called every rising edge
{
  /*
  previous_time = current_time;                   //set the previous time
  current_time = micros();                        //Read the current time
  period = check_time(previous_time, current_time);//Call the check_time function
  */
  passedTime = actualTime;
  actualTime = micros();
}

void task3(void *pvParameters)
{
  (void) pvParameters;
  for(;;)
  {
    
    signal_frequency = (1/((actualTime - passedTime)*0.000001)); //(float)((1000000/ period));//the period is in µs -> we passe the frenquency in Hz, this variable comes from the function "ReadFrequency"


    
    xSemaphoreTake(mySemaphore, portMAX_DELAY);
    la_struct.frequency = signal_frequency;
    xSemaphoreGive((mySemaphore)); 
    vTaskDelay(TimeTask3);
  }
}


// ============= task 4 =============
//Read the potentiometer
void task4(void *pvParameters)
{
  (void) pvParameters;
  float current_input_value;   
  for(;;)
  {
    current_input_value = analogRead(task4_input_analog);             // reads the value of the potentiometer (value between 0 and 1023)
    xQueueSend(myQueue, &current_input_value, portMAX_DELAY);
    /*
    if queue len = 4
      xQueueReceive(queue, 3, portMAX_DELAY);
    
      xQueueSend(queue, 0, portMAX_DELAY);//take off portMAX_DELAY? /xQueueSendToBack?

    and for task 5, either we can do read by doing 'queue[i]', either we copy the queue
      
    */
    //Variable use to controle the frequency of the task
    t1_task4 = t2_task4;
    t2_task4 = micros();
    task_period = check_time(t1_task4, t2_task4);
    
    vTaskDelay(TimeTask4);
  }
}

// ============= task 5 =============
// Do the average of the last 4 analog inputs
void task5(void *pvParameters)
{
  (void) pvParameters;
  float analog_value1 = 0;
  float analog_value2 = 0;
  float analog_value3 = 0;
  float analog_value4 = 0;
  for(;;)
  {
    
    analog_value4 = analog_value3;                          //update variables   
    analog_value3 = analog_value2;
    analog_value2 = analog_value1;
    if(xQueueReceive(myQueue, &analog_value1, portMAX_DELAY) != pdPASS) //errQUEUE_EMPTY  if not working
    {
      analog_value1 = analog_value2;                  //if the queue is not working we restore the previous data
      Serial.println("Queue failled");
    }
     
    average_analogue_in = (analog_value1 + analog_value2 + analog_value3 + analog_value4) / 4;  //Do the average
    //Serial.println(average_analogue_in);
    xSemaphoreTake(mySemaphore, portMAX_DELAY); 
    la_struct.average = average_analogue_in;
    xSemaphoreGive(mySemaphore);
    
    vTaskDelay(TimeTask5);
  }
}

// ============= task 6 =============
// launch 1000 times 1 instruction
void task6(void *pvParameters)
{
  (void) pvParameters;
  for(;;)
  {
    int i;
    for (i = 0; i < 1000; i++)
    {
      __asm__ __volatile__ ("nop");
    }
    vTaskDelay(TimeTask6);
  }
}
// ============= task 7 =============
// Change the state fo "error_code" based on the input of the potentiometer of task 4
void task7(void *pvParameters)
{
  (void) pvParameters;
  
  float average_value_received = 0;
  for(;;)
  {
    xSemaphoreTake(mySemaphore, portMAX_DELAY); 
    average_value_received = la_struct.average;
    xSemaphoreGive(mySemaphore);
    float half_of_maximum_range = 3.3 / 2;
    float half_of_maximum_range_digital = 2048; // =2^^12/2, number of bit for the ADC divided by two.
   
    if( average_value_received> half_of_maximum_range_digital)
      error_code = 1;
    else
      error_code = 0;
    
    vTaskDelay(TimeTask7);
  }
}

// ============= task 8 =============
// Turn on or off an LED by reading "error_code"
void task8(void *pvParameters)
{
  (void) pvParameters;
  for(;;)
  {
    if(error_code == 1) // check the state of error_code
      digitalWrite(task8_output, HIGH);// turn on the LED
    else
      digitalWrite(task8_output, LOW); // turn off the LED

    vTaskDelay(TimeTask8);
  }
}

// ============= task 9 =============
// print : task 2 button's state, task 3 signals' frequency, task5 analog input average

void task9(void *pvParameters)
{
    
  (void) pvParameters;
  for(;;)
  {
    
    //either this way, OR we don't use multiple global variable and use only one struct and we update these values in  each task (update struct values)

    xSemaphoreTake(mySemaphore, portMAX_DELAY); 


    if(la_struct.state == false)
    {
      Serial.print("Task 2 switch's state :");
      Serial.print(la_struct.state);
      Serial.print(", ");
  
      Serial.print("Task 3 Frequency :");
      Serial.print(la_struct.frequency);
      Serial.print(", ");
  
      Serial.print("Task 5 analog input average :");
      
      Serial.print(la_struct.average);
      Serial.print("\n");
    }
    xSemaphoreGive(mySemaphore);
    vTaskDelay(TimeTask9);
  }
}


void task10(void *pvParameters)
{
  (void) pvParameters;


  for(;;)
  {
    //for me we don't have 10 tasks but we need to add a condition on task 9
    vTaskDelay(TimeTask10);
  }
  
}

// the setup function runs once when you press reset or power the board
void setup() 
{

  Serial.begin(9600);
  myQueue = xQueueCreate( 1, sizeof( float ) );
  if (myQueue  == NULL) {
    Serial.println("Queue can not be created");
  }
  Serial.println("Queue created");
  if ( mySemaphore == NULL )  // Check to confirm that the Serial Semaphore has not already been created. // from https://create.arduino.cc/projecthub/feilipu/using-freertos-semaphores-in-arduino-ide-b3cd6c
    {
      mySemaphore = xSemaphoreCreateMutex();  // Create a mutex semaphore we will use to manage the Serial Port
      if ( ( mySemaphore ) != NULL )
        xSemaphoreGive(( mySemaphore));  // Make the Serial Port available for use, by "Giving" the Semaphore.
        Serial.println("Semaphore created");
    }
  
  // Now set up two tasks to run independently.

  pinMode(task1_output, OUTPUT); 
  pinMode(task2_input, INPUT);
  pinMode(task3_input, INPUT);
  attachInterrupt(digitalPinToInterrupt(task3_input), ReadFrequency, RISING);
  pinMode(task4_input_analog, INPUT);
  pinMode(task8_output, OUTPUT);

  
  xTaskCreate(
    task1
    ,  "1"   // A name just for humans
    ,  4096  // Stack size
    ,  NULL
    ,  1  // priority
    ,  NULL );

  xTaskCreate(
    task2
    ,  "2"   // A name just for humans
    ,  4096  // Stack size
    ,  NULL
    ,  2  // priority
    ,  NULL );

  xTaskCreate(
    task3
    ,  "3"   // A name just for humans
    ,  4096  // Stack size
    ,  NULL
    ,  2  // priority
    ,  NULL );

  xTaskCreate(
    task4
    ,  "4"   // A name just for humans
    ,  4096  // Stack size
    ,  NULL
    ,  2  // priority
    ,  NULL );

  xTaskCreate(
    task5
    ,  "5"   // A name just for humans
    ,  4096  // Stack size
    ,  NULL
    ,  2  // priority
    ,  NULL );

  xTaskCreate(
    task6
    ,  "6"   // A name just for humans
    ,  4096  // Stack size
    ,  NULL
    ,  2  // priority
    ,  NULL );

  xTaskCreate(
    task7
    ,  "7"   // A name just for humans
    ,  4096  // Stack size
    ,  NULL
    ,  2  // priority
    ,  NULL );

  xTaskCreate(
    task8
    ,  "8"   // A name just for humans
    ,  4096  // Stack size
    ,  NULL
    ,  2  // priority
    ,  NULL );

  xTaskCreate(
    task9
    ,  "9"   // A name just for humans
    ,  4096  // Stack size
    ,  NULL
    ,  2  // priority
    ,  NULL );



  // Now the task scheduler, which takes over control of scheduling individual tasks, is automatically started.
}

void loop()
{
  // Empty. Things are done in Tasks.
}
