To implement multithreading, I simply chopped the outer loop into pieces.
Each thread would run on a piece.

The running time did significantly decrease.
1, 2, 4, and 8 threads ran in:
~44s
~22s
~14s
~9s
respectively

The time it takes more threads to run isn't quite ideal;
it deviates from a linear relationship rather early,
perhaps due to the overhead of setting up threads with a subpar implementation
I would've hoped for closer to 11s and 6s for 4 and 8 threads.

The only major issue I ran into was outputting everything in the right order.
Since the threads run simultaneously, the ordering was incorrect.
Had to create another array to insert data in the proper order.
