/* Jumble Puzzle Solver jps
 * word list file must exist in current dir as a file or link
 * as wordlist
 * jps accepts one argument, word to unscramble
 * Depending on wordlist, more than one match may be found!
 *
 * jps rmscleab
 * Found!: scramble
 * Read:         38619 words
 * Examined:     6020 - 8 letter words
 * Found word #: 1125
 *
 * Free program. Enjoy
 */


#define _GNU_SOURCE
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

struct Jumble
{
	char *jumble;					/* word scramble to solve */
	int *count;					/* count of unique letters */
};	

int main( int argc, char **argv )
{
	struct Jumble jumble;
	int len,i,j;
	int found;
	int counter;
	int words_encountered=0, words_examined=0, words_found=0;
	size_t buffersize;
	unsigned int read_line;
	char *target;
	char *dictionary_file="wordlist";		/* word list. Can be link */
	FILE *df;

	if (argc != 2)
	{
		printf("Must input a word to unscramble\n");
		return 1;
	}

	len=strlen(argv[1]);
	buffersize=len+1;

	target=malloc(buffersize);
	if (target==NULL)
		return 1;
	jumble.jumble=malloc(buffersize);
	if (jumble.jumble==NULL)
		return 1;
	jumble.count=malloc(buffersize*sizeof(int));
	if (jumble.count==NULL)
		return 1;

	strcpy(jumble.jumble,argv[1]);

	/* now count unique letter occurances */
	for (i=0; i<len; i++)
	{
		counter=0;
		for (j=0; j<len; j++)
		{
			if (jumble.jumble[i] == jumble.jumble[j])
				counter++;
		}
		jumble.count[i]=counter;
	}

	df=fopen(dictionary_file, "r");				/* open dictionary */
	if (df==NULL)
	{
		printf("Could not open %s\n",dictionary_file);
		return 1;
	}

	/* now loop through entire file, checking for jumbles along the way */

	while ((read_line = getline(&target, &buffersize, df)) != -1)	/* read dictionary file until word found */
	{ 
		words_encountered++; 				/* read a word */
		if (read_line-1 != len) continue;		/* length does not match */
		words_examined++; 				/* it's the same length */
		for (i = 0; i < len; i++)
		{
			found=0;
			for (j=0; j<len; j++)			/* check count of unique letters */
			{
				if (toupper(jumble.jumble[i]) == toupper(target[j]))
					found++;
			}

			if (found != jumble.count[i])		/* letter count does not match */
			{
				found=0;
				break;
			}
		}
		if (!found) continue;				/* next word, please */
		words_found++;					/* found one, store dictionary line */
		printf("Found word!: line %d, %s\n", words_examined, target);	/* it's possible there may be more than one */
	}
	printf("Read:         %d words\n\
		\rExamined:     %d - %d letter words\n\
		\rFound word #: %d\n", words_encountered, words_examined, len, words_found);

	fclose(df);

	return 0;
}

