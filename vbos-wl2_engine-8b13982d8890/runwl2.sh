#! /bin/bash
BOT1DIR=../wl2_bot/bot20150826
BOT2DIR=../wl2_bot/bot20150825
bot1cmd="python2.7 $BOT1DIR/bot.py"
bot2cmd="python2.7 $BOT2DIR/bot.py"
mapfile=example-map.txt
#mapfile=newmap.txt
#python ./createmap.py > $mapfile
java -cp lib/java-json.jar:bin com.theaigames.game.warlight2.Warlight2 $mapfile "$bot1cmd" "$bot2cmd" 2>err.txt 1>out.txt
if grep [a-zA-Z] err.txt ; then
    echo ERRORS
    cat err.txt
else
    rounds=$(grep round out.txt | wc -l )
    cp out.txt "out_$rounds.txt"
    echo "Rounds: " $rounds
    tail -3 out.txt
fi

