#define lc 0
#define sc 1
#define lm1 3
#define lm2 5
#define rm1 6
#define rm2 9
int s1, s2, s3, s4, s5, s6, s7, s8;
float error = 0;
float prev_error = 0;
float Kp = 25;
float Kd = 100;
float P = 0;
float D = 0;
float pd = 0;
float lms, rms;
int bs = 125;
int left = 0, right = 0;
int leftthres = 400;
int rightthres = 400;
void compute_speed() {
  lms = bs + pd;
  rms = bs - pd;
}
////////////////////////////

void error_map() {
  if ((s3 == sc) && (s4 == lc) && (s5 == lc) && (s6 == sc)) {
    error = 0;
  }
  else if ((s3 == lc) && (s4 == lc) && (s5 == lc) && (s6 == sc)) {
    error = -0.5;
  }
  else if ((s3 == sc) && (s4 == lc) && (s5 == lc) && (s6 == lc)) {
    error = 0.5;
  }
  else if ((s2 == sc) && (s3 == lc) && (s4 == lc) && (s5 == sc)) {
    error = -1;
  }
  else if ((s4 == sc) && (s5 == lc) && (s6 == lc) && (s7 == sc)) {
    error = 1;
  }
  else if ((s2 == lc) && (s3 == lc) && (s4 == lc) && (s5 == sc)) {
    error = -1.5;
  }
  else if ((s4 == sc) && (s5 == lc) && (s6 == lc) && (s7 == lc)) {
    error = 1.5;
  }
  else if ((s1 == sc) && (s2 == lc) && (s3 == lc) && (s4 == sc)) {
    error = -2;
  }
  else if ((s5 == sc) && (s6 == lc) && (s7 == lc) && (s8 == sc)) {
    error = 2;
  }
  else if ((s1 == lc) && (s2 == lc) && (s3 == lc) && (s4 == sc)) {
    error = -2.5;
  }
  else if ((s5 == sc) && (s6 == lc) && (s7 == lc) && (s8 == lc)) {
    error = 2.5;
  }
}
///////////////////////////////////////

void linefollow() {
  turn();
}
void sharp_left_turn() {
  analogWrite(lm1, 0);
  analogWrite(lm2, 80);
  analogWrite(rm1, 80);
  analogWrite(rm2, 0);
}
////////////////////

void sharp_right_turn() {
  analogWrite(lm1, 80);
  analogWrite(lm2, 0);
  analogWrite(rm1, 0);
  analogWrite(rm2, 80);
}
////////////////////
void turn() {
  analogWrite(lm1, lms);
  analogWrite(lm2, 0);
  analogWrite(rm1, rms + 10);
  analogWrite(rm2, 0);
}
///////////////////////////////
void readsensors() {
  s1 = digitalRead(8);
  s2 = digitalRead(A1);
  s3 = digitalRead(A2);
  s4 = digitalRead(A3);
  s5 = digitalRead(A4);
  s6 = digitalRead(A5);
  s7 = digitalRead(2);
  s8 = digitalRead(4);
}

//////////////////////////

void setup() {
  pinMode(lm1, OUTPUT);
  pinMode(lm2, OUTPUT);
  pinMode(rm1, OUTPUT);
  pinMode(rm2, OUTPUT);
  pinMode(A0, INPUT);
  pinMode(A2, INPUT);
  pinMode(A3, INPUT);
  pinMode(A4, INPUT);
  pinMode(A5, INPUT);
  pinMode(A1, INPUT);
  pinMode(12, INPUT);
  pinMode(13, INPUT);
}

///////////////////////

void loop() {
  readsensors();
  error_map();
  if (s1 == lc) {
    left = 0;
  }
  if (s8 == lc) {
    right = 0;
  }
  if ((s1 != lc) && (s2 != lc) && (s3 != lc) && (s4 != lc) && (s5 != lc) && (s6 != lc) && (s7 != lc) && (s8 != lc)) {
    if ((left > leftthres) && (right < rightthres)) {
      readsensors();
      while ((s4 != lc) && (s5 != lc)) {
        sharp_right_turn();
        readsensors();
      }
    }
    else if ((left < leftthres) && (right > rightthres)) {
      readsensors();
      while ((s4 != lc) && (s5 != lc)) {
        sharp_left_turn();
        readsensors();
      }
    }
  }
  left = left + 1;
  right = right + 1;
  if (left > 20000) {
    left = 1000;
  }
  if (right > 20000) {
    right = 1000;
  }
  P = Kp * error;
  D = Kd * (error - prev_error);
  pd = P + D;
  compute_speed();
  linefollow();
  prev_error = error;

}



//////////////////////////

