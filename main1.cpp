#include <stdio.h>
#include <stdlib.h>
#include <locale.h>


// registro que representa os dados de um aluno
typedef struct {  
	int matricula;
	char nome[20];
    float media;
} TAluno;

// declara um global ponteiro para o arquivo
FILE *paluno;

// variáveis globais auxiliares adicionar/alterar e remover alunos
TAluno aluno_aux, aluno_nulo;

// procedimento que imprime uma linha no terminal
void linha() {
	int i;
//	printf("\n");
	for (i = 1; i <= 80; i++) {
		printf("_");
	}
	printf("\n");
}

// procedimento que imprime um cabeçalho
void cabec() {
	system("cls");
	printf("Centro Universitário \n");
	linha();
}

// procedimento que abre um arquivo
void abre_arquivo() {
	// abre o arquivo em forma d leitura
	paluno = fopen("aluno.dat", "r+b");
	// caso o arquivo não exista, ele é criado
	if (paluno == NULL) {
		paluno = fopen("aluno.dat", "w+b");
	}
}

// procedimento que insere dentro do arquivo
void inserir() {
	int resp;
	
	do {
		// imprime o cabeçalho
		cabec();
		
		printf("\n\nCadastrar novo aluno\n\n");
		printf("\nMatrícula: ");
		scanf("%d", &aluno_aux.matricula);
		fflush(stdin);
		
		printf("\nNome.....: ");
		fgets(aluno_aux.nome, 20, stdin);
		fflush(stdin);
		
		printf("\nMédia....: ");
		scanf("%f", &aluno_aux.media);
		fflush(stdin);
		
		// coloca o leitor no inicio do arquivo
		fseek(paluno, 0, SEEK_END);
		
		// escreve o registro aluno_aux no arquivo
		fwrite(&aluno_aux, sizeof(TAluno), 1, paluno);
		printf("\n\nAluno cadastrado com sucesso!\n\n");
		
		printf("\nDeseja cadastrar outro aluno (1-Sim/0-Não)? ");
		scanf("%d", &resp);
		fflush(stdin);
	} while(resp == 1);
}

// função que busca um aluno passando a sua matricula
int procura(int matp) {
	int p = 0;
	
	// retorna leitor do arquivo para o início do arquivo
	rewind(paluno);
	// faz a leitura do primeiro registro do arquivo
	fread(&aluno_aux, sizeof(TAluno), 1, paluno);
	// enquanto não chegar ao fim do arquivo faça
	while (feof(paluno) == 0) {
		// se matricula passada for igual do aluno_aux
		if (aluno_aux.matricula == matp) {
			return p; // retorna a posição do aluno no arquivo
		} else {
			// lê o próximo registro
			fread(&aluno_aux, sizeof(TAluno), 1, paluno);
			p++; // incrementa a posição lida do arquivo
		}
	}
	
	return -1; // retorna -1 caso o aluno não seja encontrado
}

// função para mostrar os dados do aluno
void mostre(int pos) {
	// desloca o leitor do arquivo para a posição passada
	// no parametro
	fseek(paluno, pos * sizeof(TAluno), SEEK_SET);
	// realiza a leitura do registro
	fread(&aluno_aux, sizeof(TAluno), 1, paluno);
	
	printf("\n\n");
	linha();
	printf("Matrícula Nome\t\t\tMédia\n");
	linha();
	printf("%9d %-20s\t\t\t %5.1f\n", aluno_aux.matricula, aluno_aux.nome, aluno_aux.media);
	linha();
}

// procedimento que pede para o usuário digitar o numero de matricula a ser procurado
void consultar() {
	int resp, matcon, posicao;
	
	do {
		// imprime o cabeçalho
		cabec();
		printf("\n\nConsultar Aluno\n\n\n");
		printf("Matrícula do aluno: ");
		scanf("%d", &matcon);
		fflush(stdin);
		
		// posicao recebe a posição passada pela função procura
		posicao = procura(matcon);
		// verifica se aluno está dentro do arquivo
		if (posicao == -1) {
			printf("\n\nMatricula não encontrada!\n\n");
		} else {
			// imprime os dados do aluno passando a posição no arquivo
			mostre(posicao);
		}
		
		printf("\n\nDeseja consultar outro aluno (1-Sim/0-Não)? ");
		scanf("%d", &resp);
		fflush(stdin);
	} while (resp == 1);
}

// função que remove um aluno do arquivo
void remover() {
	int matrem, conf, resp, posicao;
	
	aluno_nulo.matricula = 0;
	aluno_nulo.media = 0;
	do {
		// imprime o cabeçalho
		cabec();
		printf("\n\nRemover aluno\n\n\n");
		printf("Matrícula: ");
		scanf("%d", &matrem);
		fflush(stdin);
		
		// retorna a posicao do aluno arquivo
		posicao = procura(matrem);
		
		// verifica se aluno está aquivo
		if (posicao == - 1) {
			printf("\nAluno não encontrado!\n");
		} else {
			// imprime os dados do aluno encontrado
			mostre(posicao);
			printf("\n\nDeseja remove o aluno (1-Sim/0-Não)? ");
			scanf("%d", &conf);
			fflush(stdin);
			
			// caso o usuário confirme a remoção
			if (conf == 1) {
				fseek(paluno, posicao * sizeof(TAluno), SEEK_SET);
				fwrite(&aluno_nulo, sizeof(TAluno), 1, paluno);
				printf("\n\nAluno removido com sucesso!");
			} else {
				printf("\nRemoção cancelada!");
			}	
		}
		printf("\n\n\nDeseja remover outro aluno (1-Sim/0-Não)? ");
		scanf("%d", &resp);
		fflush(stdin);
		
	} while (resp == 1);
}

// função para conferir média e aplicar se esta aprovado ou reprovado na lista
void conferir(){
	if(aluno_aux.media>=6.0){
		printf("\n\t\t\t\tAprovado\n");
	}
	else{
		printf("\n\t\t\t\tReprovado\n");
	}
}

// função que lista todos os alunos do arquivo
void listagem() {
	cabec();
	printf("\n\nListagem Geral\n\n\n");
	linha();
	printf("Matrícula Nome\t\t\tMedia\n");
	linha();
	rewind(paluno);
	fread(&aluno_aux, sizeof(TAluno), 1, paluno);
	
	// percorre todo o arquivo até chegar no fim
	while(feof(paluno) == 0) {
		// verifica se o registro tem dados
		if (aluno_aux.matricula != 0)
			printf("%9d %-20s\t\t\t %5.1f\n", aluno_aux.matricula, aluno_aux.nome, aluno_aux.media);
			conferir();
			linha();
		// lê mais um registro	
		fread(&aluno_aux, sizeof(TAluno), 1, paluno);
	}
	linha();
	printf("\n\n\nDigite qualquer tecla para voltar ao menu...");
	getchar();
}

int main(int argc, char *argv[]) {
	setlocale(LC_ALL, "Portuguese");
	int op;
	
	// abre o arquivo
	abre_arquivo();
	
	do {
		cabec();
		printf("\n\nOpções:\n\n");
		printf("1. Cadastrar novo aluno\n");
		printf("2. Remover aluno\n");
		printf("3. Consultar aluno por matrícula\n");
		printf("4. Listagem geral\n");
		printf("0. Sair\n\n");
		linha();
		printf("\nInforme a opção desejada: ");
		scanf("%d", &op);
		fflush(stdin);
		
		switch(op) {
			case 1: inserir();
				break;
			case 2: remover();
				break;
			case 3: consultar();
				break;
			case 4: listagem();
				break;
//			case 5: limpar();
//				break;
			case 0: fclose(paluno);
				break;
			default:
				printf("\n\n\nOpção inválida!");
				printf("\n\nDigite qualquer tecla para continuar...");
				getchar();
				break;
		}
	} while (op != 0);
	
	return 0;
}
