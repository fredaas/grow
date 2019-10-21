GROW
--------------------------------------------------------------------------------

Grow is a directory-tree parser.

USAGE
--------------------------------------------------------------------------------

    $ grow -h

FEATURES
--------------------------------------------------------------------------------

The main idea of Grow is to provide a tool that efficiently visualizes
directory-tree structures from the command line. Additionally, Grow can

    - display file permissions,

    - display absolute paths,

    - display file sizes,

    - display directory sizes,

    - display dotfiles,

    - search multiple root directories,

    - exclude files from search, and

    - create directory structures.

Grow is written in Python 3, is self-contained in a single file, and has no
external dependencies. A Python 2 version is also available.

EXAMPLE 1: CRAWL DIRECTORY TREES
--------------------------------------------------------------------------------

    Grow can print a directory-tree structures by invoking it from the current
    directory:

        $ grow

    Or, equivalently:

        $ grow *

EXAMPLE 2: CREATE DIRECTORY TREES
--------------------------------------------------------------------------------

    Grow can create a directory-tree structures from AST representations using
    the --create flag:

        $ cat << EOF | grow -c
        > a
        >     b
        >         c.txt
        >         d.txt
        >     e
        > f
        >     g
        > EOF

EXAMPLE 3: DERIVE DIRECTORY INFORMATION
--------------------------------------------------------------------------------

    Grow can gather information about the depth, content, and size of a
    directory using the --info-report flag:

        $ grow -i
