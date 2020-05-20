output: shell.c  
	gcc shell.c  -L/usr/include -lreadline -o output

clean:
	rm  output
