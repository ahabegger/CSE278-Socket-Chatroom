main: SocketClient.cpp SocketServer.cpp
	g++ -o Client -pthread SocketClient.cpp
	g++ -o Server -pthread SocketServer.cpp -lmysqlpp