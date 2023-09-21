make_daemon: login_daemon.c
# Compile the program
	gcc -o login_daemon login_daemon.c
# Move the executable file to the /usr/sbin directory
	sudo mv login_daemon /usr/sbin/
# Copy "login_daemon.service", in the /etc/systemd/system directory
	sudo cp login_daemon.service /etc/systemd/system
# Reload the systemd daemon 
	sudo systemctl daemon-reload
# Enable the service to start at boot time 
	sudo systemctl enable login_daemon.service
# Start the service 
	sudo systemctl start login_daemon.service

$(info ************ View file /var/log/login_daemon.log after reboot ************)