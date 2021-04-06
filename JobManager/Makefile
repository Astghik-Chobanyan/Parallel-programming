all: jobmanager job consumer
		echo "Building..."
		ln -s job 1
		ln -s job 2
		ln -s job 3
		ln -s job 4
jobmanager::jobmanager.cpp
		g++ jobmanager.cpp -o jobmanager

job::jobmanager.cpp
		g++ job.cpp -o job

consumer::consumer.cpp
		g++ consumer.cpp -o consumer
clean:
		rm *.o
.PHONY: all clean