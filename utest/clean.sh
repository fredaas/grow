#!/usr/bin/env bash

ls | egrep -v "tree1.txt|tree2.txt|clean.sh|Makefile" | xargs -I{} rm -rf {}
