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
    STRING,
    DATE,
    INTEGER,
    ARRAY,
    OBJECT,
    NULL_DATA,
    BOOLEAN
} json_type_e;

struct entry {
    char* key;
    json_type_e type;
    void* data;
};

struct array_list {
    void* data;
    struct array_list* next;
};

inline int get_array_size(struct array_list* array);

struct entry* create_entry(const char* key,
                           const json_type_e type,
                           const void* data);

void delete_entry(struct entry* entry);

#endif
