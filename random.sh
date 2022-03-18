#!/bin/bash

RANDOM=$$
MIN=20
MAX=150
DIFF=$(($MAX - $MIN + 1))
for i in `seq 50`
do
    E1=$(($(($RANDOM % $DIFF)) + $MIN))
    E2=$(($(($RANDOM % $DIFF)) + $MIN))
    E3=$(($(($RANDOM % $DIFF)) + $MIN))
    E4=$(($(($RANDOM % $DIFF)) + $MIN))
    E5=$(($(($RANDOM % $DIFF)) + $MIN))
    echo $E1 $E2 $E3 $E4 $E5
    ./opt exh b $E1 $E2 $E3 $E4 $E5
    ./opt our b $E1 $E2 $E3 $E4 $E5
    echo "--------------------------------------------------"

done
