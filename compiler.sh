#!/bin/bash

# Usage: ./compiler.sh src/shell.c
mkdir -p bin

file="$1"

if [ -z "$file" ]; then
    echo "Usage: $0 <source_file.c>"
    exit 1
fi

filename=$(basename -- "$file")
name="${filename%.c}"


echo "📦 Compiling $file → bin/$name..."

# Compile and capture errors
gcc -Wall -g "$file" -o "bin/$name"

# Check if compilation succeeded
if [ $? -eq 0 ]; then
    echo "✅ Compilation successful. Run with: ./bin/$name"
else
    echo "❌ Compilation failed. Errors:"
    echo "-------------------------------"
    # Run compilation *again*, forcing the errors to be shown here with highlighting
    gcc -Wall -g "$file" -o "bin/$name"
    echo "-------------------------------"
fi

