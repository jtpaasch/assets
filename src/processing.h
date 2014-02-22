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
 *    Copyright 2014 JT Paasch
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
#define MAX_ENTRY_LENGTH 2000
#define MAX_COMMAND_LENGTH 1024


/*  ------------------------------------------------------------
 *
 *  FUNCTION PROTOTYPES
 *  Note: These functions are implemented in processing.c
 *
 *  ------------------------------------------------------------
 */

void base_path(char *variable, char *full_path);
void filename_without_extension(char *variable, char *filename);
void extension(char *variable, char *filename);
void md5(char *variable, char *path);
void process_file(char *path, struct stat *info);
void walk(char *path, char *blacklist);


#endif
