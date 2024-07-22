import subprocess
import os
import numpy as np
import scipy.stats as st

VERBOSE = False
ITER = 50
DEPTH = 6  # Must be > 2
DEPTHS = 3  # Last n iterations to use
engine_path = os.getenv('ENGINE_RELEASE')

if engine_path is not None:
    print(f"ENGINE_RELEASE: {engine_path}")
else:
    print("ENGINE_RELEASE environment variable is not set.")
    raise SystemExit


def get_average():
    # Start engine
    process = subprocess.Popen(
        [engine_path],
        stdin=subprocess.PIPE,
        stdout=subprocess.PIPE,
        stderr=subprocess.PIPE,
        text=True,
        bufsize=1
    )

    process.stdin.write('isready\n')
    process.stdin.flush()  # Ensure the command is sent immediately

    # Check engine available
    output = process.stdout.readline()
    if output != "readyok":
        RuntimeError("Unexpected output after isready: ", output)

    process.stdin.write('position startpos\n')
    process.stdin.write(f'go depth {DEPTH}\n')
    process.stdin.flush()
    process.stdout.readline()  # Discard initial "search info" line

    infos = []
    while True:
        output = process.stdout.readline()
        if VERBOSE:
            print(output)
        infos.append(output)
        if output.find(f'depth {DEPTH}') != -1:
            break

    process.stdin.write('stop\n')
    process.stdin.write('quit\n')
    process.stdin.flush()
    process.terminate()

    # Compute NPS using last two depths.
    nps = []
    for info in infos[-DEPTHS:]:
        nodes = int(info.split(' ')[7])
        seconds = float(info.split(' ')[9]) / 1000
        nps.append(nodes // seconds // 1000)

    for x, i in zip(nps, range(DEPTH - DEPTHS + 1, DEPTH + 1)):
        print(f"depth {i}: {x}k")

    avg = sum(nps) // len(nps)
    print(f"avg: {avg}k")
    print('---')

    return avg


# https://stackoverflow.com/questions/15033511/compute-a-confidence-interval-from-sample-data
def confidence_interval(data, confidence=0.95):
    return st.norm.interval(confidence,
                            loc=np.mean(data),
                            scale=np.std(data)/np.sqrt(len(data)))


if __name__ == '__main__':
    avgs = []
    for i in range(ITER):
        avgs.append(get_average())
    ci = confidence_interval(avgs)
    print(f"iter={ITER}, depth={DEPTH}, depth={DEPTHS}")
    print(f"kNPS 95% confidence interval: ({int(ci[0]), int(ci[1])})")
