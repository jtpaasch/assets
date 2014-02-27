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

// For checking character types, e.g., `isspace()`.
#include <ctype.h>

// We want to use our utilities.
#include "utilities.h"

// We want to use our logging/writing tools.
#include "logging.h"

// We need the header that declares the prototypes for this file.
#include "processing.h"


/*  ------------------------------------------------------------
 *
 *  NON-CONSTANT VARIABLES
 *
 *  ------------------------------------------------------------
 */
int max_entry_length = 1024;
int cachebust = 0;
int max_base64_size = 0;
int max_filesize_to_base64_encode = 0;
int max_chars_in_base64_strings = 0;


/*  ------------------------------------------------------------
 *
 *  FUNCTION DEFINITIONS
 *  Note: function prototypes are defined in processing.h
 *
 *  ------------------------------------------------------------
 */

/*
 *  Set the max size of each entry.
 *
 *  @param int size The size.
 *  @return void
 */
void set_max_entry_length(int size) {
  max_entry_length += size;
}

/*
 *  Set the cachebust flag.
 *
 *  @param int flag 1 to turn cachebust naming on, 0 to turn it off.
 *  @return void
 */
void set_cachebust(int flag) {
  cachebust = flag;
}

/*
 *  Set the max size of base64 content.
 *
 *  @param int size The max number of characters.
 *  @return void
 */
void set_max_filesize_to_base64_encode(int size) {
  max_filesize_to_base64_encode = size;
  max_chars_in_base64_strings = (int) ((size * 1.37) + 820);
  set_max_entry_length(max_chars_in_base64_strings);
}

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
 *  Get the md5 hash of a file.
 *
 *  @param char *variable The variable to store the hash in.
 *  @param char *path The path to the file.
 */
void md5(char *variable, char *path) {

  // Make sure md5sum is installed on the system.
  int status_code = system("which md5sum >/dev/null 2>&1");
  if (status_code == 0) {

    // Build a command to get the md5 sum for the file.
    char command[MAX_COMMAND_LENGTH];
    initialize_string(command);
    add_to_string(command, "md5sum ");
    add_to_string(command, path);

    // Open a stream that runs the command.
    FILE *stream = popen(command, "r");
    if (stream != NULL) {

      // Get the first 32 characters.
      fgets(variable, 32, stream);

      // Close the stream.
      pclose(stream);

    }

  }

}

/*
 *  Get the base64 encoded string of a file's contents.
 *
 *  @param char *variable The variable to store the encoded string in.
 *  @param char *path The path to the file.
 */
void base64(char *variable, char *path, int filesize) {

  // Make sure base64 is installed on the system.
  int status_code = system("which base64 >/dev/null 2>&1");
  if (status_code == 0) {

    // Build a command to get the base64 string of the file's contents.
    char command[MAX_COMMAND_LENGTH];
    initialize_string(command);
    add_to_string(command, "base64 --wrap=0 ");
    add_to_string(command, path);

    // Open a stream that runs the command.
    FILE *stream = popen(command, "r");
    if (stream != NULL) {

      // Read the response, character by character.
      char character;
      int i = 0;
      while ((character = fgetc(stream)) != EOF) {
        variable[i++] = character;
      }

      // Walk the string backwards and remove any spaces.
      i = i - 1;
      while (isspace(variable[i])) {
        variable[i] = '\0';
        i--;
      }

      // Close the stream.
      pclose(stream);

    }

  }

}

/*
 *  Create a cachebusted filename of the form: 
 *  key.hash.ending
 *
 *  @param char *var The variable to store the cachebusted filename in.
 *  @param char *key The key/name of the file.
 *  @param char *hash The hash to add to the filename. 
 *  @param char *ending The extension to add to the ending.
 */
void cachebust_filename(char *var, char *key, char *hash, char *ending) {
    initialize_string(var);
    add_to_string(var, key);
    add_to_string(var, ".");
    add_to_string(var, hash);
    if (strlen(ending) > 0) {
      add_to_string(var, ".");
      add_to_string(var, ending);
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

  // Get the md5 of this file.
  char hash[32];
  md5(hash, path);

  // Get the base64 encoded contents of this file.
  char base64_content[max_chars_in_base64_strings];
  if (info->st_size <= max_filesize_to_base64_encode) {
    base64(base64_content, path, info->st_size);
  }

  // We'll store the cachebusted filename here:
  char cachebusted_filename[MAX_FILENAME_LENGTH];

  // Are we going to cache bust the filename? 
  if (cachebust) {

    // Construct the cache busted filename.
    cachebust_filename(cachebusted_filename, key, hash, file_extension);

    // Construct a full path to the new cache-busted filename
    char new_path[MAX_PATH_LENGTH];
    initialize_string(new_path);
    add_to_string(new_path, file_path);
    add_to_string(new_path, cachebusted_filename);

    // Rename the file.
    int rename_success;
    rename_success = rename(path, new_path);
    if (rename_success != 0) {
      puts("Could not rename this file:");
      printf("%s\n", filename);
      exit(1);
    } 

  }

  // Start building the entry for this file.
  char entry[max_entry_length];
  initialize_string(entry);
  add_to_string(entry, "{");

  // Add the key.
  add_to_string(entry, "\"key\":\"");
  add_to_string(entry, key);
  add_to_string(entry, "\",");

  // Add the directory.
  add_to_string(entry, "\"directory\":\"");
  add_to_string(entry, file_path);
  add_to_string(entry, "\",");

  // Add the filename.
  add_to_string(entry, "\"filename\":\"");
  if (cachebust) {
    add_to_string(entry, cachebusted_filename);
  } else {
    add_to_string(entry, filename);
  }
  add_to_string(entry, "\",");

  // Add the extension.
  add_to_string(entry, "\"extension\":\"");
  add_to_string(entry, file_extension);
  add_to_string(entry, "\",");

  // Add the base64 content.
  if (info->st_size <= max_filesize_to_base64_encode) {
    add_to_string(entry, "\"base64\":\"");
    add_to_string(entry, base64_content);
    add_to_string(entry, "\",");
  }

  // Add the md5.
  add_to_string(entry, "\"md5\":\"");
  add_to_string(entry, hash);
  add_to_string(entry, "\"");

  // Finish building the entry.
  add_to_string(entry, "}");

  // Now log it.
  put_to_log(entry);

}

/*
 *  Walk a directory tree.
 *
 *  @char *path The folder to walk.
 *  @char *blacklist A comma separated list of files to ignore.
 *  @return void
 */
void walk(char *path, char *blacklist) {

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
      if (!string_is_in_list(blacklist, item->d_name)) {

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
            walk(full_path, blacklist);
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
