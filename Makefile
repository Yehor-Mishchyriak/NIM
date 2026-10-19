CC = cc
CFLAGS = -std=c17 -Wall -Wextra -Wpedantic -O2
SOURCES = src/main.c src/game/gameLoopControls.c src/player/player.c src/agent/agent.c src/agent/rl.c
HEADERS = src/game/gameStucts.h src/game/gameLoopControls.h src/player/player.h src/agent/agent.h src/agent/rl.h src/util/ctuple.h

nim: $(SOURCES) $(HEADERS)
	$(CC) $(CFLAGS) $(SOURCES) -o $@

.PHONY: test
test:
	mkdir -p build
	$(CC) $(CFLAGS) -DMAX_PILE=3 -fsanitize=address,undefined tests/test_rl.c src/game/gameLoopControls.c src/agent/agent.c src/agent/rl.c -o build/test_rl
	./build/test_rl
