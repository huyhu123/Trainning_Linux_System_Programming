main: file_locking_sh.c file_locking_ex.c 
	gcc file_locking_ex.c -o file_locking_ex
	gcc file_locking_sh.c -o file_locking_sh
	gcc Iterative_UDP_echo_Server.c -o Iterative_UDP_echo_Server
	gcc Iterative_UDP_echo_Client.c -o Iterative_UDP_echo_Client
	gcc Concurrent_TCP_echo_Server.c -o Concurrent_TCP_echo_Server -pthread
	gcc Concurrent_TCP_echo_Client.c -o Concurrent_TCP_echo_Client -pthread

clean:
	rm -rf file_locking_ex file_locking_sh Iterative_UDP_echo_Server Iterative_UDP_echo_Client Concurrent_TCP_echo_Server Concurrent_TCP_echo_Client