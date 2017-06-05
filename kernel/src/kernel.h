#ifndef KERNEL_H_
#define KERNEL_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <commons/string.h>
#include <commons/collections/list.h>
#include <commons/collections/queue.h>
#include <commons/collections/dictionary.h>
#include <commons/error.h>
#include <commons/log.h>
#include <console.h>
#include <config.h>
#include <socket.h>
#include <pcb.h>
#include "kernel_interface.h"

#include<exit_codes.h>

#define NEW_LIST 1
#define READY_LIST 2
#define EXEC_LIST 3
#define BLOCK_LIST 4
#define EXIT_LIST 5

#define FIFO 1
#define RR 2

#define CREATE "c"
#define READ "r"
#define WRITE "w"

#define PUERTO_PROG "PUERTO_PROG"
#define PUERTO_CPU "PUERTO_CPU"
#define IP_MEMORIA "IP_MEMORIA"
#define IP_FILESYSTEM "IP_FILESYSTEM"
#define PUERTO_MEMORIA "PUERTO_MEMORIA"
#define IP_FS "IP_FS"
#define PUERTO_FS "PUERTO_FS"
#define QUANTUM "QUANTUM"
#define QUANTUM_SLEEP "QUANTUM_SLEEP"
#define ALGORITMO "ALGORITMO"
#define GRADO_MULTIPROG "GRADO_MULTIPROG"
#define SEM_IDS "SEM_IDS"
#define SEM_INIT "SEM_INIT"
#define SHARED_VARS "SHARED_VARS"
#define STACK_SIZE "STACK_SIZE"
#define CONFIG_FIELDS_N 15

int p_counter;
int multiprog;
int quantum;
int stack_size;
int planning_alg;
t_dictionary* shared_vars;
t_dictionary* sem_ids;

t_queue* new_queue;
t_list* ready_list;
t_list* exec_list;
t_list* block_list;
t_queue* exit_queue;

int fs_response;
char* fs_buffer;


typedef struct {
	char* flag;
	int gfd;
	int pointer;
	int fd;
}t_open_file;

typedef struct{
	int pid;
	t_list* open_files;
}t_process_file_table;

typedef struct {
	char* path;
	int open;
	int gfd;
}t_global_file_table;

t_list* fs_global_table;
t_list* fs_process_table;


/*
typedef struct {
	int socket;
//	char* program;
//	pthread_t thread_info;
	pcb* pcb;
} t_program;
*/
//int last_socket;

typedef struct {
	bool busy;
	int socket;
} t_cpu;
t_list* cpu_list;

typedef struct {
	int socket;
	int pid;
	int state;
} t_socket_pcb;

t_socket_pcb process_struct;
t_list* socket_pcb_list;

pthread_mutex_t p_counter_mutex;
pthread_mutex_t console_mutex;
pthread_mutex_t pcb_list_mutex;
pthread_mutex_t socket_pcb_mutex;
pthread_mutex_t planning_mutex;
pthread_mutex_t cpu_mutex;
pthread_mutex_t process_in_memory_mutex;
pthread_mutex_t shared_vars_mutex;

pthread_mutex_t fs_mutex;

bool planning_running;

t_dictionary * fns;
t_log* logger;

int mem_page_size;
int process_in_memory;

void move_to_list(pcb* pcb, int list_name);
pcb* find_pcb_by_socket(int socket);
pcb* find_pcb_by_pid(int pid);
void remove_pcb_from_socket_pcb_list(pcb* n_pcb);
void remove_cpu_from_cpu_list(t_cpu* cpu);
t_cpu* find_cpu_by_socket(int socket);
t_socket_pcb* find_socket_by_pid(int pid);
void add_process_in_memory();
void substract_process_in_memory();
void short_planning();

void wait_response(pthread_mutex_t mutex);
void signal_response(pthread_mutex_t mutex);

/*
 * FILESYSTEM
 */
bool validate_file_from_fs(char* path);

//CAPA FILE SYSTEM
int open_file_in_process_table(char* path, char* flag, int pid);
bool close_file(int fd_close, int pid);
bool delete_file_from_global_table(int gfd);
bool write_file(int fd_write, int pid, char* info, int size);
t_list* add_defaults_fds();
bool is_default(int fd);

//SHOWS
void show_global_file_table();
void show_all_processes_file_table();
void show_process_file_table(int pid);


#endif /* KERNEL_H_ */
