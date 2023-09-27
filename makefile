all: 
	+$(MAKE) -C Signals_and_Timers/Signal_ignore main
	+$(MAKE) -C Signals_and_Timers/Send_and_handle_signal main
	+$(MAKE) -C Processes_and_Threads/Matrix_multi_thread main
	+$(MAKE) -C Memory_Management_in_Linux main
	+$(MAKE) -C Linux_File_System_and_system_calls/Create_hard_link_and_soft_link main	
	+$(MAKE) -C Linux_File_System_and_system_calls/Print_file_name main	
	+$(MAKE) -C Linux_File_System_and_system_calls/Write_string_to_file main	

clean:
	+$(MAKE) -C Signals_and_Timers/Signal_ignore clean
	+$(MAKE) -C Signals_and_Timers/Send_and_handle_signal clean
	+$(MAKE) -C Processes_and_Threads/Matrix_multi_thread clean
	+$(MAKE) -C Memory_Management_in_Linux clean
	+$(MAKE) -C Linux_File_System_and_system_calls/Create_hard_link_and_soft_link clean	
	+$(MAKE) -C Linux_File_System_and_system_calls/Print_file_name clean	
	+$(MAKE) -C Linux_File_System_and_system_calls/Write_string_to_file clean