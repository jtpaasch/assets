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
 *  NON-CONSTANT VARIABLES THAT CAN BE SET
 *
 *  ------------------------------------------------------------
 */
int has_blacklist_additions = 0;
char *blacklist_additions;
char *blacklist;


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
  puts("This program crawls a folder and generates a dictionary");
  puts("of all assets it finds, noting the path, type of file, etc.");
  puts("");
  puts("Usage: assets <folder> [<output-file>] [options]");
  puts(" <folder>      : is the folder to crawl for assets");
  puts(" <output-file> : is where to save the dictionary");
  puts("");
  puts("Options:");
  puts("--cachebust     : renames files with cachebusting names");
  puts("--base64 <size> : base64 encode files smaller than <size> bytes");
  puts("--ignore file1,file2,file3 : ignore the specified files"); 
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

    // We want to find certain things in the arguments.
    // Here are some flags that specify whether we find a blacklist, 
    // a directory to crawl, and an output file to write to.
    int has_folder_to_crawl = 0;
    int has_output_file = 0;

    // We'll store the path to the folder to crawl here:
    char folder_to_crawl[MAX_PATH_LENGTH];

    // Now we can process each argument.
    int i;
    for (i = 1; i < number_of_arguments; i++) {

      // Is this argument the optional "--cachebust"?
      if (strcmp(argument[i], "--cachebust") == 0) {
        set_cachebust(1);
      }

      // Is this argument the optional "--ignore"? 
      else if (strcmp(argument[i], "--ignore") == 0) {

        // The string of items to blacklist/ignore will be the next argument.
        blacklist_additions = argument[i + 1];
        has_blacklist_additions = 1;

        // Increment the counter so the next iteration of the loop
        // won't try to process the blacklist string.
        i++;

      } 

      // Is this argument the optional "--base64"?
      else if (strcmp(argument[i], "--base64") == 0) {

        // The max size of base64 content will be the next argument.
	set_max_filesize_to_base64_encode(atoi(argument[i + 1]));

	// Increment the counter so the next iteration skips that argument.
	i++;

      }

      // Otherwise, this argument isn't an optional argument.
      else {

        // Have we found the folder to crawl yet?
        if (!has_folder_to_crawl) {

          // Calculate the real path to the folder.
          set_real_path(folder_to_crawl, argument[i]);

          // Set the flag saying we've found it.
          has_folder_to_crawl = 1;

        }

        // Have we found an output file to write yet?
        else if (!has_output_file) {

          // Calculate the real path to this file.
          char output_file[MAX_PATH_LENGTH];
          set_real_path(output_file, argument[i]);

          // Set this file as our log file.
          set_log_file(output_file);

          // We need to set our logging_type to '1', 
          // which stands for 'write to file'.
          set_logging_type(1);

          // Set the flag saying te've found it.
          has_output_file = 1;

        }

      }

    }

    // If we got no folder to crawl, we can't proceed.
    if (!has_folder_to_crawl) {
      print_usage();
    }

    // Otherwise, we can get on with it.
    else {

      // Initialize the blacklist.
      if (has_blacklist_additions) {
        blacklist = malloc(strlen(blacklist_additions) + 5);
        initialize_string(blacklist);
        add_to_string(blacklist, ".,..,");
        add_to_string(blacklist, blacklist_additions);
      } else {
        blacklist = malloc(4);
        initialize_string(blacklist);
        add_to_string(blacklist, ".,..");
      }

      // Start the logging.
      start_logging();

      // Walk the tree.
      walk(folder_to_crawl, blacklist);

      // Stop the logging.
      stop_logging();

    }

  }

  return 0;

}


