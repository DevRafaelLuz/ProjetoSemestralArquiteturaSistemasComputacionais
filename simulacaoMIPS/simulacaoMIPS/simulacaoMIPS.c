#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NUM_REGISTRADORES 32 // N�mero de registradores

// Tipos de instru��es
#define TIPO_R 0
#define TIPO_I 1
#define TIPO_J 2

// Estrutura para o processador
typedef struct {
    int registradores[NUM_REGISTRADORES];
} Processador;

// Estrutura para a instru��o
typedef struct {
    int tipo;      // Tipo da instru��o (R, I, J)
    char operacao[10];
    int rd;        // Registrador destino (somente Tipo_R)
    int rs;        // Primeiro Registrador de Origem
    int rt;        // Segundo Registrador de Origem
    int imediato;
    int endereco;  // Endere�o para instru��es Tipo_J
} Instrucao;

// Inicializa os registradores com zero
void inicializar_registradores(Processador *proc) {
	int i;
    for (i = 0; i < NUM_REGISTRADORES; i++) {
        proc->registradores[i] = 0;
    }
}

// L� uma instru��o no formato leg�vel
void ler_instrucao(Instrucao *inst, int *pc) {
    scanf("%s", inst[*pc].operacao);

    if ((strcmp(inst[*pc].operacao, "ADD") == 0) || (strcmp(inst[*pc].operacao, "SUB") == 0)){
        inst[*pc].tipo = TIPO_R;
        scanf(" R%d, R%d, R%d", &inst[*pc].rd, &inst[*pc].rs, &inst[*pc].rt);
    }
    else if (strcmp(inst[*pc].operacao, "MULT") == 0) {
    	inst[*pc].tipo = TIPO_R;
    	scanf(" R%d, R%d", &inst[*pc].rs, &inst[*pc].rt);
	}
	else if (strcmp(inst[*pc].operacao, "MFLO") == 0) {
		inst[*pc].tipo = TIPO_R;
		scanf(" R%d", &inst[*pc].rd);
	}
	else if (strcmp(inst[*pc].operacao, "DIV") == 0) {
		inst[*pc].tipo = TIPO_R;
		scanf(" R%d, R%d", &inst[*pc].rs, &inst[*pc].rt);
	}
	else if (strcmp(inst[*pc].operacao, "REM") == 0) {
		inst[*pc].tipo = TIPO_R;
		scanf(" R%d, R%d, R%d", &inst[*pc].rd, &inst[*pc].rs, &inst[*pc].rt);
	}
	else if (strcmp(inst[*pc].operacao, "MOVE") == 0) {
		inst[*pc].tipo = TIPO_R;
		scanf(" R%d, R%d", &inst[*pc].rd, &inst[*pc].rs);
	}
	else if (strcmp(inst[*pc].operacao, "ADDI") == 0) {
        inst[*pc].tipo = TIPO_I;
        scanf(" R%d, R%d, %d", &inst[*pc].rs, &inst[*pc].rt, &inst[*pc].imediato);
    }
    else if (strcmp(inst[*pc].operacao, "LI") == 0) {
    	inst[*pc].tipo = TIPO_I;
    	scanf(" R%d, %d", &inst[*pc].rd, &inst[*pc].imediato);
	}
	else if (strcmp(inst[*pc].operacao, "JUMP") == 0) {
        inst[*pc].tipo = TIPO_J;
        scanf("%d", &inst[*pc].endereco);
    }
	else {
        inst->tipo = -1; // Instru��o inv�lida
    }
}

// Executa a instru��o baseada no tipo
void executar_instrucao(Processador *proc, Instrucao *inst, int *pc, int *lo) {
    switch (inst[*pc].tipo) {
        case 0:
            if (strcmp(inst[*pc].operacao, "ADD") == 0) {
                proc->registradores[inst[*pc].rd] = proc->registradores[inst[*pc].rs] + proc->registradores[inst[*pc].rt];
                printf("\nExecutou ADD: R%d = R%d + R%d\n", inst[*pc].rd, inst[*pc].rs, inst[*pc].rt);
            }
			else if (strcmp(inst[*pc].operacao, "SUB") == 0) {
                proc->registradores[inst[*pc].rd] = proc->registradores[inst[*pc].rs] - proc->registradores[inst[*pc].rt];
                printf("\nExecutou SUB: R%d = R%d - R%d\n", inst[*pc].rd, inst[*pc].rs, inst[*pc].rt);
            }
            else if (strcmp(inst[*pc].operacao, "MULT") == 0) {
            	*lo = proc->registradores[inst[*pc].rs] * proc->registradores[inst[*pc].rt];
            	printf("\nExecutou MULT: LO = R%d * R%d\n", inst[*pc].rs, inst[*pc].rt);
			}
			else if (strcmp(inst[*pc].operacao, "DIV") == 0) {
				*lo = proc->registradores[inst[*pc].rs] / proc->registradores[inst[*pc].rt];
				printf("\nExecutou DIV: LO = R%d / R%d\n", inst[*pc].rs, inst[*pc].rt);
			}
			else if (strcmp(inst[*pc].operacao, "REM") == 0) {
				proc->registradores[inst[*pc].rd] = proc->registradores[inst[*pc].rs] % proc->registradores[inst[*pc].rt];
				printf("\nExecutou REM: R%d = R%d % R%d\n", inst[*pc].rd, inst[*pc].rs, inst[*pc].rt);
			}
			else if (strcmp(inst[*pc].operacao, "MFLO") == 0) {
				proc->registradores[inst[*pc].rd] = *lo;
				printf("\nExecutou MFLO: R%d = LO\n", inst[*pc].rd);
			}
			else if (strcmp(inst[*pc].operacao, "MOVE") == 0) {
				proc->registradores[inst[*pc].rd] = proc->registradores[inst[*pc].rs];
				printf("\nExecutou MOVE: R%d = R%d\n", inst[*pc].rd, inst[*pc].rs);
			}
            break;

        case 1:
            if (strcmp(inst[*pc].operacao, "ADDI") == 0) {
                proc->registradores[inst[*pc].rs] = proc->registradores[inst[*pc].rt] + inst[*pc].imediato;
                printf("\nExecutou ADDI: R%d = R%d + %d\n", inst[*pc].rs, inst[*pc].rt, inst[*pc].imediato);
            }
            else if (strcmp(inst[*pc].operacao, "LI") == 0) {
            	proc->registradores[inst[*pc].rd] = inst[*pc].imediato;
            	printf("\nExecutou LI: R%d = %d\n", inst[*pc].rd, inst[*pc].imediato);
			}
            break;

        case 2:
            if (strcmp(inst[*pc].operacao, "JUMP") == 0) {
                printf("\nExecutou JUMP para o endereco %d\n", inst[*pc].endereco);
                *pc = inst[*pc].endereco;
            }
            break;

        default: break;
    }
}

// Exibe o estado dos registradores
void exibir_registradores(Processador *proc) {
    printf("\nEstado dos Registradores:\n");
    int i;
    for (i = 0; i < NUM_REGISTRADORES; i++) {
        printf("R%-2d: %2d | ", i, proc->registradores[i]);
        
        if((i + 1) % 8 == 0) { // Apenas pula uma linha a cada 8 registradores 
        	printf("\n");
		}
    }
}

// Fun��o principal
int main() {
    Processador proc;
    Instrucao inst[100];
    
    int max;
    int lo = 0, pc = 1;

    inicializar_registradores(&proc);

    printf("Simulador de Processador MIPS\n");
    printf("Digite as instrucoes com todas as letras em maiusculo no formato legivel:\n");
    printf("Exemplos:   ADD R1, R2, R3\n");
    printf("            LI R1, 10\n");
    printf("            JUMP 10 (OBS: '10' seria o endereco)\n");
    printf("\nInstrucoes aceitas ate o momento: \n ADD  | SUB  | MULT | DIV \n MFLO | MOVE | ADDI | REM \n LI   | JUMP \n");
    printf("\nDigite SAIR para cancelar e encerrar o programa.\n");
    printf("\nDigite EXECUTAR para executar as instrucoes.\n");

    while (1) {
        printf("\nEndereco %d: Digite uma instrucao: ", pc-1);
        ler_instrucao(inst, &pc);

        if (strcmp(inst[pc].operacao, "SAIR") == 0) {
            printf("\nEncerrando o programa.\n");
            break;
        }
        else if(strcmp(inst[pc].operacao, "EXECUTAR") == 0) {
        	max = pc;
        	break;
		}

        pc++;
    }
    
    if(strcmp(inst[pc].operacao, "SAIR") == 0) {
    	return 0;
	}
	
	pc = 1;
	
	while(pc<=max) {
		executar_instrucao(&proc, inst, &pc, &lo);
		if(strcmp(inst[pc].operacao, "JUMP") != 0) {
			pc++;
		}
	}
	
	exibir_registradores(&proc);

    return 0;
}
