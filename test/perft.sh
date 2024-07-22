#!/bin/bash

ENGINE=$ENGINE_RELEASE

# Make sure the config file was sourced.
if [[ -z "$ENGINE_DEBUG" ]]; then
  echo "Need to source config.sh."
  exit 1
fi

if [[ -z $1 ]]; then
  DEPTH=4
else
  DEPTH=$(( $1 ))
fi

echo "Perft to depth $DEPTH..."

res=$($ENGINE perft "$DEPTH" "$RESULTS")

# Process the output
while IFS= read -r line; do
  if [[ "$line" == *"FAIL"* ]]; then
    echo "$line"
  fi
  if [[ "$line" =~ ^Passed\ [0-9]+\/[0-9]+$ ]]; then
    summary="$line"
  fi
done <<< "$res"

# Print the summary at the end
echo "$summary"
