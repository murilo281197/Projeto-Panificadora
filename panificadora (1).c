#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>

struct Panificadora {
    int codigo;
    char nome[22];
    int quantidadeProdutos;
    float despesas;
    int erase;
} p;

FILE *arquivo;
FILE *backup;
int quantidadeErase = 0;

void cadastrarPanificadora();
void consultarTodasPanificadoras();
void consultarTodasPanificadorasDespesas();
void alterarDados();
void exclusaoLogica();
void exclusaoFisica();
void ordenacaoNomeCrescente();
void buscaBinaria();
void recuperarBackup();

int main (void) {

    char opcao;

    do{
        system("cls");
        printf ("\n 1 - Cadastro");
        printf ("\n 2 - Consulta todas as panificadoras");
        printf ("\n 3 - Consulta todas as panificadoras com despesas entre 40 mil e 200 mil");
        printf ("\n 4 - Alteracao");
        printf ("\n 5 - Exclusao Logica");
        printf ("\n 6 - Exclusao Fisica");
        printf ("\n 7 - Ordenacao pelo nome em ordem crescente");
        printf ("\n 8 - Busca Binaria pelo codigo");
        printf ("\n 9 - Recuperar Backup de todas as panificadoras");
        printf ("\n 0 - Sair ");
        printf ("\n Opcao: ");
        opcao = getche();
        switch(opcao){
            case '1':
                cadastrarPanificadora();
                break;
            case '2':
                consultarTodasPanificadoras();
                break;
            case '3':
                consultarTodasPanificadorasDespesas();
                break;
            case '4':
                alterarDados();
                break;
            case '5':
                exclusaoLogica();
                break;
            case '6':
                exclusaoFisica();
                break;
            case '7':
                ordenacaoNomeCrescente();
                break;
            case '8':
                buscaBinaria();
                break;
            case '9':
                recuperarBackup();
                break;
			case '0':
				printf("\n saindo ...\n");
                break;
            default :
                printf("\n opcao %c invalida  ",opcao);
                printf("\n apertar o teclado");
                getch();
                break;
        }
    }while (opcao != '0');

    return 0;
}

void cadastrarPanificadora() {

    char opcao;

    if ((arquivo = fopen("panificadora.dados", "rb+")) == NULL ) {
        if ((arquivo = fopen("panificadora.dados", "wb")) == NULL) {
            printf ("Erro na abertura!\n");
            exit(1);
        }
    }

    do {
        setbuf(stdin, NULL);
        printf ("\nEntre com o nome da panificadora: ");
        scanf ("%[^\n]", p.nome);

        printf ("Entre com o codigo da panificadora: ");
        scanf ("%d", &p.codigo);

        printf ("Entre com a quantidade de produtos: ");
        scanf ("%d", &p.quantidadeProdutos);

        printf ("Entre com o valor das despesas: ");
        scanf ("%f", &p.despesas);

        p.erase = 0;
        fseek(arquivo, 0, 2);
        fwrite(&p, sizeof(p), 1, arquivo);
        printf ("Deseja cadastrar outro (S/N): ");
        opcao = getche();
    } while (opcao == 'S' || opcao == 's');
    fclose(arquivo);
}

void consultarTodasPanificadoras() {

    if ((arquivo = fopen("panificadora.dados", "rb")) == NULL) {
        printf ("\nErro\n");
        exit(1);
    }
    fseek(arquivo, 0, 0);
    while (fread(&p, sizeof(p), 1, arquivo)) {
        if (p.erase == 0) {
            printf ("\nNome: %s \n", p.nome);
            printf ("Codigo: %d\n", p.codigo);
            printf ("Despesas: R$ %.2f\n", p.despesas);
            printf ("Quantidade de produtos: %d\n", p.quantidadeProdutos);
        }
    }
    getche();
    fclose(arquivo);
}

void consultarTodasPanificadorasDespesas() {

    int quantidade = 0;

    if ((arquivo = fopen("panificadora.dados", "rb")) == NULL) {
        printf ("\nErro\n");
        exit(1);
    }
    fseek(arquivo, 0, 0);
    while (fread(&p, sizeof(p), 1, arquivo)) {
        if (p.erase == 0) {
            if (p.despesas >= 40000 && p.despesas <= 200000) {
                printf ("\nNome: %s \n", p.nome);
                printf ("Codigo: %d\n", p.codigo);
                printf ("Despesas: R$ %.2f\n", p.despesas);
                printf ("Quantidade de produtos: %d\n", p.quantidadeProdutos);
                quantidade++;
            }
        }
    }
    if (quantidade  == 0) {
        printf ("\nNao existem panificadores com despesas entre 40 mil e 200 mil\n");
    }
    getche();
    fclose(arquivo);
}

void alterarDados() {

    int codigoAux;
    int aux = 0;
    if ((arquivo = fopen("panificadora.dados", "rb+")) == NULL) {
        printf ("\nErro");
        exit(1);
    }
    printf ("\nEntre com o codigo da panificadora para realizar alteracao: ");
    scanf ("%d", &codigoAux);

    fseek(arquivo, 0, 0);
    while (fread(&p, sizeof(p), 1, arquivo)){
        if (p.codigo == codigoAux) {
            printf ("Codigo encontrado!\n");
            aux++;
            if (p.erase == 0) {
                printf ("Entre com o novo nome: ");
                scanf ("%s", p.nome);
                printf ("Entre com o novo valor de despesa: ");
                scanf ("%f", &p.despesas);
                printf ("Entre com o novo valor de quantidade de produtos: ");
                scanf ("%d", &p.quantidadeProdutos);
                fseek(arquivo, ftell(arquivo) - sizeof(p), 0);
                fwrite(&p, sizeof(p), 1, arquivo);
                fseek(arquivo, 0, 2);
            }
        }
    }
    if (aux == 0) {
        printf ("\nCodigo nao encontrado!\n");
        getche();
    } else {
        printf ("\nDados alterados com sucesso!\n");
    }
    fclose(arquivo);
}

void exclusaoLogica() {

    int codigoAux;
    if ((arquivo = fopen("panificadora.dados", "rb+")) == NULL) {
        printf ("\nErro");
        exit(1);
    }
    printf ("\nEntre com o codigo da panificadora para ser excluida logicamente: ");
    scanf ("%d", &codigoAux);

    while (fread(&p, sizeof(p), 1, arquivo)) {
        if (p.codigo == codigoAux) {
            if (p.erase == 0) {
                p.erase = 1;
                quantidadeErase++;
                fseek(arquivo, ftell(arquivo) - sizeof(p), 0);
                fwrite(&p, sizeof(p), 1, arquivo);
                fseek(arquivo, 0, 2);
                printf ("\nPanificadora (%d) excluida logicamente!\n", codigoAux);
            }
        }
    }
    fclose(arquivo);
    getche();
}

void exclusaoFisica() {

    FILE *auxiliar;
    int excluidos = 0;
    if ((arquivo = fopen("panificadora.dados", "rb")) == NULL) {
        printf ("\nErro");
        exit(1);
    }
    if ((auxiliar = fopen("auxiliar.dados", "wb")) == NULL) {
        printf ("\nErro");
        exit(1);
    }
    if ((backup = fopen("backup.dados", "rb+")) == NULL) {
        if ((backup = fopen("backup.dados", "wb")) == NULL) {
            printf ("\nErro");
            exit(1);
        }
    }
    fseek(arquivo, 0, 0);
    while (fread(&p, sizeof(p), 1, arquivo)) {
        if (p.erase == 0) {
            fseek(auxiliar, 0, 2);
            fwrite(&p, sizeof(p), 1, auxiliar);
            fseek(backup, 0, 2);
            fwrite(&p, sizeof(p), 1, backup);
        } else {
            excluidos++;
            fseek(backup, 0, 2);
            fwrite(&p, sizeof(p), 1, backup);
        }
    }

    if (excluidos > 0) {
        printf ("\nExclusao fisica realizada com sucesso!\n");
    } else {
        printf ("\nErro na exclusao fisica!\n");
    }

    fclose(arquivo);
    fclose(auxiliar);
    fclose(backup);
    remove("panificadora.dados");
    rename("auxiliar.dados", "panificadora.dados");
    getche();
}

void ordenacaoNomeCrescente() {

    struct Panificadora pi, pj;
    int quantidade;

    if ((arquivo = fopen("panificadora.dados", "rb+")) == NULL) {
        printf ("\nErro");
        exit(1);
    }
    fseek(arquivo, 0, 2);
    quantidade = ftell(arquivo) / sizeof(p);

    for (int i = 0; i < quantidade - 1; i++) {
        for (int j = i + 1; j < quantidade; j++) {
            fseek(arquivo, i * sizeof(p), 0);
            fread(&pi, sizeof(p), 1, arquivo);
            fseek(arquivo, j * sizeof(p), 0);
            fread(&pj, sizeof(p), 1, arquivo);
            if (strcmp(pi.nome, pj.nome) > 0) {
                fseek(arquivo, i * sizeof(p), 0);
                fwrite(&pj, sizeof(p), 1, arquivo);
                fseek(arquivo, j * sizeof(p), 0);
                fwrite (&pi, sizeof(p), 1, arquivo);
            }
        }
    }
    fclose(arquivo);
}

void buscaBinaria() {

    int codigoAux;
    struct Panificadora pmid;
    int quantidade;

    if ((arquivo = fopen("panificadora.dados", "rb+")) == NULL) {
        printf ("\nErro");
        exit(1);
    }
    fseek(arquivo, 0, 2);
    quantidade = ftell(arquivo) / sizeof(p);
    printf ("\nEntre com o codigo a ser procurado: ");
    scanf ("%d", &codigoAux);

    int vetor[quantidade];

    for (int i = 0; i < quantidade; i++) {
        fseek(arquivo, (i * sizeof(p)), 0);
        fread(&pmid, sizeof(p), 1, arquivo);
        vetor[i] = pmid.codigo;
    }

    for (int i = 0; i < quantidade; i++) {
        if (vetor[i] == codigoAux) {
            printf ("\nCodigo encontrado!\n", vetor[i]);
            fseek(arquivo, (i * sizeof(p)), 0);
            fread(&pmid, sizeof(p), 1, arquivo);
            printf ("\nNome: %s", pmid.nome);
            printf ("\nCodigo: %d\n", pmid.codigo);
            printf ("Despesas: R$ %.2f\n", pmid.despesas);
            printf ("Quantidade de produtos: %d\n", pmid.quantidadeProdutos);
            getch();
            return;
        }
    }

    fclose(arquivo);
    printf ("Valor nao encotrado!\n");
    getch();
    return;
}


void recuperarBackup() {

    struct Panificadora pa;
    int contador = 0;
    int aux = 0;

    if ((arquivo = fopen("panificadora.dados", "rb+")) == NULL) {
        printf ("\nErro Arquivo");
        exit(1);
    }

    if ((backup = fopen("backup.dados", "rb+")) == NULL) {
        printf ("\nErro Backup");
        exit(1);
    }

    fseek(backup, 0, 0);
    while (fread(&pa, sizeof(p), 1, backup)) {
        if(pa.erase == 1) {
            aux++;
        }
    }

    if (aux > 0) {
        printf ("\nDados excluidos:\n");
        fseek(backup, 0, 0);
        while (fread(&pa, sizeof(p), 1, backup)) {
            if (pa.erase == 1) {
                printf ("\nNome: %s\n", pa.nome);
                printf ("Codigo: %d\n", pa.codigo);
                printf ("Despesas: R$ %.2f\n", pa.despesas);
                printf ("Quantidade de produtos: %d\n", pa.quantidadeProdutos);
            }
        }
        printf("\nRecuperando Backup!\n");
        fseek(backup, 0, 0);

        while(fread(&pa, sizeof(p), 1, backup)) {
            if (pa.erase == 1) {
                contador++;
                pa.erase = 0;
                fseek(arquivo, 0, 2);
                fwrite(&pa, sizeof(p), 1, arquivo);
                fseek(backup, ftell(backup) - sizeof(p), 0);
                fwrite(&pa, sizeof(p), 1, backup);
                fseek(backup, 0, 2);
            }
        }
    } else {
        printf ("\nNao existem dados para recuperar!\n");
    }

    if (contador > 0) {
        printf ("\nDados recuperados com sucesso!\n");
    } else {
        printf ("\nErro ao recuperar dados!\n");
    }

    fclose(backup);
    fclose(arquivo);
    getche();
}


