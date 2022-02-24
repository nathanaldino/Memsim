# Memsim
Measuring the performance of page replacement algorithms (FIFO,LRU,Segmented FIFO) on real traces 
By: Nathaniel Aldino and Anthony Sarim

To run the program:
First, compile using make command.

Then, run ./memsim <tracefile> <nframes> <fifo|lru|vms> <p(only if vms)> <debug|quiet>  

-tracefile: name of the tracefile
-nframes: number of frames to use 
-page replacement algorithms: lru = least recently used | fifo = first in first out | vms = segmented first in first out
-percentage(only if vms): 1-100 
-quiet/debug: quiet to print stats | debug to print every event that happens 