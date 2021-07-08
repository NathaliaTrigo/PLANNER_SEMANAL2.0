#ifndef STRCMP_INSENSITIVE_H
#define STRCMP_INSENSITIVE_H

#include <ctype.h>


// strcmp, pero no le importa si hay diferencias entre 'A' y 'a'
int strcmp_insensitive(char const *a, char const *b);

#endif