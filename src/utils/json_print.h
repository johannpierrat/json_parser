#ifndef JSON_PRINT_H
# define JSON_PRINT_H 1

# include <stdlib.h>
# include "json_entry.h"

/**
 * Transform string to hexa value
*/
int itoh(const char* string);

/**
 * transform json object to string
 * \param entry
*/
void json_to_string(char** string,
                    size_t* size,
                    int* index,
                    struct entry* entry);

#endif
