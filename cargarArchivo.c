#include "cargarArchivo.h"


List* load_from_file(const char* path)
{
	List *hours = createList();

    FILE *fptr = fopen(path, "r");
    if (!fptr)
    {
        printf("Error al abrir '%s'\n", path);
        return (hours);
    }

	int load_count = 0;
	char line_buffer[4096];
	int item_count = 0;
	int line_number = 0;
    while (fgets(line_buffer, sizeof(line_buffer), fptr))
    {
		// Skip empty lines
        if (isBlank(line_buffer)) { continue; }

        line_buffer[strcspn(line_buffer, "\n")] = 0;
        char *line_ptr = line_buffer;

        char piece_buffer[256];
		Hour hora_desde = {0};
		line_ptr = trim_whitespace(line_ptr);
		if (!parse_piece(&line_ptr, piece_buffer, sizeof(piece_buffer), ':', true) ||
            !string_to_int(piece_buffer, &hora_desde.hr))
		{ printf("Error leyendo la 1era hora en la linea '%s'\n", line_buffer); continue; }

		line_ptr = trim_whitespace(line_ptr);
		if (!parse_piece(&line_ptr, piece_buffer, sizeof(piece_buffer), ' ', true) ||
            !string_to_int(piece_buffer, &hora_desde.min))
		{ printf("Error leyendo el 1er minuto en la linea '%s'\n", line_buffer); continue; }


		Hour hora_hasta = {0};
		line_ptr = trim_whitespace(line_ptr);
		if (!parse_piece(&line_ptr, piece_buffer, sizeof(piece_buffer), ':', true) ||
            !string_to_int(piece_buffer, &hora_hasta.hr))
		{ printf("Error leyendo la 2da hora en la linea '%s'\n", line_buffer); continue; }

		line_ptr = trim_whitespace(line_ptr);
		if (!parse_piece(&line_ptr, piece_buffer, sizeof(piece_buffer), ' ', true) ||
            !string_to_int(piece_buffer, &hora_hasta.min))
		{ printf("Error leyendo el 2do minuto en la linea '%s'\n", line_buffer); continue; }

		// Parseado correctamente, agregar a la lista.
		Hour* new_hour = malloc(sizeof(*new_hour));
		if (!new_hour) { printf("Error pidiendo memoria para la 1era hora en la linea '%s'\n", line_buffer); abort(); }
		new_hour->hr = hora_desde.hr;
		new_hour->hr = hora_desde.min;
		pushBack(hours, new_hour);
		++load_count;

		new_hour = malloc(sizeof(*new_hour));
		if (!new_hour) { printf("Error pidiendo memoria para la 2da hora en la linea '%s'\n", line_buffer); abort(); }
		new_hour->hr = hora_hasta.hr;
		new_hour->hr = hora_hasta.min;
		pushBack(hours, new_hour);
		++load_count;
	}
	
	fclose(fptr);
	printf("Se cargaron %d eventos\n", load_count);

    return (hours);
} 