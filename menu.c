#include "menu.h"
#include "list.h"
#include "actividad.h"


bool printNombreActividad(Actividad* actividad){
  puts(actividad->nombre);
  printf("\n");
  return(true);
}
bool printNombreEvento(Evento* evento){
  puts(evento->nombre);
  printf("\n");
  return(true);
}



bool printActividad(Actividad* actividad){
  char s[1];
  printf("\n");
  printf("-----------------------------\n%10s",s);
  puts(actividad->nombre);
  printf("%10sPrioridad: %d\n",s,actividad->prioridad);
  printf("%10sFecha %d/%d\n",s,actividad->fecha.dia,actividad->fecha.mes);
  printf("%10sTiempo: %d:%d\n",s,actividad->fecha.hora.hora, actividad->fecha.hora.minuto);
  printf("-----------------------------\n");
  return(true);
}

bool printEvento(Evento* evento){
  printf("\n");
  char s[1];
  printf("-----------------------------\n%10s",s);
  puts(evento->nombre);
  printf("%10sFecha %d/%d\n",s,evento->fecha.dia,evento->fecha.mes);
  printf("%10sTiempo: %d:%d\n",s,evento->fecha.hora.hora, evento->fecha.hora.minuto);
  printf("-----------------------------\n");
  return(true);
}

bool calendario(List* hours, List* actividades, List* eventos, Dia* dias){
  int isThere;
  Fecha fecha = get_fecha();
  int dayWeek;
  Actividad* currentActividad = firstList(actividades);
  //actividades
  for(int i=0 ; i<sizeOfList(actividades) ; i++){
    isThere = 0;
    if(currentActividad->fecha.dia >= fecha.dia){
      int diff = currentActividad->fecha.dia - fecha.dia;
      dayWeek = fecha.dia_semana + diff;
      Actividad* currentLocal = firstList(dias[dayWeek].actividades);
      while(currentLocal){
        if(strcmp(currentLocal->nombre,currentActividad->nombre)==0)  isThere = 1;
        currentLocal = nextList(dias[dayWeek].actividades);
      }
      if(isThere == 0)  pushBack(dias[dayWeek].actividades, currentActividad);
    }
    currentActividad = nextList(actividades);
  }
  //Eventos
  Evento* currentEvento = firstList(eventos);
  for(int j = 0 ; j<sizeOfList(eventos) ; j++){
    isThere = 0;
    if(currentEvento->fecha.dia >= fecha.dia){
      int diff = currentEvento->fecha.dia - fecha.dia;
      dayWeek = fecha.dia_semana + diff;
      Evento* currentLocalE = firstList(dias[dayWeek].eventos);
      while(currentLocalE){
        if(strcmp(currentLocalE->nombre,currentEvento->nombre)==0)  isThere = 1;
        currentLocalE = nextList(dias[dayWeek].eventos);
      }
      if(isThere == 0)  pushBack(dias[dayWeek].eventos, currentEvento);
    }
    currentEvento = nextList(eventos);
  }
  
  return(true);
}


MENU_FUNC(mostrar_menu)
{
    // Estas son todas las optiones en el menu principal.
    // cada una tiene una letra que la causa, un mensaje a mostrar
    // y una funcion a llamar en caso de ser seleccionada.
    MenuOption options[] = {
        {"1", "Importar archivo propio de comidas y sueño", menu_cargar_archivo},
        {"2", "Agregar actividad", menu_agregar_actividad},
        {"3", "Agregar evento", menu_agregar_evento},
        {"4", "Lista de actividades y evento", menu_mostrar_lista},
        {"5", "Ver actividad o evento", menu_mostrar_actividad_o_evento},
        {"6", "Mostrar horario semanal", menu_mostrar_horario},
        {"7", "Tiempo disponible en el día", menu_tiempo_disponible},
        {"8", "Eliminar actividad o evento", menu_eliminar_actividad_o_evento},
        {"0", "Salir", menu_salir},
    };
	
	Fecha fecha = get_fecha();

    // Mostrar todas las opciones en pantalla
    printf("+-----------------+\n");
    printf("| PLANNER SEMANAL |\n");
    printf("+-----------------+\n");
	  print_fecha(&fecha);
    printf("Ingrese una opcion:\n");

    MenuOption* option;
    int options_len = sizeof(options) / sizeof(options[0]);
    for(int i = 0; i < options_len; ++i)
    {
        option = &options[i];
        printf("%s. %s\n", option->option, option->msg);
    }

    // Obtener el input del usuario.
    char line[LINE_MAX];
    leer_linea("> ", line, sizeof(line), true);

    if (!(*line))
    { 
        printf("Ingrese una opcion\n"); 
        return(true); 
    }

    // Buscamos si la opcion corresponde a alguna de la lista.
    bool found = false;
    for(int i = 0; i < options_len; ++i)
    {
        option = &options[i];
        found = strcmp_insensitive(option->option, line) == 0;

        if (found) 
        { 
            found = true; 
            break; 
        }
    }
    // De encontrar la funcion, la llamamos y retornamos lo que nos de,
    // sino, asumimos un mal input y preguntamos denuevo.
    if (found) 
    {
       return(option->func(hours, actividades, eventos, dias));
    } 
    else
    { 
        printf("Opcion '%s' invalida\n", line);
        return(true);
    }
}

//Hay que cambiarla toda
MENU_FUNC(menu_cargar_archivo){
  char respuesta[64];
  leer_linea("Ingrese el archivo a cargar: \n", respuesta, sizeof(respuesta), false);
  List* bloquesCargados = load_from_file(respuesta);
  int largo_cargados = sizeOfList(bloquesCargados);
  
  if(largo_cargados > 0){
    Evento* evento = firstList(bloquesCargados);
    int count = 0;
    while(evento){
      pushBack(eventos,evento);
      evento = nextList(bloquesCargados);
    }
    cleanList(bloquesCargados);
  }

  free(bloquesCargados);
	return(true);
}

MENU_FUNC(menu_agregar_actividad)
{
	Actividad *resultado = calloc(1, sizeof(*resultado));
	resultado->fecha = get_fecha();
	resultado->fecha.hora.segundo = 0;
	
	leer_linea("Ingrese el nombre de la actividad: ", resultado->nombre, sizeof(resultado->nombre), false);

menu_agregar_actividad_leer_prioridad:
	leer_numero("Ingrese la prioridad [1-9], mientras más cerca esté de su fecha limite mayor es su prioridad: ", &resultado->prioridad, false);
	if (resultado->prioridad < 0 || resultado->prioridad > 9)
	{ printf("La prioridad debe ser entre 1 y 9!\n"); goto menu_agregar_actividad_leer_prioridad; } 
	
menu_agregar_actividad_leer_dia:
	leer_numero("Ingrese el dia [1-31]: ", &resultado->fecha.dia, false);
	if (resultado->fecha.dia <= 0 || resultado->fecha.dia > 31) 
	{ printf("El dia debe estar entre 1 y 31!\n"); goto menu_agregar_actividad_leer_dia; }

menu_agregar_actividad_leer_mes:
	leer_numero("Ingrese el mes [1-12]: ", &resultado->fecha.mes, false);
	if (resultado->fecha.mes <= 0 || resultado->fecha.mes > 12) 
	{ printf("El mes debe estar entre 1 y 12!\n"); goto menu_agregar_actividad_leer_mes; }
menu_agregar_actividad_leer_hora:

	leer_numero("Ingrese la hora [0-24]: ", &resultado->fecha.hora.hora, false);
	if (resultado->fecha.hora.hora < 0 || resultado->fecha.hora.hora > 23) 
	{ printf("La hora debe estar entre 0 y 23!\n"); goto menu_agregar_actividad_leer_hora; }
  
menu_agregar_actividad_leer_minuto:
	leer_numero("Ingrese el minuto [0-59]: ", &resultado->fecha.hora.minuto, false);
	if (resultado->fecha.hora.minuto < 0 || resultado->fecha.hora.minuto > 60) 
	{ printf("El minuto debe estar entre 0 y 59!\n"); goto menu_agregar_actividad_leer_minuto; }

menu_agregar_actividad_leer_duracion:
	;char linea[LINE_MAX];
	char* linea_ptr = linea;
	leer_linea("Ingrese la duracion de la actividad [hh:mm]: ", linea_ptr, sizeof(linea), false);
	int dur_h, dur_m;
	int avance = string_to_int(linea_ptr, &dur_h); 
	if (!avance) { printf("Error leyendo la hora en la duracion '%s'\n", linea); goto menu_agregar_actividad_leer_duracion; }
	if (dur_h < 0 || dur_h > 23) { printf("La hora debe ser entre 0 y 23!\n"); goto menu_agregar_actividad_leer_duracion; }
	linea_ptr += avance+1;
	avance = string_to_int(linea_ptr, &dur_m);
	if (!avance) { printf("Error leyendo los minutos en la duracion '%s'\n", linea); goto menu_agregar_actividad_leer_duracion; }
	if (dur_h == 0) { if (dur_m < 1 || dur_m >= 60) { printf("Los minutos deben ser entre 1 y 59!\n"); goto menu_agregar_actividad_leer_duracion; }}
	else { if (dur_m < 0 || dur_m >= 60) { printf("Los minutos deben ser entre 0 y 59!\n"); goto menu_agregar_actividad_leer_duracion; } }
	resultado->duracion = (60 * dur_h) + dur_m;

	pushBack(actividades, resultado);
	return(true);
}

MENU_FUNC(menu_agregar_evento)
{
	Evento* resultado = calloc(1, sizeof(*resultado));
	resultado->fecha = get_fecha();
	resultado->fecha.hora.segundo = 0;

	leer_linea("Ingrese el nombre del evento: ", resultado->nombre, sizeof(resultado->nombre), false);
	Actividad* a = firstList(actividades);

menu_agregar_evento_leer_dia:
	leer_numero("Ingrese el dia [1-31]: ", &resultado->fecha.dia, false);
	if (resultado->fecha.dia <= 0 || resultado->fecha.dia > 31) 
	{ printf("El dia debe estar entre 1 y 31!\n"); goto menu_agregar_evento_leer_dia; }

menu_agregar_evento_leer_mes:
	leer_numero("Ingrese el mes [1-12]: ", &resultado->fecha.mes, false);
	if (resultado->fecha.mes <= 0 || resultado->fecha.mes > 12) 
	{ printf("El mes debe estar entre 1 y 12!\n"); goto menu_agregar_evento_leer_mes; }

menu_agregar_evento_leer_hora:
	leer_numero("Ingrese la hora [0-24]: ", &resultado->fecha.hora.hora, false);
	if (resultado->fecha.hora.hora < 0 || resultado->fecha.hora.hora > 23) 
	{ printf("La hora debe estar entre 0 y 24!\n"); goto menu_agregar_evento_leer_hora; }

menu_agregar_evento_leer_minuto:
	leer_numero("Ingrese el minuto [0-59]: ", &resultado->fecha.hora.minuto, false);
	if (resultado->fecha.hora.minuto <= 0 || resultado->fecha.hora.minuto > 59)
	{ printf("El minuto debe estar entre 0 y 59!\n"); goto menu_agregar_evento_leer_minuto; }

menu_agregar_evento_leer_duracion:
	;char linea[LINE_MAX];
	char* linea_ptr = linea;
	leer_linea("Ingrese la duracion de la actividad [hh:mm]: ", linea_ptr, sizeof(linea), false);
	int dur_h, dur_m;
	int avance = string_to_int(linea_ptr, &dur_h); 
	if (!avance) { printf("Error leyendo la hora en la duracion '%s'\n", linea); goto menu_agregar_evento_leer_duracion; }
	if (dur_h < 0 || dur_h > 23) { printf("La hora debe ser entre 0 y 23!\n"); goto menu_agregar_evento_leer_duracion; }
	linea_ptr += avance+1;
	avance = string_to_int(linea_ptr, &dur_m);
	if (!avance) { printf("Error leyendo los minutos en la duracion '%s'\n", linea); goto menu_agregar_evento_leer_duracion; }
	if (dur_h == 0) { if (dur_m < 1 || dur_m >= 60) { printf("Los minutos deben ser entre 1 y 59!\n"); goto menu_agregar_evento_leer_duracion; }}
	else { if (dur_m < 0 || dur_m >= 60) { printf("Los minutos deben ser entre 0 y 59!\n"); goto menu_agregar_evento_leer_duracion; } }
	resultado->duracion = (60 * dur_h) + dur_m;

	pushBack(eventos, resultado);
	return(1);
}

MENU_FUNC(menu_mostrar_lista){
  Actividad* currentA = firstList(actividades);
  int i;
  printf("Actividades:\n");
  for(i = 0 ; i<sizeOfList(actividades) ; i++){
    printf("%d:",i);
    printNombreActividad(currentA);
    currentA = nextList(actividades);
  }
  printf("Eventos:\n");
  Evento* currentE = firstList(eventos);
  char string[1][256];
  while(currentE){
    strcpy(string[0],currentE->nombre);
    if(isalpha(string[0][0])){
      printf("%d:",i);
      printNombreEvento(currentE);
      i++;
    }
    currentE = nextList(eventos);
  }
  printf("Presione enter para continuar...");
  getchar();
  return(true);
}

MENU_FUNC(menu_mostrar_actividad_o_evento){
  int opcion;
  while(true){
    printf("Mostrar:\n");
    printf("1) Evento\n");
    printf("2) Actividad\n");
    scanf("%d", &opcion);
    if(opcion>=1 && opcion<=2)  break;
    else{printf("Ingrese una opcion valida\n");}
  }
  char nombre[256];
  if(opcion == 1){
     leer_linea("Ingrese el nombre del evento\n", nombre, sizeof(nombre), 0);
    Evento* currentE = firstList(eventos);
    int largoEventos = sizeOfList(eventos);
    for(int i= 0 ; i<largoEventos ; i++){
      if(strcmp(currentE->nombre, nombre) == 0){
        printEvento(currentE);
      }
      else
      {
        printf("Este evento no se encuentra\n");
      }
      currentE = nextList(eventos);
    }
  }
  if(opcion==2){
    leer_linea("Ingrese el nombre de la actividad\n", nombre, sizeof(nombre), 0);
    Actividad* currentA = firstList(actividades);
    int largoAct = sizeOfList(actividades);
    for(int i= 0 ; i<largoAct ; i++){
      if(strcmp(currentA->nombre, nombre) == 0){
        printActividad(currentA);
      }
      else
      {
        printf("Esta actividad no se encuentra\n");
      }
      currentA = nextList(actividades);
    }
  }
  printf("Presione enter para continuar...");
  getchar();
   return(true);
}

MENU_FUNC(mostrar_sig_actividad)
{
	Fecha fecha_actual = get_fecha();
	
	int act_count = 0;
	printf("Actividades recomendadas a realizar:\n");
	Actividad* a = firstList(actividades);
	while(a)
	{
		// Chequeamos si el evento ya paso
		long diff_actual = get_fecha_diff(&fecha_actual, &a->fecha);
		if (diff_actual <= 0)
		{
			// Por cada evento
			bool busy = false;
			Evento* e = firstList(eventos);
			while(e)
			{
				// Chequeamos si dentro de duracion minutos mas, no hay ningun evento a colisionar.
				long diff_evento = get_fecha_diff(&e->fecha, &a->fecha);
				if (diff_evento > a->duracion)
				{
					// Hay suficiente tiempo como para recomendar la actividad, al menos para este evento.
				}
				else
				{
					busy = true;
					break;
				}
				e = nextList(eventos);
			}
			if (!busy) { printActividad(a); ++act_count; }
		}
		a = nextList(actividades);
	}

	if (!act_count) { printf("No hay actividades para recomendar que no colisionen con algun evento.\n"); }
	return(true);
}

MENU_FUNC(menu_mostrar_horario){
  calendario(hours, actividades, eventos, dias);
  
  mostrar_sig_actividad(hours, actividades, eventos, dias);
  /*
  int i, j;
  Actividad* currentActividad = firstList(dias[0].actividades);
  Evento* currentEvento = firstList(dias[0].eventos);
  int largoDiasA;
  for(i=0 ; i<7 ; i++ ){ 
    currentActividad = firstList(dias[i].actividades);
    currentEvento = firstList(dias[i].eventos);
    if(i == 0)  printf("Lunes:\n");
    else if(i == 1)  printf("Martes:\n");
    else if(i == 2)  printf("Míercoles:\n");
    else if(i == 3)  printf("Jueves:\n");
    else if(i == 4)  printf("Viernes:\n");
    else if(i == 5)  printf("Sábado:\n");
    else if(i == 6)  printf("Domingo:\n");

    while(currentActividad){        
      printf("%d:%d %3s\n",currentActividad->fecha.hora.hora,  currentActividad->fecha.hora.minuto, currentActividad->nombre); 
      currentActividad = nextList(dias[i].actividades);
    }
    while(currentEvento){        
      printf("%d:%d %3s\n",currentEvento->fecha.hora.hora,  currentEvento->fecha.hora.minuto, currentEvento->nombre); 
      currentEvento = nextList(dias[i].eventos);
    }  
  }
  */

  return(true);
}

MENU_FUNC(menu_eliminar_actividad_o_evento){
  int opcion;
  while(true){
    printf("Eliminar:\n");
    printf("1) Evento\n");
    printf("2) Actividad\n");
    scanf("%d", &opcion);
    if(opcion>=1 && opcion<=2)  break;
    else{printf("Ingrese una opcion valida\n");}
  }
  char nombre[256];
  if(opcion == 1){
    leer_linea("Ingrese el nombre del evento\n", nombre, sizeof(nombre), 0);
    Evento* current = firstList(eventos);
    int largoEventos = sizeOfList(eventos);
    for(int i= 0 ; i<largoEventos ; i++){
      if(strcmp(current->nombre, nombre) == 0){
        popCurrent(eventos);
        printf("Se ha eliminado el evento %s\n",nombre);
      }
      else
      {
        printf("Este evento no se encuentra\n");
      }
      current = nextList(eventos);
    }
  }
  if(opcion == 2){
    printf("Ingrese el nombre de la actividad\n");
    leer_linea("Ingrese el nombre de la actividad\n", nombre, 256, 0);
    Actividad* current = firstList(actividades);
    int largoActividades = sizeOfList(actividades);
    for(int i= 0 ; i<largoActividades ; i++){
      if(strcmp(current->nombre, nombre) == 0){
        popCurrent(actividades);
        printf("Se ha eliminado la actividad %s\n",nombre);
      }
      else
      {
        printf("Esta actividad no se encuentra\n");
      }
      current = nextList(actividades);
    }
  }
  printf("Presione enter para continuar...");
  getchar();
	return(true);
}

MENU_FUNC(menu_tiempo_disponible){
  return(true);
}

MENU_FUNC(menu_salir) {
	return(false);
}
