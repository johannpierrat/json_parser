PROJECT=json_parser
NAME=json_parser
CC=gcc
CFLAGS=-Werror -Wall -Wextra -std=c11 -pedantic -g3
SOURCE=src/
FILES=utils/json_entry.c utils/json_list.c utils/json_parser.c utils/json_print.c parser/parser.c
OBJ= $(FILES:.c=.o)

.PHONY: all $(NAME)

all: $(NAME)

clean:
	rm -f $(OBJ)

$(NAME): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@
