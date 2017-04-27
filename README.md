# Assignment6_COP
Trace based cache simulator and processor
We have made a trace based cache simulator and integrated it with a pipelined processor
In this, we have provided the code for the trace based cache simulator, on which you can see how a typical cache works
command for running the code is:
./sim -bs <block size> -is <instruction_cache size> -ds <data_cache size> -a <associativity> -wb -wa/nw <trace file>
where wb stands for write back; wa:write allocate and nw(optional):no write allocate
On the terminal, when the code is run,you can see various cache statistics like data miss rate, instruction miss rate, total words fetched from the main memory, no of blocks of memory replaced and total words copied back to memory from cache
The trace file contains the memory accesses and what type of access it was:
0:for load word; 1 for store word; 2 for instruction access followed by the memory address
