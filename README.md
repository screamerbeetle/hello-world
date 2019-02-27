# qfxread

This is a skeletal c parser to take an OFX formatted input, and 
make a basic journal entry in the ledger format.

It is *very* simple, and *very* fast.

I use sort and grep to find duplicates in post processing like this:

```bash
outputFile="$HOME/your.file.journal"
dupFile='/tmp/duplicates.txt'
grep ofxid "$outputFile" | sort | uniq -d > $dupFile
if [ -s $dupFile ] 
then
 echo "Duplicate entries at the following lines (ofxid)" 
 grep -C 3 -nZ "$(cat $dupFile )" "$outputFile"
fi
```

This spits out 3 lines above and below each ofxid showing up more than once in the journal.
