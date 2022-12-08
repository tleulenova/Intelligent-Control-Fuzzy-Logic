#include <16F877A.h>
#device adc=10
#include <math.h>
#FUSES XT, NOWDT, NOPROTECT, NOBROWNOUT, NOLVP, NOPUT, NODEBUG, NOCPD
#use delay(crystal=20000000)
#use rs232 (baud=9600,xmit=PIN_C6, rcv=PIN_C7, parity=N, stop=1) 
//#include <lcd.c>

#define RT0 9830   // 
#define B1 3000      // K
#define VCC 5    //Supply voltage
#define R 9880//R=10K

float temperature = 0.0f;
float prevTemperature = 0.0f;
float deltaTemperature = 0.0f;
int16 i = 0;
float T0 = 27 + 273.15;

unsigned int16 result_1; //variable for A0 value
//unsigned int16 result_2; //variable for A1 value

#int_timer0
void tmr_int()
{
   set_timer0(207);
   i++;     
   if (i==400)
   {
      deltaTemperature = temperature - prevTemperature;
      prevTemperature = temperature;
      i = 0;
   }
}

const int8 oneTo6th[6][41] = {
45,45,45,45,45,44,43,42,40,39,38,37,35,34,33,31,30,28,27,25,24,22,20,19,17,18,18,18,18,18,19,19,19,19,19,19,19,19,19,18,18
42,42,42,42,42,49,55,57,57,56,55,54,53,52,51,50,48,47,46,44,43,42,41,40,39,40,41,42,43,44,45,46,47,49,51,49,47,46,45,44,43
40,40,40,40,40,47,52,57,61,65,64,63,63,62,61,60,59,58,57,55,54,53,52,51,51,51,52,53,54,55,57,58,59,61,63,61,59,58,57,55,54
37,37,37,37,37,44,50,55,58,62,65,68,68,68,67,66,65,64,63,62,61,61,61,61,61,61,61,61,61,62,63,65,66,67,69,67,66,65,63,62,61
34,34,34,34,34,42,48,53,57,60,63,65,67,69,71,70,69,69,70,70,70,70,70,70,70,70,70,70,70,70,70,70,70,71,73,71,70,70,70,70,70
29,29,29,29,29,38,45,50,55,58,61,63,66,69,72,74,77,77,78,78,78,78,78,78,78,78,78,78,78,78,78,78,78,77,76,77,78,78,78,78,78
};

const int8 seventhTo12th[6][41] = {

24,24,24,24,24,34,41,47,52,55,60,63,67,70,73,75,78,80,82,84,86,86,86,86,86,86,86,86,86,86,86,85,85,84,84,84,85,85,86,86,86
18,18,18,18,18,29,38,45,51,55,60,63,67,70,73,75,78,80,82,84,86,88,90,92,94,94,94,94,94,94,94,94,94,94,95,94,94,94,94,94,94
17,17,17,17,17,29,37,45,51,55,60,63,67,70,73,75,78,80,82,84,86,88,90,92,94,94,94,94,94,94,94,94,94,94,95,94,94,94,94,94,94
17,17,17,17,17,29,37,45,51,55,60,63,67,70,73,75,78,80,82,84,86,88,90,92,94,94,94,94,94,94,94,94,94,94,95,94,94,94,94,94,94
17,17,17,17,17,29,37,45,51,55,60,63,67,70,73,75,78,80,82,84,86,88,90,92,94,94,94,94,94,94,94,94,94,94,95,94,94,94,94,94,94
39,39,39,39,39,56,70,78,81,84,86,88,90,92,94,95,97,99,100,102,104,105,107,109,110,111,111,112,112,113,113,114,115,116,118,116,115,114,113,113,112
};

const int8 thirteenthTo18th[6][41] = {
51,51,51,51,51,63,75,85,95,103,104,106,107,108,109,110,111,113,114,115,117,118,120,121,123,123,124,124,125,126,127,128,129,131,133,131,129,128,127,126,125
60,60,60,60,60,69,78,86,95,102,109,115,119,119,120,121,122,123,124,125,126,128,129,131,133,133,133,134,134,135,137,138,139,141,143,141,139,138,136,135,133
66,66,66,66,66,75,83,90,96,101,108,113,119,124,129,129,130,131,132,133,135,137,139,141,143,143,143,143,143,143,144,145,146,148,150,148,146,145,143,142,142
72,72,72,72,72,80,87,93,99,104,109,113,117,123,129,132,135,137,139,141,143,145,147,149,151,151,151,151,151,151,151,151,151,151,150,151,151,151,151,151,151
77,77,77,77,77,84,91,97,102,107,112,116,120,125,129,133,137,142,145,149,151,154,156,158,160,160,160,160,160,160,158,157,155,152,150,153,155,157,159,161,161
81,81,81,81,81,88,94,100,106,111,116,120,125,129,133,137,141,145,150,154,158,162,165,167,169,169,169,169,167,166,164,162,161,158,156,159,161,163,166,168,169
};

const int8 nineteenthTo24th[6][41] = {
85,85,85,85,85,92,98,104,110,115,120,125,130,134,138,143,147,151,155,159,164,168,172,176,179,178,177,176,174,173,172,170,168,167,165,167,170,172,175,177,179
90,90,90,90,90,97,103,109,115,120,126,130,135,140,144,149,153,157,162,166,171,176,180,184,188,187,186,185,184,183,182,180,179,178,176,179,181,184,187,189,192
94,94,94,94,94,101,108,114,120,126,131,136,141,146,150,155,159,164,168,173,178,183,188,194,200,199,198,197,197,196,195,195,194,193,193,195,198,201,203,206,209
94,94,94,94,94,101,108,114,120,126,131,136,141,146,150,155,159,164,168,173,178,183,188,194,200,199,198,197,197,196,195,195,194,193,193,195,198,201,203,206,209
94,94,94,94,94,101,108,114,120,126,131,136,141,146,150,155,159,164,168,173,178,183,188,194,200,199,198,197,197,196,195,195,194,193,193,195,198,201,203,206,209
94,94,94,94,94,101,108,114,120,126,131,136,141,146,150,155,159,164,168,173,178,183,188,194,200,199,198,197,197,196,195,195,194,193,193,195,198,201,203,206,209
};

const int8 twentyfifthTo30th[6][41] = {
94,94,94,94,94,101,108,114,120,126,131,136,141,146,150,155,159,164,168,173,178,183,188,194,200,199,198,197,197,196,195,195,194,193,193,195,198,201,203,206,209
94,94,94,94,94,101,108,114,120,126,131,136,141,146,150,155,159,164,168,173,178,183,188,194,200,199,198,197,197,196,195,195,194,193,193,195,198,201,203,206,209
94,94,94,94,94,101,108,114,120,126,131,136,141,146,150,155,159,164,168,173,178,183,188,194,200,199,198,197,197,196,195,195,194,193,193,195,198,201,203,206,209
94,94,94,94,94,101,108,114,120,126,131,136,141,146,150,155,159,164,168,173,178,183,188,194,200,199,198,197,197,196,195,195,194,193,193,195,198,201,203,206,209
94,94,94,94,94,101,108,114,120,126,131,136,141,146,150,155,159,164,168,173,178,183,188,194,200,199,198,197,197,196,195,195,194,193,193,195,198,201,203,206,209
94,94,94,94,94,101,108,114,120,126,131,136,141,146,150,155,159,164,168,173,178,183,188,194,200,199,198,197,197,196,195,195,194,193,193,195,198,201,203,206,209
};

const int8 thirtyfirstTo36th[6][41] = {
94,94,94,94,94,101,108,114,120,126,131,136,141,146,150,155,159,164,168,173,178,183,188,194,200,199,198,197,197,196,195,195,194,193,193,195,198,201,203,206,209
94,94,94,94,94,101,108,114,120,126,131,136,141,146,150,155,159,164,168,173,178,183,188,194,200,199,198,197,197,196,195,195,194,193,193,195,198,201,203,206,209
94,94,94,94,94,101,108,114,120,126,131,136,141,146,150,155,159,164,168,173,178,183,188,194,200,199,198,197,197,196,195,195,194,193,193,195,198,201,203,206,209
94,94,94,94,94,101,108,114,120,126,131,136,141,146,150,155,159,164,168,173,178,183,188,194,200,199,198,197,197,196,195,195,194,193,193,195,198,201,203,206,209
94,94,94,94,94,101,108,114,120,126,131,136,141,146,150,155,159,164,168,173,178,183,188,194,200,199,198,197,197,196,195,195,194,193,193,195,198,201,203,206,209
94,94,94,94,94,101,108,114,120,126,131,136,141,146,150,155,159,164,168,173,178,183,188,194,200,199,198,197,197,196,195,195,194,193,193,195,198,201,203,206,209
};

const int8 thirtyseventhTo42nd[5][41] = {
94,94,94,94,94,101,108,114,120,126,131,136,141,146,150,155,159,164,168,173,178,183,188,194,200,199,198,197,197,196,195,195,194,193,193,195,198,201,203,206,209
94,94,94,94,94,101,108,114,120,126,131,136,141,146,150,155,159,164,168,173,178,183,188,194,200,199,198,197,197,196,195,195,194,193,193,195,198,201,203,206,209
94,94,94,94,94,101,108,114,120,126,131,136,141,146,150,155,159,164,168,173,178,183,188,194,200,199,198,197,197,196,195,195,194,193,193,195,198,201,203,206,209
94,94,94,94,94,101,108,114,120,126,131,136,141,146,150,155,159,164,168,173,178,183,188,194,200,199,198,197,197,196,195,195,194,193,193,195,198,201,203,206,209
94,94,94,94,94,101,108,114,120,126,131,136,141,146,150,155,159,164,168,173,178,183,188,194,200,199,198,197,197,196,195,195,194,193,193,195,198,201,203,206,209
};

const int8 fortythirdTo48th[6][41] = {
10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,
10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,
10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,
10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,
10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,
10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10
};



const int8 temperatureArray[76] = {
15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32,33,34,35,36,37,38,39,40,41,42,43,44,45,46,47,48,49,50,51,52,53,54,
55,56,57,58,59,60,61,62,63,64,65,66,67,68,69,70,71,72,73,74,75,76,77,78,79,80,81,82,83,84,85,86,87,88,89,90
};

/*const int8 temperatureArray[60] = {
21,22,23,24,25,26,27,28,29,30,31,32,33,34,35,36,37,38,39,40,41,42,43,44,45,46,47,48,49,50,51,52,53,54,
55,56,57,58,59,60,61,62,63,64,65,66,67,68,69,70,71,72,73,74,75,76,77,78,79,80
};*/

const int8 deltaTemperatureArray[41] = {
-30,-28,-27,-25,-24,-22,-21,-19,-18,-16,-15,-13,-12,-10,-9,-7,-6,-4,-3,-1,0,1,3,4,6,7,9,10,12,13,15,16,18,19,21,22,24,25,27,28,30
};

signed int8 * SelectMatrixAndRowAndColumn(float _temperature, float _deltaTemperature)
{
   static int8 matrixAndRowAndColumn[3] = {0,0,0};
   // Select Matrix
   int8 matrixNumber = ((int8)(_temperature - temperatureArray[0]))/6;
   // Select Row
   int8 rowNumber = ((int8)(_temperature - temperatureArray[0]))%6;
   // Select Column
   int8 columnNumber = (int8)(_deltaTemperature*6.833 + 20.5);
   
   matrixAndRowAndColumn[0] = matrixNumber+1;
   matrixAndRowAndColumn[1] = rowNumber;
   matrixAndRowAndColumn[2] = columnNumber;
   
   Return(matrixAndRowAndColumn);
}

float GetDataAndInterpolate(int8 _matrix, int8 _row, int8 _column, float _temperature, float _deltaTemperature)
{
   // data1  data2
   // data3  data4
   int8 data1, data2, data3, data4;
   float interpolatedData;

   switch(_matrix)
   {
      case 1:
         data1 = oneTo6th[_row][_column-1];
         data2 = oneTo6th[_row][_column];
         if (_row == 5)
         {
            data3 = seventhTo12th[0][_column-1];
            data4 = seventhTo12th[0][_column];
         }
         else
         {
            data3 = oneTo6th[_row+1][_column-1];
            data4 = oneTo6th[_row+1][_column];
         }
         break;
      case 2:
         data1 = seventhTo12th[_row][_column-1];
         data2 = seventhTo12th[_row][_column];
         if (_row == 5)
         {
            data3 = thirteenthTo18th[0][_column-1];
            data4 = thirteenthTo18th[0][_column];
         }
         else
         {
            data3 = seventhTo12th[_row+1][_column-1];
            data4 = seventhTo12th[_row+1][_column];
         }
         break;
      case 3:
         data1 = thirteenthTo18th[_row][_column-1];
         data2 = thirteenthTo18th[_row][_column];
         if (_row == 5)
         {
            data3 = nineteenthTo24th[0][_column-1];
            data4 = nineteenthTo24th[0][_column];
         }
         else
         {
            data3 = thirteenthTo18th[_row+1][_column-1];
            data4 = thirteenthTo18th[_row+1][_column];
         }
         break;
      case 4:
         data1 = nineteenthTo24th[_row][_column-1];
         data2 = nineteenthTo24th[_row][_column];
         if (_row == 5)
         {
            data3 = twentyfifthTo30th[0][_column-1];
            data4 = twentyfifthTo30th[0][_column];
         }
         else
         {
            data3 = nineteenthTo24th[_row+1][_column-1];
            data4 = nineteenthTo24th[_row+1][_column];
         }
         break;
      case 5:
         data1 = twentyfifthTo30th[_row][_column-1];
         data2 = twentyfifthTo30th[_row][_column];
         if (_row == 5)
         {
            data3 = thirtyfirstTo36th[0][_column-1];
            data4 = thirtyfirstTo36th[0][_column];
         }
         else
         {
            data3 = twentyfifthTo30th[_row+1][_column-1];
            data4 = twentyfifthTo30th[_row+1][_column];
         }
         break;
      case 6:
         data1 = thirtyfirstTo36th[_row][_column-1];
         data2 = thirtyfirstTo36th[_row][_column];
         if (_row == 5)
         {
            data3 = thirtyseventhTo42nd[0][_column-1];
            data4 = thirtyseventhTo42nd[0][_column];
         }
         else
         {
            data3 = thirtyfirstTo36th[_row+1][_column-1];
            data4 = thirtyfirstTo36th[_row+1][_column];
         }
         break;
      default:
         data1 = thirtyseventhTo42nd[_row][_column-1];
         data2 = thirtyseventhTo42nd[_row][_column];
         data3 = thirtyseventhTo42nd[_row+1][_column-1];
         data4 = thirtyseventhTo42nd[_row+1][_column];
         break;   
   }
   
   // 2D INTERPOLATION
   // Known Steps => can be more modular
   float x1 = (float)((int8)(_deltaTemperature));
   float x2 = (float)((int8)(_deltaTemperature)+1);
   float y1 = (float)((int8)(_temperature));
   float y2 = (float)((int8)(_temperature)+1);
   float fx_y1 = ((x2-_deltaTemperature)/(x2-x1))*data1 + ((_deltaTemperature-x1)/(x2-x1))*data2;
   float fx_y2 = ((x2-_deltaTemperature)/(x2-x1))*data3 + ((_deltaTemperature-x1)/(x2-x1))*data4;
   interpolatedData = ((y2-_temperature)/(y2-y1))*fx_y1 + ((_temperature-y1)/(y2-y1))*fx_y2;
   
   return interpolatedData;
}

float TemperatureSensor()
{
   float RT, VR, ln, VRT;
     
   set_adc_channel(0); // next analog reading will be from channel 0
   delay_us(10); //allow time after channel selection and reading
   result_1 = read_adc(); //start and read A/D
      
   VRT = (5.00 / 1023.00) * result_1;      //Conversion to voltage
   VR = VCC - VRT;
   RT = VRT / (VR / R);               //Resistance of RT
   
   ln = log(RT / RT0);
   float _temperature = (1 / ((ln / B1) + (1 / T0))); //Temperature from thermistor
   _temperature = _temperature - 273.15;                 //Conversion to Celsius
      
   // LIMITING TEMPERATURE
   /*if (_temperature < 33.0)
      _temperature = 33.0;
   else if (_temperature > 74.0)
      _temperature = 74.0;*/
   return _temperature;
}

void main()
{
   setup_psp(PSP_DISABLED);
   setup_timer_1(T1_DISABLED);
   setup_timer_2(T2_DISABLED,0,1);
   setup_ccp1(CCP_PWM); //4kHz PWM signal output at CCP1 pin 17
   setup_CCP2(CCP_OFF);
              //internal clock | prescaling 256
   setup_timer_0(RTCC_INTERNAL | RTCC_DIV_256); 
   setup_timer_2(T2_DIV_BY_16, 255, 1);
   set_timer0(207);
   enable_interrupts(int_timer0);
   enable_interrupts(global); 
   delay_ms(10);
   setup_adc_ports(AN0_AN1_AN3); //A0 A1 A3 are configured for analog input pin
   setup_adc(ADC_CLOCK_DIV_32); //enable ADC and set clock for ADC
   
   set_tris_c(0x00); //set all portc pins as output

   while(1)
   {
      temperature = TemperatureSensor();
      int8 * MatrixAndRowAndColumn;
      MatrixAndRowAndColumn=SelectMatrixAndRowAndColumn(temperature, deltaTemperature);
      float fuzzyOut = GetDataAndInterpolate(MatrixAndRowAndColumn[0], MatrixAndRowAndColumn[1], MatrixAndRowAndColumn[2], temperature, deltaTemperature);
      unsigned int mypwm = (unsigned int)(fuzzyOut);
      printf("T: %f, dT: %f, out:%u\n",temperature,deltaTemperature,mypwm);
      set_pwm1_duty(mypwm);
      delay_ms(100);
   }
}
