#ifndef API_H
#define API_H

/*dimensão dos campos*/
#define CITY_ID 3 /* Tipo AAA*/
#define MAX_CITY_NAME 50
#define MAX_INDICE_LENGHT 6 /*entre 0.01 e 999.99*/
#define MAX_FILE_NAME 100   /*tamanho da string passada como argumento ao programa*/

/*MENSAGENS API*/
#define MSG_TOTAL_CITIES(TOTAL) printf("%d\n", TOTAL);
#define MSG_CITY_INFO_HEADER(CITY, STATUS, LINKS, NAME) printf("%s (%d): Existem %d ligações a partir de %s", CITY, STATUS, LINKS, NAME);
#define MSG_CITY_INFO_ITEM(TO_CITY, IH, IE, IT) printf("\t->%s: %.2f %.2f %.2f\n", TO_CITY, IH, IE, IT);
#define MSG_ROUTE_HEADER(FROM, TO, INDEX, VALUE) printf("%s->%s %c=%.2f\n", FROM, TO, INDEX, VALUE);
#define MSG_ROUTE_ITEM(PATH) printf("\t%s\n", PATH);
#define MSG_FILE_SAVED(filename) printf("SGO gravado no ficheiro %s\n", filename);

/*MENSAGENS DE ERRO*/
#define ERROR_CITY_REPEATED(CITY) printf("*Cidade %s duplicada\n", CITY);
#define ERROR_NO_CITY(CITY) printf("*Cidade %s inexistente\n", CITY);
#define ERROR_CITY_INACTIVE(CITY) printf("*Cidade %s inativa\n", CITY);
#define ERROR_NO_LINK(FROM, TO) printf("*Ligação %s->%s inexistente\n", FROM, TO);
#define ERROR_NO_ROUTE(FROM, TO) printf("*%s->%s sem ligação\n", FROM, TO);
#define ERROR_DB_EMPTY printf("*Base de dados vazia\n");
#define ERROR_FILE_EXTENSION(filename) printf("*Ficheiro %s não suportado\n", filename);

/*Strings formatadas para salvar sgo num ficheiro*/
#define ADD_CITY "A %s %s\n"
#define CHANGE_CITY_STATE "O %s %d\n"
#define ADD_LINK "C %s %s\n"
#define CHANGE_TURISTIC_INDEX "T %s %s %.2f\n"
#define CHANGE_ECONOMIC_INDEX "E %s %s %.2f\n"
#define CHANGE_TIME_INDEX "H %s %s %.2f\n"

/**
 * ESTRUTURA DE DADOS
 */

typedef struct lig
{

    struct lig *next;
    struct lig *prev;

    double indTemporal;
    double indEconomico;
    double indTuristico;

    char destino[CITY_ID + 1];
} Lig;

typedef struct cidade
{

    struct cidade *nextC;
    struct cidade *prevC;

    Lig *first;
    Lig *last;

    char id_cidade[CITY_ID + 1];

    char nome_cidade[MAX_CITY_NAME + 1]; /*fazer malloc para ficar com memoria dinamica */

    int estado;

    int numLig;

} Cidade;

typedef struct mapa
{

    int numero_cidades;

    Cidade *firstC;
    Cidade *lastC;

} Mapa;
typedef struct priorityQueueNodes
{
    int cityIndex;
    double priority;
} PriorityQueueNode;
typedef struct priorityQueue
{
    PriorityQueueNode *nodes;
    int capacity;
    int size;
} PriorityQueue;

/**
 * API - Declaração das funções públicas
 **/

Mapa *new_mapa();

void free_mapa(Mapa *m); /*Liberta memoria: */

void inserir_cidade(Mapa *m, char *codCidade, char *nome); /*Insere cidade: FEITO*/

void alterar_cidade(Mapa *m, char *codigoCidade, char *state); /*Altera cidade: FEITO*/

void apagar_cidade(Mapa *m, char *codigoCidade); /*Apaga cidade: FEITO*/

void info_cidade(Mapa *m, char *codigoCidade, char *infoCidade); /*Mostra info cidade: FEITO*/

void numero_cidade(Mapa *m); /*Mostra numero cidade: FEITO*/

void inserir_ligacao(Mapa *m, char *cidadeOrigem, char *cidadeDestino); /*Insere ligacao: FEITO */

void apagar_ligacao(Mapa *m, char *cidadeOrigem, char *cidadeDestino); /*Apaga ligacao: FEITO */

void alterar_ind_turistico(Mapa *m, char *cidadeOrigem, char *cidadeDestino, char *indiceTuristico); /*Altera indice turistico: FEITO */

void alterar_ind_economico(Mapa *m, char *cidadeOrigem, char *cidadeDestino, char *indiceEconomico); /*Altera indice turistico: FEITO*/

void alterar_ind_temporal(Mapa *m, char *cidadeOrigem, char *cidadeDestino, char *indiceTemporal); /*Altera indice temporal: FEITO */

void melhor_rota(Mapa *m, char *cOrigem, char *cDestino); /*Descobre a melhor rota: */

void guardar_dados(Mapa *m, char *filename); /*Guarda dados: FEITO*/

void listar_cidades(Mapa *m); /*Lista todas as cidades: FEITO*/

void sair_aplicacao(); /*Sai da aplicacao: FEITO*/

#endif