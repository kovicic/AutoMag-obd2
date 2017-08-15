/*
 * Sean Middleditch
 * sean@sourcemud.org
 *
 * The author or authors of this code dedicate any and all copyright interest
 * in this code to the public domain. We make this dedication for the benefit
 * of the public at large and to the detriment of our heirs and successors. We
 * intend this dedication to be an overt act of relinquishment in perpetuity of
 * all present and future rights to this code under copyright law. 
 */

 #include "telnet-client.h"

static struct termios orig_tios;
static telnet_t *telnet;
static int do_echo;
static int8_t sockTelnet;
static char carberryPort[] = "7070";
static char localHost[] = "127.0.0.1";
static char turnLed1OnBuffer[] = "GPLED LED1 SET\n";
static char turnLed1OffBuffer[] = "GPLED LED1 CLEAR\n";
static char turnLed2OnBuffer[] = "GPLED LED2 SET\n";
static char turnLed2OffBuffer[] = "GPLED LED2 CLEAR\n";

static int8_t turnLedOnLenght = 15;
static int8_t turnLedOffLenght = 17;

static char telnetBuffer[50];
static int8_t telnetBufferLenght = 0;

static const telnet_telopt_t telopts[] = {
	{ TELNET_TELOPT_ECHO,		TELNET_WONT, TELNET_DO   },
	{ TELNET_TELOPT_TTYPE,		TELNET_WILL, TELNET_DONT },
	{ TELNET_TELOPT_COMPRESS2,	TELNET_WONT, TELNET_DO   },
	{ TELNET_TELOPT_MSSP,		TELNET_WONT, TELNET_DO   },
	{ -1, 0, 0 }
};

static void _cleanup(void) {
	tcsetattr(STDOUT_FILENO, TCSADRAIN, &orig_tios);
}

static void _input(char *buffer, int size) {
	static char crlf[] = { '\r', '\n' };
	int i;

	for (i = 0; i != size; ++i) {
		/* if we got a CR or LF, replace with CRLF
		 * NOTE that usually you'd get a CR in UNIX, but in raw
		 * mode we get LF instead (not sure why)
		 */
		if (buffer[i] == '\r' || buffer[i] == '\n') {
			if (do_echo)
				printf("\r\n");
			telnet_send(telnet, crlf, 2);
		} else {
			if (do_echo)
				putchar(buffer[i]);
			telnet_send(telnet, buffer + i, 1);
		}
	}
	fflush(stdout);
}

static void _send(int sock, const char *buffer, size_t size) {
	int rs;

	/* send data */
	while (size > 0) {
		if ((rs = send(sock, buffer, size, 0)) == -1) {
			fprintf(stderr, "send() failed: %s\n", strerror(errno));
			return (void)TEL_ERROR;
		} else if (rs == 0) {
			fprintf(stderr, "send() unexpectedly returned 0\n");
			return (void)TEL_ERROR;
		}

		/* update pointer and size to see if we've got more to send */
		buffer += rs;
		size -= rs;
	}
}

static void _event_handler(telnet_t *telnet, telnet_event_t *ev,
		void *user_data) {
	int sock = *(int*)user_data;
	int i = 0;
	switch (ev->type) {

	/* data received */
	case TELNET_EV_DATA:
		strcpy(telnetBuffer, ev->data.buffer);
		telnetBufferLenght = ev->data.size;
		fflush(stdout);
		break;

	/* data must be sent */
	case TELNET_EV_SEND:
		_send(sock, ev->data.buffer, ev->data.size);
		break;

	/* request to enable remote feature (or receipt) */
	case TELNET_EV_WILL:
		/* we'll agree to turn off our echo if server wants us to stop */
		if (ev->neg.telopt == TELNET_TELOPT_ECHO)
			do_echo = 0;
		break;

	/* notification of disabling remote feature (or receipt) */
	case TELNET_EV_WONT:
		if (ev->neg.telopt == TELNET_TELOPT_ECHO)
			do_echo = 1;
		break;

	/* request to enable local feature (or receipt) */
	case TELNET_EV_DO:
		break;

	/* demand to disable local feature (or receipt) */
	case TELNET_EV_DONT:
		break;

	/* respond to TTYPE commands */
	case TELNET_EV_TTYPE:
		/* respond with our terminal type, if requested */
		if (ev->ttype.cmd == TELNET_TTYPE_SEND) {
			telnet_ttype_is(telnet, getenv("TERM"));
		}
		break;

	/* respond to particular subnegotiations */
	case TELNET_EV_SUBNEGOTIATION:
		break;

	/* error */
	case TELNET_EV_ERROR:
		fprintf(stderr, "ERROR: %s\n", ev->error.msg);
		return (void)TEL_ERROR;
	default:
		/* ignore */
		break;
	}
}


void TEL_sendData(char* sendBuffer, int8_t bufferSize)
{
	_input(sendBuffer, bufferSize);
}

int8_t TEL_pollTelnet(char* receiveBuffer)
{

	char buffer[512];
	int8_t rs;
	memset(telnetBuffer, 0x0, strlen(telnetBuffer));

 	if ((rs = recv(sockTelnet, buffer, sizeof(buffer), 0)) > 0) 
 	{
 		telnet_recv(telnet, buffer, rs);
 	}

 	else if (rs == 0) 
 	{
 		return TEL_ERROR;
 	}
 	else 
 	{
 		fprintf(stderr, "recv(client) failed: %s\n",
		strerror(errno));
 		return TEL_ERROR;
 	}

	sprintf(receiveBuffer, "%.*s", telnetBufferLenght, telnetBuffer);

 	return TEL_SUCCESS; 
}

int8_t TEL_InitTelnetClient() {

	char buffer[512];
	int rs;
	struct sockaddr_in addr;
	struct pollfd pfd[2];
	struct addrinfo *ai;
	struct addrinfo hints;
	struct termios tios;

	/* look up server host */
	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	if ((rs = getaddrinfo(localHost, carberryPort, &hints, &ai)) != 0) {
		fprintf(stderr, "getaddrinfo() failed for %s: %s\n", localHost,
				gai_strerror(rs));
		return TEL_ERROR;
	}
	
	/* create server socket */
	if ((sockTelnet = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
		fprintf(stderr, "socket() failed: %s\n", strerror(errno));
		return TEL_ERROR;
	}

	/* bind server socket */
	memset(&addr, 0, sizeof(addr));
	addr.sin_family = AF_INET;
	if (bind(sockTelnet, (struct sockaddr *)&addr, sizeof(addr)) == -1) {
		fprintf(stderr, "bind() failed: %s\n", strerror(errno));
		return TEL_ERROR;
	}

	/* connect */
	if (connect(sockTelnet, ai->ai_addr, ai->ai_addrlen) == -1) {
		fprintf(stderr, "connect() failed: %s\n", strerror(errno));
		return TEL_ERROR;
	}

	/* free address lookup info */
	freeaddrinfo(ai);

	/* set input echoing on by default */
	do_echo = 1;

	/* initialize telnet box */
	telnet = telnet_init(telopts, _event_handler, 0, &sockTelnet);

	return TEL_SUCCESS;;
}

int8_t TEL_deInitTelentClient()
{
	/* clean up */
	telnet_free(telnet);
	close(sockTelnet);

	return TEL_SUCCESS;
}

void TEL_turnOnLed1()
{
	_input(turnLed1OnBuffer, turnLedOnLenght);
}

void TEL_turnOffLed1()
{
	_input(turnLed1OffBuffer, turnLedOffLenght);
}
void TEL_turnOnLed2()
{
	_input(turnLed2OnBuffer, turnLedOnLenght);
}
void TEL_turnOffLed2()
{
	_input(turnLed2OffBuffer, turnLedOffLenght);
}