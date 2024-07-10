#!/bin/bash

# Make sure the config file was sourced.
if [[ -z "$ENGINE_DEBUG" ]]; then
  echo "Need to source config.sh."
  exit 1
fi

if [[ $1 == "release" ]]; then
  ENGINE=$ENGINE_RELEASE
elif [[ $1 == "debug" ]]; then
  ENGINE=$ENGINE_DEBUG
else
  echo "Invalid mode. Please specify 'release' or 'debug'."
  exit 1
fi

if [[ -z $2 ]]; then
  DEPTH=4
else
  DEPTH=$(( $2 ))
fi

# Assign the command to a variable
res=$($ENGINE perft "$DEPTH" "$RESULTS")
last=$(echo "$res" | tail -n 1)

# Compare the last line to the expected string
if [[ "$last" != "Passed 126/126" ]]; then
  echo "$res"
fi
