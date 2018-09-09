# Yinsh-Player

## Problem
The goal is to learn the adversarial search algorithms (mini- max and alpha beta pruning), which arise in sequential deterministic adversarial situations to make a Yinsh player.

## Setup
![alt text](https://github.com/madhur08/Yinsh-Player/blob/master/Images/yinsh_board.jpg) <br />
The game board is almost a regular hexagon of side N as shown. There are M rings with each player. Typical values for N are 5 and of M is also 5.

## Objective
There are 2 players, Black and White. The objective for both the players is to remove L rings before the other player does. To remove a ring you must create K markers in a line (explained below).
 
## Player Moves
The game proceeds turn by turn. The first M moves for both the players are placing the M rings each player has on the board on a position of their choice that is not already occupied. The subsequent moves are either moving a ring or removing a ring when you have at least 5 markers in a row. Moving a ring leaves a marker of the ring’s color behind.
### Moving a ring
You can move a ring from its initial position to another position on the board, but it can’t cross any other ring. If you are moving across markers, you can only move to the first empty space after the first continuous line of markers. Any number of spaces can be skipped before the first continuous line of markers. Any number of spaces can also be skipped if there are no markers on the line. In other words, you are allowed at most one (possibly long) jump across the markers. When a ring crosses a marker, the color of the marker toggles (black to white and white to black). This is applicable for all markers the ring crosses.
### Removing a ring
If you get 5 markers in a row, you can remove them by clicking on one of the endmarkersofthe5inaline. Iftherearemorethan5inalineortwo5lines crossing each other, clicking at the two ends will remove the 5 markers in the middle. After removing 5 continuous markers, you can remove a ring of your choice. Find more details in the link to the rules above. Catch : unlike in the rules, where you can remove any 5 continuous markers of yours choice, you can only remove 5 continuous markers from one end.

## Authors

+ Ishu Jain
**ishujain0018@gmail.com**

+ Madhur Singal
**madhursingal08@gmail.com**

*Course Project under [**Prof. Mausam**](http://homes.cs.washington.edu/~mausam/)*