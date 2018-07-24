#!/bin/bash

# CS 4318, summer 2 2018
# Research Project: Reversi

challenge='reversi'
funcSignature='int _(Board &, player whichPlayer)'
funcNames=''
objectFiles=''
declare -i numAgents
(( numAgents = 0 ))

CXX='g++'
CXXFLAGS='-O3 -Wall'   # Change -O3 to -g to get debug info.
LDLIBS='-lm'

if [[ ! -f "${challenge}.h" || ! -f "${challenge}.cpp" || ! -f "${challenge}Main.cpp" ]]
then
   echo " * ${challenge}.h, ${challenge}.cpp and ${challenge}Main.cpp are required."
   echo " * Build failed."
   exit 1
fi

echo " * ${challenge}.cpp: Compiling . . ."
if ${CXX} ${CXXFLAGS} -c "${challenge}.cpp"
then
   echo " * ${challenge}.cpp: Compilation succeeded."
else
   echo " * ${challenge}.cpp: Compilation failed."
   echo " * Build failed."
   rm *.o
   exit 3
fi

echo " * ${challenge}Main.cpp: Compiling . . ."
if ${CXX} ${CXXFLAGS} -c "${challenge}Main.cpp"
then
   echo " * ${challenge}Main.cpp: Compilation succeeded."
else
   echo " * ${challenge}Main.cpp: Compilation failed."
   echo " * Build failed."
   rm *.o
   exit 5
fi

echo " * neuralNet.cpp: Compiling . . ."
if ${CXX} ${CXXFLAGS} -c "neuralNet.cpp"
then
   echo " * neuralNetMain.cpp: Compilation succeeded."
else
   echo " * neuralNetMain.cpp: Compilation failed."
   echo " * Build failed."
   rm *.o
   exit 6
fi

#include for the oneNeuronMain

echo " * runPopMain.cpp: Compiling . . ."
if ${CXX} ${CXXFLAGS} -c "runPopMain.cpp"
then
   echo " * runPopMain.cpp: Compilation succeeded."
else
   echo " * runPopMain.cpp: Compilation failed."
   echo " * Build failed."
   rm *.o
   exit 7
fi


echo " * evolution.cpp: Compiling . . ."
if ${CXX} ${CXXFLAGS} -c "evolution.cpp"
then
   echo " * evolution.cpp: Compilation succeeded."
else
   echo " * evolution.cpp: Compilation failed."
   echo " * Build failed."
   rm *.o
   exit 8
fi

echo " * Linking . . ."
execFile="quickCommandsRunSim"
rm -f "${execFile}"
if ${CXX} ${LDLIBS} -o "${execFile}" "${challenge}Main.o" "${challenge}.o" "neuralNet.o" "evolution.o" "runPopMain.o" ${objectFiles}
then
   echo " * Linking succeeded."
   echo " * Executable file ./quickCommandsRunSim is ready to run!"
else
   echo " * Linking failed."
   echo " * Build failed."
   rm *.o
   exit 9
fi

rm *.o
