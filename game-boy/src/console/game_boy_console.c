#include "game_boy_console.h"

char* game_boy_get_input();
char** game_boy_get_arguments_from_input(char*, int);
int game_boy_get_arguments_size(char *input);
void game_boy_command_execute(char *key, t_dictionary* command_actions,
		char** arguments, int arguments_size);

void broker_new_pokemon_command(char** arguments, int arguments_size) {
	if (arguments_size != 6) {
		return;
	}
	game_boy_logger_info("BROKER NEW_POKEMON");
	t_new_pokemon* new_snd = malloc(sizeof(t_new_pokemon));
	new_snd->nombre_pokemon = strcat(string_duplicate(arguments[2]), "\0");
	new_snd->tamanio_nombre = strlen(arguments[2]) +1;
	new_snd->pos_x = atoi(arguments[3]);
	new_snd->pos_y = atoi(arguments[4]);
	new_snd->cantidad = atoi(arguments[5]);

	utils_serialize_and_send(game_boy_broker_fd, NEW_POKEMON, new_snd);
	usleep(500000);
}

void broker_appeared_pokemon_command(char** arguments, int arguments_size) {
	if (arguments_size != 6) {
		return;
	}

	game_boy_logger_info("BROKER APPEARED_POKEMON");
	t_appeared_pokemon* appeared_snd = malloc(sizeof(t_appeared_pokemon));
	appeared_snd->nombre_pokemon = strcat(string_duplicate(arguments[2]), "\0");
	appeared_snd->tamanio_nombre = strlen(arguments[2]);
	appeared_snd->pos_x = atoi(arguments[3]);
	appeared_snd->pos_y = atoi(arguments[4]);
	appeared_snd->id_correlacional = atoi(arguments[5]);

	utils_serialize_and_send(game_boy_broker_fd, APPEARED_POKEMON, appeared_snd);
	usleep(500000);
}

void broker_catch_pokemon_command(char** arguments, int arguments_size) {
	if (arguments_size != 5) {
		return;
	}
	game_boy_logger_info("BROKER CATCH_POKEMON");
	t_catch_pokemon* catch_snd = malloc(sizeof(t_catch_pokemon));
	catch_snd->nombre_pokemon = strcat(string_duplicate(arguments[2]), "\0");
	catch_snd->tamanio_nombre = strlen(arguments[2]);
	catch_snd->pos_x = atoi(arguments[3]);
	catch_snd->pos_y = atoi(arguments[4]);

	utils_serialize_and_send(game_boy_broker_fd, CATCH_POKEMON, catch_snd);
	usleep(500000);
}

void broker_caught_pokemon_command(char** arguments, int arguments_size) {
	if (arguments_size != 4) {
		return;
	}
	game_boy_logger_info("BROKER CAUGHT_POKEMON");
	t_caught_pokemon* caught_snd = malloc(sizeof(t_caught_pokemon));
	caught_snd->id_correlacional = atoi(arguments[2]);
	char* ok_fail = strcat(string_duplicate(arguments[3]), "\0");
	int result = 0;
	if (string_equals_ignore_case(ok_fail, "ok")) {
		result = 1;
	}
	caught_snd->result = result;

	utils_serialize_and_send(game_boy_broker_fd, CAUGHT_POKEMON, caught_snd);
	usleep(500000);
}

void broker_get_pokemon_command(char** arguments, int arguments_size) {
	if (arguments_size != 3) {
		return;
	}
	game_boy_logger_info("BROKER GET_POKEMON");
	t_get_pokemon* get_snd = malloc(sizeof(t_get_pokemon));
	get_snd->nombre_pokemon = strcat(string_duplicate(arguments[2]), "\0");
	get_snd->tamanio_nombre = strlen(arguments[2]);

	utils_serialize_and_send(game_boy_broker_fd, GET_POKEMON, get_snd);
	usleep(500000);
}

void team_appeared_pokemon_command(char** arguments, int arguments_size) {
	if (arguments_size != 5) {
		return;
	}
	game_boy_logger_info("TEAM APPEARED_POKEMON");
	t_appeared_pokemon* appeared_snd = malloc(sizeof(t_appeared_pokemon));
	appeared_snd->nombre_pokemon = strcat(string_duplicate(arguments[2]), "\0");
	appeared_snd->tamanio_nombre = strlen(arguments[2]);
	appeared_snd->pos_x = atoi(arguments[3]);
	appeared_snd->pos_y = atoi(arguments[4]);

	utils_serialize_and_send(game_boy_team_fd, APPEARED_POKEMON, appeared_snd);
	usleep(500000);
}

void game_card_new_pokemon_command(char** arguments, int arguments_size) {
	if (arguments_size != 7) {
		return;
	}
	game_boy_logger_info("GAMECARD NEW_POKEMON");
	t_new_pokemon* new_snd = malloc(sizeof(t_new_pokemon));
	new_snd->nombre_pokemon = strcat(string_duplicate(arguments[2]), "\0");
	new_snd->tamanio_nombre = strlen(arguments[2]);
	new_snd->pos_x = atoi(arguments[3]);
	new_snd->pos_y = atoi(arguments[4]);
	new_snd->cantidad = atoi(arguments[5]);
	new_snd->id_correlacional = atoi(arguments[6]);

	utils_serialize_and_send(game_boy_game_card_fd, NEW_POKEMON, new_snd);
	usleep(500000);
}

void game_card_catch_pokemon_command(char** arguments, int arguments_size) {
	if (arguments_size != 6) {
		return;
	}
	game_boy_logger_info("GAMECARD CATCH_POKEMON");
	t_catch_pokemon* catch_snd = malloc(sizeof(t_catch_pokemon));
	catch_snd->nombre_pokemon = strcat(string_duplicate(arguments[2]), "\0");
	catch_snd->tamanio_nombre = strlen(arguments[2]);
	catch_snd->pos_x = atoi(arguments[3]);
	catch_snd->pos_y = atoi(arguments[4]);
	catch_snd->id_correlacional = atoi(arguments[5]);

	utils_serialize_and_send(game_boy_game_card_fd, CATCH_POKEMON, catch_snd);
	usleep(500000);
}

void game_card_get_pokemon_command(char** arguments, int arguments_size) {
	if (arguments_size != 3) {
		return;
	}
	game_boy_logger_info("GAMECARD GET_POKEMON");
	t_get_pokemon* get_snd = malloc(sizeof(t_get_pokemon));
	get_snd->nombre_pokemon = strcat(string_duplicate(arguments[2]), "\0");
	get_snd->tamanio_nombre = strlen(arguments[2]);

	utils_serialize_and_send(game_boy_game_card_fd, GET_POKEMON, get_snd);
	usleep(500000);
}

t_cola get_queue_by_name(char* cola) {
	if (string_equals_ignore_case(cola, "new_queue")) {
		return NEW_QUEUE;
	} else if (string_equals_ignore_case(cola, "appeared_queue")) {
		return APPEARED_QUEUE;
	} else if (string_equals_ignore_case(cola, "localized_queue")) {
		return LOCALIZED_QUEUE;
	} else if (string_equals_ignore_case(cola, "get_queue")) {
		return GET_QUEUE;
	} else if (string_equals_ignore_case(cola, "catch_queue")) {
		return CATCH_QUEUE;
	} else if (string_equals_ignore_case(cola, "caught_queue")) {
		return CAUGHT_QUEUE;
	} else {
		return -1;
	}
}

void subscriber_command(char** arguments, int arguments_size) {
	if (arguments_size != 3 || get_queue_by_name(arguments[1]) == -1) {
		return;
	}

	game_boy_logger_info("SUBSCRIBE");
	t_subscribe* sub_snd = malloc(sizeof(t_subscribe));
	t_protocol subscribe_protocol = SUBSCRIBE;
	sub_snd->proceso = GAME_BOY;
	sub_snd->ip = game_boy_config->ip_broker;
	sub_snd->puerto = game_boy_config->puerto_broker;
	sub_snd->cola = get_queue_by_name(arguments[1]);
	sub_snd->seconds = atoi(arguments[2]);
	utils_serialize_and_send(game_boy_broker_fd, subscribe_protocol, sub_snd);
	usleep(500000);

	int protocol;
	int received_bytes;

	while (true) {
		received_bytes = recv(game_boy_broker_fd, &protocol, sizeof(int), 0);

		if (received_bytes <= 0) {
			return;
		}

		switch (protocol) {

		case NEW_POKEMON: {
			game_boy_logger_info("Se recibio un NEW");
			t_new_pokemon *new_receive = utils_receive_and_deserialize(game_boy_broker_fd, protocol);

			t_protocol ack_protocol = ACK;

			t_ack* ack_send = malloc(sizeof(t_ack));
			ack_send->id_corr_msg = new_receive->id_correlacional;
			ack_send->queue = NEW_QUEUE;
			ack_send->sender_name = "GAMEBOY";
			ack_send->ip = game_boy_config->ip_broker;
			ack_send->port = game_boy_config->puerto_broker;

			utils_serialize_and_send(game_boy_broker_fd, ack_protocol, ack_send);

			usleep(100000);
			break;
		}

		case GET_POKEMON: {
			game_boy_logger_info("Se recibio un GET");
			t_get_pokemon *get_rcv = utils_receive_and_deserialize(game_boy_broker_fd, protocol);

			t_protocol ack_protocol = ACK;

			t_ack* ack_send = malloc(sizeof(t_ack));
			ack_send->id_corr_msg = get_rcv->id_correlacional;
			ack_send->queue = GET_QUEUE;
			ack_send->sender_name = "GAMEBOY";
			ack_send->ip = game_boy_config->ip_broker;
			ack_send->port = game_boy_config->puerto_broker;

			utils_serialize_and_send(game_boy_broker_fd, ack_protocol, ack_send);

			usleep(50000);
			break;
		}

		case CATCH_POKEMON: {
			game_boy_logger_info("Se recibio un CATCH");
			t_catch_pokemon *catch_rcv = utils_receive_and_deserialize(game_boy_broker_fd, protocol);

			t_protocol ack_protocol = ACK;

			t_ack* ack_send = malloc(sizeof(t_ack));
			ack_send->id_corr_msg = catch_rcv->id_correlacional;
			ack_send->queue = CATCH_QUEUE;
			ack_send->sender_name = "GAMEBOY";
			ack_send->ip = game_boy_config->ip_broker;
			ack_send->port = game_boy_config->puerto_broker;

			utils_serialize_and_send(game_boy_broker_fd, ack_protocol, ack_send);

			usleep(50000);
			break;
		}

		case CAUGHT_POKEMON: {
			game_boy_logger_info("Se recibio un CAUGHT");
			t_caught_pokemon *caught_rcv = utils_receive_and_deserialize(game_boy_broker_fd, protocol);

			t_protocol ack_protocol = ACK;

			t_ack* ack_send = malloc(sizeof(t_ack));
			ack_send->id_corr_msg = caught_rcv->id_correlacional;
			ack_send->queue = CAUGHT_QUEUE;
			ack_send->sender_name = "GAMEBOY";
			ack_send->ip = game_boy_config->ip_broker;
			ack_send->port = game_boy_config->puerto_broker;

			utils_serialize_and_send(game_boy_broker_fd, ack_protocol, ack_send);

			usleep(50000);
			break;
		}

		case NOOP: {
			return;
		}

		case LOCALIZED_POKEMON: {
			game_boy_logger_info("Se recibio un LOCALIZED");
			t_localized_pokemon *loc_rcv = utils_receive_and_deserialize(game_boy_broker_fd, protocol);

			t_protocol ack_protocol = ACK;

			t_ack* ack_send = malloc(sizeof(t_ack));
			ack_send->id_corr_msg = loc_rcv->id_correlacional;
			ack_send->queue = LOCALIZED_QUEUE;
			ack_send->sender_name = "GAMEBOY";
			ack_send->ip = game_boy_config->ip_broker;
			ack_send->port = game_boy_config->puerto_broker;

			utils_serialize_and_send(game_boy_broker_fd, ack_protocol, ack_send);

			usleep(500000);
			break;
		}

		case APPEARED_POKEMON: {
			game_boy_logger_info("Se recibio un APPEARED");
			t_appeared_pokemon *appeared_rcv = utils_receive_and_deserialize(game_boy_broker_fd, protocol);

			t_protocol ack_protocol = ACK;

			t_ack* ack_send = malloc(sizeof(t_ack));
			ack_send->id_corr_msg = appeared_rcv->id_correlacional;
			ack_send->queue = APPEARED_QUEUE;
			ack_send->sender_name = "GAMEBOY";
			ack_send->ip = game_boy_config->ip_broker;
			ack_send->port = game_boy_config->puerto_broker;

			utils_serialize_and_send(game_boy_broker_fd, ack_protocol, ack_send);

			usleep(50000);
			break;
		}

		default:
			break;
		}
	}

}

int game_boy_console_read(t_dictionary* command_actions, char* args[], int argcount) {
	char* input = game_boy_get_input(args, argcount);
	if (input == NULL)
		return 0;

	//string_to_upper(input);
	int arguments_size = game_boy_get_arguments_size(input);

	char** arguments = game_boy_get_arguments_from_input(input, arguments_size);

	char *concated_key = string_new();
	string_append(&concated_key, arguments[1]);
	if (!string_equals_ignore_case(concated_key, "SUBSCRIBE")) {
		string_append(&concated_key, " ");
		string_append(&concated_key, arguments[2]);
	}

	char* key = concated_key;

	char* to_send[argcount -1];

	for (int i=1; i<argcount; i++){
		to_send[i-1] = arguments[i];
	}

	if (string_equals_ignore_case(key, EXIT_KEY))
		return -1;
	else
		game_boy_command_execute(key, command_actions, to_send,
				arguments_size-1);

	free(arguments);
	free(input);

	return 0;
}

char* game_boy_get_input(char* args[], int argcount) {
	char *input = malloc(INPUT_SIZE);

	for(int i=0; i < argcount; i++) {
		input = strcat(input, " ");
		input = strcat(input, args[i]);
	}

	string_trim(&input);
	string_append(&input, "\0");

	if (string_is_empty(input)) {
		free(input);
		return NULL;
	}

	return input;
}

int game_boy_get_arguments_size(char *input) {
	char last_char = 'a';
	int count = 1;
	for (int i = 0; i < string_length(input); i++) {
		if (i != 0)
			last_char = input[i - 1];
		if (input[i] == ' ' && last_char != ' ')
			count++;
	}

	return count;
}

char** game_boy_get_arguments_from_input(char* input, int arguments_size) {
	char** arguments = malloc(sizeof(char*) * arguments_size);

	if (string_contains(input, SPLIT_CHAR))
		arguments = string_split(input, SPLIT_CHAR);
	else
		arguments[0] = string_duplicate(input);

	return arguments;
}

void game_boy_command_execute(char *key, t_dictionary* command_actions,
		char** arguments, int arguments_size) {
	t_command* command = dictionary_get(command_actions, key);
	if (command != NULL)
		command->action(arguments, arguments_size);
}

t_dictionary* game_boy_get_command_actions() {
	t_dictionary* command_actions = dictionary_create();

	t_command* broker_new_command = malloc(sizeof(t_command));
	broker_new_command->key = BROKER_NEW;
	broker_new_command->action = broker_new_pokemon_command;
	dictionary_put(command_actions, BROKER_NEW, broker_new_command);

	t_command* broker_appeared_command = malloc(sizeof(t_command));
	broker_appeared_command->key = BROKER_APPEARED;
	broker_appeared_command->action = broker_appeared_pokemon_command;
	dictionary_put(command_actions, BROKER_APPEARED, broker_appeared_command);

	t_command* broker_catch_command = malloc(sizeof(t_command));
	broker_catch_command->key = BROKER_CATCH;
	broker_catch_command->action = broker_catch_pokemon_command;
	dictionary_put(command_actions, BROKER_CATCH, broker_catch_command);

	t_command* broker_caught_command = malloc(sizeof(t_command));
	broker_caught_command->key = BROKER_CAUGHT;
	broker_caught_command->action = broker_caught_pokemon_command;
	dictionary_put(command_actions, BROKER_CAUGHT, broker_caught_command);

	t_command* broker_get_command = malloc(sizeof(t_command));
	broker_get_command->key = BROKER_GET;
	broker_get_command->action = broker_get_pokemon_command;
	dictionary_put(command_actions, BROKER_GET, broker_get_command);

	t_command* team_appeared_command = malloc(sizeof(t_command));
	team_appeared_command->key = TEAM_APPEARED;
	team_appeared_command->action = team_appeared_pokemon_command;
	dictionary_put(command_actions, TEAM_APPEARED, team_appeared_command);

	t_command* game_card_new_command = malloc(sizeof(t_command));
	game_card_new_command->key = GAMECARD_NEW;
	game_card_new_command->action = game_card_new_pokemon_command;
	dictionary_put(command_actions, GAMECARD_NEW, game_card_new_command);

	t_command* game_card_catch_command = malloc(sizeof(t_command));
	game_card_catch_command->key = GAMECARD_CATCH;
	game_card_catch_command->action = game_card_catch_pokemon_command;
	dictionary_put(command_actions, GAMECARD_CATCH, game_card_catch_command);

	t_command* game_card_get_command = malloc(sizeof(t_command));
	game_card_get_command->key = GAMECARD_GET;
	game_card_get_command->action = game_card_get_pokemon_command;
	dictionary_put(command_actions, GAMECARD_GET, game_card_get_command);

	t_command* subscribe_command = malloc(sizeof(t_command));
	subscribe_command->key = SUBSCRIBER;
	subscribe_command->action = subscriber_command;
	dictionary_put(command_actions, SUBSCRIBER, subscribe_command);

	return command_actions;
}

void command_destroyer(void* command) {
	free(command);
}

void game_boy_free_command_actions(t_dictionary* command_actions) {
	dictionary_destroy_and_destroy_elements(command_actions, command_destroyer);
}
