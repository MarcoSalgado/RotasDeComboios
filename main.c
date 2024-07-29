#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "api.h"

/* Pela Análise da API*/
#define MAX_LINE 56

/* atributos da linha de comandos*/
char *attr1, *attr2, *attrnome, *attrindice;
char *comando; /*recebe uma linha do stdin*/
char indice[MAX_INDICE_LENGHT + 1];
/*ficheiro de dados*/
char *fileName;

void executeCommand(Mapa *m, char *line)
{
    switch (line[0])
    {
    case 'A': /*Adiciona Cidade*/
        memset(attr1, '\0', CITY_ID + 1);
        memset(attrnome, '\0', MAX_CITY_NAME + 1);
        strncpy(attr1, line + 2, CITY_ID);
        strncpy(attrnome, line + 6, MAX_CITY_NAME);
        inserir_cidade(m, attr1, attrnome);
        break;
    case 'O': /*Altera estado da cidade*/
        memset(attr1, '\0', CITY_ID + 1);
        strncpy(attr1, &line[2], CITY_ID);
        memset(attr2, '\0', CITY_ID + 1);
        strncpy(attr2, &line[6], 1);
        alterar_cidade(m, attr1, attr2);
        break;
    case 'Y': /*Informação sobre a cidade*/
        memset(attr1, '\0', CITY_ID + 1);
        strncpy(attr1, &line[2], CITY_ID);
        memset(attr2, '\0', MAX_INDICE_LENGHT + 1);
        strncpy(attr2, &line[6], MAX_INDICE_LENGHT);
        info_cidade(m, attr1, attr2);
        break;
    case 'Z': /*Lista Todas as Cidades*/
        listar_cidades(m);
        break;
    case 'N': /*Lista Todas as Cidades*/
        numero_cidade(m);
        break;
    case 'C': /*Cria ligação entre cidades*/
        memset(attr1, '\0', CITY_ID + 1);
        strncpy(attr1, line + 2, CITY_ID);
        memset(attr2, '\0', CITY_ID + 1);

        strncpy(attr2, line + 6, CITY_ID);
        inserir_ligacao(m, attr1, attr2);
        break;
    case 'I': /*apaga ligação entre cidades*/
        strncpy(attr2, line + 6, CITY_ID);
        strncpy(attr1, line + 2, CITY_ID);
        apagar_ligacao(m, attr1, attr2);
        break;
    case 'T': /*Altera indice turistico*/

        memset(attr1, '\0', CITY_ID + 1);
        memset(attr2, '\0', CITY_ID + 1);
        memset(indice, '\0', MAX_INDICE_LENGHT + 1);
        strncpy(attr1, line + 2, CITY_ID);
        strncpy(attr2, line + 6, CITY_ID);
        strncpy(indice, line + 10, MAX_INDICE_LENGHT);
        alterar_ind_turistico(m, attr1, attr2, indice);
        break;
    case 'E': /*altera indice económico*/

        memset(attr1, '\0', CITY_ID + 1);
        memset(attr2, '\0', CITY_ID + 1);
        memset(indice, '\0', MAX_INDICE_LENGHT + 1);
        strncpy(attr1, line + 2, CITY_ID);
        strncpy(attr2, line + 6, CITY_ID);
        strncpy(indice, line + 10, MAX_INDICE_LENGHT);
        alterar_ind_economico(m, attr1, attr2, indice);
        break;
    case 'H': /*Altera indice temporal*/

        memset(attr1, '\0', CITY_ID + 1);
        memset(attr2, '\0', CITY_ID + 1);
        memset(indice, '\0', MAX_INDICE_LENGHT + 1);
        strncpy(attr1, line + 2, CITY_ID);
        strncpy(attr2, line + 6, CITY_ID);
        strncpy(indice, line + 10, MAX_INDICE_LENGHT);
        alterar_ind_temporal(m, attr1, attr2, indice);
        break;
    case 'P': /*Apaga Cidade*/
        memset(attr1, '\0', CITY_ID + 1);
        strncpy(attr1, &line[2], CITY_ID);
        apagar_cidade(m, attr1);
        break;
    case 'R': /*Melhor Rota entre duas cidades
        memset(attr1, '\0', CITY_ID + 1);
        strncpy(attr1, &line[2], CITY_ID);
        memset(attr2, '\0', CITY_ID + 1);
        strncpy(attr2, &line[6], CITY_ID);
        memset(attrindice, '\0', MAX_INDICE_LENGHT + 1);
        strncpy(attrindice, &line[10], MAX_INDICE_LENGHT);
        calculateRoute(m, attr1, attr2, attrindice);*/
        break;
    case 'G': /*Guarda a base de dados*/
        sscanf(line, "G %s", fileName);
        if (fileName[0] == '\0')
        {
            guardar_dados(m, NULL);
        }
        else
        {
            // Check if the provided file name has the correct extension
            char *extension = strrchr(fileName, '.');
            if (extension == NULL || (strcmp(extension, ".sgo") != 0))
            {
                printf("Ficheiro %s não suportado\n", fileName);
                break;
            }
            guardar_dados(m, fileName);
        }

        break;
    case 'X': /*sai da aplicação não esquecer libertar a memória*/
        free_mapa(m);
        sair_aplicacao();
        break;
    case '#': /*Comentário não executar*/
        break;
    default:
        break;
    }
}

void importDataFile(Mapa *m) /*Abre o ficheiro e executa as linhas para carregar o programa*/
{
    FILE *f;
    f = fopen(fileName, "r");
    /*caso ficheiro não exista*/
    if (!f)
        return;

    while (fgets(comando, MAX_LINE, f))
    {
        executeCommand(m, comando);
        memset(comando, '\0', MAX_LINE * sizeof(char) + 1);
    }
    fclose(f);
}

int main(int argv, char *argc[])
{
    char *extension;

    /*FIX ME - Inicia SGO*/

    Mapa *m = new_mapa();

    /*##### NÃO ALTERAR DAQUI PARA BAIXO #####*/
    /*Aloca memória para receber atributos*/
    comando = malloc(MAX_LINE * sizeof(char) + 1); /*recebe uma linha do stdin*/
    attr1 = malloc(CITY_ID * sizeof(char) + 1);
    attr2 = malloc(CITY_ID * sizeof(char) + 1); /*tb utilizado para ler o estado*/
    attrnome = malloc(MAX_CITY_NAME * sizeof(char) + 1);
    attrindice = malloc(MAX_INDICE_LENGHT * sizeof(char) + 1);
    fileName = malloc(MAX_FILE_NAME + 1);

    /*Caso tenha sido passado ficheiro pela linha de comando carrega*/
    if (argv == 2)
    {
        strncpy(fileName, argc[1], MAX_FILE_NAME);
        if (strlen(fileName) < 4)
        {
            ERROR_FILE_EXTENSION(fileName);
            memset(fileName, '\0', MAX_FILE_NAME);
            strncpy(fileName, "data.sgo", MAX_FILE_NAME);
        }
        else
        {
            extension = &fileName[strlen(fileName) - 4];
            if (strcmp(extension, ".sgo"))
            {
                ERROR_FILE_EXTENSION(fileName);
                memset(fileName, '\0', MAX_FILE_NAME);
                strncpy(fileName, "data.sgo", MAX_FILE_NAME);
            }
            else
            {
                importDataFile(m);
            }
        }
    }
    else
        strncpy(fileName, "data.sgo", MAX_FILE_NAME);

    while (fgets(comando, MAX_LINE, stdin))
    {
        executeCommand(m, comando);
        memset(comando, '\0', MAX_LINE * sizeof(char) + 1);
    }
    return EXIT_SUCCESS;
}
