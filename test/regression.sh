#!/bin/bash
set -euo pipefail

# Ensure config variables are present
: "${ENGINE_DEBUG:?Need to source config.sh.}"
: "${ENGINE_RELEASE:?Need to source config.sh.}"
: "${REGRESSION_ENGINE:?Need to source config.sh.}"
: "${FAST_CHESS:?Need to source config.sh.}"
: "${OPENINGS:?Need to source config.sh.}"
: "${RESULT_DIR:?Need to source config.sh.}"

# Choose engine build
case "${1:-}" in
  release) ENGINE="$ENGINE_RELEASE" ;;
  debug)   ENGINE="$ENGINE_DEBUG" ;;
  *) echo "Invalid mode. Please specify 'release' or 'debug'." ; exit 1 ;;
esac

# Basic sanity checks
[[ -x "$FAST_CHESS" ]] || { echo "FAST_CHESS not executable: $FAST_CHESS"; exit 1; }
[[ -f "$OPENINGS"   ]] || { echo "OPENINGS file missing: $OPENINGS"; exit 1; }
mkdir -p "$RESULT_DIR"

# Per-run directory
ts=$(date +"%Y-%m-%d_%H-%M-%S")
RUN_DIR="${RESULT_DIR}/run_${ts}"
mkdir -p "$RUN_DIR"

# Seed + file paths inside run dir
seed=$(od -vAn -N8 -tu8 < /dev/urandom | tr -d ' ')
meta="${RUN_DIR}/meta.txt"
pgn="${RUN_DIR}/results.pgn"
fc_log="${RUN_DIR}/fastchess.log"        # structured fastchess log (includes engine I/O)
fc_err="${RUN_DIR}/fastchess.err"        # fastchess stderr
eng_console="${RUN_DIR}/engine_console.txt"  # raw console copy (pre-filter)

echo "Run directory: $RUN_DIR"
echo "fastchess seed: $seed" | tee "$meta"

# Run fastchess
stdbuf -oL -eL \
"$FAST_CHESS" \
  -engine cmd="$ENGINE"              name=prometheus_new \
  -engine cmd="$REGRESSION_ENGINE"   name=prometheus_old \
  -openings file="$OPENINGS" format=pgn order=random \
  -srand "$seed" \
  -output cutechess \
  -pgnout "$pgn" \
  -each tc=2+.1 \
  -rounds 1 \
  -games 1000 \
  -log file="$fc_log" level=info compress=false realtime=true engine=true \
  -concurrency 14 \
  2> "$fc_err" \
| tee "$eng_console" \
| grep -Ev '^(Info|Warning|Position|Moves);'

# fastchess sometimes leaves these; if present, remove them from *this* run dir
rm -f "$RUN_DIR/config.json" "$RUN_DIR/log.txt" 2>/dev/null || true

echo "Done. Artifacts:"
echo "  PGN:            $pgn"
echo "  Meta/seed:      $meta"
echo "  Fastchess log:  $fc_log"
echo "  Fastchess err:  $fc_err"
echo "  Engine console: $eng_console"
