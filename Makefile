# SDA, seria CC
# Tema1 

build: tema1

tema1: tema1.c
	gcc -Wall tema1.c -o tema1 


.PHONY:

clean:
	rm -f tema1

run:
	./tema1