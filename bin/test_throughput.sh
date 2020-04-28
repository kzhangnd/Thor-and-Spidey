#!/bin/bash

PROGRAM=bin/thor.py
WORKSPACE=/tmp/$(basename $PROGRAM).$(id -u)

error() {
    echo "$@"
    [ -r $WORKSPACE/test ] && (echo; cat $WORKSPACE/test; echo)
    FAILURES=$((FAILURES + 1))
}

cleanup() {
    STATUS=${1:-$FAILURES}
    rm -fr $WORKSPACE
    exit $STATUS
}

check_status() {
    if [ $1 -ne $2 ]; then
	echo "FAILURE: exit status $1 != $2" > $WORKSPACE/test
	return 1;
    fi

    return 0;
}

grep_latency() {
    average=$(grep -E 'TOTAL AVERAGE ELAPSED TIME' $1)
	  echo $average
}


# Setup

mkdir $WORKSPACE

trap "cleanup" EXIT
trap "cleanup 1" INT TERM


# Testing

# ------------------------------------------------------------------------------

echo
cowsay -W 72 <<EOF
On another machine, please run:

    ./bin/spidey -p PORT -c MODE

- Where PORT is a number between 9000 - 9999

- Where MODE is either single or forking
EOF
echo

HOST="$1"
while [ -z "$HOST" ]; do
    read -p "Server Host: " HOST
done

PORT="$2"
while [ -z "$PORT" ]; do
    read -p "Server Port: " PORT
done

echo
echo "Testing throughput on $HOST:$PORT ..."


# ------------------------------------------------------------------------------

printf "\n%-64s\n" "Testing small static files: 1KB .."

./$PROGRAM -t 10 -h 10 $HOST:$PORT/file_small > $WORKSPACE/test
if ! check_status $? 0; then
    error "Failure"
else
    grep_latency $WORKSPACE/test
fi

# ------------------------------------------------------------------------------

printf "\n%-64s\n" "Testing medium static files: 1MB ..."

./$PROGRAM -t 10 -h 10 $HOST:$PORT/file_medium > $WORKSPACE/test
if ! check_status $? 0; then
    error "Failure"
else
    grep_latency $WORKSPACE/test
fi

# ------------------------------------------------------------------------------

printf "\n%-64s\n" "Testing large static files: 1GB ..."

./$PROGRAM -t 5 -h 5 $HOST:$PORT/file_large > $WORKSPACE/test
if ! check_status $? 0; then
    error "Failure"
else
    grep_latency $WORKSPACE/test
fi
