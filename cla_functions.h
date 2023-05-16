#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

/* Function: get_num(char *str,int type)
 * ----------------------------
 *  Returns a string of the characters following a prefix of type, type in str.
 *      type can have the values 9 or 10:
 *      str begins with "--tdelay=" if type has the value 9
 *      str begins with "--samples=" if type has the value 10
 * 
 *  str:       a string with the prefix "--tdelay=" or "--samples="
 *  type:      the type of prefix str begins with
 * 
 *  return: a string of the characters following a substring of type, type in str
 */
char* get_num(char *str,int type);

/* Function: isvalid_cla(char *arg)
 * ----------------------------
 *  Returns the type of command line argument of arg.
 *  If arg is invalid, -1 is returned.
 * 
 *          Valid command line arguments:          Return:
 *          --system                                  1
 *          --user                                    2
 *          --graphics                                3
 *          --sequential                              4
 *          --samples=                                5
 *          --tdelay=                                 6
 *
 *  arg:       a command line argument
 * 
 *  return: the type of command line argument of arg or -1 if arg is invalid
 */
int isvalid_cla(char *arg);

/* Function: isvalid_num(char *num)
 * ----------------------------
 *  Returns true when num is composed of only numbers and false otherwise.
 *
 *  num:        the string being verified
 * 
 *  return:     true when n is composed of only numbers, false otherwise
 */
bool isvalid_num(char *num);

/* Function: set_cla(int *argc, char **argv, int* n, int* t, bool *graphics, bool *sys, bool *user, bool *seq)
 * ----------------------------
 *  Checks if all command line arguments in argv are valid and sets them if they are.
 *  Exits if any command line arguments are invalid.
 *
 *  argc:       the number of command line arguments
 *  argv:       the command line arguments
 *  n:          the number of data samples to output
 *  t:          the frequency of taking data samples in seconds
 *  graphics:   toggle to indicate if graphic should be generated
 *  sys:        toggle to indicate that only the system usage should be generated
 *  user:       toggle to indicate that only the users usage should be generated
 *  seq:        toggle to indicate that the information will be output sequentially
 * 
 *  return:     true when all command line arguments are valid, false otherwise
 * 
 * All toggles are ON when set to true and OFF when set to false
 */
bool set_cla(int *argc, char **argv, int* n, int* t, bool *graphics, bool *sys, bool *user, bool *seq);