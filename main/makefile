all: bla
	g++ FrontEnd.cpp $(ax) -o FScience `pkg-config --cflags --libs gtk+-3.0`
	./FScience

bla:
	+$(MAKE) /home/nuelzin/Desktop/fScience/back_end/headers

ax = $$(find /home/nuelzin/Desktop/fScience/back_end/headers -type f -name "*.o")
