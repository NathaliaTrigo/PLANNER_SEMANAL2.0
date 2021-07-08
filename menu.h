#pragma once

#include "common.h"
#include "parse.h"
#include "read.h"
#include "strcmp_insensitive.h"
#include "tiempo.h"
#include "actividad.h"

#include "cargarArchivo.h"

// MenuFunc es un puntero a una funcion de tipo bool name(void)
#define MENU_FUNC(name) bool name(List* hours, List* actividades, List* eventos, Dia* dias)
typedef MENU_FUNC(MenuFunc);

// MenuOption es una 
// "tupla" de (letra, mensaje a mostrar, ptr a funcion)
typedef struct MenuOption 
{ 
  const char* option;
  const char* msg; 
  MenuFunc* func;
}MenuOption;

struct node
{
    int data;          // Data
    struct node *next; // Address
} * head;

MENU_FUNC(mostrar_menu);
MENU_FUNC(menu_cargar_archivo);
MENU_FUNC(menu_agregar_actividad);
MENU_FUNC(menu_agregar_evento);
MENU_FUNC(menu_mostrar_lista);
MENU_FUNC(menu_mostrar_actividad_o_evento);
MENU_FUNC(menu_mostrar_horario);
MENU_FUNC(menu_tiempo_disponible);
MENU_FUNC(menu_eliminar_actividad_o_evento);
MENU_FUNC(menu_salir);