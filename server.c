#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <mysql/mysql.h>

// ==================== FUNCIONES MYSQL ====================

// Función para conectarse a la base de datos
MYSQL* conectar_bd()
{
    MYSQL *conn = mysql_init(NULL);
    if (conn == NULL)
    {
        printf("Error al inicializar MySQL\n");
        exit(1);
    }
    if (!mysql_real_connect(conn, "localhost", "root", "mysql", "game", 0, NULL, 0))
    {
        printf("Error al conectar con MySQL: %s\n", mysql_error(conn));
        exit(1);
    }
    return conn;
}

// Registro de usuario
void registrar_usuario(char *usuario, char *password, char *respuesta)
{
    MYSQL *conn = conectar_bd();
    char query[512];
    sprintf(query, "INSERT INTO Player (username, password) VALUES ('%s', '%s')", usuario, password);

    if (mysql_query(conn, query) == 0)
	{
		printf("Registro exitoso.");	
        strcpy(respuesta, "Registro exitoso.");		
	}
    else
	{
		printf("Error en el registro: %s\n", mysql_error(conn));
        strcpy(respuesta, "Error en el registro.");		
	}
    mysql_close(conn);
}

// Login de usuario
void login_usuario(char *usuario, char *password, char *respuesta)
{
    MYSQL *conn = conectar_bd();
    char query[512];
    sprintf(query, "SELECT username, password FROM Player WHERE username ='%s' AND password= '%s'", usuario, password);
    
    if (mysql_query(conn, query) == 0)
    {
        MYSQL_RES *res = mysql_store_result(conn);
        if (mysql_fetch_row(res) == NULL)
			strcpy(respuesta, "Usuario o contraseña incorrectos.");
		else
			strcpy(respuesta, "Login exitoso.");
        mysql_free_result(res);
    }
    else if(mysql_query(conn, query) != 0) 
        strcpy(respuesta, "Error en la consulta.");

    mysql_close(conn);
}

// Número de partidas jugadas por un usuario
void num_partidas_jugadas(char *usuario, char *respuesta)
{
    MYSQL *conn = conectar_bd();
    char query[512];
	sprintf(query, "SELECT Game FROM Participation WHERE Player = '%s')", usuario);

    if (mysql_query(conn, query) == 0)
    {
        MYSQL_RES *res = mysql_store_result(conn);
		MYSQL_ROW row;
		int num_partidas = 0;
		while ((row = mysql_fetch_row(res)) != NULL) 
		{
			num_partidas++;
		}
		sprintf(respuesta, "Partidas jugadas: %d", num_partidas);
		mysql_free_result(res);
	}
    else
        strcpy(respuesta, "Error en la consulta.");

    mysql_close(conn);
}

// Puntuación máxima de un jugador
void puntuacion_maxima(char *usuario, char *respuesta)
{
    MYSQL *conn = conectar_bd();
    char query[512];
	sprintf(query, "SELECT points FROM Game WHERE GameID IN (SELECT Game FROM Participation WHERE Player='%s')", usuario);

	int maxscore;
    if (mysql_query(conn, query) == 0)
    {
		MYSQL_RES *result = mysql_store_result (conn);
		MYSQL_ROW row = mysql_fetch_row (result);
		int score;
		if (row == NULL)
			printf ("No se han obtenido datos en la consulta\n");
		else
			maxscore = atoi (row[0]);
		row = mysql_fetch_row (result);
		while (row !=NULL) 
		{
			score = atoi (row[0]);
			if (score > maxscore)
				maxscore = score;
			row = mysql_fetch_row (result);
		}
		sprintf(respuesta, "Puntuación máxima: %d", maxscore);
		mysql_free_result(result);
    }
    else
        strcpy(respuesta, "Error en la consulta.");
		mysql_close(conn);
}

// Lista de jugadores con los que ha jugado un usuario
void jugadores_con_partidas(char *usuario, char *respuesta)
{
    MYSQL *conn = conectar_bd();
    char query[512];
    sprintf(query, "SELECT DISTINCT Player FROM Participation WHERE Game IN (SELECT Game FROM Participation WHERE Player='%s') AND Player != '%s'", usuario, usuario);

    if (mysql_query(conn, query) == 0)
{
        MYSQL_RES *res = mysql_store_result(conn);
        MYSQL_ROW row;
        strcpy(respuesta, "Jugadores: ");
        while ((row = mysql_fetch_row(res)))
        {
            strcat(respuesta, row[0]);
            strcat(respuesta, ", ");
        }
        mysql_free_result(res);
}
    else
        strcpy(respuesta, "Error en la consulta.");

    mysql_close(conn);
}

// ==================== SERVIDOR PRINCIPAL ====================

int main()

{
    int sock_conn, sock_listen;
    struct sockaddr_in serv_adr;
    char peticion[512];
    char respuesta[512];

    // Crear socket
    if ((sock_listen = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        printf("Error creando socket\n");
        return 1;
    }

// Configuración del socket
    memset(&serv_adr, 0, sizeof(serv_adr));
    serv_adr.sin_family = AF_INET;
    serv_adr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_adr.sin_port = htons(9050);

    // Bind al puerto
if (bind(sock_listen, (struct sockaddr *)&serv_adr, sizeof(serv_adr)) < 0)
    {
        printf("Error en el bind\n");
        return 1;
    }

    // Escuchar conexiones
    if (listen(sock_listen, 3) < 0)
    {
        printf("Error en el listen\n");
        return 1;
    }

    printf("Servidor en espera de conexiones...\n");

 // Aceptamos múltiples conexiones
    while (1)
    {
        sock_conn = accept(sock_listen, NULL, NULL);
        printf("Nueva conexión establecida\n");

        // Recibir petición del cliente
        int ret = read(sock_conn, peticion, sizeof(peticion));
        peticion[ret] = '\0';
        printf("Petición recibida: %s\n", peticion);

        // Analizar la petición
        char *p = strtok(peticion, "/");
        int codigo = atoi(p);
        char usuario[50], password[50];

        p = strtok(NULL, "/");
        strcpy(usuario, p);
        
        if (codigo == 0)  // Registro de usuario
        {
            p = strtok(NULL, "/");
            strcpy(password, p);
            registrar_usuario(usuario, password, respuesta);
        }
        else if (codigo == 9) // Login de usuario
        {
            p = strtok(NULL, "/");
            strcpy(password, p);
            login_usuario(usuario, password, respuesta);
        }
        else if (codigo == 1)  // Número de partidas jugadas
        {
            num_partidas_jugadas(usuario, respuesta);
        }
        else if (codigo == 2)  // Puntuación máxima de un jugador
        {
            puntuacion_maxima(usuario, respuesta);
}
        else if (codigo == 3)  // Jugadores con los que ha jugado
        {
            jugadores_con_partidas(usuario, respuesta);
        }
        else
        {
            strcpy(respuesta, "Comando no válido.");
        }

        // Enviar respuesta al cliente
        write(sock_conn, respuesta, strlen(respuesta));
        close(sock_conn);
    }
    return 0;
}





    

