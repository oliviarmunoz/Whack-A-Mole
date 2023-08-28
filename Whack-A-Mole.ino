int in1[] = {0,2,4,6};         // in1 pins for coilA+, coilB+, coilC+, coilD+
int in2[] = {1,3,5,7};         // in2 pins for coilA-, coilB-, coilC-, coilD-
int pb[4] = {A0,A1,A2,A3}; // input pins for HallA, HallB, HallC, HallD
int c[4];                      // challenge sequence
int r[4];                      // response sequence

void setup(){
  int i;
  Serial.begin(9600);
  for(i=0; i<4; i++){
    pinMode(in1[i],OUTPUT);   // set LED control pins as Teensy outputs
    pinMode(in2[i],OUTPUT);
  }
  analogReadResolution(8);    // set A/D for 8 bit resolution (values 0 to 255)
}

void led_red(int i){
  digitalWrite(in1[i],HIGH);
  digitalWrite(in2[i],LOW);
}

void led_green(int i){
  digitalWrite(in1[i],LOW);
  digitalWrite(in2[i],HIGH);
}

void led_off(int i){
  digitalWrite(in1[i],LOW);
  digitalWrite(in2[i],LOW);
}

void issue_challenge(int c[4]){
  int i;
  for(i=0; i<4; i++){
    Serial.println("i");
    // there are four trials in each challenge
    c[i] = random(0,4); // each trial = randomly selected position c[i]
    led_red(c[i]);    
    delay(200);
    led_off(c[i]);
    delay(200);
  }
}   // on return, c[] will contain the challenge

int get_one_button(){
 int i;
  while(1){
        Serial.println("while");
    for(i=0; i<4; i++){                    // loop over buttons
      if(analogRead(pb[i])<128){       // test if button i was pushed
                              // wait for 0.1 seconds
        while(1){
          led_red(i);
          Serial.println("i");
          // light LED to let player know
          delay(100);  
          if(analogRead(pb[i])>=128){
             break;
          }
        }
        led_off(i);                        // turn off LED
      delay(100);
      return i;
      }
    }
        // return the position of the pressed button
   }
 }


void get_response(int r[4]){
  int i;
  for(i=0; i<4; i++){
    r[i] = get_one_button();
  }
}   // on return, r[] will contain the response

void give_feedback(int c[4],int r[4]){
  int i,j,f;

  if(c[0]==r[0] && c[1]==r[1] && c[2]==r[2] && c[3]==r[3]){
    f = 1;
    }
    else{
      f= 0;
      }
  delay(400);
  for(j=0; j<4; j++){        // j = flash number
    for(i=0; i<4; i++){      // i = led position
      if(f){
        led_red(i);
      } else {
        led_green(i);
      }
    }
    delay(100);
    for(i=0; i<4; i++) led_off(i);
    delay(100);
  }
  delay(400);
}

void loop(){
  int c[4],r[4];              // save challenge and response in arrays
  issue_challenge(c);         // create and display a challenge
  get_response(r);            // get the player's response
  give_feedback(c,r);         // flash LEDs to indicate success/failure
}
