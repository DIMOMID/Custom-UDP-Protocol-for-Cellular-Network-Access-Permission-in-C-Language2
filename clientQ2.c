/**************************************************\
|*   Computer Network Assignment 2 (Client)  R1   *|
|*   Omid Almasi                                  *|
|*   SCU Student ID: xxx-xxxx                     *|
|*   Professor Moataghedi                         *|
|*   Refer to README.txt for how to run the code  *|
\**************************************************/


//********************** Importing necessary standard headers **********************//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdint.h>
#include <sys/time.h>
#include <strings.h>

// Constants defination per question 2 for each case
#define SERVER_PORT 5050
#define PERMITTED 0XFFFB
#define UNPAID 0XFFF9
#define NON_EXISTENT 0XFFFA
#define TECH_MISMATCH 0XFFFC

//Data packet structure for request:
typedef struct {
	uint16_t id;
	uint8_t client;
	uint16_t permission;
	uint8_t segNo;
	uint8_t pktLength;
	uint8_t tech;
	unsigned long subscriberNum;
	uint16_t endId;
} PacketRequest;

//Data packet structure for response:
typedef struct {
	uint16_t id;
	uint8_t client;
	uint16_t responseType;
	uint8_t segNo;
	uint8_t pktLength;
	uint8_t tech;
	unsigned long subscriberNum;
	uint16_t endId;
} PacketResponse;

void displayPacket(PacketRequest packet) {
	printf("=*=*=*Packet ID: %x\n", packet.id);
	printf("=*=*=*Client ID: %hhx\n", packet.client);
	printf("=*=*=*Access Permission: %x\n", packet.permission);
	printf("=*=*=*Segment No: %d\n", packet.segNo);
	printf("=*=*=*Packet Length: %d\n", packet.pktLength);
	printf("=*=*=*Technology: %d\n", packet.tech);
	printf("=*=*=*Subscriber No: %lu\n", packet.subscriberNum);
	printf("=*=*=*End of Packet ID: %x\n", packet.endId);
}

PacketRequest preparePacket() {
	PacketRequest packet;
	packet.id = 0XFFFF;
	packet.client = 0XFF;
	packet.permission = 0XFFF8;
	packet.endId = 0XFFFF;
	return packet;
}

int main(int argc, char** argv) {
	PacketRequest reqPacket;
	PacketResponse resPacket;
	char buffer[30];
	int counter = 1;
	FILE* file;

	int socketFd, status = 0;
	struct sockaddr_in addr;
	socklen_t addrSize;
	socketFd = socket(AF_INET, SOCK_DGRAM, 0);

	// set Ack_timer per questions 2
	struct timeval timeout;
	timeout.tv_sec = 3;
	timeout.tv_usec = 0;

	setsockopt(socketFd, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout));

	if (socketFd < 0) {
		printf("Failed to connect.\n");
		return -1;
	}

	memset(&addr, 0, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = htonl(INADDR_ANY);
	addr.sin_port = htons(SERVER_PORT);
	addrSize = sizeof(addr);

	reqPacket = preparePacket();

	//open input.txt per question2 to simulate the different clients. It contains Subscriber No.|Technology|Paid/not Paid status 
	file = fopen("input.txt", "rt");
	if (!file) {
		printf("Unable to open file.\n");
		return -1;
	}

	while (fgets(buffer, sizeof(buffer), file)) {
		int attempts = 0;
		status = 0;
		printf("\n=*=*=*=*=*=* New Packet =*=*=*=*=*=*\n");

		char* token;
		token = strtok(buffer, " ");
		reqPacket.pktLength = strlen(token);
		reqPacket.subscriberNum = (unsigned long)strtol(token, NULL, 10);
		token = strtok(NULL, " ");
		reqPacket.pktLength += strlen(token);
		reqPacket.tech = atoi(token);
		reqPacket.segNo = counter;

		displayPacket(reqPacket);

		while (status <= 0 && attempts < 3) {
			sendto(socketFd, &reqPacket, sizeof(PacketRequest), 0, (struct sockaddr*)&addr, addrSize);
			status = recvfrom(socketFd, &resPacket, sizeof(PacketResponse), 0, NULL, NULL);

			if (status <= 0) {
				printf("No response.\n");
				attempts++;
			}
			else {
				printf("Status: ");
				switch (resPacket.responseType) {
				case UNPAID: printf("Unpaid subscription.\n"); break;
				case TECH_MISMATCH: printf("Technology mismatch.\n"); break;
				case NON_EXISTENT: printf("Subscriber does not exist.\n"); break;
				case PERMITTED: printf("paid subscription, access granted.\n"); break;
				default: printf("Undefined status.\n");
				}
			}
		}

		if (attempts >= 3) {
			printf("Server does not repsond.\n");
			break;
		}
		counter++;
	}

	fclose(file);
	return 0;
}
