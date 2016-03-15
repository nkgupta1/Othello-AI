Michael: Implemented Minimax and alpha-beta and iterative deepening. Attempted MTD implementation. Time allocation and turn structure of doMove. Score functions. 

Nikhil: Implemented heuristics and updating heuristics. Change board operations to a bitwise comparisons. Transposition table to map game states to scores. File operations for an opening book. Optimized checkMove.

- Minimax
    - Alpha-beta
    - Attempted MTD
    - cuts off many branches reducing search time
- Time allocation
    - Time roll over
    - allows for extra depth in late mid-game
- Iterative deepening
    - goes down until allocated time runs out
    - allows us to check the most possible moves in the alloted time without going over
- Fail safe
    - return valid move if algorithm failed
    - should never be called but just in case
- Updating Heuristics
    - allows dynamic scoring based on board state because a single heuristic gets outdated
- Compiler optimization
    - using the flags
- Bitwise board operations
- Transposition table
    - Not used because MTD not working
    - Dynamic programming
    - saves previous game states so don't need to recalculate the scores for them
- File operations to read and write boards and scores from files
    - for an opening book but didn't end up doing an opening book
    - hash function for boards
- Optimized checkMove because called so often