	
target:
	g++ -o agent main.cpp agent.cpp cloud.cpp timer.cpp  \
	-lmosquittopp
	
clean:
	rm -rf agent
	