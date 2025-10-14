#!/bin/bash

# valgrind_tests.sh - Run valgrind on all tests
# Run this from the build directory

echo "=========================================="
echo "Running Valgrind on All Tests"
echo "=========================================="

# Change to build directory if not already there
if [[ $(basename $(pwd)) != "build" ]]; then
    cd ../build
fi

PASSED=0
FAILED=0

# Find all test executables (both list and queue)
for test in list_test* queue_test* hash_test*; do
    if [ ! -x "$test" ]; then
        continue
    fi
    
    echo ""
    echo "Testing: $test"
    echo "---"
    
    valgrind --leak-check=full --error-exitcode=1 ./$test > /dev/null 2> valgrind_output.txt
    
    if grep -q "no leaks are possible\|All heap blocks were freed" valgrind_output.txt; then
        LEAK_STATUS="✓ No leaks"
    else
        LEAK_STATUS="✗ Memory leaks detected"
        FAILED=$((FAILED + 1))
    fi
    
    if grep -q "ERROR SUMMARY: 0 errors" valgrind_output.txt; then
        ERROR_STATUS="✓ 0 errors"
    else
        ERROR_STATUS="✗ Errors detected"
        FAILED=$((FAILED + 1))
    fi
    
    echo "$LEAK_STATUS"
    echo "$ERROR_STATUS"
    
    if [[ "$LEAK_STATUS" == *"✓"* && "$ERROR_STATUS" == *"✓"* ]]; then
        PASSED=$((PASSED + 1))
    fi
done

rm -f valgrind_output.txt

echo ""
echo "=========================================="
echo "Summary: $PASSED tests passed, $FAILED issues detected"
echo "=========================================="

if [ $FAILED -gt 0 ]; then
    exit 1
else
    exit 0
fi