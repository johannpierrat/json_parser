#ifndef JSON_PARSER_H
# define JSON_PARSER_H 1

# include <stdio.h>
# include "json_entry.h"

# define ADVANCE_NEXT_WORD(STR, INDEX)      \
    for(;                                   \
        ' ' == STR[INDEX] ||                \
        '\n' == STR[INDEX] ||               \
        '\t' == STR[INDEX]; ++INDEX);       \

# define PRINTERR(STR, ...)           \
    fprintf(stderr, STR, __VA_ARGS__)


/**
 * Parse start of the json parsing
*/
struct entry* json_parse(const char* ptr);

#endif
