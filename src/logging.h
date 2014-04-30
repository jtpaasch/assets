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
 *    Copyright 2014 Nara Logics
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
void start_logging(void);
void stop_logging(void);
void put_to_log(const char *message);
void print_to_stdout(const char *message);
void print_to_file(const char *message, const char *path);

#endif
