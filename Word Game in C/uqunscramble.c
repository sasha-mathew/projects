#include <stdio.h>
#include <csse2310a1.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>
#include <ctype.h>
#include <stdlib.h>

#define MAX_WORD_LENGTH 50
#define MAX_GUESSES 100
#define ERROR_11 11
#define ERROR_1 1
#define ERROR_3 3
#define ERROR_7 7
#define ERROR_13 13
#define ERROR_20 20
#define SCORE_ADD 10
#define ARG_COUNT 6

typedef struct {
    int wordLength;
    char* letters;
    char* dict;
} Params;

/* usage_error()
 * -------------
 * This function takes in no arguments, and returns nothing
 * It is called when a usage error occurs
 */
void usage_error()
{
    fprintf(stderr,
            "Usage: uqunscramble [--length-min numletters] [--dictionary file] "
            "[--letters chars]\n");
    exit(ERROR_1);
}

/* length_error()
 * --------------
 *  This function takes in no arguments, and returns nothing
 *  It is called if the user specifies a --min-length out of bounds
 */
void length_error()
{
    fprintf(stderr,
            "uqunscramble: min length must be between 2 and 5 inclusive\n");
    exit(ERROR_11);
}

/* dict_error(filename)
 * --------------------
 *  filename: the filename of the dictionary
 *  This function returns nothing
 *  It is called if the user specifies a --dictionary that cannot be opened or
 *  doesn't exist
 */
void dict_error(const char* filename)
{
    fprintf(stderr, "uqunscramble: dictionary named \"%s\" cannot be opened\n",
            filename);
    exit(ERROR_3);
}

/* Global Variables that the rest of the functions constantly access
 * and update.
 */
Params params = {.letters = NULL, .wordLength = 0, .dict = NULL};
int len = 0;
int score = 0;

/* test_cmd(int argc, char* argv[])
 * --------------------------------
 *  argc: the number of arguments given in the terminal, including
 *  the file name
 *  argv: the list of string arguments given in the terminal
 *
 *  returns: 1 if the code passes all the checks, however if it fails a check
 *  the code will have exited previously with a specific exit code
 *
 *  This function is testing that the command line arguments are valid.
 *  REF: https://www.tutorialspoint.com/c_standard_library/c_function_strtol.htm
 */
int test_cmd(int argc, char* argv[])
{
    argc--;
    argv++;

    bool lettersGiven = false;
    bool minLenGiven = false;
    bool dictGiven = false;
    for (int i = 0; i < argc; i += 2) {
        if (argc % 2 != 0
                || argc > ARG_COUNT) { // checking to see if there are the
            // correct number of arguments
            usage_error();
        }
        if (strcmp(argv[i], "--length-min") == 0) {
            char* endptr; // pointer
            strtol(argv[i + 1], &endptr,
                    10); // string to long, then pointer to pointer to char
            if (*endptr != '\0') { // checks if the character after converted
                                   // digit is null
                usage_error();
            }
            if (strlen(argv[i + 1]) != 1) { // checks length of arg
                usage_error();
            }

            if (params.wordLength != 0) { // checks for repeat args
                usage_error();
            }
            params.wordLength = atoi(argv[i + 1]);
            minLenGiven = true;
        }

        else if (strcmp(argv[i], "--dictionary") == 0) {
            if (params.dict != NULL) {
                usage_error();
            }
            params.dict = argv[i + 1];
            dictGiven = true;
        } else if (strcmp(argv[i], "--letters") == 0) {
            if (params.letters != NULL) {
                usage_error();
            }
            params.letters = argv[i + 1];
            lettersGiven = true;
            len = strlen(params.letters);
        } else {
            usage_error();
        }
    }
    if (!lettersGiven) {
        params.letters = (char*)get_random_letters(7);
        len = 7;
    }

    if (!minLenGiven) {
        params.wordLength = 4;
    }

    if (!dictGiven) {
        params.dict = "/local/courses/csse2310/etc/words";
    }

    for (int j = 0; j < len; j++) {
        if (!isalpha(params.letters[j])) {
            fprintf(stderr, "uqunscramble: invalid letter set\n");
            exit(ERROR_20);
        }
    }
    return 1;
}

/* test_min()
 * ----------
 *  This function takes in no arguments and returns one if the condition is met
 *  This function tests to see if the word length entered is between
 *  2 and 5, and if not, calls the length_error() function.
 */
int test_min()
{
    if (params.wordLength < 2 || params.wordLength > 5) {
        length_error();
    }
    return 1;
}

/* test_letters()
 * --------------
 *  This function takes in no arguments and returns one if the condition is met
 *  This function tests to see if the letters given by the user meet the
 *  required conditions, and if not, calls the respective error function.
 */
int test_letters()
{
    if (len > 15) {
        fprintf(stderr, "uqunscramble: too many letters - the limit is 15\n");
        exit(2);
    }

    if (len < params.wordLength) {
        fprintf(stderr,
                "uqunscramble: more letters required for the given minimum"
                " length (%d)\n",
                params.wordLength);
        exit(ERROR_13);
    }
    return 1;
}

/* test_dict()
 * -----------
 *  This function takes in no parameters and returns one if the dictionary
 *  provided by the user exists and can be opened.
 *  This function tests to see if the dictionary provided by the user is
 *  accessible, and calls the dict_error() function if not.
 *  REF:
 * https://stackoverflow.com/questions/230062/whats-the-best-way-to-check-if-a-file-exists-in-c/230068#230068
 */
int test_dict()
{
    FILE* file;
    if (!(file = fopen(params.dict, "r"))) {
        dict_error(params.dict);
    }
    return 1;
}

/* all_checks()
 * ------------
 * argc: the number of arguments given
 * argv: the list of arguments given
 * returns: 1 if all conditions are met through the different functions.
 * This function checks that all the checks are being met in a specific order
 * and returns 1 if true, otherwise it will exit through one of the functions
 * that have been called within.
 */
int all_checks(int argc, char* argv[])
{
    if (test_cmd(argc, argv)) {
        if (test_min()) {
            if (test_letters()) {
                if (test_dict()) {
                    printf("Welcome to UQunscramble!\n");
                    printf("Enter words of length %d to %d made from the "
                           "letters \"%s\"\n",
                            params.wordLength, len, params.letters);
                }
            }
        }
    }
    return 1;
}

/* Global variables that are accessed and edited throughout the program
 * by different functions.
 */
char* guesses[MAX_GUESSES]; // Array to store previous guesses
int num_guesses = 0; // Number of previous guesses

/* contains_only_letters(const char* str)
 * --------------------------------------
 * str: the char in the string that makes the guess of the user through stdin
 * Returns: true if the function only contains letters and no non alphabetical
 * values.
 * This function checks that the input through stdin only contains alphabetical
 * letters.
 * REF: https://www.geeksforgeeks.org/isalpha-isdigit-functions-c-example/
 */
bool contains_only_letters(const char* str)
{
    while (*str) {
        if (!isalpha(*str)) { // iterates through string with pointer to check
                              // that all chars are alphabets
            return false;
        }
        str++;
    }
    return true;
}

/* is_guess_duplicate(const char* guess)
 * -------------------------------------
 *  guess: the chars that compose the string that is the users guess in stdin
 *  Returns: false if the guess has not already been used by them
 *  This function checks to see if the user has inputted a guess that they
 *  have already guessed, and it is case insensitive.
 *  REF:
 * https://stackoverflow.com/questions/31127260/strcasecmp-a-non-standard-function
 */
bool is_guess_duplicate(const char* guess)
{
    for (int i = 0; i < num_guesses; i++) {
        if (strcasecmp(guess, guesses[i])
                == 0) { // iterates through previous guesses to check for
                        // repeats and is case insensitive
            return true;
        }
    }
    return false;
}

/* can_make_word(const char* word)
 * -------------------------------
 * word: the user's guess
 * Returns: true if the word can be made from the letters provided
 * This function checks to see if the word the user guessed can be made from
 * the letters that are provided.
 * REF: https://www.tutorialspoint.com/c_standard_library/c_function_strcpy.htm
 * REF: https://www.tutorialspoint.com/c_standard_library/c_function_strchr.htm
 */
bool can_make_word(const char* word)
{
    char lettersCopy[strlen(params.letters) + 1];
    strcpy(lettersCopy, params.letters); // copy letters into copy array

    for (int i = 0; i < (int)strlen(word); i++) {
        char* found = strchr(lettersCopy,
                toupper(word[i])); // find uppercase of current char
        if (!found) {
            found = strchr(lettersCopy,
                    tolower(word[i])); // find lowercase of current char
            if (!found) {
                return false; // Letter not found
            }
        }
        *found = '_'; // Mark letter as used
    }
    return true;
}

/* Global variable that changes and is accessed throughout
 */
char line[MAX_WORD_LENGTH];

/* is_word_in_dictionary(const char* word)
 * ---------------------------------------
 *  word: the user's guess
 *  Returns: true if the user's word is in the dictionary
 *  This function checks to see if the guess is in the dictionary
 *
 *  REF: used help from this site: https://stackoverflow.com/questions/
 *  31127260/strcasecmp-a-non-standard-function
 */
bool is_word_in_dictionary(const char* word)
{
    FILE* dictFile = fopen(params.dict, "r");
    while (fgets(line, sizeof(line), dictFile)) {
        // Remove trailing newline character
        line[strcspn(line, "\n")] = '\0';

        // Case-insensitive comparison
        if (strcasecmp(word, line) == 0) {
            fclose(dictFile);
            return true;
        }
    }
    fclose(dictFile);
    return false;
}

/* update_score(const char* guess)
 * -------------------------------
 * guess: the user's guess
 * Returns nothing
 * This function updates the score according to the guess
 */
void update_score(const char* guess)
{
    score += strlen(guess);
    if (strlen(guess) == (size_t)len) {
        score = score + SCORE_ADD;
    }

    printf("OK! Your score so far is %d\n", score);
}

/* validate_guess(const char* guess)
 * ---------------------------------
 * guess: the user's guess
 * Returns: nothing
 * This function validates the user's guess and sees if it passes the
 * predetermined requirements enforced through other functions.
 * REF:
 * https://www.geeksforgeeks.org/dynamic-memory-allocation-in-c-using-malloc-calloc-free-and-realloc/
 */
void validate_guess(const char* guess)
{
    if (!contains_only_letters(guess)) {
        printf("Guess must contain only letters\n");
    } else if (strlen(guess) < (size_t)params.wordLength) {
        printf("Word too short - it must be at least %d characters long\n",
                params.wordLength);
    } else if (strlen(guess) > (size_t)len) {
        printf("Word must have a length of no more than %d characters\n", len);
    } else if (!(can_make_word(guess))) {
        printf("Word can't be made from available letters\n");
    } else if (is_guess_duplicate(guess)) {
        printf("You've guessed that word before\n");
    } else {
        guesses[num_guesses]
                = (char*)malloc((strlen(guess) + 1) * sizeof(char));
        strcpy(guesses[num_guesses], guess);
        num_guesses++;
        if (!is_word_in_dictionary(guess)) {
            printf("Word can't be found in the dictionary file\n");
            return;
        }
        update_score(guess);
    }
}

/* free_mem()
 * ----------
 *  Function to free used memory
 */
void free_mem()
{
    for (int i = 0; i < num_guesses; i++) {
        free(guesses[i]);
    }
}

/* The main function which calls upon helper functions to run the game
 */
int main(int argc, char* argv[])
{
    all_checks(argc, argv);
    char guess[MAX_WORD_LENGTH];
    while (1) {
        if (!fgets(guess, MAX_WORD_LENGTH, stdin)) {
            if (score == 0) {
                printf("No valid words guessed!\n");
                exit(ERROR_7);
            } else {
                printf("Your final score is %d\n", score);
                exit(0);
            }
        }
        guess[strcspn(guess, "\n")] = '\0'; // Remove trailing newline
        validate_guess(guess);
        if (feof(stdin)) {
            if (score == 0) {
                printf("No valid words guessed!\n");
                exit(ERROR_7);
            } else {
                printf("Your final score is %d\n", score);
                exit(0);
            }
        }
    }
    free_mem();
}
