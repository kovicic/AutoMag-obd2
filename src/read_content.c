#include "read_content.h" 
#include "dashboard.h"

static int htoi(char c)
{
	 int ret = 0;
	 
	 if('a'<= c && c <= 'f')
	 {
		ret = c - 'a' + 10;
	 }
	 else if('A' <= c && c <= 'F')
	 {
		ret = c - 'A' + 10;
	 }
	 else if('0' <= c && c <= '9')
	 {
		ret = c - '0';
	 }
	 
	 return ret;
}

void* read_Main(void * data)
{
	dashboard *db = (dashboard *)data;
	int i = 1;
	static  flag = 0;
	char try[4];
	int value = 0;
	printf(" value is %d\n", htoi(try[1]));
	
	while(1)
	{	
		if(db->count > 0) //&& flag > 0 )
		{
			//printf("ispred2:\n");
			pthread_mutex_lock(&db->lock);
			printf(" rpm %d \n kmh %d \n fuel %d \n", db->rpm, db->kmh, db->fuel);
			if(db->ID == 5500)
			{	//strcpy(try,db->kmh);
				//value = htoi(db->kmh[1]) + 10 * htoi(db->kmh[0]);
				//printf("%d\n\n",value);
				//printf(" DATA OF ID 5500 is %s ID %d\n",&db->kmh[4], db->count);
			}
			else if(db->ID == 4400)
			{
				//printf(" DATA OF ID 4400 is %s ID %d\n",&db->fuel[4], db->count);
			}
			//printf("READ data - %s i ID %d and cnt %d\n", &db->data[db->count],db->ID,db->count);
			db->count --;
			pthread_mutex_unlock(&db->lock);
			//printf("iza2:\n");
		}
	}
}

