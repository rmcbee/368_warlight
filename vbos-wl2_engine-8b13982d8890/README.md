# warlight2-engine and game engine #

This is the game engine for Warlight AI Challenge 2 modified for game visualisation. The original game engine was taken from theaigames.com (Github: https://github.com/theaigames/warlight2-engine).


## The Warlight AI Challenge 2 game engine ##

This version of our Warlight AI Challenge 2 engine has been set up for
local use, for your own convenience. Note that this does *not* include
the map generator and the visualizer.

To compile (Windows, untested):

    cd [project folder]
    dir /b /s *.java>sources.txt
    md classes
    javac -d classes @sources.txt -cp lib/java-json.jar
    del sources.txt

To compile (Linux):

    cd [project folder]
    mkdir bin/
    javac -sourcepath src/ -d bin/ -cp lib/java-json.jar `find src/ -name '*.java' -regex '^[./A-Za-z0-9]*$'`
    
To run:

    cd [project folder]
    java -cp lib/java-json.jar:bin com.theaigames.game.warlight2.Warlight2 [map file] [your bot1] [your bot2] 2>err.txt 1>out.txt

[map file] is a file that contains a string representation of the map
that the game will use. An example is included in this repository
called "example-map.txt". For other maps, go to any Warlight AI
Challenge 2 game on theaigames.com and add "/map" to the end of the
URL and copy that text to a file.

[your bot1] and [your bot2] could be any command for running a bot
process. For instance "java -cp /home/dev/starterbot/bin/
main.BotStarter" or "node /home/user/bot/Bot.js"

Errors will be logged to err.txt, output dump will be logged to
out.txt.

The output in out.txt contains sufficient information to visualize the
game afterwards.

### The Game Runner for Python Bots ###

The Game Runner `runwl2.sh` is a convenience script to run a Warlight
2 game with bots written in Python 2.7. Each bot shall be named
`bot.py` (it is ok to use the same bot twice). The Game Runner is
executed as follows:

	$ ./runwl2.sh

The game result is stored in `out_nnn.txt`, where `nnn` is the number
of game rounds.

To configure the Game Runner, edit `runwl2.sh` and change the
following variables:

	BOT1DIR=<path> # directory of the first bot
	BOT2DIR=<path> # directory of the second bot
	mapfile=example-map.txt # mapfile (in format defined by  The AI Games)

The script expects a Python 2.7 script `bot.py` to exist in both
`$BOT1DIR` and `$BOT2DIR`.

An example map file exist (from The AIGames):

	example-map.txt


## The Game Visualizer ##

The Game Visualizer is a Python 2.7 script:

	wl2dot.py

It reads the `out.txt` file created by the Game Engine and creates
HTML and SVG files to visualize the game. By opening the HTML files,
it is possible to browse through the game.

#### Dependencies ####

The Game Visualizer depends on the following:

- Python 2.7
- Graphviz (in particular `dot`)
- `svg-pan-zoom.js` (included unchanged from https://github.com/ariutta/svg-pan-zoom)
- HTML5 compatible browser (with SVG support)

#### Usage ####

To create the HTML and SVG files, run a command like:

	$ cat out.txt | python wl2dot.py <resultdir> <maxmaps>

where:

- `out.txt`is a game log created by the Game Engine (see above)
- `<resultdir>` is a directory where the HTML and SVG files will be stored
- `<maxmaps>` is an integer number indicating the maximum number of maps

Advice: `<resultdir>` shall preferably be empty and `<maxmaps>` shall
be 5 or even 6 times the number of game rounds in order to create maps
of the complete game.

## Sample session ##

Here is an sample session to run a game and create the HTML and SVG files:

	$ ./runwl2.sh
	Rounds:  162
	Nobody won
	
	Done.
	$ cat out_162.txt | python wl2dot.py result_162 1000

This creates files `map_0.{dot,html,svg}`--`map_825.{dot,html,svg}` in
directory `result_162` (the number 825 might be different).

Use a web browser to view the game, e.g.,:

	$ firefox result/map_0.html

The HTML files contain links to move back and forward in the
game. Zoom in/out on the graphical maps is possible by mousewheel as
well as with provided buttons.
