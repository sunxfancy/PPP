#!/usr/bin/env python3

# @Author: Sun Xiaofan <sxf>
# @Date:   2016-11-16
# @Email:  sunxfancy@gmail.com
# @Last modified by:   sxf
# @Last modified time: 2016-11-17
# @License: MIT License

from random import randint

def main():
    left = 0
    for i in range(1, 10000):
        print('id', end=' ')
        code = randint(0, 1)
        kk = randint(0, 9)
        if kk == 1 and left != 0:
            print(')', end=' ')
            left = left - 1
        if code == 0:
            print('+', end=' ')
        else:
            print('*', end=' ')
        if kk == 0:
            print('(', end=' ')
            left = left + 1

    print('id', end=' ')
    for i in range(left):
        print(')', end=' ')

if __name__ == '__main__':
    main()
