Assets
======

This command line tool walks a directory and generates a JSON dictionary of all assets/files in the directory.


Installation
------------

Download or clone the repo, cd into the folder, and run `make`. That will compile the executable. To install it, run `make install` (if it asks for a password, please provide it).


Usage
-----

The command takes the form

    $ assets <directory>

where `<directory>` is the path to the folder whose assets you want to examine.

For instance, type this:

    $ assets .

That will print to the screen a JSON dictionary of all assets found in the current directory (including assets in any subfolders). The JSON structure should look something like this:

    [
      {
        "key": "file1",
        "directory": "/home/users/sally/",
        "filename": "file1.txt",
        "extension": "txt",
      },
      ...
    ]

You can specify any directory you like. For instance,

    $ assets /home/public_html/public/images

will examine `/home/public_html/public/images`.


Output
------

By default, the `assets` tool prints the JSON to stdout (typically the screen), but you can pipe the output to a file if you like:

    $ assets . >> assets.json

Or, you can simply specify the name of the file as the second argument:

    $ assets . assets.json

Options
-------

If you want the `assets` program to cachebust your filenames, use the `--cachebust` flag:

    $ assets . --cachebust

That will rename all files by appending the md5 hash of the file. Files named in the form `<filename>.<extension>` become `<filename>.<md5-hash>.<extension>`.

If you want to ignore files or directories, use `--ignore`, followed by a comma separated list of filenames. For insance:

    $ assets . --ignore .git,.gitignore,dist

