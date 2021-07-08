#include "common.h"
#include "cargarArchivo.h"
#include "menu.h"
#include "list.h"
#include "parse.h"

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "actividad.h"

bool load_from_save(char* path, List* actividades, List* eventos, Dia* dias)
{
	FILE* fptr = fopen(path, "r");
	if (!fptr) { return(false); }
	int mode = 0;

	char line_buffer[LINE_MAX];
	while (fgets(line_buffer, sizeof(line_buffer), fptr))
	{
		line_buffer[strcspn(line_buffer, "\n")] = 0;

		if (isBlank(line_buffer)) { continue; }
		if (strcmp_insensitive(line_buffer, "ACTIVIDADES") == 0) { mode = 1; continue; }
		if (strcmp_insensitive(line_buffer, "EVENTOS") == 0) { mode = 2; continue;  }

		if (mode < 0) { break; }
		else if (mode == 1)
		{
			// Read actividad
			Actividad a = {0};

			char* line_ptr = line_buffer;
			char piece_buffer[LINE_MAX];
			
			// sprintf(line, "%d\t%s\t%s\t%d\n", a->prioridad, a->nombre, fecha_str, a->duracion);
			line_ptr = trim_whitespace(line_ptr);
			if (!parse_piece(&line_ptr, piece_buffer, sizeof(piece_buffer), '\t', true) ||
				!string_to_int(piece_buffer, &a.prioridad))
			{ printf("Error leyendo la prioridad de la actividad '%s'\n", line_buffer); continue; }

			line_ptr = trim_whitespace(line_ptr);
			if (!parse_piece(&line_ptr, piece_buffer, sizeof(piece_buffer), '\t', true))
			{ printf("Error leyendo el nombre de la actividad '%s'\n", line_buffer); continue; }
			strcpy(a.nombre, piece_buffer);

			line_ptr = trim_whitespace(line_ptr);
			if (!parse_piece(&line_ptr, piece_buffer, sizeof(piece_buffer), '\t', true))
			{ printf("Error leyendo la hora de la actividad '%s'\n", line_buffer); continue; }
			a.fecha = create_fecha_from_string(piece_buffer, true);
			
			line_ptr = trim_whitespace(line_ptr);
			if (!parse_piece(&line_ptr, piece_buffer, sizeof(piece_buffer), '\t', true) ||
				!string_to_int(piece_buffer, &a.duracion))
			{ printf("Error leyendo la duracion de la actividad '%s'\n", line_buffer); continue; }

			/*
			char fecha_str[LINE_MAX];
			fecha_to_string(&a.fecha, fecha_str, sizeof(fecha_str), true);
			printf("Loaded: %d\t%s\t%s\t%d\n", a.prioridad, a.nombre, fecha_str, a.duracion);
			*/

			// If all good, actually create the act.
			Actividad* new_a = malloc(sizeof(*new_a));
			new_a->duracion = a.duracion;
			new_a->prioridad = a.prioridad;
			new_a->fecha = a.fecha;
			strcpy(new_a->nombre, a.nombre);
			pushBack(actividades, new_a);
		}
		else if (mode == 2)
		{
			// Read evento
			Evento e = {0};
			char* line_ptr = line_buffer;
			char piece_buffer[LINE_MAX];
			
			// fprintf(fptr, "%s\t%s\t%d\n", e->nombre, fecha_str, e->duracion);
			line_ptr = trim_whitespace(line_ptr);
			if (!parse_piece(&line_ptr, piece_buffer, sizeof(piece_buffer), '\t', true))
			{ printf("Error leyendo el nombre del evento '%s'\n", line_buffer); continue; }
			strcpy(e.nombre, piece_buffer);

			line_ptr = trim_whitespace(line_ptr);
			if (!parse_piece(&line_ptr, piece_buffer, sizeof(piece_buffer), '\t', true))
			{ printf("Error leyendo la hora del evento '%s'\n", line_buffer); continue; }
			e.fecha = create_fecha_from_string(piece_buffer, true);
			
			line_ptr = trim_whitespace(line_ptr);
			if (!parse_piece(&line_ptr, piece_buffer, sizeof(piece_buffer), '\t', true) ||
				!string_to_int(piece_buffer, &e.duracion))
			{ printf("Error leyendo la duracion del evento '%s'\n", line_buffer); continue; }

			// If all good, actually create the act.
			Evento* new_e = malloc(sizeof(*new_e));
			new_e->duracion = e.duracion;
			new_e->fecha = e.fecha;
			strcpy(new_e->nombre, e.nombre);
			pushBack(eventos, new_e);
		}
	}

	fclose(fptr);
	return(true);
}

bool save_to_file(char* path, List* actividades, List* eventos, Dia* dias)
{
	FILE* fptr = fopen(path, "w");
	if (!fptr) { printf("Error al abrir '%s' para escritura!\n", path); return(false); }
	bool saved_correctly = true;
	// char line[LINE_MAX];
	char fecha_str[64];

	fprintf(fptr, "ACTIVIDADES\n");
	Actividad* a = firstList(actividades);
	while(a)
	{
		fecha_to_string(&a->fecha, fecha_str, sizeof(fecha_str), true);
		fprintf(fptr, "%d\t%s\t%s\t%d\n", a->prioridad, a->nombre, fecha_str, a->duracion);
		// fwrite(line, sizeof(line[0]), strlen(line), fptr);
		a = nextList(actividades);
	}

	fprintf(fptr, "EVENTOS\n");
	Evento* e = firstList(eventos);
	while(e)
	{
		fecha_to_string(&e->fecha, fecha_str, sizeof(fecha_str), true);
		fprintf(fptr, "%s\t%s\t%d\n", e->nombre, fecha_str, e->duracion);
		// fwrite(line, sizeof(line[0]), strlen(line), fptr);
		e = nextList(eventos);
	}

	fclose(fptr);
	return(saved_correctly);
}

int main(void) {

	List* actividades = createList();
	List* eventos = createList();
	List* hours = createList();
	
	Dia dias[7];
	for(int i = 0 ; i<7 ; i++){
		dias[i].actividades = createList();
    	dias[i].eventos = createList();
	}

	printf("Loading...\n");
	if (load_from_save("saved_week.txt", actividades, eventos, dias))
	{ printf("Loaded.\n"); }
	else { printf("No save file found!\n"); }

	// falta implementar las funciones de "lista" en Dia y Semana.
	bool running = true;
	while(running){
		bool result = mostrar_menu(hours, actividades, eventos, dias);
		if (!result) { running = false; }
		// else { fancy_pause(); }
	}

	for(int i = 0; i < sizeof(dias)/sizeof(dias[0]); ++i)
	{
		Dia* d = dias + i;
		Actividad* a = firstList(d->actividades);
		cleanList(d->actividades);
		cleanList(d->eventos);
		free(d->actividades);
		free(d->eventos);
	}

	save_to_file("saved_week.txt", actividades, eventos, dias);

	// Liberar lista points
	Hour* h = firstList(hours);
	while(h) { free(h); h = nextList(hours); }
	free(hours);

	Evento* e = firstList(eventos);
	while(e) { free(e); e = nextList(eventos); }
	free(eventos);
	
	Actividad* a = firstList(actividades);
	while(a) { free(a); a = nextList(actividades); }
	free(actividades);

	printf("\nGoodbye!\n");
	
    return(0);
}
