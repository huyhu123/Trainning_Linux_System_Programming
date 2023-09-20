CC = gcc
INC = -I./inc
SRC = ./src/*.c

all: login_deamon.c
# Compile the program
	gcc -o login_daemon login_daemon.c
# Move the executable file to the /usr/sbin directory:
	sudo mv login_daemon /usr/sbin/
# Create a new systemd service file, let's call it "login_daemon.service", in the /etc/systemd/system directory
	sudo cp login_daemon.service /etc/systemd/system
# Reload the systemd daemon
	sudo systemctl daemon-reload
# Enable the service to start at boot time
	sudo systemctl enable login_daemon.service
# Start the service
	sudo systemctl start login_daemon.service
# Now, the login daemon will run in the background and log user login information to the system log. You can view the log using the following command: 
	sudo tail -f /var/log/syslog