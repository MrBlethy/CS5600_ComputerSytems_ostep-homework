#!/bin/bash

#=============================================================
#CS5600 - chapter 19 question 3 
# Running Pages with TLB.c
#=============================================================
run=(taskset -c 0 ./tlb)
LOGFILE="Tlb_pinned.log"

> "$LOGFILE"

 echo "========Running Experiment ===================" | tee -a "$LOGFILE"
 for pages in 1 2 4 8 16 32 64 128 256 512 1024 2049 4096 
 do 
	 echo "==========Tesing ========="
	 "${run[@]}" "$pages" 1000000 2>1 | tee -a "$LOGFILE"
done 
#==============================================================
