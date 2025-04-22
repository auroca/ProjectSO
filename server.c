#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <mysql/mysql.h>
#include <pthread.h>

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
int i;
int sockets[100];

typedef struct{
	char nombre[20];
	int socket;
}Conectado;

typedef struct{
	Conectado conectados [100];
	int num;
}ListaConectados;

ListaConectados miLista;

// ==================== FUNCIONES MYSQL ====================

// Función para conectarse a la base de datos
MYSQL* conectar_bd()
{
    MYSQL* conn = mysql_init(NULL);
    if (conn == NULL)
    {
        printf("Error al inicializar MySQL\n");
        exit(1);
    }
    if (!mysql_real_connect(conn, "shiva2.upc.es", "root", "mysql", "M1_game", 0, NULL, 0))
    {
        printf("Error al conectar con MySQL: %s\n", mysql_error(conn));
        exit(1);
    }
    return conn;
}

// Registro de usuario
void registrar_usuario(char* usuario, char* password, char* respuesta)
{
    MYSQL* conn = conectar_bd();
    char query[512];
    sprintf(query, "INSERT INTO Player (username, password) VALUES ('%s', '%s')", usuario, password);


    if (mysql_query(conn, query) == 0)
    {
        printf("Registro exitoso.\n");
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
void login_usuario(char* usuario, char* password, char* respuesta)
{
    MYSQL* conn = conectar_bd();
    char query[512];
    sprintf(query, "SELECT username, password FROM Player WHERE username ='%s' AND password= '%s'", usuario, password);

    if (mysql_query(conn, query) == 0)
    {
        MYSQL_RES* res = mysql_store_result(conn);
        if (mysql_fetch_row(res) == NULL)
            strcpy(respuesta, "9/Usuario o contraseña incorrectos.");
        else
            strcpy(respuesta, "9/Login exitoso.");
        mysql_free_result(res);
    }
    else
    {
        strcpy(respuesta, "9/Error en la consulta.");
    }
    mysql_close(conn);
}

// Número de partidas jugadas por un usuario
void num_partidas_jugadas(char* usuario, char* respuesta)
{
    MYSQL* conn = conectar_bd();
    char query[512];
    sprintf(query, "SELECT Game FROM Participation WHERE Player = '%s'", usuario);

    if (mysql_query(conn, query) == 0)
    {
        MYSQL_RES* res = mysql_store_result(conn);
        MYSQL_ROW row;
        int num_partidas = 0;
        while ((row = mysql_fetch_row(res)) != NULL)
        {
            num_partidas++;
        }
        sprintf(respuesta, "1/Partidas jugadas: %d", num_partidas);
        mysql_free_result(res);
    }
    else
        strcpy(respuesta, "1/Error en la consulta.");

    mysql_close(conn);
}

// Puntuación máxima de un jugador
void puntuacion_maxima(char* usuario, char* respuesta)
{
    MYSQL* conn = conectar_bd();
    char query[512];
    sprintf(query, "SELECT MAX(points) FROM Game WHERE GameID IN (SELECT Game FROM Participation WHERE Player='%s')", usuario);

    if (mysql_query(conn, query) == 0)
    {
        MYSQL_RES* res = mysql_store_result(conn);
        MYSQL_ROW row = mysql_fetch_row(res);
        sprintf(respuesta, "2/Puntuacion maxima: %s", row[0] ? row[0] : "0");
        mysql_free_result(res);
    }
    else
        strcpy(respuesta, "2/Error en la consulta.");

    mysql_close(conn);
}

// Lista de jugadores con los que ha jugado un usuario
void jugadores_con_partidas(char* usuario, char* respuesta)
{
    MYSQL* conn = conectar_bd();
    char query[512];
    sprintf(query, "SELECT DISTINCT Player FROM Participation WHERE Game IN (SELECT Game FROM Participation WHERE Player='%s') AND Player != '%s'", usuario, usuario);

    if (mysql_query(conn, query) == 0)
    {
        MYSQL_RES* res = mysql_store_result(conn);
        MYSQL_ROW row;
        strcpy(respuesta, "2/Jugadores: ");
        while ((row = mysql_fetch_row(res)) != NULL)
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

int Pon (ListaConectados *lista, char nombre[20], int socket){
	//Añade nuevo conectado
	//Retorna 0 si ok
	//Retorna -1 si la lista ya estaba llena y no se ha podido añadir
	if(lista->num == 100)
	{
		return -1;
	}
	else{
		strcpy(lista->conectados[lista->num].nombre, nombre);
		lista->conectados[lista->num].socket=socket;
		lista->num++;
		return 0;
	}
}

int DamePosicion (ListaConectados *lista, char nombre[20]){
	//Devuelve la posicion en la lista o -1 si no está en la lista
	int j = 0;
	int encontrado = 0;
	while ( (j<lista->num) && !encontrado ){
		if (strcmp(lista->conectados[j].nombre, nombre)==0){
			encontrado=1;
		}
		if(!encontrado){
			j=j+1;
		}
	}
	if(encontrado){
		return j;
	}
	else{		
		return -1;
	}
}

int Elimina (ListaConectados *lista, char nombre[20]){
	//Retorna 0 si elimina y -1 si usuario no est en la lista
	int pos = DamePosicion(lista, nombre);
	if (pos==-1){
		return -1;
	}
	else{
		int j;
		for (j=pos; j<lista->num-1; j++)
		{
			strcpy (lista->conectados[j].nombre, lista->conectados[j+1].nombre);
			lista->conectados[j].socket = lista->conectados[j+1].socket;
		}
		lista->num--;
		return 0;
	}
}

void DameConectados (ListaConectados *lista, char conectados[300]){
	//Pone en conectados los nombres de todos los conectados separados por /
	//Primero pone el numero de conectados
	//Ejemplo: "3/Juan/MAria/Pedro"
	sprintf (conectados, "%d", lista->num);
	int j;
	for (j=0; j<lista->num; j++){
		sprintf (conectados, "%s/%s", conectados, lista->conectados[j].nombre);
	}
}

// ==================== SERVIDOR PRINCIPAL ====================

void *AtenderCliente (void *socket)
{
	int sock_conn = *(int*)socket;
	int sock_listen;
	
	char user[20];
	
	char peticion[512];
	char respuesta[512];

	int terminar=0;	

		while (terminar==0)
		{
			int ret = read(sock_conn, peticion, sizeof(peticion));
			if (ret <= 0)
			{
				// El cliente se ha desconectado o ocurrió un error
				printf("El cliente se ha desconectado o se produjo un error en read\n");
				break;
			}
			peticion[ret] = '\0';
			printf("Petición recibida: %s\n", peticion);
			
			// Analizar la petición
			char* p = strtok(peticion, "/");
			if (p == NULL)
			{
				strcpy(respuesta, "Formato de petición incorrecto.");
				write(sock_conn, respuesta, strlen(respuesta));
				continue;
			}
			int codigo = atoi(p);
			char usuario[50], password[50];
			
			p = strtok(NULL, "/");
			if (p == NULL)
			{
				strcpy(respuesta, "Formato de petición incorrecto.");
				write(sock_conn, respuesta, strlen(respuesta));
				continue;
			}
			strcpy(user, usuario);
			strcpy(usuario, p);
			
			// Procesar según el código
			if (codigo == 0)  // Registro de usuario
			{
				p = strtok(NULL, "/");
				if (p == NULL)
					strcpy(respuesta, "Formato de petición incorrecto.");
				else
				{
					strcpy(password, p);
					registrar_usuario(usuario, password, respuesta);
				}
			}
			else if (codigo == 9) // Login de usuario
			{
				p = strtok(NULL, "/");
				if (p == NULL)
					strcpy(respuesta, "9/Formato de petición incorrecto.");
				else
				{
					strcpy(password, p);
					
					login_usuario(usuario, password, respuesta);
					
					printf("Ejecutando login_usuario con: %s / %s\n", usuario, password);
					fflush(stdout); // Asegura que se imprime de inmediato
					
					if(strcmp(respuesta, "9/Login exitoso.")==0){
						pthread_mutex_lock(&mutex);
						if (DamePosicion(&miLista, usuario) == -1){
							//Añadimos a la lista de conectados al usuario
							int res = Pon(&miLista, usuario, socket);
						}
						pthread_mutex_unlock (&mutex);
						strcpy(user, usuario);
						
						char misConectados[500];
						DameConectados (&miLista, misConectados);
						
						char *p = strtok (misConectados, "/");
						int n = atoi(p);
						char notificacion [512];
						
						int j;
						sprintf(notificacion, "8/%d jugadores conectados:\n",n);
						for(j=0; j<n; j++){
							char nombre[20];
							p=strtok(NULL,"/");
							strcpy(nombre,p);
							sprintf(notificacion, "%s,%s\n",notificacion,nombre);
						};
						
						for(j=0; j<i; j++){
							write(sockets[j], notificacion, strlen(notificacion));
						}
					}
				}
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
			/*else if (codigo == 8)  // Dar lista de usuarios conectados
			{
				char misConectados[500];
				DameConectados (&miLista, misConectados);
				
				char *p = strtok (misConectados, "/");
				int n = atoi(p);
				
				int j;
				sprintf(respuesta, "8/%d jugadores conectados:\n",n);
				for(j=0; j<n; j++){
					char nombre[20];
					p=strtok(NULL,"/");
					strcpy(nombre,p);
					sprintf(respuesta, "%s,%s\n",respuesta,nombre);
				};
			}*/
			else if (codigo == 4)  // Enviar invitación
			{
				char usuarioOrigen[50], usuarioDestino[50];
				strcpy(usuarioOrigen, usuario);

				p = strtok(NULL, "/");
				if (p == NULL) {
					strcpy(respuesta, "4/Formato incorrecto para invitación.");
				}
				else {
					strcpy(usuarioDestino, p);
					int pos_dest = DamePosicion(&miLista, usuarioDestino);

					if (pos_dest != -1) {
						int sock_destino = miLista.conectados[pos_dest].socket;
						char notificacion[100];
						sprintf(notificacion, "4/%s", usuarioOrigen);
						write(sock_destino, notificacion, strlen(notificacion));
						sprintf(respuesta, "4/Invitación enviada a %s.", usuarioDestino);
					}
					else {
						sprintf(respuesta, "4/El usuario %s no está conectado.", usuarioDestino);
					}
				}
			}
			else if (codigo == 5)  // Respuesta a la invitación
			{
				char invitador[50];
				char decision[50];

				p = strtok(NULL, "/"); // usuario que envió la invitación
				if (p != NULL) strcpy(invitador, p);
				p = strtok(NULL, "/"); // ACEPATADA o RECHAZADA
				if (p != NULL) strcpy(decision, p);

				int pos_invitador = DamePosicion(&miLista, invitador);
				if (pos_invitador != -1)
				{
					int sock_invitador = miLista.conectados[pos_invitador].socket;
					char notificacion[100];
					sprintf(notificacion, "5/%s ha %s tu invitación.", usuario, decision);
					write(sock_invitador, notificacion, strlen(notificacion));
					sprintf(respuesta, "5/Respuesta enviada al invitador.");
				}
				else
				{
					sprintf(respuesta, "5/No se encontró al usuario que envió la invitación.");
				}
				}

			else
			{
				strcpy(respuesta, "Comando no valido.");
			}
			
			// Enviar respuesta al cliente
			write(sock_conn, respuesta, strlen(respuesta));
			fflush(stdout);
		}
		close(sock_conn);
		pthread_mutex_lock(&mutex);
		int res = Elimina(&miLista, user);
		pthread_mutex_unlock (&mutex);
		char misConectados[500];
		DameConectados (&miLista, misConectados);
		
		char *p = strtok (misConectados, "/");
		int n = atoi(p);
		char notificacion [512];
		
		int j;
		sprintf(notificacion, "8/%d jugadores conectados:\n",n);
		for(j=0; j<n; j++){
			char nombre[20];
			p=strtok(NULL,"/");
			strcpy(nombre,p);
			sprintf(notificacion, "%s,%s\n",notificacion,nombre);
		};
		
		for(j=0; j<i; j++){
			write(sockets[j], notificacion, strlen(notificacion));
		}
		return 0;
}



int main()
{
	int sock_conn, sock_listen;
	struct sockaddr_in serv_adr;
	
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
	serv_adr.sin_port = htons(50001);
	
	// Bind al puerto
	if (bind(sock_listen, (struct sockaddr*)&serv_adr, sizeof(serv_adr)) < 0)
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

	pthread_t thread;
	i=0;
	
	miLista.num = 0;
	for (;;)
	{
		printf("Servidor en espera de conexiones...\n");
		sock_conn = accept(sock_listen, NULL, NULL);		
		if (sock_conn < 0)
		{
			printf("Error en accept\n");
			continue;
		}
		printf("Nueva conexion establecida\n");
		
		sockets[i] = sock_conn;		
		pthread_create (&thread, NULL, AtenderCliente,&sockets[i] );
		i++;
	}
	
	//for (i=0; i<5;i++)
	//{
		//pthread_join (thread[i], NULL);
	//}
	
}
