terminal : main.o
      gcc main.o -lreadline -o terminal
main.o main.c
      gcc -c main.c -lreadline -o main.o
clean :
      rm *o terminal
