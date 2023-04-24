/* Jumble Puzzle Solver jps
 * aspell libs must be available
 * jps accepts one argument, word to unscramble
 * Depending on language, more than one match may be found!
 *
 * Aspell methods shamelessly borrowed from info page
 *
 * Free program. Enjoy
 */


#define _GNU_SOURCE
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <aspell.h>					/* aspell library header */

#define MAXLEN 8

#define ODD(x)	(x % 2) == 1
#define EVEN(x)	!ODD(x)

/* swap bytes in place
 * string may not be terminated
 */
static void swapbytes(int byte1, int byte2, char *string)
{
	char tmp;
	tmp = string[byte1-1];
	string[byte1-1] = string[byte2-1];
	string[byte2-1] = tmp;
}

/* shift bytes left 1 time at an offset
 * string may not be terminated.
 */
static void rollbytesl(int byte1, char *string, int len)
{
	char tmp;
	tmp = string[byte1-1];
	strncpy(&string[byte1-1], &string[byte1], len-byte1);
	string[len-1] = tmp;
}

int main( int argc, char **argv )
{
	int len,i,found=0,words_tested=0;
	int combinations=1;				/* # of combinations to test */
	char **words2test;				/* array of words to test */
	char *tmpptr;
	/* set up speller */
	AspellConfig *jumble_config = new_aspell_config();
	AspellCanHaveError *possible_err = new_aspell_speller(jumble_config);
	AspellSpeller *spell_checker = 0;

	if (argc != 2)
	{
		printf("Must input a word to unscramble\n");
		return 1;
	}

	len = strlen(argv[1]);
	if (len > MAXLEN)
	{
		fprintf(stderr, "Input string too long. Must be <= %d...Aborting\n", MAXLEN);
		return 1;
	}

	tmpptr=argv[1];
	for (i = 0; i < len; i++)
		if (isupper((int)tmpptr[i]))
			tmpptr[i] = tolower((int)tmpptr[i]);

	/* get factorials for bounded array */
	for (i = len; i > 1; i--)
		combinations *= i;

	printf("Words to be tested: %d\n", combinations);

	words2test = malloc((combinations+1) * sizeof(char *));		/* allow combinations of arrays */
	if (words2test == NULL)
		fprintf(stderr,"Memory error\n");

	for (i = 0; i <= combinations; i++)
	{
		words2test[i] = malloc(len);				/* each array will be len length not null terminated */
		if (words2test[i] == NULL)
			fprintf(stderr,"Array allocation error\n");
	}

	/* this should never fail */
	if (aspell_error_number(possible_err) != 0)
	{
		fprintf(stderr, "Aspell error: %s\n", aspell_error_message(possible_err));
		return 1;
	}
	else
		spell_checker = to_aspell_speller(possible_err);

	/* get language variable */
	tmpptr = getenv("LANG");
	if (!tmpptr)
		strcpy(tmpptr,"en_US");					/* default is en_US */
	else if (strlen(tmpptr) > 5)					/* if there is .UTF-8 or additional cruft, trim */
		tmpptr[5] = '\0';

	/* set language */
	aspell_config_replace(jumble_config, "lang", tmpptr);

	strncpy(words2test[0],argv[1],len);				/* seed 0 array */

	/* loop through all combinations on len letters until found*/
	do
	{
		/* check word  */
		found = aspell_speller_check(spell_checker, words2test[words_tested++], len);
		if (!found)						/* if not found, set up next combination */
		{
			/* rotate for next iteration */
			if (words_tested % 5040 == 0)			/* 8 length */
			{
				strncpy(words2test[words_tested], words2test[words_tested-5040], len);
				rollbytesl(len-7, words2test[words_tested], len);
			}
			else if (words_tested % 720 == 0)		/* 7 length */
			{
				strncpy(words2test[words_tested], words2test[words_tested-720], len);
				rollbytesl(len-6, words2test[words_tested], len);
			}
			else if (words_tested % 120 == 0)		/* 6 length */
			{
				strncpy(words2test[words_tested], words2test[words_tested-120], len);
				rollbytesl(len-5, words2test[words_tested], len);
			}
			else if (words_tested % 24 == 0)		/* 5 length */
			{
				strncpy(words2test[words_tested], words2test[words_tested-24], len);
				rollbytesl(len-4, words2test[words_tested], len);
			}
			else if (words_tested % 6 == 0)			/* 4 length */
			{
				strncpy(words2test[words_tested], words2test[words_tested-6], len);
				rollbytesl(len-3, words2test[words_tested], len);
			}
			else if (EVEN(words_tested))			/* min rotation */
			{
				strncpy(words2test[words_tested], words2test[words_tested-2], len);
				rollbytesl(len-2, words2test[words_tested], len);
			}
			else if (ODD(words_tested))			/* swap last two bytes */
			{
				strncpy(words2test[words_tested], words2test[words_tested-1], len);
				swapbytes(len-1, len, words2test[words_tested]);
			}
		}
		else
		{
			printf("Solved: %s %d --> ", argv[1], words_tested);
			tmpptr = words2test[words_tested-1];
			for (i = 0; i < len; i++)
				printf("%c", tmpptr[i]);
			printf("\n");
			break;
		}
	} while (words_tested < combinations);
	/* free not needed for **words2test since program ends and cleanup occurs */
}
