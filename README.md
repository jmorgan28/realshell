# realshell
by Jackson Morgan, Suwamik Paul,and Fortune Soleil

Features:
	Fork and executes commands
	You can exit the program
	You can change directories
	Parses on semicolon
	Uses < and > for redirection
	You can use | for pipes

Attempted:
	We did not attempt more than the requirments.
	Bogged down by college apps, tests, and procrastination

Bugs:
	When you just call cd without a path the program crashes
	cd must be first function in a chain

Files & Function Headers:
shell.c
	Contains all functions

	/*========int execute(char * a)=========
	Inputs: char * a
	Returns: int (0)

	Takes in a command
	Parses along space
	If cd does special case
	if exit does special case
	Else calls execvp
	====================*/


	/*======== void reOut(char* a, char* file) ==========
	Inputs: char* a
		char* file
	Returns: Nothing! It is void.

	Redirection for >
	Puts output of command to file
	====================*/

	/*======== void reIn(char* a, char* file) ==========
	Inputs: char* a
		char* file
	Returns: Nothing! It is void.

	Redirection for <
	Puts output of command to file
	====================*/

	/*======== char ** parsesemi(char * a, char * spliter) ==========
	Inputs: char* a
		char* spliter
	Returns: char **

	Takes in a line of text
	Parses based off given character
	Returns a 2d array with each command seperated
	====================*/

	/*======== void pipes(char *a, char *b) ==========
	Inputs: char* a
		char* b
	Returns: Nothing! It is void!

	Uses redirOut and redirIn to implement pipes
	====================*/


	/*======== int main() ==========
	Returns: int (0)

	Uses fgets
	Calls functions and forks based contents
	Deals with parent and child functions
	====================*/