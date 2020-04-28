#!/usr/bin/env python3

import concurrent.futures
import os
import requests
import sys
import time

# Functions

def usage(status=0):
    progname = os.path.basename(sys.argv[0])
    print(f'''Usage: {progname} [-h HAMMERS -t THROWS] URL
    -h  HAMMERS     Number of hammers to utilize (1)
    -t  THROWS      Number of throws per hammer  (1)
    -v              Display verbose output
    ''')
    sys.exit(status)

def hammer(url, throws, verbose, hid):
    ''' Hammer specified url by making multiple throws (ie. HTTP requests).

    - url:      URL to request
    - throws:   How many times to make the request
    - verbose:  Whether or not to display the text of the response
    - hid:      Unique hammer identifier

    Return the average elapsed time of all the throws.
    '''

    sum = 0.0000000

    for i in range(throws):
        start_time = time.time()
        response = requests.get(url)
        end_time = time.time()
        duration = end_time - start_time
        sum += duration

        # If we need to print out the response txt
        if verbose:
            print(response.text)

        # Print out the elapsed time of a single throw
        print('Hammer:  {}, Throw:   {}, Elapsed Time: {:.6f}'.format(hid, i, duration))

    # Print out the averaged time of a hammer
    print('Hammer:  {}, AVERAGE   , Elapsed Time: {:.6f}'.format(hid, sum/throws))

    return sum/throws

def do_hammer(args):
    ''' Use args tuple to call `hammer` '''
    return hammer(*args)

def main():
    hammers = 1
    throws  = 1
    verbose = False

    arguments = sys.argv[1:]

    # Parse command line arguments
    if not len(arguments):
        usage(1)

    while len(arguments) and arguments[0].startswith('-'):
        argument = arguments.pop(0)
        if argument == '-h':
            hammers = int(arguments.pop(0))
        elif argument == '-t':
            throws = int(arguments.pop(0))
        elif argument == '-v':
            verbose = True
        else:
            usage(1)

    if len(arguments):
        url = arguments.pop(0)
    else:   # If url is not provided
        usage(1)

    # Use generator to make the arguements
    args = ((url, throws, verbose, hid) for hid in range(hammers))

    # Create pool of workers and perform throws
    with concurrent.futures.ProcessPoolExecutor(hammers) as executor:
        runtime = executor.map(do_hammer, args)

    # Print out the total average time
    print('TOTAL AVERAGE ELAPSED TIME: {:.6f}'.format(sum(runtime)/hammers))


# Main execution

if __name__ == '__main__':
    main()

# vim: set sts=4 sw=4 ts=8 expandtab ft=python:
