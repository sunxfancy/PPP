# @Author: Sun Xiaofan <sxf>
# @Date:   2016-10-24
# @Email:  sunxfancy@gmail.com
# @Last modified by:   sxf
# @Last modified time: 2016-11-17
# @License: MIT License



build:
	-mkdir build
	-cd build && cmake .. && make ppp

run:
	-cd bin && ./ppp test.txt libparser.so 2

run100:
	-cd bin && ./ppp t100.txt libparser.so 4

run1000:
	-cd bin && ./ppp t1000.txt libparser.so 2

run10000:
	-cd bin && ./ppp t10000.txt libparser.so 2

gdb:
	-cd bin && gdb ./ppp

.PHONY: build
