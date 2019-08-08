GROW
--------------------------------------------------------------------------------

GROW is a directory crawler and builder.

USAGE
--------------------------------------------------------------------------------

    $ grow -h

EXAMPLES
--------------------------------------------------------------------------------

EXAMPLE 1: CRAWL DIRECTORIES

    Given the directory structure:

        a
            b
                c.txt
                d.txt
            e
        g
            h

    To recursively traverse the root directory and display all the files along
    the way, do:

        $ grow

EXAMPLE 2: BUILD DIRECTORIES

    To generate the directory structure from example 1, do

        $ cat << EOF | grow -c
        > a
        >     b
        >         c.txt
        >         d.txt
        >     e
        > g
        >     h
        > EOF

    Note that grow uses the indent of the first child node, in this case 'b', to
    verify the indentation of subsequent child nodes. If the indentation is
    inconsistent, grow will abort and tell you to fix your indentation levels.
    The indentation width can be any integer greater than zero. Both spaces and
    tabs are allowed.
