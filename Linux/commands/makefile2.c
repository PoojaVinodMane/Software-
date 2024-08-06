terminal:main.o ls.o cp.o mv.o grep.o
       gcc main.o ls.o cp.o mv.o grep.o -lreadline -o terminal
main.o: main.c
       gcc -c main.o -lreadline -o main.o
ls.o: ls.c
       gcc -c ls.c -lreadline -o ls.o
cp.o: cp.c
       gcc -c cp.c -lreadline -o cp.o
mv.o: mv.c
       gcc -c mv.c -lreadline -o mv.o
grep.o: grep.c
       gcc -c grep.o -lreadline -o grep.o     
