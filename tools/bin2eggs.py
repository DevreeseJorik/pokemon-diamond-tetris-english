#!/usr/bin/env python3

"""Receive binary data, process it into an "eggsloader" dummy egg, and output it.
Example:
        $ python bin2eggs.py input.bin output_eggs.bin
"""

import sys
import math

PID = 0x00000000        # type 0 (ABCD)
EGG_SIZE = 0x88
HEADER_SIZE = 0x8
EGG_DATA1_SIZE = 0x1c
EGG_DATA2_SIZE = 0x58
EGG_DATA_SIZE = EGG_DATA1_SIZE + EGG_DATA2_SIZE   # Size of data to write into one egg
EGG_DATA1_OFFSET = 0x4
EGG_DATA2_OFFSET = 0x28
EGG_DATA_SIZE = EGG_DATA1_SIZE+EGG_DATA2_SIZE

def main():
    fpath = sys.argv[1]
    outpath = sys.argv[2]
    
    data = open(fpath, "rb").read()
    size = len(data)
    egg_num = math.ceil(size / EGG_DATA_SIZE)
    
    eggs = bytearray(EGG_SIZE * egg_num)
    for i in range(egg_num):
        chunk = data[i*EGG_DATA_SIZE: (i+1)*EGG_DATA_SIZE]
        eggs[i*EGG_SIZE: (i+1)*EGG_SIZE] = make_egg(chunk)

    # Write the total number of eggs and the auto-run flag to the 1st and 4th bytes of the PID for the first Pokémon.
    # The 2nd and 3rd bytes are used for specifying the shuffle type and are not utilized.
    eggs[0] = egg_num   # eggNumber
    eggs[3] = 1         # AutoRun == TRUE
    
    open(outpath + "_eggs.bin", "wb").write(eggs)
    for i in range(egg_num):
        open(outpath + f"_egg{i:02}.bin", "wb").write(eggs[i*EGG_SIZE: (i+1)*EGG_SIZE])
        


def make_egg(data):
    egg = bytearray(EGG_SIZE)
    egg[0:4] = PID.to_bytes(4, 'little')
    egg[4] = 6  # Bad Egg & Encryption Flag ON
    egg[8] = 1  # Species 0001 = Bulbasaur
    
    # Assuming shuffle type 0 (ABCD).
    # To avoid crashes, we do not use Species ID (0x8~0xc) and move ID (0x28~0x30).
    # Therefore, data is split and stored in two locations.
    data1 = data[:EGG_DATA1_SIZE]   # block A (0xc ~ 0x28)
    data2 = data[EGG_DATA1_SIZE:]   # block BCD (0x30 ~ 0x88)
    
    egg[8+EGG_DATA1_OFFSET:8+EGG_DATA1_OFFSET+len(data1)] = data1 
    egg[8+EGG_DATA2_OFFSET:8+EGG_DATA2_OFFSET+len(data2)] = data2
    
    return egg


if __name__ == "__main__":
    main()