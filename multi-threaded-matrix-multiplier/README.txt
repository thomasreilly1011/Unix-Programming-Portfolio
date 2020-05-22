UNIX Assignment 3 submission for Thomas Reilly - 18483722

------------------------------------------------------ NOTES FOR ASSESOR ------------------------------------------------------
1)
I have included a Makefile. Both programs can be compiled with the command:
make all
Or they may be compiled seperately with the commands:
make master
make worker

2)
The Master and Worker work as two seperate programs.

Master must be called once with the command:
./master n p
Substituting n and p for the relevant numeric values.

N.B. Worker must be called p times with the command:
./worker hostname
Hostname is optional, if no arguments are given, the localhost is presumed.

3)
The reason I chose this proceedure is because it makes the most sense in that each of worker processes can be created on different, off-site, machines.

4)
Note: Upon testing the program before submission all features mentioned in the assignment specification worked approprietly.
The server in master runs concurrently using pthreads so a UNIX machine is required to run them.
Please let me know (given that you have the time) if there are any issues with compilation.
You can contact me on Moodle or by email: thomas.reilly@ucdconnect.ie
Thank you.
