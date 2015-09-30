include Makefile.rules

.PHONY: all main clean

all: main

main:
	make -C $(SOURCE)


clean:
	make -C $(SOURCE)
