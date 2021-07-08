#include "tiempo.h"

int get_dia_semana(int d, int m, int y)
{
	int weekday  = (d += m < 3 ? y-- : y - 2, 23*m/9 + d + 4 + y/4- y/100 + y/400)%7;
	--weekday;
	if (weekday < 0 ) { weekday = 6; }
	return(weekday);
}

void print_fecha(Fecha* a)
{
  printf("Fecha actual: ");
  if(a->dia_semana == 0) printf("Lunes ");
  if(a->dia_semana == 1) printf("Martes ");
  if(a->dia_semana == 2) printf("Miercoles ");
  if(a->dia_semana == 3) printf("Jueves ");
  if(a->dia_semana == 4) printf("Viernes ");
  if(a->dia_semana == 5) printf("Sábado ");
  if(a->dia_semana == 6) printf("Domingo ");
	printf("%02d-%02d-%d %02d:%02d:%02d\n",
		a->dia, a->mes, a->year, a->hora.hora, a->hora.minuto, a->hora.segundo);
}

Fecha get_fecha()
{
	time_t current_time = time(NULL);
	struct tm tm = *localtime(&current_time);

	Fecha f = {0};
	f.dia = tm.tm_mday;
	f.mes = tm.tm_mon + 1;
	f.year = tm.tm_year + 1900;
	f.dia_semana = tm.tm_wday-1;
	if(f.dia_semana<0) f.dia_semana=6;

	f.hora.hora = tm.tm_hour;
	f.hora.minuto = tm.tm_min;
	f.hora.segundo = tm.tm_sec;
	return(f);
}

long get_fecha_diff(Fecha* a, Fecha* b)
{
	Fecha diff = {0};

	diff.year = b->year - a->year;
	diff.mes = b->mes - a->mes;
	diff.dia = b->dia - a->dia;

	diff.hora.hora = b->hora.hora - a->hora.hora; 
	diff.hora.minuto = b->hora.minuto - a->hora.minuto;
	diff.hora.segundo = b->hora.segundo - a->hora.segundo;
	
	const long min_factor = 60;
	const long hor_factor = 60 * min_factor;
	const long dia_factor = 24 * hor_factor;
	const long mes_factor = 31 * dia_factor;
	const long year_factor = 365 * dia_factor;

	long d = diff.hora.segundo + min_factor * diff.hora.minuto + hor_factor*diff.hora.hora;
	d += diff.mes * mes_factor + diff.year * year_factor;

	return(d);
}


void update_fecha(Fecha* a)
{
	Fecha new_f = get_fecha();
	a->dia = new_f.dia;
	a->mes = new_f.mes;
	a->year = new_f.year;
	a->dia_semana = new_f.dia_semana;

	a->hora = new_f.hora;
}

Fecha copy_fecha(Fecha* f) { return create_fecha_from_hora(f, 0, 0, 0); }


Fecha create_fecha(int dia, int mes, int year, int hora, int minuto, int segundo)
{
	Fecha f = get_fecha();
	if (dia) { f.dia = dia; }
	if (mes) { f.mes = mes; }
	if (year) { f.year = year; }
	if (hora) { f.hora.hora = hora; }
	if (minuto) { f.hora.minuto = minuto; }
	if (segundo) { f.hora.segundo = segundo; }
	f.dia_semana = get_dia_semana(f.dia, f.mes, f.year);
	return(f);
}

Fecha create_fecha_from_hora(Fecha* f, int new_hora, int new_minuto, int new_segundo)
{
	Fecha new_f = *f;
	if (new_hora) { new_f.hora.hora = new_hora; }
	if (new_minuto) { new_f.hora.minuto = new_minuto; }
	if (new_segundo) { new_f.hora.segundo = new_segundo; }
	return(new_f);
}

Fecha create_fecha_from_string(const char* s, bool has_hour)
{
	Fecha f = {0};
	int advance = 0;

	advance = string_to_int(s, &f.dia);
	if (!advance) { printf("Invalid day!\n"); return(f); }
	s += advance;
	if (*s == '-' || *s == '/') { ++s; }
	advance = string_to_int(s, &f.mes);
	if (!advance) { printf("Invalid month!\n"); return(f); }
	s += advance;
	if (*s == '-' || *s == '/') { ++s; }
	advance = string_to_int(s, &f.year);
	if (!advance) { printf("Invalid year!\n"); return(f); }
	s += advance;
	if (*s == ' ' || *s == '\t') { ++s; }

	if (has_hour)
	{
		advance = string_to_int(s, &f.hora.hora);
		if (!advance) { printf("Invalid hours!\n"); return(f); }
		s += advance;
		if (*s == ':' || *s == '.') { ++s; }
		if (*s == ':' || *s == '.') { ++s; }
		advance = string_to_int(s, &f.hora.minuto);
		if (!advance) { printf("Invalid minutes!\n"); return(f); }
		s += advance;
		if (*s == ':' || *s == '.') { ++s; }
		if (*s == ':' || *s == '.') { ++s; }
		advance = string_to_int(s, &f.hora.segundo);
		if (!advance) { printf("Invalid seconds!\n"); return(f); }
	}

	f.dia_semana = get_dia_semana(f.dia, f.mes, f.year);
	return(f);
}


char* fecha_to_string(Fecha* f, char* buffer, int buffer_size, bool include_hour)
{
	const char with_hour[] = "%02d-%02d-%4d %02d:%02d:%02d";
	const char without_hour[] = "%02d-%02d-%d";

	if (include_hour)
	{
		if (buffer_size < sizeof(with_hour)) 
		{ printf("Buffer is too small, minimum size needed is %zu bytes!\n", sizeof(with_hour)); return(NULL); }
		sprintf(buffer, with_hour, f->dia, f->mes, f->year, f->hora.hora, f->hora.minuto, f->hora.segundo);
	}
	else
	{
		if (buffer_size < sizeof(without_hour)) 
		{ printf("Buffer is too small, minimum size needed is %zu bytes!\n", sizeof(without_hour)); return(NULL); }
		sprintf(buffer, without_hour, f->dia, f->mes, f->year);
	}
	return(buffer);
}

int compare_fecha_strings(const char* a, const  char* b)
{
	int dia_a, mes_a, year_a;
	int advance = 0;

	advance = string_to_int(a, &dia_a);
	if (!advance) { printf("Invalid first day!\n"); return(0); }
	a += advance;
	if (*a == '-' || *a == '/') { ++a; }
	advance = string_to_int(a, &mes_a);
	if (!advance) { printf("Invalid first month!\n"); return(0); }
	a += advance;
	if (*a == '-' || *a == '/') { ++a; }
	advance = string_to_int(a, &year_a);
	if (!advance) { printf("Invalid first year!\n"); return(0); }
	a += advance;
	// if (*a == '-' || *a == '/') { ++a; }

	int dia_b, mes_b, year_b;
	advance = string_to_int(b, &dia_b);
	if (!advance) { printf("Invalid second day!\n"); return(0); }
	b += advance;
	if (*b == '-' || *b == '/') { ++b; }
	advance = string_to_int(b, &mes_b);
	if (!advance) { printf("Invalid second month!\n"); return(0); }
	b += advance;
	if (*b == '-' || *b == '/') { ++b; }
	advance = string_to_int(b, &year_b);
	if (!advance) { printf("Invalid second year!\n"); return(0); }
	b += advance;
	// if (*b == '-' || *b == '/') { ++b; }

	if (year_a != year_b) { return(-(year_b - year_a)); }
	if (mes_a != mes_b) { return(-(mes_b - mes_a)); }
	if (dia_a != dia_b) { return(-(dia_b - dia_a)); }
	return(0);
}