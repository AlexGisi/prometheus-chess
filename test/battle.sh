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

if [[ ! -x "$FAST_CHESS" ]]; then
  echo "The FAST_CHESS path is not valid or not executable: $FAST_CHESS"
  exit 1
fi

current_datetime=$(date +"%Y-%m-%d_%H-%M-%S")

"$FAST_CHESS" \
  -engine cmd="$ENGINE" name=prometheus_new \
  -engine cmd="$BATTLE_ENGINE" name="$BATTLE_ENGINE_NAME" \
  -openings file="$OPENINGS" format=pgn -randomseed \
  -output format=cutechess \
  -pgnout file="${RESULT_DIR}/test_${current_datetime}.pgn" \
  -each tc=2+.1 \
  -rounds 1 \
  -games 1000 \
  -log file=log.txt level=fatal compress=false \
  -concurrency 7

rm config.json
rm log.txt
