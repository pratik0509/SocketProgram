all:	server
	echo 'Built everything from scratch!'
server:
	g++ src/server.cpp -o test/server/server.out
	echo 'Server successfully built!'
