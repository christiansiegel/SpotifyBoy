SRC = SpotifyBoy.c
ROM = SpotifyBoy.gb

$(ROM): $(SRC)
	docker run --rm -i christiansiegel/gbdk-n-builder < $(SRC) > $(ROM)

.PHONY: format
format:
	clang-format --style=google -i $(SRC)

.PHONY: clean
clean:
	rm -f $(ROM)

.PHONY: run
run:
	vba $(ROM)
