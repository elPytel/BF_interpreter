#!/bin/bash
# By: Pytel
: '
Description: 
Test script for bf
'

#DEBUG=true
DEBUG=false

VERBOSE=false

# redirect to log_file
log_file="test.log"
# remove log file
rm $log_file


# functions
function printHelp() {
    echo -e "Description: Test script for bf"
    echo -e "OPTIONS:"
	echo -e "  -h, --help \t\t print this text"
    echo -e "  -d, --debug \t\t debug mode"
    echo -e "  -v, --verbose \t verbose mode"
}

function check_valgrind_return_code() { # (return_code)
    local return_code=$1
    $DEBUG && echo -e "Return code: $return_code" >> $log_file
    # array of valid codes
    valid_codes=(0 2)
    # check if return code is valid
    for code in ${valid_codes[@]}; do
        if [ $return_code -eq $code ]; then
            return 0
        fi
    done
    faigled=$((faigled+1))
    return 1
}

# parse input
$DEBUG && echo "Args: [$@]"
arg=$1
while [ $# -gt 0 ] ; do
	$DEBUG && echo "Arg: $arg remain: $#"

	# vyhodnoceni
	case $arg in
		-h | --help) 	printHelp; exit 2;;
		-d | --debug) 	DEBUG=true;;
		-v | --verbose) VERBOSE=true;;
		*) echo -e "Unknown parametr: $arg"; exit 1;;
	esac

	# next arg
	shift
	arg=$1
done

faigled=0

# make sure that bf is compiled
echo "Compiling bf..." >> $log_file
make >> $log_file 2>&1
echo -e "Done \n" >> $log_file

# Check for memory leaks

## no arguments
test_name="No arguments"
valgrind ./bf >> $log_file 2>&1
return_code=$?
validity=""
check_valgrind_return_code $return_code && validity="VALID" || validity="FAILED"
echo -e "$test_name: $validity \n" >> $log_file

## non-existing file
test_name="Non-existing file"
valgrind ./bf ./tests/no_file.txt >> $log_file 2>&1
return_code=$?
validity=""
check_valgrind_return_code $return_code && validity="VALID" || validity="FAILED"
echo -e "$test_name: $validity \n" >> $log_file

## empty file
test_name="Empty file"
valgrind ./bf ./tests/empty.txt >> $log_file 2>&1
return_code=$?
validity=""
check_valgrind_return_code $return_code && validity="VALID" || validity="FAILED"
echo -e "$test_name: $validity \n" >> $log_file

: '
## hello_world.txt
valgrind ./bf ./tests/hello_world.txt
'

# check if all tests passed
if [ $faigled -eq 0 ]; then
    echo -e "All tests passed. \n" >> $log_file
else
    echo -e "Some tests failed. \n" >> $log_file
fi

# clean up
make clean >> $log_file 2>&1

# show log file
if $VERBOSE; then
    echo "Log file:"
    cat $log_file
fi

# fail if some tests failed
exit $faigled

# END
