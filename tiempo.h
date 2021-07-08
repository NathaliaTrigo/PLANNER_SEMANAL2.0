#pragma once
#include "common.h"
#include <time.h>

#include "parse.h"


typedef struct Hora
{
	int hora;
	int minuto;
	int segundo;
} Hora;

typedef struct Fecha
{
	int dia;
	int mes;
	int year; // Como escribo a~o? xd
  int dia_semana; // ? wots dis? es el dia de la semana de 0 a 6 para ubicarlo mas easy

	Hora hora;
} Fecha;


int get_dia_semana(int d, int m, int y);
void print_fecha(Fecha* a);

Fecha get_fecha();
long get_fecha_diff(Fecha* a, Fecha* b);

void update_fecha(Fecha* a);
Fecha copy_fecha(Fecha* f);

// Cualquier valor dejado en cero, se reemplaza con el actual, para no tener que escribir tanto.
Fecha create_fecha(int dia, int mes, int year, int hora, int minuto, int segundo);
// Deja dia/mes/a~o como los actuales, solo cambia la hora. (aplica lo de los ceros).
Fecha create_fecha_from_hora(Fecha* f, int new_hora, int new_minuto, int new_segundo);
Fecha create_fecha_from_string(const char* s, bool has_hour);

char* fecha_to_string(Fecha* f, char* buffer, int buffer_size, bool include_hour);
int compare_fecha_strings(const char* a, const char* b);