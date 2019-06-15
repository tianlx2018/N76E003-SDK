

#ifndef _TEMP_H
#define _TEMP_H

//#include <stdint.h>
#include "float.h"
double ntc_voltage_to_temp(uint16_t voltage);
double ntc_temperature_predict(double t2 , double t1 , double t0,uint8_t *direction);
double tempntc2water( double t );

#endif /*_TEMP_H */


