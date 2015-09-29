#include <stdlib.h>
#include <string.h>
#include "json_list.h"

void add_json_to_list(struct json_list** list, struct entry* json)
{
    if (NULL == json)
        return;

    struct json_list* head_list =
        (struct json_list*) malloc(sizeof(struct json_list));
    if (NULL == head_list)
        return;

    head_list->json = json;
    head_list->timestamp = (char*) get_json_item(json, "timestamp");
    head_list->sid = (char*) get_json_item(json, "sid");
    head_list->next = *list;

    *list = head_list;
    return;
}

struct entry* pop_json(struct json_list** list)
{
    if (NULL == list || NULL == *list)
        return NULL;

    struct entry* res = (*list)->json;
    struct json_list* tmp = *list;

    *list = (*list)->next;
    free(tmp);

    return res;
}

struct entry* find_json(struct json_list** list,
                        const char* timestamp,
                        const char* sid)
{
    struct json_list* tmp;

    if (NULL == list || NULL == *list)
        return NULL;

    for (tmp = *list;
         tmp != NULL &&
         strcmp(tmp->timestamp, timestamp) != 0 &&
         strcmp(tmp->sid, sid) != 0;
         tmp = tmp->next);

    return (NULL == tmp) ? NULL : tmp->json;
}

struct entry* remove_json(struct json_list** list,
                          const char* timestamp,
                          const char* sid)
{
    struct json_list* tmp;
    struct json_list* prev = NULL;
    struct entry* res;

    if (NULL == list || NULL == *list)
        return NULL;

    tmp = *list;
    while(tmp != NULL &&
         strcmp(tmp->timestamp, timestamp) != 0 &&
         strcmp(tmp->sid, sid) != 0)
    {
        prev = tmp;
        tmp = tmp->next;
    }

    if (tmp == NULL)
        return NULL;

    prev->next = tmp->next;
    res = tmp->json;
    free(tmp);

    return res;
}
