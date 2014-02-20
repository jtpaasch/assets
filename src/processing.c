/***************************************************************
 *
 *    ASSETS
 *
 *    This program crawls a directory tree and
 *    makes a record of all assets it finds.
 *
 *    This file provides a number of utiities
 *    for processing information about files.
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

// We want to use our utilities.
#include "utilities.h"

// We want to use our logging/writing tools.
#include "logging.h"

// We need the header that declares the prototypes for this file.
#include "processing.h"


/*  ------------------------------------------------------------
 *
 *  FUNCTION DEFINITIONS
 *  Note: function prototypes are defined in processing.h
 *
 *  ------------------------------------------------------------
 */

/*
 *  Find the path of a directory (everything up to its filename).
 *
 *  @param char *variable The variable to store the path in.
 *  @param char *full_path The full path.
 *  @return void
 */
void base_path(char *variable, char *full_path) {

  // We'll need to find the index of the last "/".
  int index;
  int needle = '/';

  // Find the position of that last "/".
  last_char_pos(&index, full_path, needle);

  // If there's no slash in the path, then we can just use the full path.
  if (index == 0) {
    initialize_string(variable);
    add_to_string(variable, full_path);
  }

  // Otherwise, we want everything up through the last slash.
  else {
    substr(variable, full_path, index + 1);
  }

}

/*
 *  Strip the extension off the filename.
 *
 *  @param char *variable The variable to store the result in.
 *  @param char *filename The filename to examine.
 *  @return void
 */
void filename_without_extension(char *variable, char *filename) {

  // We'll need to find the index of the last dot.
  int index;

  // Find the position of that last dot.
  last_char_pos(&index, filename, '.');

  // If there's no dot in the filename, then we want the full filename.
  if (index == 0) {
    initialize_string(variable);
    add_to_string(variable, filename);
  }

  // Otherwise, we want to get everything up to dot's index.
  else {
    substr(variable, filename, index);
  }

}

/*
 *  Store the specified filename's extension ("png", "svg", etc.) 
 *  in the specified variable.
 *
 *  @param char *variable The variable to store the extension in.
 *  @param char *filename The filename to extract the extension from.
 *  @return void
 */
void extension(char *variable, char *filename) {

  // Make sure the variable is initialized (reset to nothing).
  initialize_string(variable);

  // Where is everything after the last dot?
  char *dot = strrchr(filename, '.');

  // In some cases, we have no extension.
  if (!dot || dot == filename) {
    add_to_string(variable, "");
  }

  // Otherwise, add the extension (starting 1 character after the dot).
  else {
    add_to_string(variable, dot + 1);
  }

}

/*
 *  Process a file and gather information about it.
 *
 *  @param char *path The path to the file.
 *  @param struct stat *info Info about the file returned by `stat()`.
 *  @return void
 */
void process_file(char *path, struct stat *info) {

  // Get the filename from this path.
  char *filename = basename(path);

  // Get the extension for this file.
  char file_extension[MAX_EXTENSION_LENGTH];
  extension(file_extension, filename);

  // Get the filename without the extension.
  char key[MAX_FILENAME_LENGTH];
  filename_without_extension(key, filename);

  // Get the base path for this file.
  char file_path[MAX_PATH_LENGTH];
  base_path(file_path, path);

  // Start building the entry for this file.
  char entry[MAX_ENTRY_LENGTH];
  initialize_string(entry);
  add_to_string(entry, "{");

  // Add the key.
  add_to_string(entry, "\"key\": \"");
  add_to_string(entry, key);
  add_to_string(entry, "\",");

  // Add the directory.
  add_to_string(entry, "\"directory\": \"");
  add_to_string(entry, file_path);
  add_to_string(entry, "\",");

  // Add the filename.
  add_to_string(entry, "\"filename\": \"");
  add_to_string(entry, filename);
  add_to_string(entry, "\",");

  // Add the extension.
  add_to_string(entry, "\"extension\": \"");
  add_to_string(entry, file_extension);
  add_to_string(entry, "\",");

  // Add the local.
  add_to_string(entry, "\"locale\": \"en-us\"");

  // Finish building the entry.
  add_to_string(entry, "}");

  // Now log it.
  put_to_log(entry);

}

/*
 *  Walk a directory tree.
 *
 *  @char *path The folder to walk.
 *  @char *blacklist[] A list of files to ignore.
 *  @int length_of_blacklist The number of items on the blacklist.
 *  @return void
 */
void walk(char *path, char *blacklist[], int length_of_blacklist) {

  // When we open a stream to the path, we'll store it here:
  DIR *stream;

  // When we read a list of items from the directory, 
  // we'll store each item here:
  struct dirent *item;

  // When we try to stat() a file, we'll store the info
  // it returns here:
  struct stat info;

  // Open a stream to the path/directory.
  stream = opendir(path);

  // Did we get a valid stream? 
  if (stream != NULL) {

    // Read the stream one item at a time.
    while ((item = readdir(stream))) {

      // If the item is not in the blacklist, we can proceed.
      if (!string_is_in_list(blacklist, length_of_blacklist, item->d_name)) {

        // Construct the path to this file/folder item.
        char full_path[MAX_PATH_LENGTH];
        build_path(full_path, path, item->d_name);

        // Try to get some info on this item.
        int got_info = stat(full_path, &info);

        // If got_info is 0, that means we got information about the file.
        // I know, it's weird, but in C, `stat()` returns a status code, 
        // not a boolean. So `0` = success, and `1` = fail. To make it
        // a boolean, let's reverse it's value.
        int success = !got_info;

        // Now we can check if we were successful in a truthy sort of way.
        if (success) {

          // Is it a directory? If so, look in it (recursively).
          if (is_dir(&info)) {
            walk(full_path, blacklist, length_of_blacklist);
          }

          // Is it a file? If so, process it.
          else if (is_file(&info)) {
            process_file(full_path, &info);
          }

        } 
        // If we didn't get any information about the file,
        // print a message saying so.
        else {
          puts("Could not get any information on this file:");
          printf("%s\n", full_path);
          exit(1);
	}

      }

    }

    // Close the stream to the directory.
    (void) closedir(stream);

  }

  // If we couldn't open the directory, exit with a message.
  else {
    puts("Could not open the following path:");
    printf("%s\n", path);
    exit(1);    
  }

}
