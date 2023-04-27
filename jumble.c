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
#define MINLEN 3
#define MAXCOMBOS 24					/* max combination words found */
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
	int len,i,j,is_a_word=0,words_tested=0;
	int combinations=1;				/* # of combinations to test */
	int found=0;					/* count of words found */
	char **words2test;				/* array of words to test */
	char **words_found;				/* array of found words */
	char *tmpptr;
	/* set up speller */
	AspellConfig *jumble_config = new_aspell_config();
	AspellCanHaveError *possible_err = new_aspell_speller(jumble_config);
	AspellSpeller *spell_checker = 0;

	int mod_test[5][2] = {				/* simplify word test with constants */
		{5040,7},
		{720,6},
		{120,5},
		{24,4},
		{6,3}
	};

	if (argc != 2)
	{
		printf("Must input a word to unscramble\n");
		return 1;
	}

	len = strlen(argv[1]);
	if (len > MAXLEN)
	{
		fprintf(stderr, "Input string too long. Must be <= %d letters...Aborting\n", MAXLEN);
		return 1;
	}

	if (len < MINLEN)
	{
		fprintf(stderr, "Input string too short. Must be at least %d letters...Aborting\n", MINLEN);
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

	words_found = malloc(MAXCOMBOS * sizeof(char *));		/* allow combinations of arrays */
	for (i = 0; i < MAXCOMBOS; i++)
	{
		words_found[i] = malloc(len);				/* each array will be len length not null terminated */
		if (words_found[i] == NULL)
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

	/* loop through all combinations on len letters */
	do
	{
		/* check word  */
		is_a_word = aspell_speller_check(spell_checker, words2test[words_tested++], len);
//		if (!is_a_word)						/* if not is_a_word, set up next combination */
		{
			/* rotate for next iteration */
			for (i = 0; i < 5; i++)
			{
				if (words_tested % mod_test[i][0] == 0)	/* 8-4 length */
				{
					strncpy(words2test[words_tested], words2test[words_tested-mod_test[i][0]], len);
					rollbytesl(len-mod_test[i][1], words2test[words_tested], len);
					break;
				}
			}
			/* if i == 5 then process last 3 letters */
			if (i == 5)
			{
				if (EVEN(words_tested))			/* 3 length min rotation */
				{
					strncpy(words2test[words_tested], words2test[words_tested-2], len);
					rollbytesl(len-2, words2test[words_tested], len);
				}
				else if (ODD(words_tested))		/* swap last two bytes */
				{
					strncpy(words2test[words_tested], words2test[words_tested-1], len);
					swapbytes(len-1, len, words2test[words_tested]);
				}
			}
		}
		if (is_a_word)						/* word found, show it */
		{
			tmpptr = words2test[words_tested-1];
			if (found == 0)					/* first word found */
				strncpy(words_found[0], tmpptr, len);
			else if (found > 0)				/* 2nd+ word found */
			{
				if (found > MAXCOMBOS)
				{
					fprintf(stderr,">%d many combinations found...\n", MAXCOMBOS);
					break;
				}
				for (j = 0; j <= found; j++)		/* have we seen this word before? */
					if (!strncmp(words_found[j], tmpptr, len))
					       break;			/* yes, found */

				if (j > found)				/* no, this is a new word */
					strncpy(words_found[found-1], tmpptr, len);
				else
					continue;			/* keep going */
			}
			/* print first found word or any new combinations */
			found++;
			printf("Solved: %s %d --> ", argv[1], words_tested);
			for (j = 0; j < len; j++)
				printf("%c", tmpptr[j]);
			printf("\n");
		}
	} while (words_tested < combinations);
	/* free not needed for **words2test/words_found since program ends and cleanup occurs */
}
