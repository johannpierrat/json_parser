#ifndef JSON_ENTRY_H
#define JSON_ENTRY_H value

# define FREE_PTR(PTR)          \
    {                           \
        if (NULL != PTR)        \
        {                       \
            free(PTR);          \
        }                       \
        PTR = NULL;             \
    }

typedef enum {
    ERROR,
    STRING,
    INTEGER,
    ARRAY,
    OBJECT,
    NULL_DATA,
    BOOLEAN
} json_type_e;

const char* json_type_string[] = {
    "ERROR",
    "STRING",
    "INTEGER",
    "ARRAY",
    "OBJECT",
    "NULL_DATA",
    "BOOLEAN"
};

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

inline int get_array_size(struct array_list* array);

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

#endif
