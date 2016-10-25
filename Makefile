build:
	-mkdir build
	-cd build && cmake .. && make ppp

run:
	-cd bin && ./ppp test.txt libparser.so 2

gdb:
	-cd bin && gdb ./ppp

.PHONY: build
