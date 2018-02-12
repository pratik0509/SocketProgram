all:	server client
	echo 'Built everything from scratch!'
server:
	g++ Q1/Server/server.cpp -o Q1/Server/server
	g++ Q2/Server/server_nonpersistent.cpp -o Q2/Server/server_nonpersistent
	g++ Q2/Server/server_persistent.cpp -o Q2/Server/server_persistent
	echo 'Server successfully built!'
client:
	g++ Q1/Client/client.cpp -o Q1/Client/client
	g++ Q2/Client/client_nonpersistent.cpp -o Q2/Client/client_nonpersistent
	g++ Q2/Client/client_persistent.cpp -o Q2/Client/client_persistent
	echo 'Client successfully built!'
