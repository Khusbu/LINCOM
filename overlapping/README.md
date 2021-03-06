#LINCOM for overlapping clusters

This package finds out overlapping clusters and its overlapping modularity for different INS values (0.4 to 0.85 interval of 0.05)

- Commands:
```
$ make                               #builds the Louvain code for your architecture
$ ./script_overlap.sh <filename>     #runs iterations on LINCOM_disjoint for different INS values.
```

- Sample Output (On Console) :

```
r = 0.7
No. of communities before mod max : 2
Initial Overlapping modularity: 0.729892
Number of communities after mod max: 1
Final Modularity: 0.000000


r = 0.75
No. of communities before mod max : 2
Initial Overlapping modularity: 0.729892
Number of communities after mod max: 1
Final Modularity: 0.000000


r = 0.8
No. of communities before mod max : 4
Initial Overlapping modularity: 0.666745
Number of communities after mod max: 2
Final Modularity: 0.713413
```

- Generated output file:         
  filename: overlapping_ins        
  Output format:          
  (ins_value) (no_of_comm) (overlapping_modularity)        
