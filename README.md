# Custom-UDP-Protocol-for-Cellular-Network-Access-Permission-in-C-Language - Q2 Response
/**************************************************\

|*   Computer Network Assignment 1 (Client)       *|

|*   Omid Almasi - 8/14/2023                      *|

|*   SCU Student ID: xxxxxxxxx                    *|

|*   Professor Moataghedi                         *|

|*   Refer to README.txt for how to run the code  *|

\**************************************************/
## Breif Introduction
Leveraging the power of UDP, we've developed a specialized communication protocol facilitating device identifications and access permissions. In the era of seamless connectivity, our protocol acts as a bridge, ensuring efficient and effective communication between a cell phone (client's software module) and the cellular network (server).

## Features:

Packet Structure: Our packets have unique identifiers (0XFFFF) for both the start and end, ensuring proper delineation.

Device Identification: Each client has a distinct ID supporting up to 255 unique devices.

Access Permission Request: The Acc_Per (0XFFF8) field specifies the access permission request, also capturing technology type (2G, 3G, 4G, 5G) and a maximum subscriber number of 0XFFFFFFFF (4294967295 Decimal).

Server Verification: The server cross-references the "Verification_Database.txt", containing data like Subscriber Number, Technology, and Payment Status.

Responsive Feedback: The server generates feedback:

Not Paid: Informing the client if a subscription isn't paid.

Not Exist: Communicating if the subscriber number doesn't exist or if the technology doesn't match.

Access_OK: Confirming when a subscriber is permitted to access the network.

## Workflow:

A client's software module (representing a cell phone) sends an identification request.

The server evaluates the request against the "Verification_Database.txt".

Based on the verification, the server sends a response - either granting permission, indicating payment issues, or stating non-existence in the database.

Imagine a world where your cell phone intelligently communicates with the network, ensuring seamless access and maintaining the integrity of the service. This protocol aims to do just that - enabling efficient, reliable, and secure connectivity for devices.

## How to run the code

The following are instruction for runnning both client.c & server.c for question #2:

1. Launch Terminal:
	* Start your Ubuntu terminal.
2. Open the Code:
	* Run the command code . within the terminal to open Visual Studio Code in the current directory.
3. Navigate to Source Directory:
	* In Visual Studio Code, navigate to the folder where both client.c and server.c files are located.
	* Right-click the folder and choose "Open in Integrated Terminal".
4. Verification_Database.txt & input.txt files:
	* Create a database.txt file with following format:
		Subscriber No | Technology | Paid/unpaid
	* Create an input.txt file with following format:
		Subscriber No | Technology | Paid/unpaid
	* Note that input.txt shall include mismath information compared to Verification_database.txt to simulate all Subscriber conditions.
5. Setup Terminal:
	* Split the integrated terminal by clicking the split terminal button (usually represented by a square and arrow) to simulate both client and server concurrently.
6. Compile the Server Code:
	* Enter the following command to build and compile the server:
	"gcc -o server server.c"
7. Compile the Client Code:
	* Enter the following command to build and compile the client:
	* "gcc -o client client.c"
8. Run the Server:
	* First, execute the server by entering the following command:
	* "./server <port number>"
	* For example: "./server "
9. Run the Client:
	* With the server running, switch to the other terminal pane. Execute the client with:
	* ./client <host name> <port number>
	* For example: "./client "
