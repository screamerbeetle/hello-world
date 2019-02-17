/* This reads an ofx from stdin, does a set of string comparisons, and 
 * prints to stdout in ledger format.
 *
 * For more information, see http://www.ledger-cli.org 
 * or http://www.plaintextaccounting.org
 *
 * It's not smart, but it's fast!
 *
 * I prefer to compile this with:
 * " musl-gcc -static -O3 -o ledger-import qfxread.c "
 *
 */
#include <stdio.h>
#include <string.h>

// Cap the length of a line at 255 chars.
#define N 256
#define ShortBlock 20
#define LongScan "%255s"

int main () {
FILE *fp;
char str[N];

char bank[N];
char account[N];
char datePosted[N];
char fid[N];
float  amount ;
char fitid[N];
char memo[N];
char * block;
char * data;

/* opening file for reading */
fp = fopen("/dev/stdin" , "r");
if(fp == NULL) {
	perror("Error opening file");
	return(-1);
}

while ( fgets(str, N, fp) != NULL ) {

	/* Split str into managable pieces 
	 * from the second character to '>' is our key
	 * named 'block' and the rest is the data (named 'data')
	 * since data format is "<SOMETHING>INFO WE WANT\r\n\0"
	 * make the pointers point to the second letter and after the >...
	 */

	if ( (block = strchr(str, '<') ) != NULL ) {
	data = strchr(str, '>');
	* block = '\0';
	* data = '\0';
	block++;
	data++;

	/*
	 * need to set up an if/else if block anyways,
	 * to ingest the block info 
	 * could implement a switch block as a function, probably not worth the effort.
	 */

	if (!strcmp(block,"ORG")) { //bank
		sscanf(data,LongScan,&bank);
	}
	if (!strcmp(block,"FID")) { //bank id for ofxid
		// ofxid is FID.ACCTID.FITID
		sscanf(data,LongScan,&fid);
	}
	if (!strcmp(block,"ACCTID")) { //account
		sscanf(data,LongScan,&account);
	}
	if (!strcmp(block,"DTPOSTED")) { //datePosted
		// do it with pointers...
		datePosted[0] = *data++;
		datePosted[1] = *data++;
		datePosted[2] = *data++;
		datePosted[3] = *data++;
		datePosted[4] = '/';
		datePosted[5] = *data++;
		datePosted[6] = *data++;
		datePosted[7] = '/';
		datePosted[8] = *data++;
		datePosted[9] = *data++;
		datePosted[10] = '\0';
	}
	if (!strcmp(block,"TRNAMT")) { //amount
		sscanf(data,"%f",&amount);
	}
	if (!strcmp(block,"FITID")) { //fitid uniq?
		sscanf(data,LongScan,&fitid);
	}
	if (!strcmp(block,"MEMO")) { //memo
		strcpy(memo,data);
		// we want to keep the long string, and then pull out the newline.
		// ofx uses CRLF, kill them both.
		char * clean;
		clean = strchr(memo,'\n');
		*clean-- = '\0';
		*clean = '\0';
	}
	if (!strcmp(block,"/STMTTRN")) { //print out the block for ledger
		printf("%s \"%s\"\n",datePosted,memo);
		printf("    %s:%s        \t\t\t$%1.2f\n", bank, account, amount);
		printf("    ; ofxid: %s.%s.%s\n",fid,account,fitid);
		printf("    Expenses:Unknown    \t\t\t$%1.2f\n\n",-amount);
	}

	}


}
fclose(fp);

return(0);
}
