CIS3110 Assignment 1

Author: Bejamin Mcconville
###############################################################################
References: 
1.
line 37:	getline(&line,&length,stdin)
being used inside the malloc on line 37.
Form: http://man7.org/linux/man-pages/man3/getline.3.html
Quote-"On success, getline() and getdelim() return the number of characters
       read, including the delimiter character.."
0.5% of code.

2.	pid = fork();
	if(pid==0){
		else if(execvp(args[0], args) ==-1){
			perror("shell");
		}
		exit(0);
	else if(pid<0){
                perror("shell");
	
	}
	else{
		do{
		waitpid(pid,&status, WUTRACED);
		}while(!WIFEXITED(status)&&!WIFSIGNALED(status));
	}
1% of code.
Used inside the sh_launch function.
From: http://stephen-brennan.com/2015/01/16/write-a-shell-in-c/

3. 
line 169:	void handler(int sig){
 
   		pid_t pid;

   		pid=wait(NULL);

   		printf("Pid %d exited.\n",pid);

   		signal(SIGCHLD, SIG_IGN);
}
and line 124    signal(SIGCHLD,handler);
0.5% of code.
used for &
From: http://www.linuxquestions.org/questions/programming-9/how-a-father-process-know-which-child-process-send-the-signal-sigchld-266290/

4.
line 87:	dup2(pd[0],0);
line 93:	execlp(args[pos+1],args[pos+1],args[pos+1], NULL);
and
line 110:	dup2(pd[1],1);
line 116: 	
execlp(tempArgs[0],tempArgs[0],tempArgs[1],NULL);
0.5% of code
Used for piping insoide of sh_launch
From: http://stackoverflow.com/questions/2191414/implement-piping-using-c-fork-used

Note: The % of code is based on lines inside of code, not on the usefulness and importants 
of each line relative to the rest of the implementation.
###############################################################################

How to run:
	-type make while inside the folder
	To execute shell:
	-type ./CIS3110sh to execute the program on linux
	-On the command line after type the variables to be calculated

##########################################################################################
Files included:
	shell.c		makefile

##########################################################################################
The program only implements sections 1 and 2. Not 3.

Section 1.1:
Test case 1:
> exit
logout

[Process completed]
Test case 2:
> kill
logout

[Process completed]

Section 1.2:
Test case 1:
a.out         changes.txt              foo       oldshell.txt  shell.c
Algo          CIS3110sh.exe            makefile  OS            test
bullitin.txt  CIS3110sh.exe.stackdump  mcconvil  readme.txt    test.c
Test case 2:
> date
Mon, Feb 11, 2016  9:13:17 PM

Section 1.3:
Test case 1:
> ls -l
total 113
-rw-rw-r--+ 1 betty None     0 Feb 15 20:29 a.out
drwxrwx---+ 1 betty None     0 Feb  9 18:54 Algo
-rwxrwx---+ 1 betty None  1450 Feb  6 11:13 bullitin.txt
-rwxrwx---+ 1 betty None  2835 Feb 14 10:47 changes.txt
-rwxrwxr-x+ 1 betty None 68886 Feb 15 21:08 CIS3110sh.exe
-rwxrwx---+ 1 betty None   501 Feb 15 17:56 CIS3110sh.exe.stackdump
-rw-rw-r--+ 1 betty None     4 Feb 15 12:00 foo
-rw-rw-r--+ 1 betty None   118 Feb 13 14:48 makefile
drwxrwx---+ 1 betty None     0 Feb  6 11:30 mcconvil
-rwxrwx---+ 1 betty None  3064 Feb 13 15:10 oldshell.txt
drwxrwx---+ 1 betty None     0 Feb  4 13:17 OS
-rwxrwx---+ 1 betty None  3101 Feb 15 21:14 readme.txt
-rwxrwxr--+ 1 betty None  6086 Feb 15 20:58 shell.c
-rw-rw-r--+ 1 betty None     0 Feb 14 21:04 test
-rwxrwx---+ 1 betty None   461 Feb  6 11:08 test.c

Section 1.4:
Test case 1:
> ls &
[1] 3060
> a.out       changes.txt              foo       oldshell.txt  shell.c
Algo          CIS3110sh.exe            makefile  OS            test
bullitin.txt  CIS3110sh.exe.stackdump  mcconvil  readme.txt    test.c
Pid 3060 exited.

Section 2.3:
Test case 1:
input: > ls -l > foo
output in file foo:
total 97
drwxrwx---+ 1 betty None     0 Feb  9 18:54 Algo
-rwxrwxr-x+ 1 betty None 69608 Feb 15 23:00 CIS3110sh.exe
-rwxrwx---+ 1 betty None  3856 Feb 15 22:45 files.rar
-rw-rw-r--+ 1 betty None     0 Feb 15 23:01 foo
-rw-rw-r--+ 1 betty None   118 Feb 13 14:48 makefile
drwxrwx---+ 1 betty None     0 Feb 15 21:29 mcconvil
drwxrwx---+ 1 betty None     0 Feb  4 13:17 OS
-rwxrwx---+ 1 betty None  4285 Feb 15 21:23 readme.txt
-rwxrwxr--+ 1 betty None  6167 Feb 15 22:55 shell.c

Section 2.4:
Test case 1:
> sort < foo
a
a
d
e
f
f
q
Inside file foo:
a
f
d
a
e
f
q

Section 2.5:
Test case 1:
> ls -l | more
total 101
drwxrwx---+ 1 betty None     0 Feb  9 18:54 Algo
-rwxrwxr-x+ 1 betty None 68674 Feb 15 23:34 CIS3110sh.exe
-rwxrwx---+ 1 betty None   320 Feb 15 23:18 CIS3110sh.exe.stackdump
-rwxrwx---+ 1 betty None  3856 Feb 15 22:45 files.rar
-rw-rw-r--+ 1 betty None   653 Feb 15 23:34 foo
-rw-rw-r--+ 1 betty None   118 Feb 13 14:48 makefile
drwxrwx---+ 1 betty None     0 Feb 15 21:29 mcconvil
drwxrwx---+ 1 betty None     0 Feb  4 13:17 OS
-rwxrwx---+ 1 betty None  4954 Feb 15 23:36 readme.txt
-rwxrwxr--+ 1 betty None  5984 Feb 15 23:34 shell.c
-rw-rw-r--+ 1 betty None    21 Feb 15 23:33 test
-rwxrwx---+ 1 betty None    18 Feb 15 23:18 testfile.txt
Test case 2:
> ls -l | grep .txt
-rwxrwx---+ 1 betty None  4954 Feb 15 23:36 readme.txt
-rwxrwx---+ 1 betty None    18 Feb 15 23:18 testfile.txt


##########################################################################################
For section 2 # 5: There is an error when piping that if you type something like ls -a -l | more
It will execute then exit the shell.

