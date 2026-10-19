# Nim

A terminal Nim game in C with a tabular Q-learning agent trained against a random opponent. Choose from three piles; taking the last stick loses. Each pile supports up to 25 sticks.

Requires a C17 compiler and Make.

```sh
make
./nim
```

Run checks with `make test` (requires AddressSanitizer and UndefinedBehaviorSanitizer support).

Implementation lives in `src/`; tests live in `tests/`. Training runs before each game; the Q-table is kept in memory only.
