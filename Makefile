all:
	g++ TimeCode.cpp TimeCodeTestsfinal.cpp -o tct

run:
	./tct

try: all run