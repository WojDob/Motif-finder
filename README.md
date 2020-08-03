# Motif-finder
Bioinformatic tool for finding cliques in biological sequences.


This program loads FASTQ format sequences and uses a heuristic algorithm to find the best sequence motif.

## How it works

First split your FASTQ sequences into separate files with the nucletide sequence in one file and quality in the other. Example files are available in the repository. 

The program first reads the two files:

![Image of Simpilistity](https://i.imgur.com/yPUnRmQ.png)

Then, it prompts you to enter the values of the k-mer size on which you want to find your biggest clique, as well as the thershold of the quality of reads.
After that, a graph is built, with nodes being the representation of a subsequence the size of the k-mer input, where edges are made based on the quality threshold to other, simillar nodes.

![Image of Simpilistity](https://i.imgur.com/9ecShlY.png)

A heuristic algorithm starts to analyse the graph, finding the best clique between the input sequences. The algorithm finds the clique in a **polynomial time**.

![Image of Simpilistity](https://i.imgur.com/ECYSNEk.png)
