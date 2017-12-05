#!/bin/bash

gnuplot --persist << EOF
set term wxt 0
set xlabel "Generation"
set ylabel "Mean Fitness"
plot 	'./pthreads/fitness.out' title 'Pthreads' with linespoints, \
	'./serial/fitness.out' title 'Serial' with linespoints
set term wxt 1
set xlabel "Generation"
set ylabel "Time(ms)"
plot 	'./pthreads/time.out' title 'Pthreads' with linespoints, \
	'./serial/time.out' title 'Serial' with linespoints
EOF
