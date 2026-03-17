# rbt-leaderboard

In order to run the program, simply run the leaderboard.cpp file under the IDE of your choice (CLion was used to code the program).
Once running, type one operation per line. Enter a blank line to exit.

ADD <player> <scoreADD> 
Add a new player with the given score

UPDATE <player> <delta>
Adjust a player's score (use negative delta to decrease)

REMOVE <player>
Remove a player from the leaderboard

TOP <k>
Print the top k players, highest score first

Example input:
ADD Alice 120
ADD Bob 90
ADD Carol 150
UPDATE Bob 50
TOP 2
REMOVE Carol
TOP 2

Example output:
Carol 150
Bob 140
Bob 140
Alice 120
