#include <SPI.h>                                          // Подключаем библиотеку для работы с шиной SPI
#include <nRF24L01.h>                                     // Подключаем файл настроек из библиотеки RF24
#include <RF24.h>                                         // Подключаем библиотеку для работы с nRF24L01+
RF24           radio(9, 10);                              // Создаём объект radio для работы с библиотекой RF24, указывая номера выводов nRF24L01+ (CE, CSN)
int            mapData[5][5];  // карта, которую мы принимаем от других роботов
int            comand;       
int mySpottedDirects;
  int mapp[5][5];//Карта, которая обрабатывается роботом(его карта)
const int varX[] = {1, 0, -1, 0 };
const int varY[] = {0, 1,  0, -1}; 

bool isStepMod = 0;
int coordX = 0;
int coordY = 0;
int direct = 0;
int mycomand = 0;

int coordXold = 0;
int coordYold = 0;
int robot_id = 3;




void setup() {
      delay(1000);
   radio.begin();                                        // Инициируем работу nRF24L01+
    radio.setChannel(5);                                  // Указываем канал приёма данных (от 0 до 127), 5 - значит приём данных осуществляется на частоте 2,405 ГГц (на одном канале может быть только 1 приёмник и до 6 передатчиков)
    radio.setDataRate     (RF24_1MBPS);                   // Указываем скорость передачи данных (RF24_250KBPS, RF24_1MBPS, RF24_2MBPS), RF24_1MBPS - 1Мбит/сек
    radio.setPALevel      (RF24_PA_HIGH);                 // Указываем мощность передатчика (RF24_PA_MIN=-18dBm, RF24_PA_LOW=-12dBm, RF24_PA_HIGH=-6dBm, RF24_PA_MAX=0dBm)
    radio.openReadingPipe (1, 0x1234567890LL);
    radio.openWritingPipe ( 0x1234567890LL);// Открываем 1 трубу с идентификатором 0x1234567890 для приема данных (на ожном канале может быть открыто до 6 разных труб, которые должны отличаться только последним байтом идентификатора)
   
  // put your setup code here, to run once:  
}




void loop() {
  // put your main code here, to run repeatedly:

}





void coordinator()//Узнаём направление робота на карте
{
  for(int i = 0; i < 4; i++)
    {
      if(varX[i] == coordX - coordXold && varY[i] == coordY - coordYold)
        direct = i;
    }
}




void Move(int direct, bool lool/*проверяет движемся ли мы или строим карту.*/)
{
  if(lool)
  {
    if(direct == 1)
    {
     analogWrite(5, 0);
     analogWrite(6, 0);
    }
    if(direct == 0)
    {
     digitalWrite(4, HIGH);
     digitalWrite(7, HIGH);
     analogWrite(5, 255);
     analogWrite(6, 255);
    }
  }
  if(!lool)
  {
    if(direct>0)
    {
      for(int j = 0; j < direct; j++)
      {
        digitalWrite(4, LOW);
        digitalWrite(7, HIGH);
        analogWrite(5, 255);
        analogWrite(6, 255);
        //encoder oneLove :* <3
      }
    }
    if(direct<0)
    {
      for(int j = 0; j < direct; j++)
      {
        digitalWrite(4, HIGH);
        digitalWrite(7, LOW);
        analogWrite(5, 255);
        analogWrite(6, 255);
        //encoder oneLove :* <3
      }
    }
  }
}




void check()//Правит карту в ближайщих четырёх клетках
{
  for(int i = 0; i < 5; i++)
  {
    if(coordX + varX[i] < 5 && coordX + varX[i] > -1 && coordY + varY[i] < 5 && coordY + varY[i] > -1 && mapp[coordX + varX[i]][coordY + varY[i]] == 0 )
    {
      int q = direct-i;//                   }
      if(abs(q)>2&&q!=0)Move(-(q)%2,0);//    }  поворот для чека тумана войны.
      else if(q!=0)Move(q,0);//             }
      if(digitalRead(A0) == 1)
       mapp[coordX + varX[i]][coordY + varY[i]] == 1;
       else
        mapp[coordX + varX[i]][coordY + varY[i]] == 2;
    //                                                     Процесс чека of тумае войны и правка карты
         
    }
    
  }
}



void leftHand()//Алгоритм прохода условного лабиринта по правилу левой руки
{
  int way = (direct+3)%4;
   coordXold = coordX;
   coordYold = coordY;
     
  {
     if(mapp[coordX + varX[way]][coordY+varY[way]] == 1)
     {
      int q = way;
      if(abs(q)>2&&q!=0)Move(-(q)%2,0);
      else if(q!=0)Move(q,0);
      Move(0,1);
      coordX = coordX + varX[way];
      coordY = coordY + varY[way]; 
     }
     else 
     {
      if(mapp[coordX + varX[direct]][coordY+varY[direct]] == 1)
      {
        Move(0,1);
      coordX = coordX + varX[direct];
      coordY = coordY + varY[direct]; 
          
      }
      else
      {
        if(mapp[coordX + varX[(direct+1)%4]][coordY+varY[(direct+1)%4]] == 1)
        {
          int q = (direct+1)%4;
          if(abs(q)>2&&q!=0)Move(-(q)%2,0);
          else if(q!=0)Move(q,0);
          Move(0,1);
             coordX = coordX + varX[(direct+1)%4];
             coordY = coordY + varY[(direct+1)%4]; 
   
        }
        else
        {
          Move(2,0);
          Move(0,1);
             coordX = coordXold + coordX;
             coordXold = coordX - coordXold;
             coordX = coordX - coordXold;
             
             coordY = coordYold + coordY;
             coordYold = coordY - coordYold;
             coordY = coordY - coordYold; 
   
        }
      }
     }
     
     
  }  
}       
//Получение данных по радио
void recieve()
{
  
        radio.startListening  ();            
                                    // Если в буфере имеются принятые данные
        radio.read(&comand, sizeof(comand));                  // Читаем данные в массив data и указываем сколько байт читать
        radio.read(&mapData, sizeof(mapData)); 
    /*if(comand == 1)
      function();
      */
      
}

void makemap()//Слияние своей карты с полученной.
{
  for(int i = 0; i < 5; i++)
    for(int j = 0; j < 5; j++)
    {
      if(mapp[i][j] != mapData[i][j] && mapData[i][j] != 0 && mapData[i][j] != 3 )
        mapp[i][j] = mapData[i][j];  
    }
}

void transmite()//Передача данных на второго робота
  {
    radio.stopListening  ();           
        radio.write(&mycomand, sizeof(mycomand));
        radio.write(&mapp, sizeof(mapp)); 
  }

void searchLines()
{
  for()
}

