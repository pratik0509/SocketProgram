all:	server client
	echo 'Built everything from scratch!'
server:
	g++ src/Q1/Server/server.cpp -o src/Q1/Server/server
	g++ src/Q2/Server/server.cpp -o src/Q2/Server/server
	echo 'Server successfully built!'
client:
	g++ src/Q1/Client/client.cpp -o src/Q1/Client/client
	g++ src/Q2/Client/client.cpp -o src/Q2/Client/client
	echo 'Client successfully built!'
