#include <stdlib.h>
#include <string.h>
#include "json_parser.h"
#include "json_print.h"

# define PARSE_RETURN_ERROR(TYPE)               \
    {                                           \
        (TYPE) = ERROR;                         \
        return;                                 \
    }

extern char* json_type_string[];

static void parse_data(const char* ptr,
                       unsigned int* index,
                       void** data,
                       json_type_e* type)
{
    char* tmp = NULL;
    char* key = NULL;
    void* data_tmp = NULL;
    json_type_e type_tmp;
    unsigned int end_index;
    struct entry* curr_entry;
    struct entry* prev_entry;

    if ('{' == ptr[*index]) {
        ++(*index);
        *type = OBJECT;

        while ('}' != ptr[*index])
        {
            ADVANCE_NEXT_WORD(ptr, *index);

            if ('\"' != ptr[*index])
                PARSE_RETURN_ERROR(*type);

            ++(*index);
            end_index = strchr(ptr + *index, '\"') - ptr;

            key = (char*) realloc(key,
                                  sizeof(char) * (end_index - *index + 1));

            if (NULL == key)
            {
                PARSE_RETURN_ERROR(*type);
            }
            memcpy(key, ptr + *index, end_index - *index + 1);
            key[end_index - *index] = '\0';
            *index = end_index + 1;

            if (':' != ptr[*index])
            {
                FREE_PTR(key);
                PARSE_RETURN_ERROR(*type)
            }

            ++(*index);
            ADVANCE_NEXT_WORD(ptr, *index);

            parse_data(ptr, index, &data_tmp, &type_tmp);

            if (ERROR != type_tmp)
            {
                curr_entry = create_entry(key, type_tmp, data_tmp);

                if (NULL == *data)
                    *data = curr_entry;
                if (NULL != prev_entry)
                    prev_entry->next = curr_entry;
            }

            FREE_PTR(key);
            FREE_PTR(data_tmp);

            ADVANCE_NEXT_WORD(ptr, *index);

            if (',' != ptr[*index] && '}' != ptr[*index])
                PARSE_RETURN_ERROR(*type);

            if (',' == ptr[*index])
            {
                prev_entry = curr_entry;
                (*index)++;
            }
        }

        return;
    }
    else if ('[' == ptr[*index])
    {
        *type = ARRAY;
        *data = realloc(*data, sizeof(struct array_list));
        struct array_list *tmp = (struct array_list*) *data;

        if (NULL == *data)
            PARSE_RETURN_ERROR(*type);

        memset(*data, 0, sizeof(struct array_list));

        while (ptr[*index] != ']')
        {
            ++(*index);
            parse_data(ptr, index, &(tmp->data), &(tmp->type));
            ADVANCE_NEXT_WORD(ptr, *index);
            if (',' == ptr[*index])
            {
                tmp->next = (struct array_list*)
                    malloc(sizeof(struct array_list));
                tmp = tmp->next;
                if (NULL == tmp)
                    PARSE_RETURN_ERROR(*type);
                memset(tmp, 0, sizeof(struct array_list));
            }

            if (',' != ptr[*index] && ']' != ptr[*index])
                PARSE_RETURN_ERROR(*type);
        }
        ++(*index);

        return;
    }
    else if ('"' == ptr[*index])
    {
        *type = STRING;
        ++(*index);
        end_index = strchr(ptr + *index, '\"') - ptr;
        *data = realloc(*data, (end_index - *index + 1) * sizeof (char));
        if (NULL == *data)
            PARSE_RETURN_ERROR(*type);

        strncpy((char*) *data, ptr + *index, end_index - *index + 1);
        ((char*)(*data))[end_index] = '\0';

        *index = end_index + 1;
        ADVANCE_NEXT_WORD(ptr, *index);

        return;
    }
    else if (strncmp("null", ptr + *index, 4) == 0)
    {
        *data = NULL;
        *type = NULL_DATA;
        index += 5;
        ADVANCE_NEXT_WORD(ptr, *index);
        return;
    }
    else if (strncmp("true", ptr + *index, 4) == 0)
    {
        *data = realloc(*data, sizeof (int));
        if (NULL == *data)
            PARSE_RETURN_ERROR(*type);

        *(int*)(*data) = 1;
        *type = BOOLEAN;
        index += 5;
        ADVANCE_NEXT_WORD(ptr, *index);
        return;
    }
    else if (strncmp("false", ptr + *index, 5) == 0)
    {
        *data = realloc(*data, sizeof (int));
        *(int*)(*data) = 0;
        *type = BOOLEAN;
        index += 6;
        ADVANCE_NEXT_WORD(ptr, *index);
        return;
    }
    else if (strncmp("0x", ptr + *index, 2) == 0)
    {
        *index += 2;
        *type = HEXA;
        *data = realloc(*data, sizeof(int));
        for (end_index = *index;
             IS_HEXA(ptr[end_index]);
             ++end_index);

        tmp = (char*) realloc(tmp, ((end_index - (*index) + 1) * sizeof(char)));
        if (NULL == tmp)
            PARSE_RETURN_ERROR(*type);

        strncpy(tmp, ptr + (*index), end_index - *index + 1);
        tmp[end_index + *index] = '\0';

        *(int*)(*data) = itoh(tmp);
        FREE_PTR(tmp);

        *index = end_index;
        ADVANCE_NEXT_WORD(ptr, *index);
        return;
    }
    else if (ptr[*index] >= '0' && ptr[*index] <= '9')
    {
        // Integer value
        *type = INTEGER;
        *data = realloc(*data, sizeof(int));
        if (NULL == *data)
            PARSE_RETURN_ERROR(*type);

        for (end_index = *index;
             ptr[end_index] >= '0' && ptr[end_index] <= '9';
             ++end_index);

        tmp = (char*) realloc(tmp, (end_index - (*index) + 1) * sizeof (char));
        if (NULL == tmp)
            PARSE_RETURN_ERROR(*type);

        strncpy(tmp, ptr + (*index), end_index - *index + 1);
        tmp[end_index - *index] = '\0';

        *(int*)(*data) = atoi(tmp);
        FREE_PTR(tmp);

        *index = end_index;
        ADVANCE_NEXT_WORD(ptr, *index);
        return;
    }
}

struct entry* json_parse(const char* ptr)
{
    unsigned int index = 0;
    int end_index;
    char* key = NULL;
    void* data = NULL;
    json_type_e type = ERROR;

    if (NULL == ptr) {
        return NULL;
    }

    struct entry* json = NULL;
    struct entry* prev_entry = NULL;
    struct entry* curr_entry = NULL;

    if ('{' != ptr[index])
        goto error;

    ++index;

    while ('}' != ptr[index])
    {
        ADVANCE_NEXT_WORD(ptr, index);

        if ('\"' != ptr[index])
            goto error;

        ++index;
        end_index = strchr(ptr + index, '\"') - ptr;

        key = (char*) realloc(key, sizeof(char) * (end_index - index + 1));
        if (NULL == key)
            goto error;
        memcpy(key, ptr + index, end_index - index + 1);
        key[end_index - index] = '\0';
        index = end_index + 1;

        ADVANCE_NEXT_WORD(ptr, index);

        if (':' != ptr[index])
            goto error;

        ++index;
        ADVANCE_NEXT_WORD(ptr, index);
        parse_data(ptr, &index, &data, &type);

        if (ERROR != type)
        {
            curr_entry = create_entry(key, type, data);

            if (NULL == json)
                json = curr_entry;
            if (NULL != prev_entry)
                prev_entry->next = curr_entry;
        }

        FREE_PTR(key);
        FREE_PTR(data);

        ADVANCE_NEXT_WORD(ptr, index);

        if (',' != ptr[index] && '}' != ptr[index])
            goto error;

        if (',' == ptr[index])
        {
            prev_entry = curr_entry;
            index++;
        }
    }

    return json;


error:
    PRINTERR("KEY (addr: %p, value: %s)\n", key, key);
    FREE_PTR(key);
    PRINTERR("TYPE (value: %s)\n", json_type_string[type]);
    PRINTERR("DATA (addr: %p, value: %s)\n",
             data, (NULL == data) ? "null" : (char*) data);
    FREE_PTR(data);
    PRINTERR("Parsing (index: %u, value: %c\n\trest: %s\n",
             index, ptr[index], index + ptr);
    if (NULL != json)
        delete_entry(&json);

    return NULL;
}
