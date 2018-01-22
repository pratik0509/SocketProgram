all:	server client
	echo 'Built everything from scratch!'
server:
	g++ src/server.cpp -o test/server/server.out
	echo 'Server successfully built!'
client:
	g++ src/client.cpp -o test/client/client.out
	echo 'Client successfully built!'
