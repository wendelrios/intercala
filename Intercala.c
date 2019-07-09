#include <stdio.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>

typedef struct _Endereco Endereco;

struct _Endereco
{
    char logradouro[72];
    char bairro[72];
    char cidade[72];
    char uf[72];
    char sigla[2];
    char cep[8];
    char lixo[2];
};

void selecionar(){
    FILE *f;
    Endereco e;
    long fim;

    f = fopen("cep.dat","r");
    FILE *cria = fopen("selecao.dat", "w");
    fseek (f, 0, SEEK_END);
    fim = (ftell(f)/sizeof(Endereco))-1;
    rewind (f);
    srand((unsigned) time (NULL));
    
    for (int i = 0; i < 80; i++){
        fseek(f, (rand()%fim)*sizeof(Endereco), SEEK_SET);
        fread(&e, sizeof(Endereco), 1, f);
        fwrite(&e, sizeof(Endereco), 1, cria);
    }
    fclose(f);
    fclose(cria);
}

int comparar(const void *e1, const void *e2){
    return strncmp((*(Endereco*)e1).cep,(*(Endereco*)e2).cep, 8);
}

void dividir(){
    FILE *g = fopen("selecao.dat", "r");
    Endereco e;
    for (int i = 1; i <= 8; i++){
        char oito_n[20];
        sprintf(oito_n, "oito_%d.dat", i);
        FILE *menor;
        menor = fopen(oito_n, "a");
        for (int j = 0; j < 10; j++){
            fread(&e, sizeof(Endereco), 1, g);
            fwrite(&e, sizeof(Endereco), 1, menor);
        }
        fclose(menor);
    } 
    fclose(g);
}

void ordenar()
{
    FILE *f, *saida;
    Endereco *e;
    long quantidade, posicao;

    for(int i = 1; i <= 8; i++){
        char oito_n [20];
        sprintf(oito_n, "%s%d.dat", "oito_", i);
        f = fopen(oito_n, "r");
        char em_ordem[20];

        fseek (f, 0, SEEK_END);
        posicao = ftell(f);
        rewind(f);
        quantidade = posicao/sizeof(Endereco);

        e = (Endereco*) malloc(posicao);

	fread(e, sizeof(Endereco), quantidade, f);

        qsort(e, quantidade, sizeof(Endereco), compara);

        sprintf(em_ordem, "%s%d.dat", "em_ordem", i);
        saida = fopen(em_ordem, "w");

	fwrite(e, sizeof(Endereco), quantidade, saida);

        fclose(saida);
    }
    fclose(f);
}

int main(int argc, char**argv)
{
    selecionar();
    dividir();
    ordenar();
}
