#include "read.h"

bool leer_linea(char *msg, char *buffer, int buffer_len, bool accept_empty)
{
_leer_linea_start:
    printf("%s", msg);
    if (!fgets(buffer, buffer_len, stdin))
    {
        printf("Error leyendo la linea de la consola!\n");
        return (false);
    }
    buffer[strcspn(buffer, "\n")] = 0;

    // vaciamos stdin
    fseek(stdin, 0, SEEK_END);

    if (!accept_empty)
    {
        if (buffer[0] == 0)
        {
            printf("No ingrese una linea vacia.\n");
            goto _leer_linea_start;
        }
    }

    return (true);
}

bool leer_numero(char *msg, int *dest, bool accept_empty)
{
    char linea[LINE_MAX];

_leer_numero_start:
    leer_linea(msg, linea, sizeof(linea), accept_empty);
    if (!linea[0])
    {
        return (false);
    }

    char *end_ptr;
    long num = strtol(linea, &end_ptr, 10);
    if (linea == end_ptr)
    {
        if (!accept_empty)
        {
            goto _leer_numero_start;
        }
        return (false);
    }

    if (dest)
    {
        *dest = (int)num;
    }
    return (true);
}