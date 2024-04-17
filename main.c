#include "tdas/list.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAXCHAR 51

typedef struct
{
  char nombre[MAXCHAR];
  short edad;
  char sintoma[MAXCHAR];
  char prioridad[MAXCHAR];
} paciente;

// Función para limpiar la pantalla
void limpiarPantalla() { system("clear"); }

void presioneTeclaParaContinuar() {
  puts("Presione una tecla para continuar...");
  getchar(); // Consume el '\n' del buffer de entrada
  getchar(); // Espera a que el usuario presione una tecla
}

void convertirMayusculas(char *cadena)
{
  char *caracter = cadena;
  while (*caracter != '\0')
  {
    *caracter = toupper(*caracter);
    caracter++;
  }
}

void ordenarPorPrioridad(List *pacientes)
{
  if (pacientes == NULL || list_first(pacientes) == NULL)
  {
    printf("No hay pacientes en la lista.\n");
    return;
  }

  List *alta = list_create();
  List *media = list_create();
  List *baja = list_create();
  paciente *infoPaciente = (paciente *)list_first(pacientes);

  while (list_first(pacientes) != NULL)
  {
    if (strcmp(infoPaciente->prioridad, "ALTA") == 0)
      list_pushBack(alta, infoPaciente);
    else if (strcmp(infoPaciente->prioridad, "MEDIA") == 0)
      list_pushBack(media, infoPaciente);
    else
      list_pushBack(baja, infoPaciente);

    infoPaciente = (paciente *)list_next(pacientes);
    list_popFront(pacientes);
  }

  infoPaciente = (paciente *)list_first(alta);
  while (infoPaciente != NULL)
  {
    list_pushBack(pacientes, infoPaciente);
    infoPaciente = (paciente *)list_next(alta);
    list_popFront(alta);
  }
  infoPaciente = (paciente *)list_first(media);
  while (list_first(media) != NULL)
  {
    list_pushBack(pacientes, infoPaciente);
    infoPaciente = (paciente *)list_next(media);
    list_popFront(media);
  }
  infoPaciente = (paciente *)list_first(baja);
  while (list_first(baja) != NULL)
  {
    list_pushBack(pacientes, infoPaciente);
    infoPaciente = (paciente *)list_next(baja);
    list_popFront(baja);
  }
}

// Menú principal
void mostrarMenuPrincipal() {
  limpiarPantalla();
  puts("========================================");
  puts("     Sistema de Gestión Hospitalaria");
  puts("========================================");

  puts("1) Registrar paciente");
  puts("2) Asignar prioridad a paciente");
  puts("3) Mostrar lista de espera");
  puts("4) Atender al siguiente paciente");
  puts("5) Mostrar pacientes por prioridad");
  puts("6) Salir");
}

void registrar_paciente(List *pacientes)
{
  limpiarPantalla();
  printf("========================================\n");
  printf("       Registrar nuevo paciente\n");
  printf("========================================\n\n");
  paciente *temp = (paciente *)malloc(sizeof(paciente));
  if (temp == NULL)
  {
    printf("No hay suficiente memoria disponiblen\n");
    printf("Finalizando ejecucion del programa");
    exit(EXIT_FAILURE);
  }

  getchar();
  printf("Ingrese el nombre del paciente: ");
  scanf("%50[^\n]s", temp->nombre);
  convertirMayusculas(temp->nombre);
  getchar();

  printf("Ingrese la edad del paciente: ");
  scanf("%hd", &temp->edad);
  convertirMayusculas(temp->sintoma);
  getchar();

  printf("Ingrese el síntoma del paciente: ");
  scanf("%50[^\n]s", temp->sintoma);

  printf("Prioridad asignada por defecto: BAJA\n");
  strcpy(temp->prioridad, "BAJA");

  paciente *aux = (paciente *)list_first(pacientes);
  list_pushBack(pacientes, temp);
  printf("\n***Paciente registrado con exito***\n");
}

void asignar_prioridad(List *pacientes)
{
  char nombre[MAXCHAR];
  char prioridad[MAXCHAR];
  paciente *infoPaciente = list_first(pacientes);

  limpiarPantalla();
  printf("========================================\n");
  printf("    Cambio de prioridad de paciente\n");
  printf("========================================\n\n");
  if (infoPaciente == NULL)
  {
    printf("No se han registrado pacientes en la lista de espera\n");
    return;
  }

  getchar();
  printf("Ingrese el nombre del paciente a cambiar prioridad: ");
  scanf("%50[^\n]s", nombre);
  convertirMayusculas(nombre);
  getchar();
  do
  {
    printf("Ingrese la nueva prioridad (Alta, Media, Baja): ");
    scanf("%50[^\n]s", prioridad);
    convertirMayusculas(prioridad);
  } while ((strcmp(prioridad, "ALTA") != 0) &&
           (strcmp(prioridad, "MEDIA") != 0) &&
           (strcmp(prioridad, "BAJA") != 0));

  while (infoPaciente != NULL)
  {
    if (strcmp(infoPaciente->nombre, nombre) == 0)
    {
      strcpy(infoPaciente->prioridad, prioridad);
      printf("Se ha cambiado exitosamente la prioridad de %s\n", infoPaciente->nombre);
      ordenarPorPrioridad(pacientes);
      return;
    }
    infoPaciente = (paciente *)list_next(pacientes);
  }

  printf("\nNo se ha encontrado paciente en la lista de espera\n");
}

void mostrar_lista_pacientes(List *pacientes)
{
  paciente *infoPaciente = list_first(pacientes);
  
  limpiarPantalla();
  printf("========================================\n");
  printf("          Lista de pacientes\n");
  printf("========================================\n\n");
  if (infoPaciente == NULL)
  {
    printf("No se han registrado pacientes en la lista de espera\n");
    return;
  }
  
  // Mostrar pacientes en la cola de espera
  printf("Pacientes en espera: \n");
  // Aquí implementarías la lógica para recorrer y mostrar los pacientes
  while (infoPaciente != NULL)
  {
    printf("  Nombre: %s\n", infoPaciente->nombre);
    printf("  Edad: %hd\n", infoPaciente->edad);
    printf("  Sintomas: %s\n", infoPaciente->sintoma);
    printf("  Prioridad: %s\n\n", infoPaciente->prioridad);
    infoPaciente = list_next(pacientes);
  }
}

void AtenderSiguientePaciente(List *pacientes)
{
  paciente *infoPaciente = list_first(pacientes);
  if (infoPaciente == NULL)
  {
    printf("\nNo se han registrado pacientes en la lista de espera\n");
    return;
  }

  list_popFront(pacientes);
  printf("\n\nSe ha atendido a: \n");
  printf("  Nombre: %s\n", infoPaciente->nombre);
  printf("  Edad: %hd\n", infoPaciente->edad);
  printf("  Sintomas: %s\n", infoPaciente->sintoma);
  printf("  Prioridad: %s\n\n", infoPaciente->prioridad);

  if (list_first(pacientes) == NULL)
    printf("Se han atentido todos los pacientes registro en la lista de espera");
}

void mostrarPacientesPorPrioridad(List *pacientes)
{
  paciente *infoPaciente = list_first(pacientes);

  limpiarPantalla();
  printf("========================================\n");
  printf("      Ver pacientes por prioridad\n");
  printf("========================================\n\n");
  if (infoPaciente == NULL)
  {
    printf("No se han registrado pacientes a la lista de espera\n");
    return;
  }
  char prioridad[MAXCHAR];
  size_t contador = 0;
  
  do
  {
    printf("Ingrese que quiere ver(Alta, Media, Baja): ");
    getchar();
    scanf("%50[^\n]s", prioridad);
    convertirMayusculas(prioridad);
  } while ((strcmp(prioridad, "ALTA") != 0) &&
          (strcmp(prioridad, "MEDIA") != 0) &&
          (strcmp(prioridad, "BAJA") != 0));

  printf("\nPacientes de prioridad %s en espera: \n", prioridad);
  while (infoPaciente != NULL)
  {
    if (strcmp(infoPaciente->prioridad, prioridad) == 0)
    {
      printf("  Nombre: %s\n", infoPaciente->nombre);
      printf("  Edad: %hd\n", infoPaciente->edad);
      printf("  Sintomas: %s\n", infoPaciente->sintoma);
      printf("  Prioridad: %s\n\n", infoPaciente->prioridad);
      contador++;
    }
    infoPaciente = list_next(pacientes);
  }

  if (contador == 0)
    printf("\nNo se han encontrado pacientes con la prioridad %s", prioridad);
}

int main() {
  char opcion;
  List *pacientes = list_create(); // puedes usar una lista para gestionar los pacientes
  if (pacientes == NULL)
  {
    printf("No hay memoria disponible para iniciar la lista de espera\n");
    printf("Se finalizara el programa");
    exit(EXIT_FAILURE);
  }

  do {
    mostrarMenuPrincipal();
    printf("Ingrese su opción: ");
    scanf(" %c", &opcion); // Nota el espacio antes de %c para consumir el
                           // newline anterior

    switch (opcion) {
    case '1':
      registrar_paciente(pacientes);
      break;
    case '2':
      // Lógica para asignar prioridad
      asignar_prioridad(pacientes);
      break;
    case '3':
      mostrar_lista_pacientes(pacientes);
      break;
    case '4':
      // Lógica para atender al siguiente paciente
      AtenderSiguientePaciente(pacientes);
      break;
    case '5':
      // Lógica para mostrar pacientes por prioridad
      mostrarPacientesPorPrioridad(pacientes);
      break;
    case '6':
      puts("\nSaliendo del sistema de gestión hospitalaria...");
      break;
    default:
      puts("\nOpción no válida. Por favor, intente de nuevo.");
    }
    presioneTeclaParaContinuar();

  } while (opcion != '6');

  // Liberar recursos, si es necesario
  list_clean(pacientes);

  return 0;
}