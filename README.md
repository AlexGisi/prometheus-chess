# prometheus
> Prometheus is an ancient and fully personalized mythological 
> character, prominent already in Hesiod, yet his name is one of 
> transparent meaning. It is a regularly-formed Greek word, and
> means Forethinker (or perhaps Forethought in the abstract). (Guthrie, 1986)

Promestheus is a conventional chess engine. 

# Build
You can build prometheus on your machine using cmake. From the repo's root directory:
```
cmake -DCMAKE_BUILD_TYPE=Release -S . -B build-dir/
cmake --build build-dir/
```
Then the executable is located at `build-dir/prometheus`.

# Play
prometheus has a bot account on lichess: https://lichess.org/@/prometheus-bot

# Complete
- Board representation
- Board setup/FEN parsing
- Check square attacked
- Move representation
- Move generation
- Do/undo moves
- Perft testing
- Hash/principal variation table
- Move Search
- UCI communication

# To-Do
- Improve quiescence
- Improve position evaluation
- Optimization

# Acknowledgment
The board representation and move generation adapts [Bluefever Software's](https://www.youtube.com/channel/UCFkfibjxPzrP0e2WIa8aJCg) "Programming a Chess Engine in C".

# Work cited
Guthrie, William Keith Chambers. In the Beginning: Some Greek Views on the Origins of Life and the Early State of Man. United States, Bloomsbury Academic, 1986.
