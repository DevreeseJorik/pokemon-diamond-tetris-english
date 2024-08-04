#!/usr/bin/env python3

usage = '''
Convert binary files to PAR codes or other formats.
Options:
  -i                Path to the input binary file
  -o                Path to the output file
  -a                Address to place the binary (Address offset if using the --pointer option)
  -p                Address of the reference pointer
  --mode=par        Output PAR codes to write the binary
  --mode=parFlat    Output PAR codes to write the binary (for desmume)
  --mode=word       Split the binary into words (4 bytes) and output in hexadecimal
  --mode=wordDec    Split the binary into words (4 bytes) and output in decimal
  --mode=script     Output game script to write the binary into memory (hexadecimal)
  --mode=scriptDec  Output game script to write the binary into memory (decimal)
  --mode=csv        Not implemented
  --nozero          Ignore values that are 0
  --pointer         Output pointer codes for PAR

'''

from bin2par import *
import os
import sys
import enum
from collections import deque
from typing import List

def main():
    option = parse(sys.argv[1:])
    bin = bin2Text(option.inputPath, option.address, option.pointer)
    with open(option.outputPath, 'w', encoding='utf-8', newline='\n') as f:
        if option.mode == Mode.par:
            if option.isPointer:
                f.writelines("\n".join(bin.toWordWritePointerParCode()))
            else:
                f.writelines("\n".join(bin.toWordWriteParCode()))
        elif option.mode == Mode.parFlat:
            lines = bin.toWordWritePointerParCode() if option.isPointer else bin.toWordWriteParCode()
            lines = list(map(lambda x: x.replace(' ', ''), lines))
            # Split every 300 lines
            codes = chunks(lines, 300)
            scriptName = os.path.basename(option.inputPath)
            for i, code in enumerate(codes):
                line = 'AR 1 ' + ','.join(code) + f';{scriptName}_{i:02}\n'
                f.write(line)
        elif option.mode == Mode.word:
            f.writelines("\n".join(bin.toString(dec=False)))
        elif option.mode == Mode.wordDec:
            f.writelines("\n".join(bin.toString(dec=True)))
        elif option.mode == Mode.script:
            f.writelines("\n".join(bin.toByteWriteScript(dec=False, nozero=option.nozero)))
        elif option.mode == Mode.scriptDec:
            f.writelines("\n".join(bin.toByteWriteScript(dec=True, nozero=option.nozero)))
        elif option.mode == Mode.csv:
            pass
        else:
            raise Exception()


class Mode(enum.Enum):
    par = 'par'
    parFlat = 'parFlat'
    word = 'word'
    wordDec = 'wordDec'
    script = "script"
    scriptDec = 'scriptDec'
    csv = 'csv'

class Option:
    def __init__(self, 
        inputPath="",
        outputPath="",
        address="0",
        pointer="0",
        mode=Mode.par,
        nozero=False,
        isPointer=False,
        ):
        self.inputPath = inputPath
        self.outputPath = outputPath
        self.address = int(address, 16)
        self.pointer = int(pointer, 16)
        self.mode = mode
        self.nozero = nozero
        self.isPointer = isPointer
    
    def setAddress(self, address):
        self.address = int(address, 16)

    def setPointer(self, pointer):
        self.pointer = int(pointer, 16)


def parse(args: List[str]) -> Option:
    if len(args) == 0:
        print(usage)
        exit(0)

    option = Option()
    argument_from_queue = deque(args)
    while argument_from_queue:
        argument = argument_from_queue.popleft()
        if argument in ['-h', '--help']:
            print(usage)
            sys.exit(0)
        elif argument == '-i':
            option.inputPath = argument_from_queue.popleft()
        elif argument == '-o':
            option.outputPath = argument_from_queue.popleft()
        elif argument == '-a':
            option.setAddress(argument_from_queue.popleft())
        elif argument == '-p':
            option.setPointer(argument_from_queue.popleft())
        elif argument == '--mode=par':
            option.mode = Mode.par
        elif argument == '--mode=parFlat':
            option.mode = Mode.parFlat
        elif argument == '--mode=word':
            option.mode = Mode.word
        elif argument == '--mode=wordDec':
            option.mode = Mode.wordDec
        elif argument == '--mode=script':
            option.mode = Mode.script
        elif argument == '--mode=scriptDec':
            option.mode = Mode.scriptDec
        elif argument == '--mode=csv':
            option.mode = Mode.csv
        elif argument == '--nozero':
            option.nozero = True
        elif argument == '--pointer':
            option.isPointer = True
        else:
            print(f"Bad argument '{argument}'")
            print("With '-h' option to show usage")
            sys.exit(0)

    return option


def chunks(l, n):
    for i in range(0, len(l), n):
        yield l[i:i + n]


if __name__ == "__main__":
    main()