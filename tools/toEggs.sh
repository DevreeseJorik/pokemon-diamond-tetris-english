#!/bin/bash

# Convert binary data into eggs (multiple) for eggsloader and generate the PAR code.
# The eggs are placed starting from the second Pokémon in Box 1.

BIN=$1
NAME=${BIN%.bin}
SCRIPTDIR=$(cd $(dirname $(readlink -f $0));pwd)

# Convert binary to a Bad Egg
python3 ${SCRIPTDIR}/bin2eggs.py ${BIN} ${NAME}

# Generate PAR script
${SCRIPTDIR}/converter.py -i ${NAME}_eggs.bin -o ${NAME}_eggs_par.txt --pointer -a 0xC3A0 -p 0x02106FC0

# Generate PAR script for desmume
${SCRIPTDIR}/converter.py --mode=parFlat -i ${NAME}_eggs.bin -o ${NAME}_eggs_parflat.txt --pointer -a 0xC3A0 -p 0x02106FC0
