#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include "parser.h"
#include "../utils/json_parser.h"
#include "../utils/json_entry.h"

void parser(char* filename)
{
    FILE* file = NULL;
    char* ptr = NULL;
    unsigned int index = 0;
    struct entry* entry = NULL;

    file = fopen(filename, "r");

    ptr = (char*) malloc(255 * sizeof(char));
    memset(ptr, 0, 255 * sizeof(char));

    if (file == NULL)
    {
        PRINTERR("Error openning file %s. Error: %d\n", filename, errno);
        goto error;
    }
    while (fread(ptr + index, 1, 1, file))
    {
        // One json per line
        if ('\n' == ptr[index])
            entry = json_parse(ptr);

        if (entry == NULL)
            goto error;
    }

    delete_entry(&entry);

error:
    FREE_PTR(ptr);
    if (NULL != file)
        fclose(file);
    return;
}
