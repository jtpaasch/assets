/***************************************************************
 *
 *    ASSETS
 *
 *    This program crawls a directory tree and
 *    makes a record of all assets it finds.
 *
 *    This file is the header for logging.c 
 *
 *    Author JT Paasch
 *    Copyright 2014 JT Paasch
 *    License MIT (included with this source code)
 *
 **************************************************************/

#ifndef LOGGING_H
#define LOGGING_H


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
 *  Note: These functions are implemented in logging.c
 *
 *  ------------------------------------------------------------
 */

void set_logging_type(int new_value);
void set_log_file(char *path);
void start_logging();
void stop_logging();
void put_to_log(char *message);
void print_to_stdout(char *message);
void print_to_file(char *message, char *path);

#endif
