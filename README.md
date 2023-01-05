# Client Server Application

## How to run the project
* To run this project any distribution of the Linux operating system should be installed in your system.

* Before running the project enter the input text in the ***intext.txt*** file.

* After entering the input text in the file then enter the below commands to run the project.
    
* First, we should run the server so, open a new terminal and run the below commands.

        gcc server.c -o server
        gcc receiver.c -o receiver -lpthread -lrt
        ./server
    ***The execl system call is used in the project, so don't change the execution file name while running the project. It throws an error if the file name changes.***
* After executing the above commands server is running in one terminal. Now we should run the client. 

* To run the client open a new terminal and run the below commands.
	
        gcc physical.c -o physical
        gcc dataLink -o dataLink
        gcc client.c -o client
        gcc main.c -o main
        ./main

* The output for the given text can be seen in the ***result.txt*** file or in the terminal.



