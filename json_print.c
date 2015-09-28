#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "json_print.h"

#define INSERT_DATA(STR, SZ, IDX, DATA, DATA_LEN)           \
    {                                                       \
        if ((IDX + DATA_LEN) > SZ)                          \
        {                                                   \
            SZ >>= 1;                                       \
            STR = (char*)realloc(STR, SZ);                  \
        }                                                   \
        memcpy(STR + IDX, DATA, DATA_LEN);                  \
        IDX += DATA_LEN;                                    \
    }

#define INSERT_STRING(STR, SZ, IDX, DATA)                   \
    INSERT_DATA(STR, SZ, IDX, DATA, strlen(DATA))

static int itos(char* string, int data)
{
    if ((data / 10) == 0) {
        string[0] = data + '0';
        return 0;
    }

    int index = itos(string, data / 10);
    string[index] = (data % 10) + '0';

    return ++index;
}

static void array_to_string(char** string,
                            size_t* size,
                            int* index,
                            struct array_list* list)
{
    struct array_list* tmp;
    char* tmp_str = NULL;

    INSERT_STRING(*string, *size, *index, "[");

    for (tmp = list; NULL != tmp; tmp = tmp->next)
    {
        switch (tmp->type){
            case STRING:
                INSERT_STRING(*string, *size, *index, (char*)tmp->data);
                break;
            case BOOLEAN:
                INSERT_STRING(*string, *size, *index,
                              (*(int*)(tmp->data)) ?
                              "\"true\"" : "\"false\"");
                break;
            case INTEGER:
                tmp_str = (char*) malloc(10 * sizeof(char));
                memset(tmp_str, 0, 10 * sizeof(char));
                itos(tmp_str, *(int*)(tmp->data));
                free(tmp_str);
                break;
            case OBJECT:
                json_to_string(string, size, index,
                               *(struct entry*)(tmp->data));
                break;
            case NULL_DATA:
                INSERT_STRING(*string, *size, *index, "null");
                break;
            case ARRAY:
                array_to_string(string, size, index,
                                (struct array_list*) tmp->data);
                break;
            case ERROR:
            default:
                return;
        }
        if (NULL != tmp->next)
        {
            INSERT_STRING(*string, *size, *index, ",");
        }
    }
    INSERT_STRING(*string, *size, *index, "]");
}

void json_to_string(char** string,
                    size_t* size,
                    int* index,
                    struct entry* entry)
{
    char* tmp_str = NULL;

    INSERT_STRING(*string, *size, *index, "{\"");
    while (NULL != entry)
    {
        INSERT_STRING(*string, *size, *index, entry->key);
        INSERT_STRING(*string, *size, *index, "\":");

        switch (entry->type){
            case STRING:
                INSERT_STRING(*string, *size, *index, (char*)entry->data);
                break;
            case BOOLEAN:
                INSERT_STRING(*string, *size, *index,
                              (*(int*)(entry->data)) ?
                              "\"true\"" : "\"false\"");
                break;
            case INTEGER:
                tmp_str = (char*) malloc(10 * sizeof(char));
                memset(tmp_str, 0, 10 * sizeof(char));
                itos(tmp_str, *(int*)(entry->data));
                free(tmp_str);
                break;
            case OBJECT:
                json_to_string(string, size, index,
                               *(struct entry*)(entry->data));
                break;
            case NULL_DATA:
                INSERT_STRING(*string, *size, *index, "null");
                break;
            case ARRAY:
                array_to_string(string, size, index,
                                (struct array_list*) entry->data);
                break;
            case ERROR:
            default:
                return;
        }
        if (NULL != entry->next)
        {
            INSERT_STRING(*string, *size, *index, ",");
        }
        entry = entry->next;
    }

    INSERT_STRING(*string, *size, *index, "}");
    *string[*index] = '\0';
}
