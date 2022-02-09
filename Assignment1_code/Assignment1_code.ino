// Initialise constant, the number represents the pin where the wires and conected to the microcrontroler. The "const" keyword is added because we do not want these values to be changed.
const int LED1 = 14;
const int LED2 = 27;
const int Switch1 = 35;
const int Switch2 = 34;

void signal_A(int a, int b, int c, int d)
{
  int delay_value = 50;//*10000;
  digitalWrite(LED2, HIGH); //send a signal for 50µs
  delayMicroseconds(delay_value);
  digitalWrite(LED2, LOW);  //turn it low
  int i;
  for(i=0; i<c; i++)  // 
  {
    digitalWrite(LED1, HIGH); //turn on LED1 for a µs
    delayMicroseconds(a);                 
    
    digitalWrite(LED1, LOW);  //turn off LED1 for b µs
    delayMicroseconds(b); 
    
    a = a + 50;            //Increment a variables
  }
  delayMicroseconds(d);                   //do a last wait
}

void signal_mod2(int a, int b, int c, int d)
{//change a 
  int delay_value = 50;//*10000;
  digitalWrite(LED2, HIGH); //send a signal for 50µs
  delayMicroseconds(delay_value);
  digitalWrite(LED2, LOW);  //turn it low
  int i;
  for(i=0; i<c; i++)
  {
    digitalWrite(LED1, HIGH);//turn on LED1 for a µs
    delayMicroseconds(a);
    digitalWrite(LED1, LOW);//turn off LED1 for b µs
    delayMicroseconds(b); 
    a = a - 50;          //change variable
  }
  delayMicroseconds(d);
}
void signal_B()
{
  int delay_value = 50;//*10000;
  digitalWrite(LED2, HIGH); //send a signal for 50µs
  delayMicroseconds(delay_value);
  digitalWrite(LED2, LOW);  //turn it low
}




void setup() //set the input and output pins
{
  pinMode(Switch1, INPUT);  //set ... as input
  pinMode(Switch2, INPUT);
  
  pinMode(LED1, OUTPUT);    //set ... as output
  pinMode(LED2, OUTPUT);
  
}

void loop() 
{
  int Switchstate1 = digitalRead(Switch1);    //create varaible to make it easier to read
  int Switchstate2 = digitalRead(Switch2);

  int a, b, c, d;
  a = 1000 ;//* 1000 000;
  b = 100 ;//* 1000 000;
  c = 17;
  d = 6500 ;//* 1000 000 ;
  
  if(Switchstate2 == LOW)                    //if the first switch is on
  {
    //signal_B();                               //send the first signal
    if(Switchstate1 == LOW)                  //check if siwtch 2 is hight
    {
      //signal_B();  
      signal_mod2(a, b, c, d);                //if yes, send the second mod
    }
    else                                      //otherwise send the classic signal
    {
      //signal_B();  
      signal_A(a, b, c, d);
    }
  }
  else                                        //if not, turn everything off
  {
    digitalWrite(LED1, LOW);
    digitalWrite(LED2, LOW);
  }
}
