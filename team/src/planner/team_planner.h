#ifndef PLANNER_TEAM_PLANNER_H_
#define PLANNER_TEAM_PLANNER_H_
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <semaphore.h>
#include <commons/collections/list.h>
#include <commons/collections/queue.h>
#include "../logger/team_logger.h"
#include "../config/team_config.h"
#include "../../../shared-common/common/sockets.h"
#include "../../../shared-common/common/utils.h"


typedef enum {
	NEW, READY, BLOCK, EXEC, EXIT
} e_state;

typedef struct {
	char* name;
	t_position* position;
} t_pokemon;

typedef struct {
	int id;
	e_state state;
	t_position* position;
	t_list* pokemons;
	t_list* targets;
	int current_burst_time;
	int total_burst_time;
	float estimated_burst;
	float previus_estimation;
	float previus_burst;
	bool status;
	pthread_t hilo_entrenador;
	pthread_mutex_t sem_move_trainers;
	t_list* list_id_catch;
	t_pokemon* pokemon_a_atrapar; 
	bool deadlock;
	int diferencia;
	bool se_movio;
} t_entrenador_pokemon;

typedef struct {
	char* name;
	t_list* pos;
} t_pokemon_received;

int team_socket;

sem_t sem_entrenadores_disponibles;
sem_t sem_message_on_queue;
sem_t sem_trainers_in_ready_queue;
sem_t sem_pokemons_to_get;
sem_t sem_planificador;

pthread_t planificator;
pthread_t algoritmo_cercania_entrenadores;

pthread_mutex_t cola_pokemons_a_atrapar;
pthread_mutex_t cola_exec;

t_list* new_queue;
t_list* ready_queue;
t_list* block_queue;
t_list* exit_queue;
t_list* exec_queue;
t_list* pokemon_to_catch;
t_list* total_targets_pokemons;
t_list* message_catch_sended;
t_list* pokemones_pendientes;
t_list* got_pokemons;
t_list* lista_auxiliar;
t_list* pokemons_localized;
t_list* real_targets_pokemons;
t_list* get_id_corr;

void team_planner_init();
void team_planner_destroy();
void team_planner_run_planification();
void team_planner_algoritmo_cercania();
void move_trainers_and_catch_pokemon();
void team_planner_finish_trainner(t_entrenador_pokemon*);
void team_planner_delete_from_bloqued_queue(t_entrenador_pokemon*, int);
void team_planner_new_cpu_cicle(t_entrenador_pokemon*);
void team_planner_add_to_ready_queue(t_entrenador_pokemon*);
void team_planner_delete_from_new_queue(t_entrenador_pokemon*);
void team_planner_entrenadores_listos();
void team_planner_print_fullfill_target();
void team_planner_solve_deadlock();
void team_planner_remove_pokemon_from_catch (t_pokemon*);
void team_planner_get_real_targets();
void team_planner_check_SJF_CD_time(t_entrenador_pokemon*);
void team_planner_check_RR_burst(t_entrenador_pokemon*);
void team_planner_destroy_pokemons(t_pokemon* pokemon);
void team_planner_destroy_entrenador(t_entrenador_pokemon* entrenador);
void team_planner_exit();
void team_planner_eliminar_pokemon_de_objetivos(t_list*, char*);
void team_planner_remove_from_pokemons_list(t_entrenador_pokemon*, t_pokemon*);
t_entrenador_pokemon* team_planner_set_algorithm();
t_entrenador_pokemon* team_planner_find_trainer_by_id_corr(uint32_t);
t_entrenador_pokemon* team_planner_entrenador_que_necesita(t_pokemon*);
t_entrenador_pokemon* team_planner_apply_RR();
t_entrenador_pokemon* team_planner_apply_FIFO();
t_entrenador_pokemon* team_planner_apply_SJF();
t_list* team_planner_create_ready_queue();
t_list* team_planner_get_trainners();
t_list* team_planner_trainers_waiting_messages();
t_list* team_planner_get_trainners();
t_list* team_planner_filter_by_deadlock();
t_list* team_planner_filter_by_deadlock();
t_list* team_planner_remover_de_lista (t_list*, t_pokemon*);
float team_planner_calculate_exponential_mean(t_entrenador_pokemon*);
t_pokemon* team_planner_ver_a_quien_no_necesita(t_entrenador_pokemon*, t_list*);
t_pokemon* team_planner_pokemon_create(char*);
bool team_planner_is_SJF_algorithm();
bool team_planner_block_queue_is_not_empty();
bool team_planner_trainer_completed_with_success(t_entrenador_pokemon*);
bool team_planner_all_queues_are_empty_except_block();
bool team_planner_all_finished();
int team_planner_get_least_estimate_index();
int team_planner_calcular_diferencia(t_entrenador_pokemon*);



#endif /* PLANNER_TEAM_PLANNER_H_ */
