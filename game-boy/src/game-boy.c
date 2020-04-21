#include "game-boy.h"

int main(int argc, char *argv[]) {
	if (game_boy_load() < 0)
		return EXIT_FAILURE;
	game_boy_init();
	game_boy_exit();

	return EXIT_SUCCESS;
}

int game_boy_load() {
	int response = game_boy_logger_create();
	if (response < 0)
		return response;

	response = game_boy_config_load();
	if (response < 0) {
		game_boy_logger_destroy();
		return response;
	}

	return 0;
}

void connect_to_broker() {
	game_boy_broker_fd = socket_connect_to_server(game_boy_config->ip_broker,
			game_boy_config->puerto_broker);
	if (game_boy_broker_fd < 0) {
		game_boy_logger_warn("No se pudo conectar con BROKER");
		socket_close_conection(game_boy_broker_fd);
	} else {
		game_boy_logger_info("Conexion con BROKER establecida correctamente!");
	}
}

void connect_to_team() {
	game_boy_team_fd = socket_connect_to_server(game_boy_config->ip_team,
			game_boy_config->puerto_team);
	if (game_boy_team_fd < 0) {
		game_boy_logger_warn("No se pudo conectar con TEAM");
		socket_close_conection(game_boy_team_fd);
	} else {
		game_boy_logger_info("Conexion con TEAM establecida correctamente!");
	}
}

void connect_to_game_card() {
	game_boy_game_card_fd = socket_connect_to_server(
			game_boy_config->ip_gamecard, game_boy_config->puerto_gamecard);
	if (game_boy_game_card_fd < 0) {
		game_boy_logger_warn("No se pudo conectar con GAME CARD");
		socket_close_conection(game_boy_game_card_fd);
	} else {
		game_boy_logger_info(
				"Conexion con GAME CARD establecida correctamente!");
	}
}

void gb_create_send_broker_new_pikachu() {
	t_new_pokemon* new_snd = malloc(sizeof(t_new_pokemon));
	new_snd->nombre_pokemon = string_duplicate("pikachu");
	new_snd->id = 1;
	new_snd->id_correlacional = 1;
	new_snd->tamanio_nombre = 8;
	new_snd->pos_x = 1;
	new_snd->pos_y = 1;
	t_protocol new_protocol = NEW_POKEMON;
	game_boy_logger_info("Envio de New Pokemon");
	utils_serialize_and_send(game_boy_broker_fd, new_protocol, new_snd);
}

void gb_create_send_broker_appeared_raichu() {
	t_appeared_pokemon* appeared_snd = malloc(sizeof(t_appeared_pokemon));
	t_protocol appeared_protocol = APPEARED_POKEMON;
	appeared_snd->nombre_pokemon = string_duplicate("Raichu");
	appeared_snd->tamanio_nombre = 7;
	appeared_snd->id_correlacional = 2;
	appeared_snd->pos_x = 1;
	appeared_snd->pos_y = 1;
	appeared_snd->cantidad = 1;
	game_boy_logger_info("Envio de APPEARED Pokemon");
	utils_serialize_and_send(game_boy_broker_fd, appeared_protocol,
					appeared_snd);
}

void game_boy_console(){
	command_actions = game_boy_get_command_actions();
	while (game_boy_console_read(command_actions) == 0)
		;
}

void game_boy_init() {
	game_boy_logger_info("Inicando GAME BOY..");

	pthread_attr_t attrs;
	pthread_attr_init(&attrs);
	pthread_attr_setdetachstate(&attrs, PTHREAD_CREATE_JOINABLE);
	pthread_t tid;

	game_boy_logger_info(
				"Creando un hilo para enviar al broker %d");
	pthread_create(&tid, NULL, (void*) connect_to_broker, NULL);
	pthread_detach(tid);

/*
	game_boy_logger_info(
				"Creando un hilo para enviar al team");
	pthread_create(&tid, NULL, (void*) connect_to_team, NULL);
	pthread_detach(tid);

	game_boy_logger_info(
				"Creando un hilo para enviar al game card");
	pthread_create(&tid, NULL, (void*) connect_to_game_card, NULL );
	pthread_detach(tid);
*/
	game_boy_logger_info("Creando un hilo para consola");
	pthread_create(&tid, NULL, (void*) game_boy_console, NULL);
	pthread_detach(tid);
	for(;;);
}


void game_boy_exit() {
	socket_close_conection(game_boy_broker_fd);
	socket_close_conection(game_boy_team_fd);
	socket_close_conection(game_boy_game_card_fd);
	game_boy_free_command_actions(command_actions);
	game_boy_config_free();
	game_boy_logger_destroy();
}
