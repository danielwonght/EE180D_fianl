#!/bin/bash

echo ""
echo ">>>>COMPILING<<<<"
make clean
make

echo ""
echo ">>>>Start in-field data collection system <<<<"
./inField_Collector

echo ""
read -p "Press any key to start training nerual networks" -n1 -s

echo ""
echo ">>>>Start training neural networks <<<<"
./inField_Consumer

echo""
read -p "Press any key to start the real time system" -n1 -s

echo ""
echo ">>>>Starting real time System<<<<"
./dataCollector &
DATACOLLECTOR_ID=$!
./dataConsumer &
DATACONSUMER_ID=$!
read -p "Press any key to kill producer/consumer..." -n1 -s
kill $DATACOLLECTOR_ID $DATACONSUMER_ID


