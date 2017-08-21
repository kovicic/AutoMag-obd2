#include "telnet-client.h"
#include "can_library.h"
#include "initialize.h"
#include "read_content.h"
#include "dashboard.h"



int main()
{
	dashboard data;
	data.head = 0;
	data.count = 0;
	pthread_mutex_init(&data.lock, NULL);
	
    pthread_t initReceive;
    pthread_t readContent;

	pthread_create(&initReceive, 0, init_Main, (void *)&data);
	pthread_create(&readContent, 0, read_Main, (void *)&data);

	
	pthread_join(initReceive, 0);
	pthread_join(readContent, 0);
	pthread_mutex_destroy(&data.lock);
    return 0;
}
