# System Stats Application

This application is used to display the real-time system statistics of a Linux machine. N samples of data are taken and displayed every T seconds. The stats that can be displayed are:

- System usage:  Reports the current amount of CPU utilization and the amount of used and free memory
- User usage:   Reports the session(s) the current users are connected to 

# Requirements:
- This program is only intended to be used on a Linux machine
- The size of the terminal should be able to fit ALL of the lines in one page (without scrolling)
- A C compiler

# To run the program:
1) Compile the program by typing the 'make' command
2) Run the executable that was created, with flags if wanted
      eg. Without flags: ./systemStats
      eg. With flags: ./systemStats --system --graphics

Optional flags:
--system        |Only generate system usage

--user          |Only generate user usage

--graphics      |Generate graphics

--sequential    |Output data sequentially

--samples=N     |N is an integer that indicates how many samples are going to be taken and displayed

--tdealy=T      |T is an integer that indicates the amount of time (in seconds) that passes  between each data output

Positional arguments:
--samples=N and --tdealy=T can be used as positional arguments if no flag is indicated in the corresponding order:N T
Positional arguments must be specified as the FIRST arguments.


Default (no flags): 10 samples of system usage and user usage will be displayed non-sequentially, without graphics. 
                    There will be a 1 second delay between each sample.

CLA Misusages: 
            For --tdelay=T and --samples=N, T and N MUST be integers.
            T and N should NOT be set more than once.

            For positional arguments, N and T are expected as the FIRST arguments in that EXACT order. If they are located anywhere else, it 
            would not be considered a valid positional argument. Below are some examples:
            ./systemStats 100                  will set N to 100
            ./systemStats --user 4             will be invalid because 4 is not the first argument

