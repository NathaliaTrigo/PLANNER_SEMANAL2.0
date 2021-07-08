#pragma once


#include "common.h"
#include <math.h>

#include "list.h"
#include "read.h"
#include "parse.h"

#include "strcmp_insensitive.h"

typedef struct Hour
{
    int hr;
    int min;
} Hour;

List *load_from_file(const char *path);