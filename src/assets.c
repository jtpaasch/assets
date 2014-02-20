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

// Our own utilities are defined in utilities.h.
#include "utilities.h"

// Our tools for processing files are defined in processing.h.
#include "processing.h"

// Our tools for logging/writing output are defined in logging.h.
#include "logging.h"


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
 * 
 *  ------------------------------------------------------------
 */
void print_usage();


/*  ------------------------------------------------------------
 *
 *  FUNCTION DEFINITIONS
 * 
 *  ------------------------------------------------------------
 */

/*
 *  Print usage instructions.
 *
 *  @return void
 */
void print_usage() {
  puts("");
  puts("Assets 0.1");
  puts("----------");
  puts("This program crawls a folder and makes a detailed dictionary");
  puts("of all assets it finds, noting the path, type of file, etc.");
  puts("");
  puts("Usage: assets <folder> <output-file>");
  puts("-- <folder> is the folder to crawl for assets");
  puts("-- <output-file> is where to save the dictionary");
  puts("");
  puts("Example: assets . assets.json");
  puts("-- This will crawl the current directory (\".\")");
  puts("   and create a dictionary of all assets it finds");
  puts("   in a file called \"assets.json\".");
  puts("");
}



/*  ------------------------------------------------------------
 *
 *  MAIN FUNCTION
 *
 *  ------------------------------------------------------------
 *
 *  This is the MAIN entry point into the program,
 *  meaning that when the program runs,
 *  this function gets executed first.
 *
 *  @param int number_of_arguments The number of arguments.
 *  @param char *argument[] The list of arguments.
 *  @return int A status code.
 */
int main(int number_of_arguments, char *argument[]) {

  // Are there at least two arguments? If not, print the help.
  if (number_of_arguments < 2) {
    print_usage();
  } 

  // Otherwise, we can proceed.
  else {

    // The first argument is the folder to crawl.
    char folder_to_crawl[MAX_PATH_LENGTH];
    set_real_path(folder_to_crawl, argument[1]);

    // Is there a second argument?
    if (number_of_arguments > 2) {

      // The second argument is the output file to write to.
      char output_file[MAX_PATH_LENGTH];
      set_real_path(output_file, argument[2]);

      // Set this file as our log file.
      set_log_file(output_file);

      // We need to set our logging_type to '1', 
      // which stands for 'write to file'.
      set_logging_type(1);

    }

    // What files should we ignore?
    int length_of_blacklist = 2;
    char *blacklist[length_of_blacklist];
    blacklist[0] = ".";
    blacklist[1] = "..";

    // Start the logging.
    start_logging();

    // Walk the tree.
    walk(folder_to_crawl, blacklist, length_of_blacklist);

    // Stop the logging.
    stop_logging();

  }

  exit(0);

}


