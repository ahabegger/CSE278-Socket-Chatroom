# CSE 278 Project

CHATTING ROOM (2 PEOPLE REQUIREMENTS) REPORT

Team Members : Alex Habegger (habeggaj) and Nathan Abdoo (abdoojm)\
Contact Email : habeggaj@miamioh.edu, abdoojm@miamioh.edu\
Class : CSE 278B Systems I\
Professor : Xianglong Feng

INCLUDED FUNCTIONALITIES

- Login check
- Chatting freely - Type anytime, receive messages instantly
	- Send messages to all users - (Enter message as normal)
	- Send message privately to one user - (/p user message)
- Send files
	- Send files to all users - (/f file.txt)
	- Send file privately to one user - (/u user file.txt)
- Exit client (without Ctrl + C) - (/e OR /exit)


FILES AND FUNCTIONS

1. SocketServer.cpp
	- thread_server (int socket) 
		- Check if received message is loginAttempt
	- loginAttempt (char* loginInfo, int socket)
		- SQL Query of Database
	- thread_Send (int socket)
	- thread_Receive (int socket)
		- Check if received message is loginAttempt
	- int main (int argc, char const *argv[])
		- Creates Threads
2. SocketClient.cpp
	- thread_Send (int socket)
		- Checks for /e or /exit
			- Exits the client
		- Checks for /f file.txt 
			- Checks if file.txt exists
			- Reads file.txt 
			- Sends file.txt and usertitle to Server
		- Checks for /u targetUser file.txt   
			- Checks if file.txt exists
			- Reads file.txt 
			- Sends file.txt and usertitle and targetUser to Server
	- thread_Receive (int socket)
		- Checks for /f file.txt 
			- Creates file.txt
			- Prints file sent message
		- Checks for /u targetUser file.txt   
			- Checks if targetUser = usertitle
			- Creates file.txt if true 
			- Prints file sent message with “(PM)” if true
		- Checks for /p targetUser 
			- Checks if targetUser = usertitle
			- Prints message plus “(PM)” if true
	- checkLogin (string username, string password, int socket)
		- Sends loginAttempt to Server using /login-Username_Password#
			- Creates usertitle to username 
	- main (int argc, char const *argv[])
		- Creates a checkLogin Loop
		- Creates Send and Receive Threads
3. Makefile
	- Compiles both the SocketServer.cppp and SocketClient.cpp


