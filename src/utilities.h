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
 *    Copyright 2014 Nara Logics
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
void add_to_string(char *string, const char *addition);
void set_real_path(char *variable, const char *stipulated_path);
void build_path(char *variable, const char *base_path, const char *filename);
int string_is_in_list(const char *haystack, const char *needle);
int is_on_filesystem(const char *path);
int is_dir(struct stat *info);
int is_image(const char *extension);
int is_file(struct stat *info);
void first_char_pos(int *index, const char *haystack, int needle);
void last_char_pos(int *index, const char *haystack, int needle);
void substr(char *substring, const char *haystack, int index);
int delimiter_count(const char *haystack, const char *delimiter);
void explode(char *variable[], char *haystack, const char *delimiter);

#endif
