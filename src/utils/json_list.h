#ifndef JSON_LIST_H
# define JSON_LIST_H 1

#include "json_entry.h"

struct json_list
{
    struct entry* json;
    char* timestamp;
    char* sid;
    struct json_list* next;
};

/**
 * Add the json to the head of the list
 * \param list existing list of json
 * \param json Json to add
*/
void add_json_to_list(struct json_list** list, struct entry* json);

/**
 * Pop the JSON from the top of list
 * \param list List of json
*/
struct entry* pop_json(struct json_list** list);

/**
 * Find a json in the list, searching by timestamp and sid only
 * \param list List of json
 * \param timestamp Timestamp to look for
 * \param sid SID to look for
 * \return json if found or NULL if not found
*/
struct entry* find_json(struct json_list** list,
                        const char* timestamp,
                        const char* sid);

/**
 * Remove a specifi json from the List
 * \param list List of json
 * \param timestamp Timestamp to look for
 * \param sid SID to look for
 * \return json if found or NULL if not found
*/
struct entry* remove_json(struct json_list** list,
                          const char* timestamp,
                          const char* sid);

#endif
