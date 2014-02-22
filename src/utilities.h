/***************************************************************
 *
 *    ASSETS
 *
 *    This program crawls a directory tree and
 *    makes a record of all assets it finds.
 *
 *    This file is the header for utilities.c 
 *
 *    Author JT Paasch
 *    Copyright 2014 JT Paasch
 *    License MIT (included with this source code)
 *
 **************************************************************/

#ifndef UTILITIES_H
#define UTILITIES_H


/*  ------------------------------------------------------------
 *
 *  DEF/CONSTANTS
 *
 *  ------------------------------------------------------------
 */
#define MAX_PATH_LENGTH 1024


/*  ------------------------------------------------------------
 *
 *  FUNCTION PROTOTYPES
 *  Note: These functions are implemented in utilities.c
 *
 *  ------------------------------------------------------------
 */

void initialize_string(char *name);
void add_to_string(char *string, char *addition);
void set_real_path(char *variable, char *stipulated_path);
void build_path(char *variable, char *base_path, char *filename);
int string_is_in_list(char *haystack, char *needle);
int is_on_filesystem(char *path);
int is_dir(struct stat *info);
int is_file(struct stat *info);
void first_char_pos(int *index, char *haystack, int needle);
void last_char_pos(int *index, char *haystack, int needle);
void substr(char *substring, char *haystack, int index);
int delimiter_count(char *haystack, char *delimiter);
void explode(char *variable[], char *haystack, char *delimiter);

#endif
