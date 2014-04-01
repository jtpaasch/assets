/***************************************************************
 *
 *    ASSETS
 *
 *    This program crawls a directory tree and
 *    makes a record of all assets it finds.
 *
 *    This file provides a number of utiities
 *    for the ASSETS program.
 *
 *    Author JT Paasch
 *    Copyright 2014 JT Paasch
 *    License MIT (included with this source code)
 *
 **************************************************************/


/*  ------------------------------------------------------------
 *
 *  IMPORT LIBRARIES
 *
 *  ------------------------------------------------------------
 */

// The standard C library.
#include <stdio.h>

// For things like `exit(0)`.
#include <stdlib.h>

// For using the `stat()` function.
#include <sys/stat.h>

// For working with strings, e.g., `strcat()`.
#include <string.h>

// For working with directory entities (on POSIX systems).
#include <dirent.h>

// For functions like `basename()`.
#include <libgen.h>

// We need the header that declares the prototypes for this file.
#include "utilities.h"


/*  ------------------------------------------------------------
 *
 *  FUNCTION DEFINITIONS
 *  Note: function prototypes are defined in utilities.h
 *
 *  ------------------------------------------------------------
 */

/*
 *  Create (or re-initialize) a string (char array).
 *
 *  @param char *name The string to initialize.
 *  @return void
 */
void initialize_string(char *name) {
  name[0] = '\0';
}

/*
 *  Add a string to a string (concat).
 *
 *  @param char *string The string to add to.
 *  @param char *addition The string to add.
 *  @return void
 */
void add_to_string(char *string, char *addition) {
  strcat(string, addition);
}

/*
 *  Calculate the real path of the stipulated path,
 *  and store it in the specified variable.
 *
 *  @param char *variable The variable to store the real path in.
 *  @param char *stipulated_path The path to resolve.
 *  @return void
 */
void set_real_path(char *variable, char *stipulated_path) {

  // Make sure the variable is initialized (reset to nothing).
  initialize_string(variable);

  // Calculate the actual path.
  realpath(stipulated_path, variable);

}

/*
 *  Build a path by appending a filename to a base path.
 *
 *  @return char *variable The variable to store the built path in.
 *  @return char *base_path The base path.
 *  @return char *filename The filename.
 *  @return void
 */
void build_path(char *variable, char *base_path, char *filename) {
  initialize_string(variable);
  add_to_string(variable, base_path);
  add_to_string(variable, "/");
  add_to_string(variable, filename);
}

/*
 *  Find if a string is in a comma separated list of strings.
 *
 *  @param char *haystack The comma separated list to look in.
 *  @param char *needle The string to look for.
 *  @return int 1 if it was found, 0 if not.
 */
int string_is_in_list(char *haystack, char *needle) {

  // strtok is destructive, so we need to work on a copy of the haystack.
  char haystack_copy[strlen(haystack)];
  strcpy(haystack_copy, haystack);

  // Walk through the haystack copy, looking for the needle.
  char *token;
  char delimiter[] = ",";
  token = strtok(haystack_copy, delimiter);
  while (token != NULL) {
    if (strncmp(token, needle, strlen(needle)) == 0) {
      return 1;
    }
    token = strtok(NULL, delimiter);
  }

  // No match was found if we got to here.
  return 0;

}

/*
 *  Check if the stipulated file or folder exists.
 *
 *  @param char *path The path to check.
 *  @return int 1 if true, 0 if false.
 */
int is_on_filesystem(char *path) {

  // We'll store information about the file/folder 
  // we find at the end of that path here:
  struct stat info;

  // Try to run stat() on the file/folder, and get info about it.
  // If this succeeds, we know the file exists on the system.
  if (stat(path, &info) == 0) {
    return 1;
  }

  // If that failed, there was an error running stat().
  else {
    puts("Error: could not gather information about this file:");
    printf("%s\n", path);
    return 0;
  }

}

/*
 *  Is the item described by `info` a directory?
 *
 *  @param struct stat *info The info returned by `stat()`.
 *  @return int 1 If yes, 0 if no.
 */
int is_dir(struct stat *info) {
  return (info->st_mode & S_IFDIR);
}

/*
 *  Is the item described by `info` an image?
 *
 *  @param *extension file extension.
 *  @return int 1 If true, 0 if false.
 */
int is_image(char *extension) {
  if(strcmp(extension, "gif") == 0) {
    return 1;
  }
  if(strcmp(extension, "jpg") == 0) {
    return 1;
  }
  if(strcmp(extension, "jpeg") == 0) {
    return 1;
  }
  if(strcmp(extension, "png") == 0) {
    return 1;
  }
  if(strcmp(extension, "svg") == 0) {
    return 1;
  }
  return 0;
}

/*
 *  Is the item described by `info` a file?
 *
 *  @param struct stat *info The info returned by `stat()`.
 *  @return int 1 If yes, 0 if no.
 */
int is_file(struct stat *info) {
  return (info->st_mode & S_IFREG);
}

/*
 *  Find the first index/position of a character in a string.
 *
 *  @param int index The variable to store the index in.
 *  @param char *haystack The string to search in.
 *  @param int needle The character to search for.
 *  @return void
 */
void first_char_pos(int *index, char *haystack, int needle) {
  char *match;
  match = strchr(haystack, needle);
  *index = (int) (match - haystack);
}

/*
 *  Find the last index/position of a character in a string.
 *
 *  @param int index The variable to store the index in.
 *  @param char *haystack The string to search in.
 *  @param int needle The character to search for.
 *  @return void
 */
void last_char_pos(int *index, char *haystack, int needle) {
  char *match;
  match = strrchr(haystack, needle);
  if (match != NULL) {
    *index = (int) (match - haystack);
  }
}

/*
 *  Get a substring from a haystack.
 *
 *  @param char *substring The variable to store the substring in.
 *  @param char *haystack The string to extract the substring from.
 *  @param int index The number of characters to extract from the haystack.
 */
void substr(char *substring, char *haystack, int index) {
  strncpy(substring, haystack, index);
  substring[index] = '\0';
}

/*
 *  Find how many times a delimiter character occurs in a string.
 *
 *  @param char *haystack The string to search in.
 *  @param char *delimiter The character to count how many times it occurs.
 *  @return int The number of occurrences.
 */
int delimiter_count(char *haystack, char *delimiter) {
  char *token_position;
  int number_of_tokens = 0;
  token_position = strpbrk(haystack, delimiter);
  while (token_position != NULL) {
    number_of_tokens++;
    token_position = strpbrk(token_position + 1, delimiter);
  }
  if (number_of_tokens > 0) {
    return number_of_tokens + 1;
  } else {
    return 0;
  }
}

/*
 *  Break a string up into substrings.
 *
 *  @param char *variable[] The array to store the substrings in.
 *  @param char *haystack The string to break up.
 *  @param char *delimiter The character to split the string by.
 *  @return void
 */
void explode(char *variable[], char *haystack, char *delimiter) {
  char *token_position;
  int i = 0;
  token_position = strtok(haystack, delimiter);
  while (token_position != NULL) {
    variable[i++] = token_position;
    token_position = strtok(NULL, delimiter);
  }
}
