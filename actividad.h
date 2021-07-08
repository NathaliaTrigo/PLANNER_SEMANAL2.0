#pragma once
#include "list.h"
#include "tiempo.h"

typedef struct Actividad
{
	int prioridad;
	char nombre[256];
	Fecha fecha;
	int duracion;
} Actividad;

typedef struct Evento
{
	char nombre[256];
	Fecha fecha;
	// Hora hora;
	int duracion;
} Evento;

typedef struct Dia
{
	List* actividades;
  List* eventos;
	int cantidad;
	int maximo;
} Dia;

