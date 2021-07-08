#pragma once

#include "common.h"

bool leer_linea(char *msg, char *buffer, int buffer_len, bool accept_empty);

bool leer_numero(char *msg, int *dest, bool accept_empty);