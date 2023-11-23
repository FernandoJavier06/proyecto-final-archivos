#include <stdio.h>
#include <mysql/mysql.h>
#include "datos_conexion.h"
#include <string.h>

typedef struct Pelicula
{
    int id;
    char nombre[45];
    char descripcion[150];
    char idioma[45];
    char duracion[10];
    char fechaEstreno[15];
} Pelicula;

typedef struct Cliente
{
    unsigned int nit;
    char nombre[45];
    char apellido[45];
    unsigned int edad;
    char telefono[8];
    char correo[45];
} Cliente;

typedef struct Boleto
{
    unsigned int id;
    unsigned int idPelicula;
    unsigned int nitCliente;
    char sala[45];
    char asiento[45];
    double precio;
    char hora[10];
    char fecha[15];
} Boleto;

// Sentencias preparadas para ejecutarlas las veces que se quiera
#define QUERY_INSERTAR_PELICULA "INSERT INTO Pelicula(nombre,descripcion,idioma,duracion,fechaEstreno) VALUES(?,?,?,?,?)"
#define QUERY_INSERTAR_CLIENTE "INSERT INTO Cliente(nit,nombre,apellido,edad,telefono,correo) VALUES(?,?,?,?,?,?)"
#define QUERY_INSERTAR_BOLETO "INSERT INTO Boleto(idPelicula,nitCliente,sala,asiento,precio,hora,fecha) VALUES(?,?,?,?,?,?,?)"

#define QUERY_LEER_TODO_PELICULA "SELECT * FROM Pelicula"
#define QUERY_LEER_PELICULA "SELECT * FROM Pelicula where id = ?"
#define QUERY_LEER_TODO_ClIENTE "SELECT * FROM Cliente"
#define QUERY_LEER_ClIENTE "SELECT * FROM Cliente where nit = ?"
#define QUERY_LEER__TODO_BOLETO "SELECT * FROM Boleto"
#define QUERY_LEER_BOLETO "SELECT * FROM Boleto where id = ?"

#define QUERY_ACTUALIZAR_PELICULA "UPDATE Pelicula SET nombre = ? WHERE id = ?"
#define QUERY_ACTUALIZAR_CLIENTE "UPDATE Cliente SET correo = ? WHERE nit = ? "
#define QUERY_ACTUALIZAR_BOLETO "UPDATE Boleto SET asiento = ? WHERE id = ? "

#define QUERY_ELIMINAR_PELICULA "DELETE FROM Pelicula WHERE id = ?"
#define QUERY_ELIMINAR_CLIENTE "DELETE FROM Cliente WHERE nit = ?"
#define QUERY_ELIMINAR_BOLETO "DELETE FROM Boleto WHERE id = ?"

int conectar(MYSQL **conexion);

void ejecutarSentencia(MYSQL_STMT *stmt);

void asignarBindPelicula();

void asignarBindCliente();

void asignarBindBoleto();

int menu(int *opMenu, int *opTabla, char *tabla, int *opQuery);

void seleccionarTabla(int *opTabla, char *tabla);

void seleccionarVista(int *opQuery);

void seleccionarActualizacion(int *opTabla);

void limpiarBuffer();

Pelicula pelicula;
Cliente cliente;
Boleto boleto;
unsigned long length[10];
int opMenus = 0;

MYSQL *conexion;
MYSQL_STMT *stmt;
MYSQL_BIND bind[10];

MYSQL_RES *res_ptr;
MYSQL_FIELD *campo;
MYSQL_ROW res_fila;

int main(int argc, char const *argv[])
{
    int opTabla = 0, opQuery = 0, error_conexion, error_consulta, filas, columnas, i, j, telefono;
    char *tabla = (char *)malloc(sizeof(char) * 8);
    bool is_null[10];

    error_conexion = conectar(&conexion);

    if (!error_conexion)
    {
        stmt = mysql_stmt_init(conexion);
        if (stmt)
        {
            do
            {

                menu(&opMenus, &opTabla, tabla, &opQuery);

                system("clear");
                switch (opMenus)
                {
                case 1: // INSERTAR

                    printf("-------------- Insertar registro --------------\n");
                    memset(bind, 0, sizeof(bind));

                    if (opTabla == 1)
                    {
                        if (!mysql_stmt_prepare(stmt, QUERY_INSERTAR_PELICULA, strlen(QUERY_INSERTAR_PELICULA)))
                        {
                            asignarBindPelicula();

                            if (!mysql_stmt_bind_param(stmt, bind))
                            {
                                printf("Ingrese el nombre de la pelicula:\n");
                                limpiarBuffer();
                                fgets(pelicula.nombre, 44, stdin);
                                pelicula.nombre[strlen(pelicula.nombre) - 1] = '\0';
                                length[0] = strlen(pelicula.nombre);
                                printf("Ingrese una descripcion:\n");
                                // limpiarBuffer();
                                fgets(pelicula.descripcion, 149, stdin);
                                pelicula.descripcion[strlen(pelicula.descripcion) - 1] = '\0';
                                length[1] = strlen(pelicula.descripcion);
                                printf("Ingrese el idioma:\n");
                                scanf("%s", pelicula.idioma);
                                length[2] = strlen(pelicula.idioma);
                                printf("Ingrese la duracion:\n");
                                scanf("%s", pelicula.duracion);
                                length[3] = strlen(pelicula.duracion);
                                printf("Ingrese la fecha de estreno:\n");
                                scanf("%s", pelicula.fechaEstreno);
                                length[4] = strlen(pelicula.fechaEstreno);

                                if (!mysql_stmt_execute(stmt))
                                {
                                    if (mysql_stmt_affected_rows(stmt) > 0)
                                        printf("\nRegistro insertado exitosamente\n");
                                    else
                                        printf("\nError al insertar registro\n");
                                }
                                else
                                    printf("No se corrio la sentencia");
                            }
                            else
                                printf("Error al asociar los parametros a la sentencia preparada.\n");
                        }
                        else
                            printf("Error al preparar la sentencia.\n");
                    }

                    else if (opTabla == 2)
                    {

                        if (!mysql_stmt_prepare(stmt, QUERY_INSERTAR_CLIENTE, strlen(QUERY_INSERTAR_CLIENTE)))
                        {
                            asignarBindCliente();

                            if (!mysql_stmt_bind_param(stmt, bind))
                            {
                                printf("Ingrese el nit:\n");
                                scanf("%d", &cliente.nit);
                                printf("Ingrese el nombre:\n");
                                scanf("%s", cliente.nombre);
                                length[0] = strlen(cliente.nombre);
                                printf("Ingrese el apellido:\n");
                                scanf("%s", cliente.apellido);
                                length[1] = strlen(cliente.apellido);
                                printf("Ingrese la edad:\n");
                                scanf("%d", &cliente.edad);
                                printf("Ingrese el telefono:\n");
                                scanf("%s", cliente.telefono);
                                length[2] = strlen(cliente.telefono);
                                printf("Ingrese el correo:\n");
                                scanf("%s", cliente.correo);
                                length[3] = strlen(cliente.correo);

                                if (!mysql_stmt_execute(stmt))
                                {
                                    if (mysql_stmt_affected_rows(stmt) > 0)
                                        printf("\nRegistro insertado exitosamente\n");
                                    else
                                        printf("\nError al insertar registro\n");
                                }
                            }
                            else
                                printf("Error al asociar los parametros a la sentencia preparada.\n");
                        }
                        else
                            printf("Error al preparar la sentencia.\n");
                    }

                    else if (opTabla == 3)
                    {
                        if (!mysql_stmt_prepare(stmt, QUERY_INSERTAR_BOLETO, strlen(QUERY_INSERTAR_BOLETO)))
                        {
                            asignarBindBoleto();

                            if (!mysql_stmt_bind_param(stmt, bind))
                            {
                                printf("Ingrese el id de la pelicula:\n");
                                scanf("%d", &boleto.idPelicula);
                                printf("Ingrese el nit del cliente:\n");
                                scanf("%d", &boleto.nitCliente);
                                printf("Ingrese la sala:\n");
                                scanf("%s", boleto.sala);
                                length[0] = strlen(boleto.sala);
                                printf("Ingrese el asiento:\n");
                                scanf("%s", boleto.asiento);
                                length[1] = strlen(boleto.asiento);
                                printf("Ingrese el precio:\n");
                                scanf("%lf", &boleto.precio);
                                printf("Ingrese la hora:\n");
                                scanf("%s", boleto.hora);
                                length[2] = strlen(boleto.hora);
                                printf("Ingrese la fecha:\n");
                                scanf("%s", boleto.fecha);
                                length[3] = strlen(boleto.fecha);

                                if (!mysql_stmt_execute(stmt))
                                {
                                    if (mysql_stmt_affected_rows(stmt) > 0)
                                        printf("\nRegistro insertado exitosamente\n");
                                    else
                                        printf("\nError al insertar registro\n");
                                }
                            }
                            else
                                printf("Error al asociar los parametros a la sentencia preparada");
                        }
                        else
                            printf("Error al preparar la sentencia.\n");
                    }

                    break;

                case 2: // BUSCAR
                    printf("------------------------------------------------------------------------------------------ Consultar ------------------------------------------------------------------------------------------\n\n");
                    memset(bind, 0, sizeof(bind));

                    switch (opQuery)
                    {
                    case 1:
                        bind[0].buffer_type = MYSQL_TYPE_LONG;
                        bind[0].buffer = (char *)&pelicula.id;
                        bind[0].is_null = 0;
                        bind[0].length = 0;

                        printf("Introduzca el id de la pelicula: ");
                        scanf("%d", &pelicula.id);
                        error_consulta = mysql_stmt_prepare(stmt, QUERY_LEER_PELICULA, strlen(QUERY_LEER_PELICULA));
                        break;
                    case 2:

                        bind[0].buffer_type = MYSQL_TYPE_LONG;
                        bind[0].buffer = (char *)&cliente.nit;
                        bind[0].is_null = 0;
                        bind[0].length = 0;

                        printf("Introduzca el nit del cliente: ");
                        scanf("%d", &cliente.nit);
                        error_consulta = mysql_stmt_prepare(stmt, QUERY_LEER_ClIENTE, strlen(QUERY_LEER_ClIENTE));
                        break;
                    case 3:
                        bind[0].buffer_type = MYSQL_TYPE_LONG;
                        bind[0].buffer = (char *)&boleto.id;
                        bind[0].is_null = 0;
                        bind[0].length = 0;

                        printf("Introduzca el id del boleto: ");
                        scanf("%d", &boleto.id);
                        error_consulta = mysql_stmt_prepare(stmt, QUERY_LEER_BOLETO, strlen(QUERY_LEER_BOLETO));
                        break;
                    case 4:
                        error_consulta = mysql_stmt_prepare(stmt, QUERY_LEER_TODO_PELICULA, strlen(QUERY_LEER_TODO_PELICULA));
                        break;
                    case 5:
                        error_consulta = mysql_stmt_prepare(stmt, QUERY_LEER_TODO_ClIENTE, strlen(QUERY_LEER_TODO_ClIENTE));
                        break;
                    case 6:
                        error_consulta = mysql_stmt_prepare(stmt, QUERY_LEER__TODO_BOLETO, strlen(QUERY_LEER__TODO_BOLETO));
                        break;
                    default:
                        error_consulta = 1;
                        printf("Opcion incorrecta.\n");
                        break;
                    }

                    if (!error_consulta)
                    {

                        if (!mysql_stmt_bind_param(stmt, bind))
                        {

                            if (!mysql_stmt_execute(stmt))
                            {
                                res_ptr = mysql_stmt_result_metadata(stmt);

                                if (res_ptr)
                                {
                                    columnas = mysql_num_fields(res_ptr);

                                    memset(bind, 0, sizeof(bind));

                                    if (opQuery == 1 || opQuery == 4)
                                    {
                                        asignarBindPelicula();

                                        if (!mysql_stmt_bind_result(stmt, bind))
                                        {
                                            if (!mysql_stmt_store_result(stmt))
                                            {
                                                while (campo = mysql_fetch_field(res_ptr))
                                                {
                                                    printf("%23s", campo->name);
                                                }
                                                printf("\n");

                                                while (!mysql_stmt_fetch(stmt))
                                                {
                                                    printf("%22d %22s %22s %22s %22s %22s\n", pelicula.id, pelicula.nombre,
                                                           "NULL", pelicula.idioma, pelicula.duracion, pelicula.fechaEstreno);
                                                }
                                                printf("\n");
                                            }
                                            else
                                                printf("Error en el mysql_stmt_store_result(): %s\n", mysql_stmt_error(stmt));
                                        }
                                        else
                                            printf("Error al cargar los resultados con el bind\n");
                                    }
                                    else if (opQuery == 2 || opQuery == 5)
                                    {

                                        asignarBindCliente();
                                        bind[4].buffer_type = MYSQL_TYPE_LONG;
                                        bind[4].buffer = (char *)&telefono;
                                        bind[4].is_null = 0;
                                        bind[4].length = 0;

                                        if (!mysql_stmt_bind_result(stmt, bind))
                                        {
                                            if (!mysql_stmt_store_result(stmt))
                                            {
                                                while (campo = mysql_fetch_field(res_ptr))
                                                {
                                                    printf("%23s", campo->name);
                                                }
                                                printf("\n");

                                                while (!mysql_stmt_fetch(stmt))
                                                {
                                                    printf("%22d %22s %22s %22d %22d %22s\n", cliente.nit, cliente.nombre,
                                                           cliente.apellido, cliente.edad, telefono, cliente.correo);
                                                }
                                                printf("\n");
                                            }
                                            else
                                                printf("Error en el mysql_stmt_store_result(): %s\n", mysql_stmt_error(stmt));
                                        }
                                        else
                                            printf("Error al cargar los resultados con el bind\n");
                                    }
                                    else if (opQuery == 3 || opQuery == 6)
                                    {
                                        asignarBindBoleto();
                                        if (!mysql_stmt_bind_result(stmt, bind))
                                        {
                                            if (!mysql_stmt_store_result(stmt))
                                            {
                                                while (campo = mysql_fetch_field(res_ptr))
                                                {
                                                    printf("%23s", campo->name);
                                                }
                                                printf("\n");

                                                while (!mysql_stmt_fetch(stmt))
                                                {
                                                    printf("%22d %22d %22d %22s %22s %22.2lf %22s %22s\n", boleto.id, boleto.idPelicula,
                                                           boleto.nitCliente, boleto.sala, boleto.asiento, boleto.precio,
                                                           boleto.hora, boleto.fecha);
                                                }
                                                printf("\n");
                                            }
                                            else
                                                printf("Error en el mysql_stmt_store_result(): %s\n", mysql_stmt_error(stmt));
                                        }
                                        else
                                            printf("Error al cargar los resultados con el bind\n");
                                    }
                                }
                                else
                                    printf("Error al obtener resultados.\n");
                            }
                            else
                                printf("Error al ejecutar la sentencia.\n");
                        }
                        else
                            printf("Error al asociar los parametros a la sentencia preparada.\n");
                    }
                    else
                        printf("Error al preparar la sentencia.\n");

                    break;

                case 3: // ACTUALIZAR
                    printf("-------------- Actualizar registro --------------\n\n");
                    memset(bind, 0, sizeof(bind));

                    if (opTabla == 1)
                    {
                        bind[0].buffer_type = MYSQL_TYPE_STRING;
                        bind[0].buffer = (char *)pelicula.nombre;
                        bind[0].buffer_length = sizeof(pelicula.nombre);
                        bind[0].is_null = 0;
                        bind[0].length = &length[0];

                        bind[1].buffer_type = MYSQL_TYPE_LONG;
                        bind[1].buffer = (char *)&pelicula.id;
                        bind[1].is_null = 0;
                        bind[1].length = 0;

                        printf("Ingrese el id de la pelicula: ");
                        scanf("%d", &pelicula.id);
                        printf("Ingrese el nuevo nombre de la pelicula: ");
                        limpiarBuffer();
                        fgets(pelicula.nombre, 44, stdin);
                        pelicula.nombre[strlen(pelicula.nombre) - 1] = '\0';
                        length[0] = strlen(pelicula.nombre);
                        error_consulta = mysql_stmt_prepare(stmt, QUERY_ACTUALIZAR_PELICULA, strlen(QUERY_ACTUALIZAR_PELICULA));
                    }
                    else if (opTabla == 2)
                    {
                        bind[0].buffer_type = MYSQL_TYPE_STRING;
                        bind[0].buffer = (char *)cliente.correo;
                        bind[0].buffer_length = sizeof(cliente.correo);
                        bind[0].is_null = 0;
                        bind[0].length = &length[0];

                        bind[1].buffer_type = MYSQL_TYPE_LONG;
                        bind[1].buffer = (char *)&cliente.nit;
                        bind[1].is_null = 0;
                        bind[1].length = 0;

                        printf("Ingrese el nit del cliente: ");
                        scanf("%d", &cliente.nit);
                        printf("Ingrese el nuevo correo del cliente: ");
                        scanf("%s", cliente.correo);
                        length[0] = strlen(cliente.correo);
                        error_consulta = mysql_stmt_prepare(stmt, QUERY_ACTUALIZAR_CLIENTE, strlen(QUERY_ACTUALIZAR_CLIENTE));
                    }
                    else if (opTabla == 3)
                    {
                        bind[0].buffer_type = MYSQL_TYPE_STRING;
                        bind[0].buffer = (char *)&boleto.asiento;
                        bind[0].buffer_length = sizeof(boleto.asiento);
                        bind[0].is_null = 0;
                        bind[0].length = &length[0];

                        bind[1].buffer_type = MYSQL_TYPE_LONG;
                        bind[1].buffer = (char *)&boleto.id;
                        bind[1].is_null = 0;
                        bind[1].length = 0;

                        printf("Ingrese el id del boleto: ");
                        scanf("%d", &boleto.id);
                        printf("Ingrese el nuevo asiento: ");
                        scanf("%s", boleto.asiento);
                        length[0] = strlen(boleto.asiento);
                        error_consulta = mysql_stmt_prepare(stmt, QUERY_ACTUALIZAR_BOLETO, strlen(QUERY_ACTUALIZAR_BOLETO));
                    }
                    else
                    {
                        error_consulta = 1;
                        printf("Opcion incorrecta.\n");
                    }

                    if (!error_consulta)
                    {
                        if (!mysql_stmt_bind_param(stmt, bind))
                        {
                            if (!mysql_stmt_execute(stmt))
                            {
                                if (mysql_stmt_affected_rows(stmt) > 0)
                                    printf("Registro actualizado con éxito.\n");
                                else
                                    printf("No se pudo actualizar el registro o no existe.\n");
                            }
                            else
                                printf("Error al ejecutar la sentencia.\n");
                        }
                        else
                            printf("Error al asociar los parametros a la sentencia.\n");
                    }
                    else
                        printf("Error al preparar la sentencia.\n");

                    break;

                case 4: // ELIMINAR

                    printf("-------------- Eliminar registro --------------\n\n");
                    memset(bind, 0, sizeof(bind));

                    if (opTabla == 1)
                    {

                        bind[0].buffer_type = MYSQL_TYPE_LONG;
                        bind[0].buffer = (char *)&pelicula.id;
                        bind[0].is_null = 0;
                        bind[0].length = 0;

                        printf("Ingrese el id de la pelicula: ");
                        scanf("%d", &pelicula.id);
                        error_consulta = mysql_stmt_prepare(stmt, QUERY_ELIMINAR_PELICULA, strlen(QUERY_ELIMINAR_PELICULA));
                    }
                    else if (opTabla == 2)
                    {

                        bind[0].buffer_type = MYSQL_TYPE_LONG;
                        bind[0].buffer = (char *)&cliente.nit;
                        bind[0].is_null = 0;
                        bind[0].length = 0;

                        printf("Ingrese el nit del cliente: ");
                        scanf("%d", &cliente.nit);
                        error_consulta = mysql_stmt_prepare(stmt, QUERY_ELIMINAR_CLIENTE, strlen(QUERY_ELIMINAR_CLIENTE));
                    }
                    else if (opTabla == 3)
                    {

                        bind[0].buffer_type = MYSQL_TYPE_LONG;
                        bind[0].buffer = (char *)&boleto.id;
                        bind[0].is_null = 0;
                        bind[0].length = 0;

                        printf("Ingrese el id del boleto: ");
                        scanf("%d", &boleto.id);
                        error_consulta = mysql_stmt_prepare(stmt, QUERY_ELIMINAR_BOLETO, strlen(QUERY_ELIMINAR_BOLETO));
                    }
                    else
                    {
                        error_consulta = 1;
                        printf("Opcion incorrecta.\n");
                    }

                    if (!error_consulta)
                    {
                        if (!mysql_stmt_bind_param(stmt, bind))
                        {
                            if (!mysql_stmt_execute(stmt))
                            {
                                if (mysql_stmt_affected_rows(stmt) > 0)
                                    printf("Registro eliminado con éxito.\n");
                                else
                                    printf("No se pudo eliminar el registro o no existe.\n");
                            }
                            else
                                printf("Error al ejecutar la sentencia.\n");
                        }
                        else
                            printf("Error al asociar los parametros a la sentencia.\n");
                    }
                    else
                        printf("Error al preparar la sentencia.\n");

                    break;

                case 5:
                    printf("Saliendo del programa...\n");
                    break;
                }
                system("read -p '\nPresiona enter para continuar...' var");
            } while (opMenus != 5);
        }
        else
            printf("Error al inicializar la sentencia preparada.\n");

        mysql_free_result(res_ptr);
        mysql_stmt_close(stmt);
        mysql_close(conexion);
    }

    return 0;
}

int conectar(MYSQL **conexion)
{
    int error;
    *conexion = mysql_init(NULL);

    if (mysql_real_connect(*conexion, HOST, USERNAME, PASSWORD,
                           DATABASE, PORT, NULL, 0) != NULL)
    {
        printf("Se establecio la conexion con la base de datos\n");
        error = 0;
    }
    else
    {
        printf("Error al conectarse con la base de datos\n");
        error = 1;
    }
    return error;
}

void asignarBindPelicula()
{
    if (opMenus == 1)
    {
        bind[0].buffer_type = MYSQL_TYPE_STRING;
        bind[0].buffer = (char *)pelicula.nombre;
        bind[0].buffer_length = 45;
        bind[0].is_null = 0;
        bind[0].length = &length[0];

        bind[1].buffer_type = MYSQL_TYPE_STRING;
        bind[1].buffer = (char *)pelicula.descripcion;
        bind[1].buffer_length = 100;
        bind[1].is_null = 0;
        bind[1].length = &length[1];

        bind[2].buffer_type = MYSQL_TYPE_STRING;
        bind[2].buffer = (char *)pelicula.idioma;
        bind[2].buffer_length = 45;
        bind[2].is_null = 0;
        bind[2].length = &length[2];

        bind[3].buffer_type = MYSQL_TYPE_STRING;
        bind[3].buffer = (char *)pelicula.duracion;
        bind[3].buffer_length = 10;
        bind[3].is_null = 0;
        bind[3].length = &length[3];

        bind[4].buffer_type = MYSQL_TYPE_STRING;
        bind[4].buffer = (char *)pelicula.fechaEstreno;
        bind[4].buffer_length = 15;
        bind[4].is_null = 0;
        bind[4].length = &length[4];
    }
    else
    {
        bind[0].buffer_type = MYSQL_TYPE_LONG;
        bind[0].buffer = (char *)&pelicula.id;
        bind[0].is_null = 0;
        bind[0].length = 0;

        bind[1].buffer_type = MYSQL_TYPE_STRING;
        bind[1].buffer = (char *)pelicula.nombre;
        bind[1].buffer_length = sizeof(pelicula.nombre);
        bind[1].is_null = 0;
        bind[1].length = &length[0];

        bind[2].buffer_type = MYSQL_TYPE_STRING;
        bind[2].buffer = (char *)pelicula.descripcion;
        bind[2].buffer_length = sizeof(pelicula.descripcion);
        bind[2].is_null = 0;
        bind[2].length = &length[1];

        bind[3].buffer_type = MYSQL_TYPE_STRING;
        bind[3].buffer = (char *)pelicula.idioma;
        bind[3].buffer_length = sizeof(pelicula.idioma);
        bind[3].is_null = 0;
        bind[3].length = &length[2];

        bind[4].buffer_type = MYSQL_TYPE_STRING;
        bind[4].buffer = (char *)pelicula.duracion;
        bind[4].buffer_length = sizeof(pelicula.duracion);
        bind[4].is_null = 0;
        bind[4].length = &length[3];

        bind[5].buffer_type = MYSQL_TYPE_STRING;
        bind[5].buffer = (char *)pelicula.fechaEstreno;
        bind[5].buffer_length = sizeof(pelicula.fechaEstreno);
        bind[5].is_null = 0;
        bind[5].length = &length[4];
    }
}

void asignarBindCliente()
{
    bind[0].buffer_type = MYSQL_TYPE_LONG;
    bind[0].buffer = (char *)&cliente.nit;
    bind[0].is_null = 0;
    bind[0].length = 0;

    bind[1].buffer_type = MYSQL_TYPE_STRING;
    bind[1].buffer = (char *)cliente.nombre;
    bind[1].buffer_length = sizeof(cliente.nombre);
    bind[1].is_null = 0;
    bind[1].length = &length[0];

    bind[2].buffer_type = MYSQL_TYPE_STRING;
    bind[2].buffer = (char *)cliente.apellido;
    bind[2].buffer_length = sizeof(cliente.apellido);
    bind[2].is_null = 0;
    bind[2].length = &length[1];

    bind[3].buffer_type = MYSQL_TYPE_LONG;
    bind[3].buffer = (char *)&cliente.edad;
    bind[3].is_null = 0;
    bind[3].length = 0;

    bind[4].buffer_type = MYSQL_TYPE_STRING;
    bind[4].buffer = (char *)cliente.telefono;
    bind[4].buffer_length = sizeof(cliente.telefono);
    bind[4].is_null = 0;
    bind[4].length = &length[2];

    bind[5].buffer_type = MYSQL_TYPE_STRING;
    bind[5].buffer = (char *)cliente.correo;
    bind[5].buffer_length = sizeof(cliente.correo);
    bind[5].is_null = 0;
    bind[5].length = &length[3];
}

void asignarBindBoleto()
{
    if (opMenus == 1)
    {
        bind[0].buffer_type = MYSQL_TYPE_LONG;
        bind[0].buffer = (char *)&boleto.idPelicula;
        bind[0].is_null = 0;
        bind[0].length = 0;

        bind[1].buffer_type = MYSQL_TYPE_LONG;
        bind[1].buffer = (char *)&boleto.nitCliente;
        bind[1].is_null = 0;
        bind[1].length = 0;

        bind[2].buffer_type = MYSQL_TYPE_STRING;
        bind[2].buffer = (char *)boleto.sala;
        bind[2].buffer_length = 45;
        bind[2].is_null = 0;
        bind[2].length = &length[0];

        bind[3].buffer_type = MYSQL_TYPE_STRING;
        bind[3].buffer = (char *)boleto.asiento;
        bind[3].buffer_length = 45;
        bind[3].is_null = 0;
        bind[3].length = &length[1];

        bind[4].buffer_type = MYSQL_TYPE_DOUBLE;
        bind[4].buffer = (char *)&boleto.precio;
        bind[4].is_null = 0;
        bind[4].length = 0;

        bind[5].buffer_type = MYSQL_TYPE_STRING;
        bind[5].buffer = (char *)boleto.hora;
        bind[5].buffer_length = 10;
        bind[5].is_null = 0;
        bind[5].length = &length[2];

        bind[6].buffer_type = MYSQL_TYPE_STRING;
        bind[6].buffer = (char *)boleto.fecha;
        bind[6].buffer_length = 15;
        bind[6].is_null = 0;
        bind[6].length = &length[3];
    }
    else
    {
        bind[0].buffer_type = MYSQL_TYPE_LONG;
        bind[0].buffer = (char *)&boleto.id;
        bind[0].is_null = 0;
        bind[0].length = 0;

        bind[1].buffer_type = MYSQL_TYPE_LONG;
        bind[1].buffer = (char *)&boleto.idPelicula;
        bind[1].is_null = 0;
        bind[1].length = 0;

        bind[2].buffer_type = MYSQL_TYPE_LONG;
        bind[2].buffer = (char *)&boleto.nitCliente;
        bind[2].is_null = 0;
        bind[2].length = 0;

        bind[3].buffer_type = MYSQL_TYPE_STRING;
        bind[3].buffer = (char *)boleto.sala;
        bind[3].buffer_length = 45;
        bind[3].is_null = 0;
        bind[3].length = &length[0];

        bind[4].buffer_type = MYSQL_TYPE_STRING;
        bind[4].buffer = (char *)boleto.asiento;
        bind[4].buffer_length = 45;
        bind[4].is_null = 0;
        bind[4].length = &length[1];

        bind[5].buffer_type = MYSQL_TYPE_DOUBLE;
        bind[5].buffer = (char *)&boleto.precio;
        bind[5].is_null = 0;
        bind[5].length = 0;

        bind[6].buffer_type = MYSQL_TYPE_STRING;
        bind[6].buffer = (char *)boleto.hora;
        bind[6].buffer_length = 10;
        bind[6].is_null = 0;
        bind[6].length = &length[2];

        bind[7].buffer_type = MYSQL_TYPE_STRING;
        bind[7].buffer = (char *)boleto.fecha;
        bind[7].buffer_length = 15;
        bind[7].is_null = 0;
        bind[7].length = &length[3];
    }
}

int menu(int *opMenu, int *opTabla, char *tabla, int *opQuery)
{
    system("clear");
    printf("******* Menu *******\n");
    printf("1.Insertar.\n");
    printf("2.Consultar.\n");
    printf("3.Actualizar.\n");
    printf("4.Eliminar.\n");
    printf("5.Salir.\n\n");

    printf("Ingrese una opcion: ");
    scanf("%d", opMenu);

    system("clear");
    switch (*opMenu)
    {
    case 1:
        seleccionarTabla(opTabla, tabla);
        break;
    case 2:
        seleccionarVista(opQuery);
        break;
    case 3:
        seleccionarActualizacion(opTabla);
        break;
    case 4:
        seleccionarTabla(opTabla, tabla);
        break;
    default:
        printf("Opcion incorrecta!\n");
        break;
    }
}

void seleccionarTabla(int *opTabla, char *tabla)
{
    printf("\n¿Sobre qué tabla desea realizar la operación?\n");
    printf("1.Pelicula.\n");
    printf("2.Cliente.\n");
    printf("3.Boleto.\n\n");
    printf("Seleccione una opción: ");
    scanf("%d", opTabla);
}

void seleccionarVista(int *opQuery)
{
    printf("Vistas disponibles:\n");
    printf("1.Buscar pelicula por id\n");
    printf("2.Buscar cliente por nit\n");
    printf("3.Buscar boleto por id\n");
    printf("4.Mostrar todas las peliculas.\n");
    printf("5.Mostrar todos los clientes.\n");
    printf("6.Mostrar todos los boletos.\n\n");
    printf("Introduzca la opcion deseada: ");
    scanf("%d", opQuery);
}

void seleccionarActualizacion(int *opTabla)
{
    printf("Opciones disponibles:\n");
    printf("1.Actualizar nombre de una pelicula.\n");
    printf("2.Actualizar correo de un cliente.\n");
    printf("3.Actualizar asiento de un boleto.\n\n");
    printf("Ingrese la opcion deseada: ");
    scanf("%d", opTabla);
}

void limpiarBuffer()
{
    int c;
    while ((c = getchar()) != '\n' && c != EOF)
    {
        // Consumir caracteres hasta encontrar un salto de línea o el final de archivo
    }
}
