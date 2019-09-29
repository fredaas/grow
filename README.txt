GROW
--------------------------------------------------------------------------------

Grow is a directory-tree crawler.

USAGE
--------------------------------------------------------------------------------

    $ grow -h

FEATURES
--------------------------------------------------------------------------------

The main idea of grow is to provide a tool that efficiently visualizes
directory-tree structures from the command line. Additionally, grow can

    - display file permissions,

    - display absolute paths,

    - display file sizes,

    - display dotfiles,

    - search multiple root directories,

    - exclude files from search, and

    - create directory structures.

Grow is written in Python 3, is self-contained in a single file, and has no
external dependencies.

EXAMPLE 1: CRAWL DIRECTORY TREES
--------------------------------------------------------------------------------

   Consider the following directory structure:

        a
            b
                c.txt
                d.txt
            e
        f
            g

    The directory structure can be printed to stdout by invoking grow from the
    root directory:

        $ grow

    Or, equivalently:

        $ grow *

EXAMPLE 2: CREATE DIRECTORY TREES
--------------------------------------------------------------------------------

    To create the directory structure from example 1 you can pipe a textual
    representation of the directory structure to grow using the --create (-c) flag:

        $ cat << EOF | grow -c
        > a
        >     b
        >         c.txt
        >         d.txt
        >     e
        > f
        >     g
        > EOF

    Note: Grow uses the indent of the first child node, in this case 'b', to
    verify the indentation of subsequent child nodes. If the indentation is
    inconsistent, grow will abort and tell you to fix your indentation levels.
    The indentation width can be any integer greater than zero. Both spaces and
    tabs are allowed.
