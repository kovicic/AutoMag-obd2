#include "telnet-client.h"
#include "can_library.h"
#include "initialize.h"
#include "dashboard.h"

#define PID0020 "0100"
#define PID2140 "0120"
#define PID4160 "0140"

#define PID_RPM             "1111"
#define PID_SPEED           "1111"
#define PID_COOLANT_TEMP    "0105"
#define PID_FUEL_LEVEL      "012F"


int main()
{

    pthread_t tryThread1;

	pthread_create(&tryThread1, 0, init_Main, 0);

	pthread_join(tryThread1, 0);
    return 0;
}
