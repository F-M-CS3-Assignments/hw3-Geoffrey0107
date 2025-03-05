all:
	g++ -g -Wall TimeCode.cpp TimeCodeTestsfinal.cpp -o tct
	g++ -g -Wall TimeCode.cpp NasaLaunchAnalysis.cpp -o nasa
	g++ -g -Wall TimeCode.cpp PaintDryTimer.cpp -o pdt

run:
	./tct
	./nasa
	./pdt

try: all run