# 2048
2048 game made from scratch in C using ncurses.

This project took me around 20 hours.

The game takes place on a 4x4 matrix in which the player must join blocks
of the same value to form higher value cells, having as values ​​powers of 2,
starting with 2 and ending with 2048. When the player manages to combine
2 boxes of 1024 points the game ends.

The cells move in the direction of the arrows and combine at the moment of collision.

The algorithm that was able to reach 11000 points, making a cell of 1024 in the process, 
automatically makes a move after 10 seconds of inactivity.

To open the menu, press the Q key, and to use the bomb, press the B key.

More information about the project can be seen in the commentaries from the source code.
