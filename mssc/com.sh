#!/bin/bash
./a.out
dot -Tps mssc-output.txt -o msscoutfile.ps
gnome-open msscoutfile.ps
