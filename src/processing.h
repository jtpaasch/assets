/***************************************************************
 *
 *    ASSETS
 *
 *    This program crawls a directory tree and
 *    makes a record of all assets it finds.
 *
 *    This file is the header for processing.c 
 *
 *    Author JT Paasch
 *    Copyright 2014 Nara Logics
 *    License MIT (included with this source code)
 *
 **************************************************************/

#ifndef PROCESSING_H
#define PROCESSING_H


/*  ------------------------------------------------------------
 *
 *  DEF/CONSTANTS
 *
 *  ------------------------------------------------------------
 */
#define MAX_PATH_LENGTH 1024
#define MAX_EXTENSION_LENGTH 32
#define MAX_FILENAME_LENGTH 100
#define MAX_COMMAND_LENGTH 1024


/*  ------------------------------------------------------------
 *
 *  FUNCTION PROTOTYPES
 *  Note: These functions are implemented in processing.c
 *
 *  ------------------------------------------------------------
 */

void set_max_entry_length(int size);
void set_cachebust(int flag);
void set_max_filesize_to_base64_encode(int size);
void base_path(char *variable, const char *full_path);
void filename_without_extension(char *variable, const char *filename);
void extension(char *variable, const char *filename);
void md5(char *variable, const char *path);
void base64(char *variable, const char *path);
void cachebust_filename(char *var, const char *key, const char *hash, const char *ending);
void process_file(char *path, struct stat *info);
void walk(char *path, const char *blacklist);


#endif
