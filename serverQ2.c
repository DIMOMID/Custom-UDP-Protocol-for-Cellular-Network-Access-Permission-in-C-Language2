/**************************************************\
|*   Computer Network Assignment 2 (Server) R1    *|
|*   Omid Almasi                                  *|
|*   SCU Student ID: xxx-xxxx                     *|
|*   Professor Moataghedi                         *|
|*   Refer to README.txt for how to run the code  *|
\**************************************************/


//********************** Importing necessary standard headers **********************//
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// Constants defination per question 2 for each case
#define PORT 5050
#define PACKET_LENGTH 10
#define PAID                 0XFFFB
#define NOTPAID              0XFFF9
#define DOESNOTEXIST         0XFFFA
#define TECHNOLOGYMISMATCH   0XFFFC

//Data packet structure for request:
typedef struct {
	uint16_t packetID;
	uint8_t clientID;
	uint16_t accPermission;
	uint8_t segmentNo;
	uint8_t packetLength;
	uint8_t techType;
	unsigned long long subscriberNo;
	uint16_t endPacketID;
} RequestPacket;

//Data packet structure for response:
typedef struct {
	uint16_t packetID;
	uint8_t clientID;
	uint16_t responseType;
	uint8_t segmentNo;
	uint8_t packetLength;
	uint8_t techType;
	unsigned long subscriberNo;
	uint16_t endPacketID;
} ResponsePacket;

//Data packet for subscriber status:
typedef struct {
	unsigned long subscriberNo;
	uint8_t techType;
	int status;
} SubscriberData;

void displayPacketContent(const RequestPacket packet) {
	printf("=*=*=*Packet ID: %x\n", packet.packetID);
	printf("=*=*=*Client ID: %hhx\n", packet.clientID);
	printf("=*=*=*Access Permission: %x\n", packet.accPermission);
	printf("=*=*=*Segment No: %d\n", packet.segmentNo);
	printf("=*=*=*Packet Length: %d\n", packet.packetLength);
	printf("=*=*=*Technology: %d\n", packet.techType);
	printf("=*=*=*Subscriber No: %llu\n", packet.subscriberNo);
	printf("=*=*=*End of Packet ID: %x\n", packet.endPacketID);
}

// Loading/reading database file (Verification_Database.txt)
void loadSubscriberData(SubscriberData database[]) {
	char line[18];
	int i = 0;
	FILE* file = fopen("Verification_Database.txt", "rt");

	if (!file) {
		printf("Error opening file.\n");
		return;
	}

	while (fgets(line, sizeof(line), file) != NULL) {
		char* token = NULL;
		token = strtok(line, " ");
		database[i].subscriberNo = (unsigned)atol(token);
		token = strtok(NULL, " ");
		database[i].techType = atoi(token);
		token = strtok(NULL, " ");
		database[i].status = atoi(token);
		i++;
	}
	fclose(file);
}


// subscriber verification from database (Verification_Database.txt)

int subscriberStatus(const SubscriberData database[], unsigned int subscriberNo, uint8_t techType) {
	for (int i = 0; i < PACKET_LENGTH; i++) {
		if (database[i].subscriberNo == subscriberNo) {
			if (database[i].techType == techType)
				return database[i].status;
			else return 2;
		}
	}
	return -1;
}

// Data packet to Create Response 
ResponsePacket createResponse(const RequestPacket request) {
	ResponsePacket response;
	response.packetID = request.packetID;
	response.clientID = request.clientID;
	response.segmentNo = request.segmentNo;
	response.packetLength = request.packetLength;
	response.techType = request.techType;
	response.subscriberNo = request.subscriberNo;
	response.endPacketID = request.endPacketID;
	return response;
}

int main() {
	RequestPacket request;
	ResponsePacket response;
	SubscriberData database[PACKET_LENGTH];

	loadSubscriberData(database);

	int socketDescriptor;
	struct sockaddr_in serverAddr;
	struct sockaddr_storage serverStorage;
	socklen_t addrSize;

	socketDescriptor = socket(AF_INET, SOCK_DGRAM, 0);

	memset(&serverAddr, 0, sizeof(serverAddr));
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	serverAddr.sin_port = htons(PORT);

	bind(socketDescriptor, (struct sockaddr*)&serverAddr, sizeof(serverAddr));
	addrSize = sizeof serverAddr;

	printf("Server is now up and running...\n");

	while (1) {
		int received = recvfrom(socketDescriptor, &request, sizeof(RequestPacket), 0, (struct sockaddr*)&serverStorage, &addrSize);
		displayPacketContent(request);
		if (request.segmentNo == 11) break;
		if (received > 0 && request.accPermission == 0XFFF8) {
			response = createResponse(request);
			//checking for all 4 condition of question 2
			int status = subscriberStatus(database, request.subscriberNo, request.techType);

			switch (status) {
			case 0:
				printf("Unpaid subscription.\n");
				response.responseType = NOTPAID;
				break;
			case 1:
				printf("paid subscription, access granted.\n");
				response.responseType = PAID;
				break;
			case -1:
				printf("Subscriber does not exist.\n");
				response.responseType = DOESNOTEXIST;
				break;
			default:
				printf("Technology mismatch.\n");
				response.responseType = TECHNOLOGYMISMATCH;
				break;
			}
			sendto(socketDescriptor, &response, sizeof(ResponsePacket), 0, (struct sockaddr*)&serverStorage, addrSize);
		}
		printf("\n=*=*=*=*=*=* New Packet =*=*=*=*=*=*\n");

	}
}
