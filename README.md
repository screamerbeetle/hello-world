# qfxread
---------
This is a small, simple c program that reads in an OFX/QFX stream 
and spits out entries in the ledger format.

It does not currently:
 * deduplicate entries
 * send email
 * tell you how to use it

I use a two pass approach to list the duplicates:
$journalFile=your.output.file
grep ofxid "$journalFile" | sort | uniq -d > /tmp/duplicates.txt
[ -s /tmp/duplicates.txt ] && grep -C 3 -nZ "$(cat /tmp/duplicates.txt )" $journalFile

You are welcome to come up with a method that better suits your workflow!

--
Mike!
