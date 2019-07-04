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

        ./
            a
                b
                    c.txt
                    d.txt
                e
            g
                h
    
    To display the structure like above, do

        $ grow ./

EXAMPLE 2: BUILD DIRECTORIES

    To generate the directory structure from example 1, do

        $ cat << EOF | grow -c ./
          >    a
          >        b
          >            c.txt
          >            d.txt
          >        e
          >    g
          >        h
          > EOF
