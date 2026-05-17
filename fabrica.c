#include "fabrica.h"

char  nombres[MAX_PRODUCTOS][MAX_NOMBRE];
int   cantidades[MAX_PRODUCTOS];
float tiempos[MAX_PRODUCTOS];
float recursos[MAX_PRODUCTOS];
int   totalProductos = 0;

float tiempoDisponible    = 200.00;
float recursosDisponibles = 5000.00;



/* Limpia todo lo que quedo en el buffer de entrada hasta el siguiente
   salto de linea o fin de archivo. Siempre llamar despues de scanf. */
void limpiarBuffer(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

/* Devuelve 1 si la cadena tiene solo letras y espacios, 0 si no.
   Tambien rechaza cadenas vacias (largo 0). */
int solo_Letras(char *nombre) {
    int i;
    if (nombre[0] == '\0') {      /* cadena vacia → invalida */
        return 0;
    }
    for (i = 0; nombre[i] != '\0'; i++) {
        if (!isalpha((unsigned char)nombre[i]) && nombre[i] != ' ') {
            return 0;
        }
    }
    return 1;
}

/*
   Si el usuario escribe letras, descarta la linea y pide de nuevo.
   Devuelve el entero leido. */
int leerEntero(void) {
    int valor;
    int leido;
    /* scanf devuelve la cantidad de valores que leyo correctamente.
       Si no leyo 1 entero (escribieron letras), limpia y reintenta. */
    while ((leido = scanf("%d", &valor)) != 1) {
        limpiarBuffer();
        printf("  Error: debe ingresar un numero entero. Intente de nuevo: ");
    }
    limpiarBuffer();
    return valor;
}


float leerFlotante(void) {
    float valor;
    int   leido;
    while ((leido = scanf("%f", &valor)) != 1) {
        limpiarBuffer();
        printf("  Error: debe ingresar un numero. Intente de nuevo: ");
    }
    limpiarBuffer();
    return valor;
}




int buscarProducto(char nombres[][MAX_NOMBRE], int num_prod,
                   char *terminoBusqueda) {
    int i;
    for (i = 0; i < num_prod; i++) {
        if (strcmp(nombres[i], terminoBusqueda) == 0) {
            return i;
        }
    }
    return -1;
}


void ingresarProductos(void) {
    int i;
    int nuevos;

    printf("\n=== INGRESO DE PRODUCTOS ===\n");

    if (totalProductos >= MAX_PRODUCTOS) {
        printf("Error: ya se alcanzo el limite de %d productos.\n",
               MAX_PRODUCTOS);
        return;
    }

    printf("Cuantos productos desea ingresar? (max %d disponibles): ",
           MAX_PRODUCTOS - totalProductos);

    nuevos = leerEntero();  /* usa leerEntero: no se rompe con letras */

    while (nuevos < 1 || nuevos > MAX_PRODUCTOS - totalProductos) {
        printf("  Valor invalido. Ingrese entre 1 y %d: ",
               MAX_PRODUCTOS - totalProductos);
        nuevos = leerEntero();
    }

    for (i = totalProductos; i < totalProductos + nuevos; i++) {
        printf("\n-- Producto %d --\n", i + 1);

       
        printf("Nombre (solo letras y espacios): ");
        fgets(nombres[i], MAX_NOMBRE, stdin);
        nombres[i][strcspn(nombres[i], "\n")] = '\0';

        while (!solo_Letras(nombres[i])) {
            printf("  Error: solo letras y espacios, sin numeros ni simbolos.\n");
            printf("  Ingrese el nombre nuevamente: ");
            fgets(nombres[i], MAX_NOMBRE, stdin);
            nombres[i][strcspn(nombres[i], "\n")] = '\0';
        }

        printf("Cantidad demandada (1 - 1000 unidades): ");
        cantidades[i] = leerEntero();

        while (cantidades[i] < 1 || cantidades[i] > 1000) {
            printf("  Error: ingrese un valor entre 1 y 1000: ");
            cantidades[i] = leerEntero();
        }
        printf("Tiempo de fabricacion por unidad (1 - 1000 horas): ");
        tiempos[i] = leerFlotante();

        while (tiempos[i] <= 0.00 || tiempos[i] > 1000.00) {
            printf("  Error: ingrese un valor entre 1 y 1000: ");
            tiempos[i] = leerFlotante();
        }
        printf("Recursos por unidad (1 - 1000): ");
        recursos[i] = leerFlotante();

        while (recursos[i] <= 0.00 || recursos[i] > 1000.00) {
            printf("  Error: ingrese un valor entre 1 y 1000: ");
            recursos[i] = leerFlotante();
        }
    }

    totalProductos += nuevos;
    printf("\nProductos ingresados correctamente!\n");
}

void mostrarProductos(void) {
    int i;

    if (totalProductos == 0) {
        printf("\nNo hay productos registrados.\n");
        return;
    }

    printf("\n=== LISTA DE PRODUCTOS ===\n");
    printf("%-4s %-20s %-10s %-12s %-10s\n",
           "#", "Nombre", "Cantidad", "Tiempo/u(h)", "Recursos/u");
    printf("----------------------------------------------------------\n");

    for (i = 0; i < totalProductos; i++) {
        printf("%-4d %-20s %-10d %-12.2f %-10.2f\n",
               i + 1, nombres[i], cantidades[i], tiempos[i], recursos[i]);
    }
}

void calcularResultados(int *cant, float *tiempos_p, float *recursos_p,
                        int num_prod, float limit_t, float limit_r) {
    int   i;
    float tiempoTotal   = 0.00;
    float recursosTotal = 0.00;

    if (num_prod == 0) {
        printf("\nNo hay productos para calcular.\n");
        return;
    }

    printf("\n=== RESULTADOS DE PRODUCCION ===\n");
    printf("%-20s %-14s %-12s\n", "Producto", "Tiempo (h)", "Recursos");
    printf("----------------------------------------------------------\n");

    for (i = 0; i < num_prod; i++) {
        float tiempoProd   = *(tiempos_p  + i) * *(cant + i);
        float recursosProd = *(recursos_p + i) * *(cant + i);

        printf("%-20s %-14.2f %-12.2f\n",
               nombres[i], tiempoProd, recursosProd);

        tiempoTotal   += tiempoProd;
        recursosTotal += recursosProd;
    }

    printf("----------------------------------------------------------\n");
    printf("TIEMPO TOTAL REQUERIDO   : %.2f horas\n",    tiempoTotal);
    printf("RECURSOS TOTALES         : %.2f unidades\n", recursosTotal);
    printf("\n--- Evaluacion de capacidad ---\n");
    printf("Tiempo disponible   : %.2f h -> %s\n",
           limit_t,
           (tiempoTotal <= limit_t)
               ? "OK - dentro del limite"
               : "EXCEDE el limite");
    printf("Recursos disponibles: %.2f   -> %s\n",
           limit_r,
           (recursosTotal <= limit_r)
               ? "OK - dentro del limite"
               : "EXCEDE el limite");

    if (tiempoTotal <= limit_t && recursosTotal <= limit_r) {
        printf("\nLa fabrica PUEDE cumplir con la demanda.\n");
    } else {
        printf("\nLa fabrica NO puede cumplir con la demanda actual.\n");
    }
}

void editarProducto(void) {
    char  nombre[MAX_NOMBRE];
    char  nuevoNombre[MAX_NOMBRE];
    int   idx;
    int   nuevaCant;
    float nuevoTiempo;
    float nuevosRec;

    printf("\n=== EDITAR PRODUCTO ===\n");
    printf("Nombre del producto a editar: ");
    fgets(nombre, MAX_NOMBRE, stdin);
    nombre[strcspn(nombre, "\n")] = '\0';

    /* Validar que lo que escribio para buscar tenga solo letras */
    if (!solo_Letras(nombre)) {
        printf("  Nombre invalido: solo letras y espacios.\n");
        return;
    }

    idx = buscarProducto(nombres, totalProductos, nombre);

    if (idx == -1) {
        printf("Producto '%s' no encontrado.\n", nombre);
        return;
    }

    printf("Producto encontrado: %s\n", nombres[idx]);

    printf("Nuevo nombre (Enter para conservar '%s'): ", nombres[idx]);
    fgets(nuevoNombre, MAX_NOMBRE, stdin);
    nuevoNombre[strcspn(nuevoNombre, "\n")] = '\0';

    if (nuevoNombre[0] != '\0') {
        /* Solo actualiza si paso la validacion de letras */
        while (!solo_Letras(nuevoNombre)) {
            printf("  Error: solo letras y espacios.\n");
            printf("  Nuevo nombre (Enter para conservar '%s'): ",
                   nombres[idx]);
            fgets(nuevoNombre, MAX_NOMBRE, stdin);
            nuevoNombre[strcspn(nuevoNombre, "\n")] = '\0';
            /* Si presiona Enter de nuevo, conserva el original */
            if (nuevoNombre[0] == '\0') break;
        }
        if (nuevoNombre[0] != '\0') {
            strcpy(nombres[idx], nuevoNombre);
        }
    }

    printf("Nueva cantidad (0 para conservar %d): ", cantidades[idx]);
    nuevaCant = leerEntero();

    while (nuevaCant < 0 || nuevaCant > 1000) {
        printf("  Error: ingrese entre 0 y 1000 (0 = conservar): ");
        nuevaCant = leerEntero();
    }
    if (nuevaCant > 0) cantidades[idx] = nuevaCant;

    printf("Nuevo tiempo/unidad (0 para conservar %.2f): ", tiempos[idx]);
    nuevoTiempo = leerFlotante();

    while (nuevoTiempo < 0.00 || nuevoTiempo > 1000.00) {
        printf("  Error: ingrese entre 0 y 1000 (0 = conservar): ");
        nuevoTiempo = leerFlotante();
    }
    if (nuevoTiempo > 0.00) tiempos[idx] = nuevoTiempo;

    printf("Nuevos recursos/unidad (0 para conservar %.2f): ", recursos[idx]);
    nuevosRec = leerFlotante();

    while (nuevosRec < 0.00 || nuevosRec > 1000.00) {
        printf("  Error: ingrese entre 0 y 1000 (0 = conservar): ");
        nuevosRec = leerFlotante();
    }
    if (nuevosRec > 0.00) recursos[idx] = nuevosRec;

    printf("Producto actualizado correctamente.\n");
}

void eliminarProducto(char nombres[][MAX_NOMBRE], int *cant,
                      float *tiempos_p, float *recursos_p,
                      int *num_prod, int indice) {
    int i;
    for (i = indice; i < *num_prod - 1; i++) {
        strcpy(nombres[i], nombres[i + 1]);
        *(cant       + i) = *(cant       + i + 1);
        *(tiempos_p  + i) = *(tiempos_p  + i + 1);
        *(recursos_p + i) = *(recursos_p + i + 1);
    }
    (*num_prod)--;
}


void ajustarLimites(void) {
    printf("\n=== AJUSTAR LIMITES DE LA FABRICA ===\n");

    printf("Nuevo tiempo disponible (actual %.2f h): ", tiempoDisponible);
    tiempoDisponible = leerFlotante();

    while (tiempoDisponible <= 0.00) {
        printf("  Error: debe ser mayor a 0: ");
        tiempoDisponible = leerFlotante();
    }

    printf("Nuevos recursos disponibles (actual %.2f): ", recursosDisponibles);
    recursosDisponibles = leerFlotante();

    while (recursosDisponibles <= 0.00) {
        printf("  Error: debe ser mayor a 0: ");
        recursosDisponibles = leerFlotante();
    }

    printf("Limites actualizados correctamente.\n");
}
