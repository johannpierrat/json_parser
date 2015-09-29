#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include "parser.h"
#include "../utils/json_parser.h"
#include "../utils/json_entry.h"
#include "../utils/json_list.h"
#include "../utils/json_print.h"

void parser(const char* filename)
{
    FILE* file = NULL;
    char* ptr = NULL;
    char c;
    unsigned int index = 0;
    unsigned int size = 255;
    struct entry* entry = NULL;
    struct json_list* list = NULL;

    file = fopen(filename, "r");

    ptr = (char*) malloc(size * sizeof(char));
    memset(ptr, 0, size * sizeof(char));

    if (file == NULL)
    {
        PRINTERR("Error openning file %s. Error: %d\n", filename, errno);
        goto error;
    }
    while (fread(&c, 1, 1, file) != 0)
    {
        ptr[index++] = c;
        if (index > size)
        {
            size <<= 1;
            ptr = (char*) realloc(ptr, size * sizeof(char));
            memset(ptr + index, 0, (size - index) * sizeof(char));
        }
        // One json per line
        if ('\n' == ptr[index - 1])
        {
            char* output = NULL;
            int index2 = 0;
            size_t size2 = 0;

            entry = json_parse(ptr);
            if (NULL == entry)
            {
                PRINTERR("Error Parsing: %s\n", ptr);
            }
            else {
                json_to_string(&output, &size2, &index2, entry);
                add_json_to_list(&list, entry);
            }

            // Reset data
            index = 0;
            size = 255;
            ptr = (char*) realloc(ptr, size * sizeof(char));
            memset(ptr, 0, size * sizeof(char));
        }
    }

    FREE_PTR(ptr);
    delete_entry(&entry);

error:
    FREE_PTR(ptr);
    if (NULL != file)
        fclose(file);
    return;
}

int main(int argc, char** argv)
{
    if (argc < 2)
        exit(1);

    parser(argv[1]);
}
