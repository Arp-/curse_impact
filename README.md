# curse_impact
A space impact clone for terminal emulators using ncurses

Features still missing:
* The boss's script sholud be repeatable
* I shoud make some textures that indicate landscape for the game levels like in the original space impact.
* There should be ultimate attack for the ship
* You sohuld be able to modify your own ship's apperance via simple text file.
* A health/score bar should be at the top of the screen.
* I should learn a bit more about ncurse's window features.
* I should be able to make multiple scripts for a level and at the xml specify which is the next level's xml description and load that.
* And there are many other features missing, that i probably forgot to include in this list.

Currently i use a simple bash script to run g++ compiler, so if you use clang, or omething else then you have to modify it to compile.

The other thing this program is made for ncurses compatibe terminal emulators, so this program probably only compiles in a linux environment.

## Movement

 * 'h' - backwards
 * 'l' - forwards
 * 'j' - upward
 * 'k' - downward
 * 'space' - shoot
 
## Map generation

You can create a simple map, that currently means only how and where the enemies apper and script movement, and attack to them, inside the *res/game_script.xml* file.
If you want to change the apperance of the enemy ships you can modify  them inside the *res/texture/<my_enemy.xml>* files.
Sadly currently you can only modify your own ship's apperance inside the code :(


 
