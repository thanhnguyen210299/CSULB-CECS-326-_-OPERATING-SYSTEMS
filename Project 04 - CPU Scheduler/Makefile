# make fcfs - for FCFS scheduling
# make priority - for Priority scheduling
# make rr - for Round-Robin scheduling

driver.o: driver.c
	gcc -Wall -c driver.c

list.o: list.c list.h
	gcc -Wall -c list.c

CPU.o: CPU.c CPU.h
	gcc -Wall -c CPU.c

fcfs: driver.o list.o CPU.o schedule_fcfs.o
	gcc -Wall -o fcfs driver.o schedule_fcfs.o list.o CPU.o

priority: driver.o list.o CPU.o schedule_priority.o
	gcc -Wall -o priority driver.o schedule_priority.o list.o CPU.o
	
rr: driver.o list.o CPU.o schedule_rr.o
	gcc -Wall -o rr driver.o schedule_rr.o list.o CPU.o

schedule_fcfs.o: schedule_fcfs.c
	gcc -Wall -c schedule_fcfs.c

schedule_priority.o: schedule_priority.c
	gcc -Wall -c schedule_priority.c

schedule_rr.o: schedule_rr.c
	gcc -Wall -c schedule_rr.c

clean:
	rm -rf *.o
	rm -rf fcfs
	rm -rf priority
	rm -rf rr
