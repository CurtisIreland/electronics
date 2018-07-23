int pd=2;
int act_led=13;
int senRead=0;
//int limit=850;
int limit=995;

void setup()    
{  
  pinMode(pd,OUTPUT);  
  pinMode(act_led,OUTPUT);  

  digitalWrite(pd,HIGH);
  digitalWrite(act_led,LOW);
  
  Serial.begin(9600);
}  

void loop()  
{  
  int val=analogRead(senRead);
  Serial.println(val);

  if(val <= limit)              
  {  
    digitalWrite(act_led,HIGH); 
    delay(20);  
  }  
  else if(val > limit)
  {  
    digitalWrite(act_led,LOW);
    delay(20);  
  }  
}  
