#!/bin/bash

# This script process multiple cable files at once.

# captures command line arguments
PATH_APP=$1
UNITS=$2
DIR_INPUT=$3
DIR_OUTPUT=$4

# creates a loop that iterates over all input directory files
for i in $(ls $DIR_INPUT); do
  echo $PATH_APP -v --units=$UNITS $i $DIR_OUTPUT/$(basename ${i%.*}).cable
done
