#ifndef JSON_ENTRY_H
# define JSON_ENTRY_H 1

# include <stdio.h>

# define FREE_PTR(PTR)          \
    {                           \
        if (NULL != PTR)        \
        {                       \
            free(PTR);          \
        }                       \
        PTR = NULL;             \
    }

typedef enum {
    ERROR = -1,
    STRING = 0,
    INTEGER,
    HEXA,
    ARRAY,
    OBJECT,
    NULL_DATA,
    BOOLEAN
} json_type_e;

struct entry {
    char* key;
    json_type_e type;
    void* data;
    struct entry* next;
};

struct array_list {
    void* data;
    json_type_e type;
    struct array_list* next;
};

int get_array_size(struct array_list* array);

/**
 * Create a json entry
 * \param key key of the entry
 * \param type type of the entry
 * \param data value of the entry
*/
struct entry* create_entry(const char* key,
                           const json_type_e type,
                           const void* data);

/**
 * free the entry
*/
void delete_entry(struct entry** entry);

/**
 * return a value corresponding to the key
 * \param json json entry
 * \param key key to look for
 * \return value or NULL if not found
*/
void* get_json_item(struct entry* json, const char* key);

#endif
