#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "json_entry.h"

const char* json_type_string[] = {
    "ERROR",
    "STRING",
    "INTEGER",
    "ARRAY",
    "OBJECT",
    "NULL_DATA",
    "BOOLEAN"
};

int get_array_size(struct array_list* array) {
    int size = 0;
    struct array_list* tmp = array;

    for (; NULL != tmp; tmp = tmp->next);

    return size;
}

struct entry* create_entry(const char* key,
                           const json_type_e type,
                           const void* data)
{
    struct entry* entry = NULL;

    entry = (struct entry*) malloc(sizeof(struct entry));

    if (NULL == entry)
    {
        return NULL;
    }

    memset(entry, 0, sizeof (struct entry));

    if (NULL == key) {
        goto error;
    }

    entry->key = (char*) malloc(strlen(key) * sizeof(char));
    memcpy(entry->key, key, strlen(key) * sizeof(char));

    if (NULL == data && type != NULL_DATA)
    {
        goto error;
    }

    switch (type) {
        case STRING:
            entry->data = malloc(strlen((char*) data) * sizeof(char));
            memcpy(entry->data, data, strlen((char*) data) * sizeof(char));
            break;
        case INTEGER:
            entry->data = malloc(sizeof(int));
            memcpy(entry->data, data,  sizeof(int));
            break;
        case BOOLEAN:
            entry->data = malloc(sizeof(char));
            memcpy(entry->data, data,  sizeof(char));
            break;
        case OBJECT:
            entry->data = malloc(sizeof(struct entry));
            memcpy(entry->data, data,  sizeof(struct entry));
            break;
        case NULL_DATA:
            entry->data = NULL;
        case ARRAY:
            entry->data = malloc(sizeof(struct array_list));
            memcpy(entry->data, data, sizeof(struct array_list));
        default:
            goto error;
    }

    entry->type = type;

    return entry;

error:
    if (entry == NULL) {
        return NULL;
    }

    if (NULL != entry->key) {
        free(entry->key);
    }

    if (NULL != entry->data)
    {
        free(entry->data);
    }
    free(entry);

    return NULL;
}

void delete_entry(struct entry** entry) {
    struct array_list* tmp_ptr;
    struct array_list* tmp_del_ptr;

    struct entry* tmp = *(entry);
    struct entry* tmp2;

    if (NULL == entry)
    {
        return;
    }

    while (tmp != NULL)
    {
        FREE_PTR(tmp->key);
        switch (tmp->type)
        {
            case STRING:
            case INTEGER:
            case OBJECT:
            case BOOLEAN:
                FREE_PTR(tmp->data);
                break;
            case ARRAY:
                tmp_ptr = (struct array_list*) tmp->data;
                while (tmp_ptr != NULL)
                {
                    tmp_del_ptr = tmp_ptr;
                    tmp_ptr = tmp_ptr->next;
                    free(tmp_del_ptr);
                }
                break;
            case ERROR:
            case NULL_DATA:
                break;
        }
        tmp2 = tmp;
        tmp = tmp->next;

        free(tmp2);
    }

    *entry = NULL;
}

void* get_json_item(struct entry* entry, const char* key)
{
    while (entry != NULL)
    {
        if (0 == strcmp(entry->key, key))
            return entry->data;
        entry = entry->next;
    }

    return NULL;
}
