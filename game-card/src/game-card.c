#include "game-card.h"


/* Local functions */
static void *handle_connection(void *arg);
static void exit_gracefully(int status);

int main(int argc, char *argv[]) {
	game_card_logger_create();
	game_card_config_load();
	game_card_and_broker_connection_init();
	game_card_logger_destroy();
	game_card_exit();
}

void game_card_and_broker_connection_init(){
	
	game_card_fd = socket_connect_to_server(game_card_config->ip_broker, game_card_config->puerto_broker);

	if (game_card_fd < 0) {
		game_card_logger_warn("No se pudo conectar con BROKER");
		socket_close_conection(game_card_fd);
		game_card_and_gameboy_connection_init();
	}
	else {
		game_card_logger_info("Conexion con BROKER establecida correctamente!");
		/*
		for(;;) {
			received_bytes = recv(client_fd, &protocol, sizeof(int), 0);

			if(received_bytes <= 0) {
				game_card_logger_error("Error al recibir operacion del Broker");
				game_card_logger_error("Se desconecto del broker");
				return NULL;
			}

			switch(protocol) {
				case NEW_POKEMON:
					game_card_logger_info("Recibi un NEW POKEMON");
					break;
				case CATCH_POKEMON:
					game_card_logger_info("Recibi una CATCH POKEMON");
					break;
				case GET_POKEMON:
					game_card_logger_info("Recibi una CATCH POKEMON");
					break;
				default:
					break;
	-		}
		}
			*/
	}
	//gcfs_create_structs();
}

void game_card_and_gameboy_connection_init() {
	game_card_socket = socket_create_listener(LOCAL_IP, LOCAL_PORT);
	
	if(game_card_socket < 0) {
		switch (game_card_socket) {
		case NO_FD_ERROR:
			game_card_logger_info("No hay file descriptor disponible para el listener.");
			break;
		case BIND_ERROR:
			game_card_logger_info("Error al intentar bindear.");
			break;
		case LISTEN_ERROR:
			game_card_logger_info("Error al intentar crear cola de escucha.");
			break;
		}
		exit_gracefully(EXIT_FAILURE);
	}

	game_card_logger_info("Game card se puso en modo Servidor! Esperando conexion del Gameboy...");

	struct sockaddr_in client_info;
	socklen_t addrlen = sizeof client_info;

	pthread_attr_t attrs;
	pthread_attr_init(&attrs);
	pthread_attr_setdetachstate(&attrs, PTHREAD_CREATE_JOINABLE);

	for(;;) {
		int accepted_fd;
		pthread_t tid;

		if((accepted_fd = accept(game_card_socket, (struct sockaddr *) &client_info, &addrlen)) != -1) {
			pthread_create(&tid, NULL, (void*) handle_connection, (void*) &accepted_fd);
			pthread_detach(tid);
			game_card_logger_info("Creando un hilo para atender una conexión en el socket %d", accepted_fd);
			usleep(500000);
		}
		else {
			game_card_logger_error("Error al conectar con Gameboy");
		}
	}
	*/

	    pthread_attr_t attrs;
		pthread_attr_init(&attrs);
		pthread_attr_setdetachstate(&attrs, PTHREAD_CREATE_JOINABLE);
		pthread_t tid;
		pthread_t tid2;
		pthread_t tid3;

		t_cola cola = NEW_QUEUE;

		 game_card_logger_info(
				"Creando un hilo para subscribirse a la cola NEW del broker %d");

		pthread_create(&tid, NULL, (void*) subscribe_to, (void*) &cola);
		pthread_detach(tid);
		usleep(500000);
		game_card_logger_info(
				"Creando un hilo para subscribirse a la cola CATCH del broker %d");


		cola = CATCH_QUEUE;
		pthread_create(&tid2, NULL, (void*) subscribe_to, (void*) &cola);
		pthread_detach(tid2);
		usleep(500000);

		game_card_logger_info(
						"Creando un hilo para subscribirse a la cola GET del broker %d");
		cola = GET_QUEUE;
		pthread_create(&tid3, NULL, (void*) subscribe_to, (void *)&cola);
		pthread_detach(tid3);
		usleep(500000);
		for(;;);

}
void subscribe_to(void *arg) {

	t_cola cola = *((int *) arg);
	int new_broker_fd = socket_connect_to_server(game_card_config->ip_broker,
			game_card_config->puerto_broker);

	if (new_broker_fd < 0) {
		game_card_logger_warn("No se pudo conectar con BROKER");
		socket_close_conection(new_broker_fd);
	} else {
		game_card_logger_info("Conexion con BROKER establecida correctamente!");
	}

	t_subscribe* sub_snd = malloc(sizeof(t_subscribe));

	t_protocol subscribe_protocol = SUBSCRIBE;
	sub_snd->ip = string_duplicate(game_card_config->ip_game_card);
	sub_snd->puerto = game_card_config->puerto_game_card;
	sub_snd->proceso = TEAM;
	sub_snd->cola = cola;
	sub_snd->f_desc = -1;
	utils_serialize_and_send(new_broker_fd, subscribe_protocol, sub_snd);
	recv_game_card(new_broker_fd);

}


static void *handle_connection(void *arg) {
	int client_fd = *((int *) arg);
	recv_game_card(client_fd);
	return NULL;
}

	game_card_logger_info("Conexion establecida con cliente GAME BOY: %d", client_fd);
	int received_bytes;
	int protocol;
	
	for(;;) {
		received_bytes = recv(client_fd, &protocol, sizeof(int), 0);

		if(received_bytes <= 0) {
			game_card_logger_error("Error al recibir operacion del GAME BOY");
			game_card_logger_error("Se desconecto GAME BOY");
			return NULL;
		}
		switch(protocol) {
			case NEW_POKEMON:
				game_card_logger_info("Recibi un NEW POKEMON");
				break;
			case CATCH_POKEMON:
				game_card_logger_info("Recibi una CATCH POKEMON");
				break;
			case GET_POKEMON:
				game_card_logger_info("Recibi una CATCH POKEMON");
				break;
			default:
				break;

			}
		}
}

void game_card_exit() {
	socket_close_conection(game_card_fd);
	socket_close_conection(game_card_socket);
	gcfs_free_bitmap();
	game_card_config_free();
	game_card_logger_destroy();
	exit_gracefully(EXIT_FAILURE);
}

static void exit_gracefully(int status) {
	exit(status);
}