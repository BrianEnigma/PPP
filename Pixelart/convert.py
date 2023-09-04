#!/usr/bin/env python3

def convertBinToHex(s:str) -> str:
    result:int = 0
    while len(s) > 0:
        result = result << 1
        if s[0] == '1':
            result |= 0x01
        s = s[1:]
    resultStr:str = "{0:X}".format(result)
    if len(resultStr) == 1:
        resultStr = '0' + resultStr
    return resultStr


with open('icon.txt') as f:
    while True:
        line:str = f.readline()
        if line is None or len(line) == 0:
            break
        line = line.strip()
        while len(line) > 7:
            bits:str = line[0:8]
            line = line[8:]
            print('0x{0}, '.format(convertBinToHex(bits)), end='')
            #print("{0} ==> {1}".format(bits, convertBinToHex(bits)))
        print('')
