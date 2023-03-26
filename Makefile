all:	email_filter calendar_filter location_updater threaded_location_updater eft mmap_location_updater

email_filter: src/hw1/email_filter.c
	gcc -o email_filter src/hw1/email_filter.c src/hw1/calendar.c src/hw1/filter.c
calendar_filter: src/hw1/calendar_filter.c
	gcc -o calendar_filter src/hw1/calendar_filter.c src/hw1/calendar.c src/hw1/filter.c

location_updater: src/hw1/location_updater.c
	gcc -o location_updater src/hw1/location_updater.c
threaded_location_updater: src/hw2/threaded_location_updater.c
	gcc -o threaded_location_updater src/hw2/threaded_location_updater.c src/hw2/queue.c src/hw1/calendar.c src/hw1/filter.c 

eft: src/hw3/eft.c
	gcc -o eft src/hw3/eft.c src/hw3/bank.c src/hw3/ringbuffer.c 

mmap_location_updater: src/hw4/mmap_location_updater.c
	gcc -o mmap_location_updater src/hw4/mmap_location_updater.c src/hw4/shared_ring_buffer.c src/hw1/calendar.c src/hw1/filter.c 

clean:
	rm -rf email_filter calendar_filter location_updater threaded_location_updater eft mmap_location_updater
