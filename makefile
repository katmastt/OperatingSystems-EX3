
main: main.o simulation.o queue_funct.o semaphores_funct.o 

main.o : simulation.c interface_sim.h
	cc -c main.c
simulation.o : queue_funct.c semaphores_funct.c interface.h definitions.h
	cc -c simulation.c
queue_funct.o : interface.h definitions.h
	cc -c queue_funct.c
semaphores_funct.o :interface.h definitions.h
	cc -c semaphores_funct.c
clean :
	rm main main.o simulation.o queue_funct.o semaphores_funct.o