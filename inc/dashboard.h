#ifndef GLOBAL_H_
#define GLOBAL_H_

#include <stdint.h>

//!< Lamps
#define DASHBOARD_ABS         0x001
#define DASHBOARD_ESP         0x002
#define DASHBOARD_HANDBRAKE   0x004
#define DASHBOARD_ENGINE      0x008
#define DASHBOARD_WARN        0x010
#define DASHBOARD_WARN_BLINK  0x020
#define DASHBOARD_FUEL_LOW    0x040
#define DASHBOARD_LEFT        0x080
#define DASHBOARD_RIGHT       0x100

//!< Alarm
#define DASHBOARD_LOW_OIL        0x01
#define DASHBOARD_TIRE_PRESSURE  0x02
#define DASHBOARD_TIRE_DEFECT    0x04


typedef struct _dashboard
{
	int8_t channel;
	int boudrate;
	int ID;
	char data[100];
	uint32_t kmh;
	uint32_t  fuel;
	int32_t rpm;
	pthread_mutex_t lock;
	int count;
	int head;
	int tail;
	int8_t align;
}dashboard;

#endif 

