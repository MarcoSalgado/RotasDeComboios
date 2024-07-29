#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "api.h"

/**
 * ######### Funções internas da biblioteca #########
 * ##################################################
 **/

Mapa *new_mapa()
{
    Mapa *m = malloc(sizeof(Mapa));

    m->firstC = NULL;
    m->lastC = NULL;
    m->numero_cidades = 0;

    return m;
}

/* Procura a posição da cidade e organiza por ordem alfabética as cidades*/
Cidade *procura_pos(Mapa *m, char *codigoCidade)
{
    Cidade *atual = m->firstC;
    int valorTemp = 0, valorCod = 0;
    while (atual)
    {
        int i = 0;
        for (i = 0; i < strlen(codigoCidade); i++)
        {
            valorTemp = atual->id_cidade[i];
            valorCod = codigoCidade[i];
            if (valorTemp == valorCod)
            {
                continue;
            }
            if (valorCod < valorTemp)
            {
                return atual;
            }
            else
            {
                break;
            }
        }

        atual = atual->nextC;
    }

    return NULL;
}

/* Procura cidade e retorna a estrura da cidade recebida no parametro */
Cidade *procura_cidade(Mapa *m, char *codigoCidade)
{
    Cidade *atual = m->firstC;
    while (atual)
    {
        if (strcmp(atual->id_cidade, codigoCidade) == 0)
        {
            return atual;
        }
        atual = atual->nextC;
    }
    return NULL;
}

/* Procura a ligação entre as cidades e retorna a estrura da ligação recebida no parametros */
Lig *procura_ligacao(Mapa *m, Cidade *cidadeOrigem, Cidade *cidadeDestino)
{
    Lig *ligacao = cidadeOrigem->first;

    while (ligacao != NULL)
    {
        if (strcmp(ligacao->destino, cidadeDestino->id_cidade) == 0)
        {
            return ligacao;
        }

        ligacao = ligacao->next;
    }

    return NULL;
}

void free_mapa(Mapa *m)
{
    Cidade *atual = m->firstC;
    while (atual != NULL)
    {
        Lig *ligacao = atual->first;
        while (ligacao != NULL)
        {
            Lig *temp = ligacao;
            ligacao = ligacao->next;
            free(temp);
        }
        
        Cidade *temp = atual;
        atual = atual->nextC;
        free(temp);
    }
    
    free(m);
}

void inserir_cidade(Mapa *m, char *codCidade, char *nome)
{

    Cidade *atual = procura_cidade(m, codCidade);
    if (atual)
    {
        ERROR_CITY_REPEATED(atual->id_cidade);
        return;
    }

    atual = malloc(sizeof(Cidade));
    m->numero_cidades++;
    strcpy(atual->id_cidade, codCidade);
    strcpy(atual->nome_cidade, nome);
    atual->estado = 1;
    atual->numLig = 0;

    /* Ver se a cidade está em primeiro  */
    if (!m->firstC)
    {
        atual->prevC = NULL;
        atual->nextC = NULL;
        m->firstC = atual;
        m->lastC = atual;

        return;
    }

    Cidade *pos = procura_pos(m, codCidade);
    /* Ver se a cidade está no meio  */
    if (pos == NULL)
    {
        atual->nextC = NULL;
        atual->prevC = m->lastC;
        m->lastC->nextC = atual;
        m->lastC = atual;
        return;
    }

    /* Ver se a cidade está no fim  */
    if (pos == m->firstC)
    {
        m->firstC->prevC = atual;
        atual->nextC = m->firstC;
        atual->prevC = NULL;
        m->firstC = atual;
        return;
    }

    /* Passa para a proxima cidade */
    atual->nextC = pos;
    atual->prevC = pos->prevC;
    pos->prevC->nextC = atual;
    pos->prevC = atual;
}

void numero_cidade(Mapa *m)
{
    printf("%d\n", m->numero_cidades);
}

/* Apresenta as cidades ... */
void listar_cidades(Mapa *m)
{
    if (m->numero_cidades == 0)
    {
        ERROR_DB_EMPTY;
        return;
    }

    Cidade *atual = m->firstC;
    while (atual)
    {
        MSG_CITY_INFO_HEADER(atual->id_cidade, atual->estado, atual->numLig, atual->nome_cidade);
        atual = atual->nextC;
    }

    return;
}

void alterar_cidade(Mapa *m, char *codigoCidade, char *state)
{
    Cidade *cidade = procura_cidade(m, codigoCidade);

    if (cidade == NULL)
    {
        ERROR_NO_CITY(codigoCidade);
        return;
    }

    int novoEstado = atoi(state);

    if (novoEstado < 0 || novoEstado > 1)
    {
        printf("Estado inválido. Deve ser 0 (inativa) ou 1 (ativa).\n");
        return;
    }

    cidade->estado = novoEstado;
}

void info_cidade(Mapa *m, char *codigoCidade, char *infoCidade)
{
    int estado = 0;
    Cidade *atual = procura_cidade(m, codigoCidade);

    if (atual == NULL)
    {
        ERROR_NO_CITY(codigoCidade);
        return;
    }
    Lig *ligacao = atual->first;
    estado = atoi(infoCidade);
    MSG_CITY_INFO_HEADER(atual->id_cidade, atual->estado, atual->numLig, atual->nome_cidade);

    if (estado == 1)
    {
        while (ligacao)
        {
            MSG_CITY_INFO_ITEM(ligacao->destino, ligacao->indTemporal, ligacao->indEconomico, ligacao->indTuristico);
            ligacao = ligacao->next;
        }
    }
}

void apagar_cidade(Mapa *m, char *codigoCidade)
{

    Cidade *cidade = procura_cidade(m, codigoCidade);

    if (cidade == NULL)
    {
        ERROR_NO_CITY(codigoCidade);
        return;
    }

    if (cidade->prevC != NULL)
    {
        cidade->prevC->nextC = cidade->nextC;
    }

    else
    {
        m->firstC = cidade->nextC;
    }

    if (cidade->nextC != NULL)
    {
        cidade->nextC->prevC = cidade->prevC;
    }
    else
    {
        m->lastC = cidade->prevC;
    }

    // Remove a cidade
    Lig *ligacao = cidade->first;
    while (ligacao != NULL)
    {
        Lig *nextLigacao = ligacao->next;
        free(ligacao);
        ligacao = nextLigacao;
    }
    m->numero_cidades--;
    Cidade *outraCidade = m->firstC;
    while (outraCidade != NULL)
    {
        Lig *ligacao = outraCidade->first;
        while (ligacao != NULL)
        {
            if (strcmp(ligacao->destino, codigoCidade) == 0)
            {
                Lig *ligacaoToDelete = ligacao;
                ligacao = ligacao->next;

                
                if (ligacaoToDelete->prev != NULL)
                {
                    ligacaoToDelete->prev->next = ligacaoToDelete->next;
                }
                else
                {
                    outraCidade->first = ligacaoToDelete->next;
                }

                if (ligacaoToDelete->next != NULL)
                {
                    ligacaoToDelete->next->prev = ligacaoToDelete->prev;
                }
                else
                {
                    outraCidade->last = ligacaoToDelete->prev;
                }

                free(ligacaoToDelete);
                outraCidade->numLig--;
            }
            else
            {
                ligacao = ligacao->next;
            }
        }

        outraCidade = outraCidade->nextC;
    }
}


void inserir_ligacao(Mapa *m, char *cidadeOrigem, char *cidadeDestino)
{
    Cidade *origem = procura_cidade(m, cidadeOrigem);
    if (origem == NULL)
    {
        ERROR_NO_CITY(cidadeOrigem);
        return;
    }

    Cidade *destino = procura_cidade(m, cidadeDestino);
    if (destino == NULL)
    {
        ERROR_NO_CITY(cidadeDestino);
        return;
    }

    Lig *ligacao = origem->first;
    while (ligacao)
    {
        if (strcmp(ligacao->destino, cidadeDestino) == 0)
        {
            ERROR_CITY_REPEATED(cidadeDestino);
            return;
        }
        ligacao = ligacao->next;
    }

    Lig *novaLigacao = malloc(sizeof(Lig));
    strcpy(novaLigacao->destino, cidadeDestino);
    novaLigacao->indTemporal = 1.0;
    novaLigacao->indEconomico = 1.0;
    novaLigacao->indTuristico = 1.0;
    novaLigacao->next = NULL;
    novaLigacao->prev = NULL;

    if (origem->first == NULL)
    {
        origem->first = novaLigacao;
        origem->last = novaLigacao;
    }
    else
    {
        novaLigacao->prev = origem->last;
        origem->last->next = novaLigacao;
        origem->last = novaLigacao;
    }

    origem->numLig++;
}

void apagar_ligacao(Mapa *m, char *cidadeOrigem, char *cidadeDestino)
{
    Cidade *origem = procura_cidade(m, cidadeOrigem);
    Cidade *destino = procura_cidade(m, cidadeDestino);

    if (origem == NULL)
    {
        ERROR_NO_CITY(cidadeOrigem);
        return;
    }

    if (destino == NULL)
    {
        ERROR_NO_CITY(cidadeDestino);
        return;
    }

    Lig *ligacao = origem->first;

    while (ligacao != NULL)
    {
        if (strcmp(ligacao->destino, cidadeDestino) == 0)
        {
            if (ligacao->prev != NULL)
            {
                ligacao->prev->next = ligacao->next;
            }
            else
            {
                origem->first = ligacao->next;
            }

            if (ligacao->next != NULL)
            {
                ligacao->next->prev = ligacao->prev;
            }
            else
            {
                origem->last = ligacao->prev;
            }

            free(ligacao);
            origem->numLig--;
            return;
        }

        ligacao = ligacao->next;
    }

    ERROR_NO_LINK(cidadeOrigem, cidadeDestino);
}

void alterar_ind_economico(Mapa *m, char *cidadeOrigem, char *cidadeDestino, char *indiceEconomico)
{
    Cidade *cOrigem = procura_cidade(m, cidadeOrigem);

    if (cOrigem == NULL)
    {
        ERROR_NO_CITY(cidadeOrigem);
    }

    Cidade *cDestino = procura_cidade(m, cidadeDestino);

    if (cDestino == NULL)
    {
        ERROR_NO_CITY(cidadeDestino);
    }

    if(cOrigem == NULL || cDestino == NULL)
        return;

    Lig *ligacao = procura_ligacao(m, cOrigem, cDestino);

    if (ligacao == NULL)
    {
        ERROR_NO_LINK(cOrigem->id_cidade, cDestino->id_cidade);
        return;
    }

    float indice = 0;
    indice = atof(indiceEconomico);
    ligacao->indEconomico = indice;
}

void alterar_ind_turistico(Mapa *m, char *cidadeOrigem, char *cidadeDestino, char *indiceTuristico)
{
    Cidade *cOrigem = procura_cidade(m, cidadeOrigem);

    if (cOrigem == NULL)
    {
        ERROR_NO_CITY(cidadeOrigem);
    }

    Cidade *cDestino = procura_cidade(m, cidadeDestino);

    if (cDestino == NULL)
    {
        ERROR_NO_CITY(cidadeDestino);
    }

    if (cOrigem == NULL || cDestino == NULL)
    {
        return;
    }

    Lig *ligacao = procura_ligacao(m, cOrigem, cDestino);

    if (ligacao == NULL)
    {
        ERROR_NO_LINK(cOrigem->id_cidade, cDestino->id_cidade);
        return;
    }

    float indice = 0;
    indice = atof(indiceTuristico);
    ligacao->indTuristico = indice;
}

void alterar_ind_temporal(Mapa *m, char *cidadeOrigem, char *cidadeDestino, char *indiceTemporal)
{
    Cidade *cOrigem = procura_cidade(m, cidadeOrigem);

    if (cOrigem == NULL)
    {
        ERROR_NO_CITY(cidadeOrigem);
    }

    Cidade *cDestino = procura_cidade(m, cidadeDestino);

    if (cDestino == NULL)
    {
        ERROR_NO_CITY(cidadeDestino);
    }

    if (cOrigem == NULL || cDestino == NULL)
    {
        return;
    }

    Lig *ligacao = procura_ligacao(m, cOrigem, cDestino);

    if (ligacao == NULL)
    {
        ERROR_NO_LINK(cOrigem->id_cidade, cDestino->id_cidade);
        return;
    }

    float indice = 0;
    indice = atof(indiceTemporal);
    ligacao->indTemporal = indice;
}

/* Guardar dados no ficheiro */

void guardar_dados(Mapa *m, char *filename)
{   
    /* Se o utlizador não passar o nome do ficheiro, por default cria -> data.sgo */
    if (filename == NULL)
    {
        filename = "data.sgo";
    }

    /* Cria ligação com o ficheiro em modo de escrita */
    FILE *arquivo = fopen(filename, "w");

    Cidade *atual = m->firstC;

    /* A - Adiciona uma Cidade */
    while (atual != NULL)
    {
        fprintf(arquivo, "A %s %s", atual->id_cidade, atual->nome_cidade);
        atual = atual->nextC;
    }

    /* Atual volta a apontar para a primeira cidade */
    atual = m->firstC;


    /* C -> Adiciona uma ligação entre cidades */
    while (atual != NULL)
    {
        Lig *ligacao = atual->first;
        while (ligacao != NULL)
        {
            fprintf(arquivo, "C %s %s\n", atual->id_cidade, ligacao->destino);
            ligacao = ligacao->next;
        }

        atual = atual->nextC;
    }

    atual = m->firstC;

    /* T -> Altera o Indice Turístico de uma ligação  */
    while (atual != NULL)
    {
        Lig *ligacao = atual->first;
        while (ligacao != NULL)
        {
            if (ligacao->indTuristico != 1)
            {
                fprintf(arquivo, "T %s %s %.2lf\n", atual->id_cidade, ligacao->destino, ligacao->indTuristico);
            }
            ligacao = ligacao->next;
        }

        atual = atual->nextC;
    }

    atual = m->firstC;

    /* E -> Altera o Indice Economico de uma ligação  */
    while (atual != NULL)
    {
        Lig *ligacao = atual->first;
        while (ligacao != NULL)
        {
            if (ligacao->indEconomico != 1)
            {
                fprintf(arquivo, "E %s %s %.2lf\n", atual->id_cidade, ligacao->destino, ligacao->indEconomico);
            }
            ligacao = ligacao->next;
        }

        atual = atual->nextC;
    }

    atual = m->firstC;

    /* H -> Altera o Indice Temporal de uma ligação  */
    while (atual != NULL)
    {
        Lig *ligacao = atual->first;
        while (ligacao != NULL)
        {
            if (ligacao->indTemporal != 1)
            {
                fprintf(arquivo, "H %s %s %.2lf\n", atual->id_cidade, ligacao->destino, ligacao->indTemporal);
            }
            ligacao = ligacao->next;
        }

        atual = atual->nextC;
    }

    atual = m->firstC;

    fclose(arquivo);
    MSG_FILE_SAVED(filename);
}

void sair_aplicacao()
{
    exit(EXIT_SUCCESS);
}

/**
 * ######### Implementação das Funções da Biblioteca#########
 * ###########################################################
 **/