#!/bin/bash

if [ ! -d "./tmp" ]; then
    mkdir ./tmp
else
    if [ "$(ls -A ./tmp/)" ]; then
        rm ./tmp/*
    fi
fi

if [ -z "$1" ]; then
    echo "Error: Need a build target"
    echo "    Ex: ./build.sh <target>"
    exit 1
fi

target=$1

verilog_compile="iverilog -g2001 -o tmp/tb_${target} source/tb_${target}.v"
verilog_simulate="vvp tmp/tb_${target}"

echo "Running: ${verilog_compile}"
$(${verilog_compile})

echo "Running: ${verilog_simulate}"
${verilog_simulate}

#gtkwave --save=./views/views.wfm.gtkw ./tmp/rawwaves_testrun.vcd 
