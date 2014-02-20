Assets
======

This command line tool walks a directory and generates a JSON dictionary of all assets/files in the directory.


Installation
------------

Download or clone the repo, cd into the folder, and run `make`. That will compile the executable. To install it, run `make install` (if it asks for a password, please provide it).


Usage
-----

Type this:

    $ assets .

That will print to the screen a JSON dictionary of all assets found in the current directory (including assets in any subfolders). The JSON structure should look something like this:

    [
      {
        "key": "file1",
        "directory": "/home/users/sally/",
        "filename": "file1.txt",
        "extension": "txt",
        "locale": "en-us"
      },
      ...
    ]

You can specify any directory you like. For instance:

    $ assets /home/public_html/public/images

You can pipe the output to a file:

    $ assets . >> assets.json

Or, you can simply specify the name of the file as the second argument:

    $ assets . assets.json

