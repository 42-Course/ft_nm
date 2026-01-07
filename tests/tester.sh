#!/bin/bash

# Configuration
ORIG_DIR="out_orig"
MINE_DIR="out_mine"
FILES=("test" "test32" "/usr/lib32/crt1.o" "ft_nm" "/usr/local/lib/libSDL2-2.0.so.0.3100.0")
OPTIONS=$1

# Create directories
mkdir -p $ORIG_DIR $MINE_DIR

# Formatting colors
GREEN='\033[0;32m'
RED='\033[0;31m'
BLUE='\033[0;34m'
BOLD='\033[1m'
NC='\033[0m' # No Color

echo -e "${BOLD}Starting NM Comparison Test${NC}"
echo -e "Options: ${BLUE}${OPTIONS:-"None"}${NC}\n"

# Header for the table
printf "%-25s | %-10s\n" "File Name" "Status"
echo "-------------------------------------------"

# Run tests and store results
results=()
diffs=()

for file in "${FILES[@]}"; do
    if [ ! -f "$file" ]; then
        printf "%-25s | %-10s\n" "$(basename "$file")" "NOT FOUND"
        continue
    fi

    base=$(basename "$file")
    orig_out="$ORIG_DIR/${base}.txt"
    mine_out="$MINE_DIR/${base}.txt"

    # Execute commands
    nm $OPTIONS "$file" > "$orig_out" 2>&1
    ./ft_nm $OPTIONS "$file" > "$mine_out" 2>&1

    # Compare
    if diff "$orig_out" "$mine_out" > /dev/null; then
        printf "%-25s | ${GREEN}OK${NC}\n" "$base"
    else
        printf "%-25s | ${RED}KO${NC}\n" "$base"
        # Capture the diff for verbose output later
        diff_val=$(diff -u "$orig_out" "$mine_out")
        diffs+=("FILE: $base\n$diff_val")
    fi
done

# Verbose Output: Show Diffs
if [ ${#diffs[@]} -ne 0 ]; then
    echo -e "\n\n${BOLD}--- DETAILED DIFFERENCES ---${NC}"
    for d in "${diffs[@]}"; do
        echo -e "\n${BLUE}================================================================${NC}"
        echo -e "$d"
        echo -e "${BLUE}================================================================${NC}"
    done
else
    echo -e "\n${GREEN}All tests passed perfectly!${NC}"
fi