#include "fabrica.h"

int main(void) {
    int  opcion;
    char buscar[MAX_NOMBRE];
    int  idx;

    printf("==========================================================\n");
    printf("  SISTEMA DE OPTIMIZACION DE PRODUCCION - FABRICA RC1\n");
    printf("==========================================================\n");

    do {
        printf("\n--- MENU PRINCIPAL ---\n");
        printf("1. Ingresar productos\n");
        printf("2. Mostrar productos\n");
        printf("3. Calcular resultados de produccion\n");
        printf("4. Editar un producto\n");
        printf("5. Eliminar un producto\n");
        printf("6. Ajustar limites de la fabrica\n");
        printf("7. Salir\n");
        printf("Opcion: ");

        /* leerEntero() no se rompe si escriben letras como "abc" */
        opcion = leerEntero();

        switch (opcion) {
            case 1:
                ingresarProductos();
                break;

            case 2:
                mostrarProductos();
                break;

            case 3:
                calcularResultados(cantidades, tiempos, recursos,
                                   totalProductos,
                                   tiempoDisponible,
                                   recursosDisponibles);
                break;

            case 4:
                editarProducto();
                break;

            case 5:
                if (totalProductos == 0) {
                    printf("\nNo hay productos registrados para eliminar.\n");
                    break;
                }
                printf("\n=== ELIMINAR PRODUCTO ===\n");
                printf("Nombre del producto a eliminar: ");
                fgets(buscar, MAX_NOMBRE, stdin);
                buscar[strcspn(buscar, "\n")] = '\0';

                /* Validar que el termino de busqueda tenga solo letras */
                if (!solo_Letras(buscar)) {
                    printf("  Nombre invalido: solo letras y espacios.\n");
                    break;
                }

                idx = buscarProducto(nombres, totalProductos, buscar);

                if (idx != -1) {
                    eliminarProducto(nombres, cantidades, tiempos,
                                     recursos, &totalProductos, idx);
                    printf("Producto '%s' eliminado correctamente.\n", buscar);
                } else {
                    printf("Producto '%s' no encontrado.\n", buscar);
                }
                break;

            case 6:
                ajustarLimites();
                break;

            case 7:
                printf("Saliendo del programa...\n");
                break;

            default:
                printf("Opcion no valida. Ingrese un numero entre 1 y 7.\n");
        }
    } while (opcion != 7);

    return 0;
}
