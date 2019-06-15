//#include <stdint.h>
#include <math.h>
#include <float.h>

typedef unsigned short uint16_t;

#ifdef FAST_GET_TEMP
enum
{
  LOW2HIGHT = 0,
  HIGHT2LOW = 1
};
#endif
//uint8_t  temp_dire        = LOW2HIGHT;

/*******************************************************************************
* Function Name  : ntc_voltage_to_temp
* Description    : ��ѹֵת��Ϊ�¶�ֵ
* Input          : ���������ѹֵ
* Output         : None
* Return         : NTC���������¶�
*******************************************************************************/
double ntc_voltage_to_temp(uint16_t voltage)
{
  const double B    = 3435.0; // NTC���������Bֵ
  //const double B    = 3950.0; // NTC���������Bֵ
  const double R25  = 10.0; // ���µ���ֵ 10k��
  const double R    = 10.0; // ��ѹ����ֵ 10k��

  double       x    = (double)voltage; // ���������ѹֵ
  double       temp = 0.0; // �¶�ֵ
  double       r    = 0.0; // ����������ֵ

  //if(x>=3300.0)x = 2299.0;
  
  if(x < 16)//when T = 100 r = 48,voltage = 16mV
    x = 16;
  else if(x > 3000)
    x = 3000;

  r    = R * x /(3300.0-x);

  temp = 1.0 / ( log( r / R25 ) / B + 1.0 / 298.15 ) - 273.15; // ͨ���������ߣ�������ֵת��Ϊ�¶�ֵ
  temp = temp > 99.0 ? 99.0 : temp;
  temp = temp < 0.0 ? 0.0 : temp;

  return temp;
}

/******************************************************************************
* ������  : tempntc2water
* ����    : ��NTC���������¶�ת��Ϊˮ��
* ����    : NTC���������¶�
* ���    : ��
* ����    : ˮ��
* ����    : ���ⲿ����
* ��ע    : ��
-----------------------------------------------------------------------------*/
double tempntc2water( double t )
{
  double temp = 0.0;// ˮ��
  /*
   <35�Ȼ���û��ˮ������ˮˮ����С��
   ����ϵ��Ҳ�ϴ󣬸���ˮˮ���㣬����ϵ��С
  */
  if(t<35)
    temp = t + 1;
  else if(t<65)
    temp = 1.17*t + 1;
  else
    temp = 1.10*t + 0.5;
  // ��ȫ���.���ð���������
  temp = temp > 99.0 ? 99.0 : temp;
  temp = temp < 0.0 ? 0.0 : temp;

  return temp;
}

#ifdef FAST_GET_TEMP
/******************************************************************************
* ������  : ntc_temperature_predict
* ����    : ͨ��NTC������¶ȱ仯���ƣ�Ԥ��ˮ�µ�ֵ
* ����    : t0         ��1��NTC���������¶�ֵ
            t1         ��2��NTC���������¶�ֵ
            t2         ��3��NTC���������¶�ֵ
            direction  �仯����
* ���    : ��
* ����    : ˮ��
-----------------------------------------------------------------------------*/
double ntc_temperature_predict(double t2 , double t1 , double t0,uint8_t *direction)
{
  const double   K      = 72.0; // �ȴ���ϵ��
  uint32_t   period     = 2340; // 3s,��������
  double     temp       = 0.0; // ˮ��
  double     slope      = 0.0; // ����б��
  double     dif_t2t1   = 0.0;
  if(t2<t1 && t1<t0 && t2<t0)
  {
    *direction = HIGHT2LOW;
  }
  else
  {
    *direction = LOW2HIGHT;
  }
  
  //if((t2>t1 && t1>t0 && t2>t0) || (t2<t1 && t1<t0 && t2<t0)) // ���ݷ���һ��ʱ�ų���
  {
    slope = K * (double)( 1000.0 / (double)period ); // ͨ���ȴ���ϵ���Ͳ������ڼ���б��
    dif_t2t1  = t2 - t0;
    /* 
       <65���ˮ�仯���Ǵﲻ��1��,�ڿ��ٲ���ʱ�����ã�����ˮ���Ա����ټ�⵽
       ����ʱ�þ�ֵ����Ϊ���ٲ��������ڸ��²�����̫��������������ĸо�
    */
    if(dif_t2t1>=1)
      temp  = t1 + slope;//(t0+t1+t2)/3.0; 
    else
      temp  = t1 + slope * dif_t2t1; // ����ţ����ȴ����Ԥ��ˮ��
  }
  // ��ȫ���.���ð���������
  temp = temp > 99.0 ? 99.0 : temp;
  temp = temp < 0.0 ? 0.0 : temp;
  
  return temp;
}
#endif
