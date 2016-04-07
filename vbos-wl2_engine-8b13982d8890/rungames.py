#! /usr/bin/python2.7
import sys
import subprocess
import random
import os
import os.path

class Competition:
    def __init__(self, rounds, bots):
        self.bots = bots
        self.rounds = rounds
        self.wins = {}
        self.losses = {}
        self.draws = {}

def get_maps(dname):
    maps = []
    for root, dirs, files in os.walk(dname):
        for f in files:
            maps.append(os.path.join(root, f))
    return maps
    

def play_game(b0, b1, mapsdir):
    #sys.stdout.write("bot0: %s, bot1: %s\n" % (b0, b1))
    bot0cmd = "python2.7 " + b0
    bot1cmd = "python2.7 " + b1
    #mapfile = "example-map.txt"

    maps = get_maps(dname=mapsdir)
    mapfile = random.choice(maps)
    
    cmd = ["java", "-cp", "lib/java-json.jar:bin", "com.theaigames.game.warlight2.Warlight2", mapfile, bot0cmd, bot1cmd]

    process = subprocess.Popen(cmd,
                               stdout=subprocess.PIPE, 
                               stderr=subprocess.PIPE, 
                               close_fds=True)

    outdata, errdata = process.communicate()

    if len(errdata) > 0:
        sys.stderr.write(errdata)
        sys.exit(1)

    outlines = outdata.split('\n')

    #sys.stdout.write("\n".join(outlines[-4:]))
    result = outlines[-4].strip()
    #sys.stdout.write(result + "\n")
    score = 0
    if result == "player1 won":
        score = 1
        #sys.stdout.write("bot0 won\n")
    elif result == "player2 won":
        score = -1
        #sys.stdout.write("bot1 won\n")
    else:
        pass
        #sys.stdout.write("No winner\n")
    return score

def run_games(competition, mapsdir, quiet):
    bots = competition.bots
    for b in bots:
        competition.wins[b] = 0
        competition.losses[b] = 0
        competition.draws[b] = 0
    rounds = competition.rounds
    for r in range(rounds):
        sys.stdout.write("round %d " % r)
        for b0 in bots:
            for b1 in bots:
                if b0 != b1:
                    sys.stdout.write(". ")
                    sys.stdout.flush()
                    score = play_game(b0, b1, mapsdir)
                    if score<0:
                        competition.losses[b0] += 1
                        competition.wins[b1] += 1
                    elif score == 0:
                        competition.draws[b0] += 1
                        competition.draws[b1] += 1
                    else:
                        competition.wins[b0] += 1
                        competition.losses[b1] += 1
        if not quiet:
            sys.stdout.write("\n")
            report_results(competition)
            sys.stdout.write("\n")

def report_results(competition):
    sorted_wins = sorted(competition.wins.keys(),
                         key=lambda b: competition.wins[b])
    line_template = "%s : %s : %s : %s\n"
    sys.stdout.write(line_template % ("BOT", "WINS", "LOSSES", "DRAWS"))
    for b in sorted_wins:
        sys.stdout.write(line_template % (b, 
                                          competition.wins[b], 
                                          competition.losses[b],
                                          competition.draws[b]))

if __name__ == "__main__":

    bots = []

    
    QUIET = False
    if sys.argv[1] == "-q":
        QUIET = True
        del sys.argv[1]

    maps_dir = "maps"
    if os.path.isdir(sys.argv[1]):
        maps_dir = sys.argv[1]
        del sys.argv[1]
        
    competition = Competition(rounds=int(sys.argv[1]), bots=sys.argv[2:])

    run_games(competition, maps_dir, QUIET)
    if QUIET:
        sys.stdout.write("\n")
        report_results(competition)
        sys.stdout.write("\n")
        
