#!/bin/bash
./a.out
dot -Tps uitkomst.txt -o outfile.ps
gnome-open outfile.ps
