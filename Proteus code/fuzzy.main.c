#include <16F877A.h>
#device adc=10
#include <math.h>
#FUSES XT, NOWDT, NOPROTECT, NOBROWNOUT, NOLVP, NOPUT, NODEBUG, NOCPD
#use delay(crystal=20000000)
#use rs232 (baud=9600,xmit=PIN_C6, rcv=PIN_C7, parity=N, stop=1) 
#include <lcd.c>

float temperature = 0.0f;
float load = 0.0f;

unsigned int16 result_1; //variable for A0 value
unsigned int16 result_2; //variable for A1 value

const int8 oneTo6th[6][41] = {
4502,4502,4502,4502,4416,4279,4141,4001,3850,3697,3540,3369,3193,3018,2834,2655,2478,2291,2109,1926,1734,1758,1782,1807,1832,1859,1885,1913,1938,1929,1904,1876,1851,1824,1799,1774,1750,1734,1734,1734,1734
4242,4242,4242,4242,4820,5529,5721,5616,5503,5395,5283,5163,5034,4876,4719,4564,4413,4259,4112,4013,3922,4013,4112,4220,4341,4474,4624,4791,4984,4917,4733,4572,4427,4300,4182,4078,3981,3922,3922,3922,3922
4009,4009,4009,4009,4560,5244,5805,6271,6431,6347,6259,6165,6062,5922,5787,5652,5521,5394,5274,5164,5062,5164,5274,5394,5525,5668,5827,6000,6194,6127,5939,5773,5618,5481,5352,5237,5129,5062,5062,5062,5062
3720,3720,3720,3720,4310,5009,5547,5982,6368,6705,6849,6772,6686,6561,6439,6311,6188,6108,6108,6108,6108,6108,6108,6108,6194,6330,6481,6643,6823,6762,6587,6430,6282,6150,6108,6108,6108,6108,6108,6108,6108
3369,3369,3369,3369,4021,4786,5370,5824,6184,6472,6705,6958,7076,6956,6936,6958,6973,6981,6981,6981,6981,6981,6981,6981,6981,6981,6981,7033,7199,7143,6981,6981,6981,6981,6981,6981,6981,6981,6981,6981,6981
2924,2924,2924,2924,3643,4480,5115,5606,5994,6303,6616,6968,7284,7576,7731,7759,7779,7789,7789,7789,7789,7789,7789,7789,7789,7789,7789,7722,7616,7654,7753,7789,7789,7789,7789,7789,7789,7789,7789,7789,7789
};

const int8 seventhTo12th[6][41] = {
2395,2395,2395,2395,3186,4102,4793,5327,5809,6277,6680,7039,7360,7658,7935,8199,8449,8598,8598,8598,8598,8598,8598,8598,8598,8568,8525,8472,8405,8429,8490,8540,8580,8598,8598,8598,8598,8598,8598,8598,8598
1784,1784,1784,1784,2746,3787,4588,5260,5810,6278,6680,7039,7361,7659,7936,8200,8455,8707,8955,9189,9427,9427,9427,9430,9434,9439,9443,9448,9452,9450,9445,9441,9436,9432,9428,9427,9427,9427,9427,9427,9427
1734,1734,1734,1734,2657,3747,4588,5260,5810,6278,6680,7039,7361,7659,7936,8200,8455,8707,8956,9193,9420,9423,9426,9430,9434,9439,9443,9448,9452,9450,9445,9441,9436,9432,9428,9425,9422,9420,9420,9420,9420
1734,1734,1734,1734,2657,3747,4588,5260,5810,6278,6680,7039,7361,7659,7936,8200,8455,8707,8956,9193,9420,9423,9426,9430,9434,9439,9443,9448,9452,9450,9445,9441,9436,9432,9428,9425,9422,9420,9420,9420,9420
1734,1734,1734,1734,2657,3747,4588,5260,5810,6278,6680,7039,7361,7659,7936,8200,8455,8707,8956,9193,9420,9423,9426,9430,9434,9439,9443,9448,9452,9450,9445,9441,9436,9432,9428,9425,9422,9420,9420,9420,9420
3922,3922,3922,3922,5259,7030,7907,8231,8510,8767,8997,9218,9425,9629,9827,10026,10227,10433,10644,10849,11047,11087,11135,11193,11263,11346,11445,11565,11710,11658,11522,11409,11316,11238,11172,11082,10963,10899,10899,10899,10899
};

const int8 thirteenthTo18th[6][41] = {
5062,5062,5062,5062,6085,7477,8715,9823,10389,10530,10664,10804,10941,11086,11234,11389,11552,11726,11909,12088,12263,12322,12392,12476,12577,12695,12836,13002,13201,13130,12943,12786,12653,12511,12349,12208,12090,12026,12026,12026,12026
5959,5959,5959,5959,6753,7809,8811,9774,10631,11412,11869,11960,12055,12163,12277,12404,12541,12691,12858,13092,13324,13324,13324,13412,13526,13659,13815,13999,14215,14138,13934,13742,13557,13389,13233,13134,13097,13077,13077,13077,13077
6633,6633,6633,6633,7332,8270,9093,9814,10516,11230,11884,12492,12893,12976,13068,13174,13308,13537,13778,14017,14251,14251,14251,14251,14251,14368,14529,14716,14934,14855,14638,14440,14260,14164,14134,14105,14081,14067,14067,14067,14067
7181,7181,7181,7181,7817,8675,9433,10102,10682,11190,11739,12418,13008,13397,13716,13926,14148,14385,14635,14881,15122,15122,15122,15122,15122,15122,15122,15081,15031,15049,15102,15124,15117,15107,15092,15078,15065,15058,15058,15058,15058
7657,7657,7657,7657,8250,9053,9766,10396,10967,11518,12042,12545,13051,13578,14074,14550,14971,15220,15483,15742,15995,15995,15995,15995,15995,15831,15615,15377,15111,15209,15483,15740,15980,16095,16096,16096,16095,16094,16094,16094,16094
8095,8095,8095,8095,8660,9427,10119,10767,11368,11931,12467,12982,13480,13970,14464,14969,15453,15927,16375,16652,16924,16924,16924,16773,16598,16410,16203,15973,15716,15827,16119,16391,16642,16874,17090,17216,17228,17234,17234,17234
};

const int8 nineteenthTo24th[6][41] = {
8524,8524,8524,8524,9070,9836,10549,11213,11828,12405,12955,13484,13996,14503,15009,15518,16032,16536,17030,17495,17934,17796,17653,17501,17340,17169,16981,16773,16540,16668,16974,17270,17559,17818,18040,18246,18433,18548,18548,18548,18548
8957,8957,8957,8957,9523,10314,11048,11730,12361,12952,13517,14060,14588,15114,15641,16172,16715,17275,17849,18347,18817,18695,18571,18442,18309,18170,18020,17856,17676,17829,18145,18452,18758,19062,19368,19633,19812,19918,19918,19918,19918
9420,9420,9420,9420,10004,10818,11571,12268,12913,13517,14095,14651,15195,15740,16286,16841,17409,17998,18613,19262,19955,19861,19771,19684,19600,19521,19443,19369,19298,19487,19801,20113,20431,20753,21087,21431,21779,22011,22011,22011,22011
9420,9420,9420,9420,10004,10818,11571,12268,12913,13517,14095,14651,15195,15740,16286,16841,17409,17998,18613,19262,19955,19861,19771,19684,19600,19521,19443,19369,19298,19487,19801,20113,20431,20753,21087,21431,21779,22011,22011,22011,22011
9420,9420,9420,9420,10004,10818,11571,12268,12913,13517,14095,14651,15195,15740,16286,16841,17409,17998,18613,19262,19955,19861,19771,19684,19600,19521,19443,19369,19298,19487,19801,20113,20431,20753,21087,21431,21779,22011,22011,22011,22011
9420,9420,9420,9420,10004,10818,11571,12268,12913,13517,14095,14651,15195,15740,16286,16841,17409,17998,18613,19262,19955,19861,19771,19684,19600,19521,19443,19369,19298,19487,19801,20113,20431,20753,21087,21431,21779,22011,22011,22011,22011
};

const int8 twentyfifthTo30th[6][41] = {
9420,9420,9420,9420,10004,10818,11571,12268,12913,13517,14095,14651,15195,15740,16286,16841,17409,17998,18613,19262,19955,19861,19771,19684,19600,19521,19443,19369,19298,19487,19801,20113,20431,20753,21087,21431,21779,22011,22011,22011,22011
9420,9420,9420,9420,10004,10818,11571,12268,12913,13517,14095,14651,15195,15740,16286,16841,17409,17998,18613,19262,19955,19861,19771,19684,19600,19521,19443,19369,19298,19487,19801,20113,20431,20753,21087,21431,21779,22011,22011,22011,22011
9420,9420,9420,9420,10004,10818,11571,12268,12913,13517,14095,14651,15195,15740,16286,16841,17409,17998,18613,19262,19955,19861,19771,19684,19600,19521,19443,19369,19298,19487,19801,20113,20431,20753,21087,21431,21779,22011,22011,22011,22011
9420,9420,9420,9420,10004,10818,11571,12268,12913,13517,14095,14651,15195,15740,16286,16841,17409,17998,18613,19262,19955,19861,19771,19684,19600,19521,19443,19369,19298,19487,19801,20113,20431,20753,21087,21431,21779,22011,22011,22011,22011
9420,9420,9420,9420,10004,10818,11571,12268,12913,13517,14095,14651,15195,15740,16286,16841,17409,17998,18613,19262,19955,19861,19771,19684,19600,19521,19443,19369,19298,19487,19801,20113,20431,20753,21087,21431,21779,22011,22011,22011,22011
9420,9420,9420,9420,10004,10818,11571,12268,12913,13517,14095,14651,15195,15740,16286,16841,17409,17998,18613,19262,19955,19861,19771,19684,19600,19521,19443,19369,19298,19487,19801,20113,20431,20753,21087,21431,21779,22011,22011,22011,22011
};

const int8 thirtyfirstTo36th[6][41] = {
9420,9420,9420,9420,10004,10818,11571,12268,12913,13517,14095,14651,15195,15740,16286,16841,17409,17998,18613,19262,19955,19861,19771,19684,19600,19521,19443,19369,19298,19487,19801,20113,20431,20753,21087,21431,21779,22011,22011,22011,22011
9420,9420,9420,9420,10004,10818,11571,12268,12913,13517,14095,14651,15195,15740,16286,16841,17409,17998,18613,19262,19955,19861,19771,19684,19600,19521,19443,19369,19298,19487,19801,20113,20431,20753,21087,21431,21779,22011,22011,22011,22011
9420,9420,9420,9420,10004,10818,11571,12268,12913,13517,14095,14651,15195,15740,16286,16841,17409,17998,18613,19262,19955,19861,19771,19684,19600,19521,19443,19369,19298,19487,19801,20113,20431,20753,21087,21431,21779,22011,22011,22011,22011
9420,9420,9420,9420,10004,10818,11571,12268,12913,13517,14095,14651,15195,15740,16286,16841,17409,17998,18613,19262,19955,19861,19771,19684,19600,19521,19443,19369,19298,19487,19801,20113,20431,20753,21087,21431,21779,22011,22011,22011,22011
9420,9420,9420,9420,10004,10818,11571,12268,12913,13517,14095,14651,15195,15740,16286,16841,17409,17998,18613,19262,19955,19861,19771,19684,19600,19521,19443,19369,19298,19487,19801,20113,20431,20753,21087,21431,21779,22011,22011,22011,22011
9420,9420,9420,9420,10004,10818,11571,12268,12913,13517,14095,14651,15195,15740,16286,16841,17409,17998,18613,19262,19955,19861,19771,19684,19600,19521,19443,19369,19298,19487,19801,20113,20431,20753,21087,21431,21779,22011,22011,22011,22011
};

const int8 thirtyseventhTo42nd[5][41] = {
9420,9420,9420,9420,10004,10818,11571,12268,12913,13517,14095,14651,15195,15740,16286,16841,17409,17998,18613,19262,19955,19861,19771,19684,19600,19521,19443,19369,19298,19487,19801,20113,20431,20753,21087,21431,21779,22011,22011,22011,22011
9420,9420,9420,9420,10004,10818,11571,12268,12913,13517,14095,14651,15195,15740,16286,16841,17409,17998,18613,19262,19955,19861,19771,19684,19600,19521,19443,19369,19298,19487,19801,20113,20431,20753,21087,21431,21779,22011,22011,22011,22011
9420,9420,9420,9420,10004,10818,11571,12268,12913,13517,14095,14651,15195,15740,16286,16841,17409,17998,18613,19262,19955,19861,19771,19684,19600,19521,19443,19369,19298,19487,19801,20113,20431,20753,21087,21431,21779,22011,22011,22011,22011
9420,9420,9420,9420,10004,10818,11571,12268,12913,13517,14095,14651,15195,15740,16286,16841,17409,17998,18613,19262,19955,19861,19771,19684,19600,19521,19443,19369,19298,19487,19801,20113,20431,20753,21087,21431,21779,22011,22011,22011,22011
9420,9420,9420,9420,10004,10818,11571,12268,12913,13517,14095,14651,15195,15740,16286,16841,17409,17998,18613,19262,19955,19861,19771,19684,19600,19521,19443,19369,19298,19487,19801,20113,20431,20753,21087,21431,21779,22011,22011,22011,22011
};




const int8 temperatureArray[41] = {
40,41,42,43,44,45,46,47,48,49,50,51,52,53,54,
55,56,57,58,59,60,61,62,63,64,65,66,67,68,69,70,71,72,73,74,75,76,77,78,79,80
};

const int8 loadArray[41] = {
20,21,22,23,24,25,26,27,28,29,30,31,32,33,34,35,36,37,38,39,
40,41,42,43,44,45,46,47,48,49,50,51,52,53,54,55,56,57,58,59,60
};

signed int8 * SelectMatrixAndRowAndColumn(float _temperature, float _load)
{
   static int8 matrixAndRowAndColumn[3] = {0,0,0};
   // Select Matrix
   int8 matrixNumber = ((int8)(_temperature - temperatureArray[0]))/6;
   // Select Row
   int8 rowNumber = ((int8)(_temperature - temperatureArray[0]))%6;
   // Select Column
   int8 columnNumber = ((int8)(_load - loadArray[0])+1);
   
   matrixAndRowAndColumn[0] = matrixNumber+1;
   matrixAndRowAndColumn[1] = rowNumber;
   matrixAndRowAndColumn[2] = columnNumber;
   
   Return(matrixAndRowAndColumn);
}

float GetDataAndInterpolate(int8 _matrix, int8 _row, int8 _column, float _temperature, float _load)
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
         data1 = thirtyfirstTo36th[_row][_column-1];
         data2 = thirtyfirstTo36th[_row][_column];
         data3 = thirtyfirstTo36th[_row+1][_column-1];
         data4 = thirtyfirstTo36th[_row+1][_column];
         break;   
   }
   
   // 2D INTERPOLATION
   // Known Steps => can be more modular
   float x1 = (float)((int8)(_load));
   float x2 = (float)((int8)(_load)+1);
   float y1 = (float)((int8)(_temperature));
   float y2 = (float)((int8)(_temperature)+1);
   float fx_y1 = ((x2-_load)/(x2-x1))*data1 + ((_load-x1)/(x2-x1))*data2;
   float fx_y2 = ((x2-_load)/(x2-x1))*data3 + ((_load-x1)/(x2-x1))*data4;
   interpolatedData = ((y2-_temperature)/(y2-y1))*fx_y1 + ((_temperature-y1)/(y2-y1))*fx_y2;
   
   return interpolatedData;
}

void main()
{
   lcd_init();
   setup_psp(PSP_DISABLED);
   setup_timer_1(T1_DISABLED);
   setup_timer_2(T2_DISABLED,0,1);
   setup_ccp1(CCP_PWM); //4kHz PWM signal output at CCP1 pin 17
   setup_CCP2(CCP_OFF);
   setup_timer_2(T2_DIV_BY_16, 255, 1);
   delay_ms(10);
   setup_adc_ports(AN0_AN1_AN3); //A0 A1 A3 are configured for analog input pin
   setup_adc(ADC_CLOCK_DIV_32); //enable ADC and set clock for ADC
   
   set_tris_c(0x00); //set all portc pins as output

   while(1)
   {
      set_adc_channel(0); // next analog reading will be from channel 0
      delay_us(10); //allow time after channel selection and reading
      result_1 = read_adc(); //start and read A/D
      delay_us(10); //allow time after channel selection and reading
      set_adc_channel(1); // next analog reading will be from channel 1
      delay_us(10); //allow time after channel selection and reading
      result_2 = read_adc(); //start and read A/D
      delay_us(10); //allow time after channel selection and reading
      temperature = 0.0391*result_1+40;
      load = 0.0587*result_2-30;
      int8 * MatrixAndRowAndColumn;
      MatrixAndRowAndColumn=SelectMatrixAndRowAndColumn(temperature, load);
      float fuzzyOut = GetDataAndInterpolate(MatrixAndRowAndColumn[0], MatrixAndRowAndColumn[1], MatrixAndRowAndColumn[2], temperature, load);
      //unsigned int mypwm = (unsigned int)(fuzzyOut/100.0);
      float mypwm = (float)(fuzzyOut/100.0);
      printf(lcd_putc,"\fT:%f,l:%f\nout:%f",temperature,load,mypwm);
      //set_pwm1_duty(mypwm);
      delay_ms(100);
   }
}