# Psy-Q PSYLINK Command File Maker

[![Build Status](https://github.com/devon-artmeier/make-psylink-file/actions/workflows/cmake-multi-platform.yml/badge.svg)](https://github.com/devon-artmeier/make-psylink-file/actions/workflows/cmake-multi-platform.yml)

Tool to create a Psy-Q PSYLINK command file from a list of object files and instructions file.

## Usage

    make_psylink_file -c [instructions] -o [output]  [object files]
    
        -c [instructions] - Linker instructions file
        -o [output]       - Output file
        [object files]    - List of object files to link
