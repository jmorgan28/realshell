shell: shell.c
	gcc shell.c -o shell

run: shell
	./shell

clean:
	rm shell
	rm *~
	rm *dump 
	rm *txt
