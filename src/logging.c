/***************************************************************
 *
 *    ASSETS
 *
 *    This program crawls a directory tree and
 *    makes a record of all assets it finds.
 *
 *    This file provides utilities for writing/logging.
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

// For working with strings, e.g., `strcat()`.
#include <string.h>

// We need the header that declares the prototypes for this file.
#include "logging.h"


/*  ------------------------------------------------------------
 *
 *  NON-CONSTANTS THAT CAN BE SET.
 *
 *  ------------------------------------------------------------
 */

// This specifies the type of logging we want.
// 0 - STDOUT
// 1 - Write to a file.
int logging_type = 0;

// The path to a file to write logging to.
char *log_file_path;

// A delimiter to separate logged records. 
char delimiter[1];

// A flag to say if we're using delimiters or not.
int use_delimiter = 0;


/*  ------------------------------------------------------------
 *
 *  FUNCTION DEFINITIONS
 *  Note: function prototypes are defined in logging.h
 *
 *  ------------------------------------------------------------
 */

/*
 *  Set the logging type.
 *
 *  @param int new_value The new value to set it to.
 *  @return void
 */
void set_logging_type(int new_value) {
  logging_type = new_value;
}

/*
 *  Set the path to the log file.
 *
 *  @param char *path The path to the file.
 *  @return void
 */
void set_log_file(char *path) {
  log_file_path = path;
}

/*
 *  Start the logging.
 *
 *  @return void
 */
void start_logging() {

  // If we're writing to a file,
  // delete the file first (it will be recreated).
  if (logging_type == 1) {
    remove(log_file_path);
  }

  // Start with no delimiter.
  delimiter[0] = '\0';

  // Open with an opening brace.
  put_to_log("[");

  // Now we can use delimiters.
  use_delimiter = 1;

}

/*
 *  Stop the logging.
 *
 *  @return void
 */
void stop_logging() {
  use_delimiter = 0;
  put_to_log("]");
}

/*
 *  Log the specified message.
 *
 *  @param char *message The message to log.
 *  @return void
 */
void put_to_log(char *message) {

  // If the logging type is "0", we just print to STDOUT.
  if (logging_type == 0) {
    print_to_stdout(message);
  }

  // If it's "1", we write to a file.
  else if (logging_type == 1) {
    print_to_file(message, log_file_path);
  }

}

/*
 *  Print a message to stdout.
 *
 *  @param char *message The message to print.
 *  @return void
 */
void print_to_stdout(char *message) {
  if (use_delimiter) {
    printf("%s%s", delimiter, message);
    if (delimiter[0] == '\0') {
      delimiter[0] = ',';
    }
  } else {
    printf("%s", message);
  }
}

/*
 *  Write a message to a file.
 *
 *  @param char *message The message to write.
 *  @return void
 */
void print_to_file(char *message, char *path) {

  // We'll store the stream to the file here:
  FILE *file;

  // Open the file.
  file = fopen(path, "a");

  // Did it work?
  if (file != NULL) {

    // Write the message.
    if (use_delimiter) {
      fprintf(file, "%s%s", delimiter, message);
      if (delimiter[0] == '\0') {
        delimiter[0] = ',';
      }
    } else {
      fprintf(file, "%s", message);
    }

    // Close the stream.
    fclose(file);

  }

  // Otherwise, print a message.
  else {
    puts("Could not write to this file:");
    printf("%s\n", path);
  }

}
