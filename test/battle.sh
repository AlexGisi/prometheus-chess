#!/bin/bash
set -euo pipefail

# Ensure config variables are present
: "${ENGINE_DEBUG:?Need to source config.sh.}"
: "${ENGINE_RELEASE:?Need to source config.sh.}"
: "${BATTLE_ENGINE:?Need to source config.sh.}"
: "${BATTLE_ENGINE_NAME:?Need to source config.sh.}"
: "${FAST_CHESS:?Need to source config.sh.}"
: "${OPENINGS:?Need to source config.sh.}"
: "${RESULT_DIR:?Need to source config.sh.}"

# Choose engine build
case "${1:-}" in
  release) ENGINE="$ENGINE_RELEASE" ;;
  debug)   ENGINE="$ENGINE_DEBUG" ;;
  *) echo "Invalid mode. Please specify 'release' or 'debug'." ; exit 1 ;;
esac

# Sanity checks
[[ -x "$FAST_CHESS" ]] || { echo "FAST_CHESS not executable: $FAST_CHESS"; exit 1; }
[[ -f "$OPENINGS"   ]] || { echo "OPENINGS file missing: $OPENINGS"; exit 1; }
mkdir -p "$RESULT_DIR"

# Resolve openings path to absolute so we can cd into run dir safely
resolve_abs() {
  if command -v realpath >/dev/null 2>&1; then realpath "$1"; else readlink -f "$1"; fi
}
OPENINGS_ABS="$(resolve_abs "$OPENINGS")"

# Per-run directory
ts=$(date +"%Y-%m-%d_%H-%M-%S")
RUN_DIR="${RESULT_DIR}/run_${ts}"
mkdir -p "$RUN_DIR"

# Seed + file paths (all inside run dir)
seed=$(od -vAn -N8 -tu8 < /dev/urandom | tr -d ' ')
echo "Run directory: $RUN_DIR"
echo "fastchess seed: $seed" | tee "${RUN_DIR}/meta.txt"

# Run fastchess from the run directory so any side files land here
(
  cd "$RUN_DIR"

  stdbuf -oL -eL \
  "$FAST_CHESS" \
    -engine cmd="$ENGINE"          name=prometheus_new \
    -engine cmd="$BATTLE_ENGINE"   name="$BATTLE_ENGINE_NAME" \
    -openings file="$OPENINGS_ABS" format=pgn order=random \
    -srand "$seed" \
    -output cutechess \
    -pgnout "results.pgn" \
    -each tc=2+.1 \
    -rounds 1 \
    -games 1000 \
    -log file="fastchess.log" level=info compress=false realtime=true engine=true \
    -concurrency 15 \
    2> "fastchess.err" \
  | tee "engine_console.txt" \
  | grep -Ev '^(Info|Warning|Position|Moves);'

  # Tidy any small side files if fastchess drops them
  rm -f config.json log.txt 2>/dev/null || true
)

echo "Done. Artifacts saved in: $RUN_DIR"
echo "  PGN:            $RUN_DIR/results.pgn"
echo "  Meta/seed:      $RUN_DIR/meta.txt"
echo "  Fastchess log:  $RUN_DIR/fastchess.log   (includes engine I/O)"
echo "  Fastchess err:  $RUN_DIR/fastchess.err"
echo "  Engine console: $RUN_DIR/engine_console.txt"
