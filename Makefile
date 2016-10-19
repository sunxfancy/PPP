build:
	-mkdir build
	-cd build && cmake .. && make ppp

run:
	-cd bin && ./ppp test.txt

gdb:
	-cd bin && gdb ./ppp

.PHONY: build
