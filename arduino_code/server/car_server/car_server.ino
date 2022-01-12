#include <Servo.h>   //引入函式庫

Servo myservo; //宣告伺服馬達

const int fa = 4; //右馬達正端
const int fb = 5; //右馬達負端
const int fc = 6; //左馬達正端
const int fd = 7; //左馬達負端

const int ena = 8; //右馬達PWM
const int enb = 2; //左馬達PWM

const int s = 10; //伺服馬達PWM

const int ori_angle = 30; //角度原始位置
const int motor_v = 180; //馬達轉速

float data_array[2] ; //參數陣列
boolean f = false; //投餵狀態

void setup() {

  pinMode(fa, OUTPUT); //右馬達正端接角
  pinMode(fb, OUTPUT); //右馬達負端接角
  pinMode(fc, OUTPUT); //左馬達正端接角
  pinMode(fd, OUTPUT); //左馬達負端接角
  pinMode(ena, OUTPUT); //右馬達PWM接角
  pinMode(enb, OUTPUT); //左馬達PWM接角

  myservo.attach(s); //伺服馬達接角

  Serial.begin(115200); //接收鮑率
  Serial.setTimeout(10); //接收時長

}


void loop() {

    test(); //測試伺服馬達運轉
  
    if (Serial.available()>0){ //若有接收資料
            
      String data = Serial.readString(); //讀取輸入字串
      
      float temp; //宣告一浮點數變數:溫度
      float area_r; //宣告一浮點數變數:飼料面積比

      temp = getValue(data, 'T'); //取溫度數值
      area_r = getValue(data, 'A'); //取面積飼料比數值

      data_array[0] = temp; //將溫度存入陣列
      data_array[1] = area_r; //將飼料面積比存入陣列
      
      Serial.print("Temp : "); //印出溫度
      Serial.println(temp);
      Serial.print("Area_Ratio : "); //印出面積飼料比
      Serial.println(area_r);
     
    }


    if(data_array[0]!=0 || data_array[1]!=0){ //若溫度或面積飼料比不為0
      feed_control(data_array); //進入副函式:判斷是否需餵食及餵食量
      delay(3000); //延遲3秒
    }
    
    if (f == true){ //若有餵食則清空數值陣列
          data_array[0] = 0.0;
          data_array[1] = 0.0;
          f = false; //改變投餵狀態
        }
        
    else{
      Serial.println("Reveived failed"); //無接收到資料
    }
    

}

void test(){ //測試伺服馬達

  myservo.write(ori_angle); //伺服馬達轉回原角度

  delay(1000);
  

}

float getValue(String data, char kind){ //抓取參數數值

  float v; //宣告一浮點數
  String d; //宣告一字串
  
  for (int i=0; i<data.length(); i++){ //讀取接收字串之每一字元
    
    if (data.charAt(i) == kind){ //確認參數類別
      
      d = data.substring(i+2,i+7); //取出參數數值
      v = d.toFloat(); //轉換格式
      break; //跳出迴圈
      
    }
    
    }

  return v; //回傳參數數值
}

void feed_control(float data_array[2]){ //投餵控制副函式
  //cut out the value of the temperature
  //motor angle depends on received temp
  //less:<10, plenty:>40
  //feed_conditions : 17 < temp < 25, area_r < 40
  // 20 < area_r < 40 : half_opened, 
  // 10 < area_r < 20 : three-forth_opened, 
  //      area_r < 10 : complete_opened

  float temp = data_array[0]; //溫度
  float area = data_array[1]; //面積飼料比

  if (area < 40 ||  temp > 17 && temp < 25){ //利用溫度及飼料面積比判斷投餵狀態
  //若面積飼料比小於40或溫度介於17~25℃之間:需投餵

//    Serial.print("area ratio : ");
//    Serial.println(area);
//    Serial.print("Temperature : ");
//    Serial.println(temp);
    f = true; //改變投餵狀態:須投餵
    Serial.println("in feed_control"); //進入副函式
    feed_start(); //移動至養殖池
    
  }
// 判斷投餵量
  if (f == true && area > 20){ //須投餵且飼料面積比大於20

    Serial.println("half_opened"); //投餵器開口開一半
    myservo.write(20); //開口一半須轉大約20度
    delay(3000); 
    f = false; //改變投餵狀態
    feed_end(); //移動回原位置
  }

  if (f == true && area > 10){

    Serial.println("3/4_opened"); //投餵器開口開3/4
    myservo.write(15); //開口一半須轉大約15度
    delay(3000);
    f = false; //改變投餵狀態
    feed_end(); //移動回原位置
  
  }

  if (f == true && area < 10){

    Serial.println("complete_opened"); //投餵器開口全開
    myservo.write(0); //開口一半須轉大約0度
    delay(3000);
    f = false; //改變投餵狀態
    feed_end(); //移動回原位置
  
}

void feed_start(){ //移動至養殖池
  mb();
  delay(1000);
  mstop();
  delay(3000);
  
}

void feed_end(){ //投餵結束
  myservo.write(ori_angle); //開口關閉
  mf(); //移動回原位
  delay(1000);
  mstop();
  delay(3000);
}


void mf(){ //往前移動
  digitalWrite(fa,HIGH);
  digitalWrite(fb, LOW);
  analogWrite(ena, motor_v);
  digitalWrite(fc,HIGH);
  digitalWrite(fd, LOW);
  analogWrite(enb, motor_v);
}

void mb(){ //往後移動
  digitalWrite(fa, LOW);
  digitalWrite(fb,HIGH);
  analogWrite(ena, motor_v);
  digitalWrite(fc, LOW);
  digitalWrite(fd,HIGH);
  analogWrite(enb, motor_v);
}


void mstop(){ //停止
  digitalWrite(fa, LOW);
  digitalWrite(fb, LOW);
  digitalWrite(fc, LOW);
  digitalWrite(fd, LOW);
}

#include <Servo.h>   //引入函式庫

Servo myservo; //宣告伺服馬達

const int fa = 4; //右馬達正端
const int fb = 5; //右馬達負端
const int fc = 6; //左馬達正端
const int fd = 7; //左馬達負端

const int ena = 8; //右馬達PWM
const int enb = 2; //左馬達PWM

const int s = 10; //伺服馬達PWM

const int ori_angle = 30; //角度原始位置
const int motor_v = 180; //馬達轉速

float data_array[2] ; //參數陣列
boolean f = false; //投餵狀態

void setup() {

  pinMode(fa, OUTPUT); //右馬達正端接角
  pinMode(fb, OUTPUT); //右馬達負端接角
  pinMode(fc, OUTPUT); //左馬達正端接角
  pinMode(fd, OUTPUT); //左馬達負端接角
  pinMode(ena, OUTPUT); //右馬達PWM接角
  pinMode(enb, OUTPUT); //左馬達PWM接角

  myservo.attach(s); //伺服馬達接角

  Serial.begin(115200); //接收鮑率
  Serial.setTimeout(10); //接收時長

}


void loop() {

    test(); //測試伺服馬達運轉
  
    if (Serial.available()>0){ //若有接收資料
            
      String data = Serial.readString(); //讀取輸入字串
      
      float temp; //宣告一浮點數變數:溫度
      float area_r; //宣告一浮點數變數:飼料面積比

      temp = getValue(data, 'T'); //取溫度數值
      area_r = getValue(data, 'A'); //取面積飼料比數值

      data_array[0] = temp; //將溫度存入陣列
      data_array[1] = area_r; //將飼料面積比存入陣列
      
      Serial.print("Temp : "); //印出溫度
      Serial.println(temp);
      Serial.print("Area_Ratio : "); //印出面積飼料比
      Serial.println(area_r);
     
    }


    if(data_array[0]!=0 || data_array[1]!=0){ //若溫度或面積飼料比不為0
      feed_control(data_array); //進入副函式:判斷是否需餵食及餵食量
      delay(3000); //延遲3秒
    }
    
    if (f == true){ //若有餵食則清空數值陣列
          data_array[0] = 0.0;
          data_array[1] = 0.0;
          f = false; //改變投餵狀態
        }
        
    else{
      Serial.println("Reveived failed"); //無接收到資料
    }
    

}

void test(){ //測試伺服馬達

  myservo.write(ori_angle); //伺服馬達轉回原角度

  delay(1000);
  

}

float getValue(String data, char kind){ //抓取參數數值

  float v; //宣告一浮點數
  String d; //宣告一字串
  
  for (int i=0; i<data.length(); i++){ //讀取接收字串之每一字元
    
    if (data.charAt(i) == kind){ //確認參數類別
      
      d = data.substring(i+2,i+7); //取出參數數值
      v = d.toFloat(); //轉換格式
      break; //跳出迴圈
      
    }
    
    }

  return v; //回傳參數數值
}

void feed_control(float data_array[2]){ //投餵控制副函式
  //cut out the value of the temperature
  //motor angle depends on received temp
  //less:<10, plenty:>40
  //feed_conditions : 17 < temp < 25, area_r < 40
  // 20 < area_r < 40 : half_opened, 
  // 10 < area_r < 20 : three-forth_opened, 
  //      area_r < 10 : complete_opened

  float temp = data_array[0]; //溫度
  float area = data_array[1]; //面積飼料比

  if (area < 40 ||  temp > 17 && temp < 25){ //利用溫度及飼料面積比判斷投餵狀態
  //若面積飼料比小於40或溫度介於17~25℃之間:需投餵

//    Serial.print("area ratio : ");
//    Serial.println(area);
//    Serial.print("Temperature : ");
//    Serial.println(temp);
    f = true; //改變投餵狀態:須投餵
    Serial.println("in feed_control"); //進入副函式
    feed_start(); //移動至養殖池
    
  }
// 判斷投餵量
  if (f == true && area > 20){ //須投餵且飼料面積比大於20

    Serial.println("half_opened"); //投餵器開口開一半
    myservo.write(20); //開口一半須轉大約20度
    delay(3000); 
    f = false; //改變投餵狀態
    feed_end(); //移動回原位置
  }

  if (f == true && area > 10){

    Serial.println("3/4_opened"); //投餵器開口開3/4
    myservo.write(15); //開口一半須轉大約15度
    delay(3000);
    f = false; //改變投餵狀態
    feed_end(); //移動回原位置
  
  }

  if (f == true && area < 10){

    Serial.println("complete_opened"); //投餵器開口全開
    myservo.write(0); //開口一半須轉大約0度
    delay(3000);
    f = false; //改變投餵狀態
    feed_end(); //移動回原位置
  
}

void feed_start(){ //移動至養殖池
  mb();
  delay(1000);
  mstop();
  delay(3000);
  
}

void feed_end(){ //投餵結束
  myservo.write(ori_angle); //開口關閉
  mf(); //移動回原位
  delay(1000);
  mstop();
  delay(3000);
}


void mf(){ //往前移動
  digitalWrite(fa,HIGH);
  digitalWrite(fb, LOW);
  analogWrite(ena, motor_v);
  digitalWrite(fc,HIGH);
  digitalWrite(fd, LOW);
  analogWrite(enb, motor_v);
}

void mb(){ //往後移動
  digitalWrite(fa, LOW);
  digitalWrite(fb,HIGH);
  analogWrite(ena, motor_v);
  digitalWrite(fc, LOW);
  digitalWrite(fd,HIGH);
  analogWrite(enb, motor_v);
}


void mstop(){ //停止
  digitalWrite(fa, LOW);
  digitalWrite(fb, LOW);
  digitalWrite(fc, LOW);
  digitalWrite(fd, LOW);
}
