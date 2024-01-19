#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Paciente {
    char NomeCompleto[100];
    char Mae[100];
    char DataDeNascimento[11];
    char Sexo;
    char CPF[12];
};

struct Atendimento {
    struct Paciente Paciente;
    char Data[11];
    char Procedimento[50];
    int DuracaoMinutos;
};

struct ClinicaCDI {
    struct Paciente *pacientes;
    struct Atendimento *atendimentos;
    int numPacientes;
    int numAtendimentos;
};

void InicializarClinica(struct ClinicaCDI *clinica) {
    clinica->pacientes = NULL;
    clinica->atendimentos = NULL;
    clinica->numPacientes = 0;
    clinica->numAtendimentos = 0;
}

void CadastrarPaciente(struct ClinicaCDI *clinica) {
    clinica->numPacientes++;
    clinica->pacientes = realloc(clinica->pacientes, sizeof(struct Paciente) * clinica->numPacientes);

    struct Paciente novoPaciente;

    printf("Nome Completo: ");
    scanf("%s", novoPaciente.NomeCompleto);

    printf("Nome da Mãe: ");
    scanf("%s", novoPaciente.Mae);

    printf("Data de Nascimento (DD/MM/AAAA): ");
    scanf("%s", novoPaciente.DataDeNascimento);

    printf("Sexo (M/F): ");
    scanf(" %c", &novoPaciente.Sexo);

    printf("CPF: ");
    scanf("%11s", novoPaciente.CPF);

    clinica->pacientes[clinica->numPacientes - 1] = novoPaciente;

    printf("Paciente %s cadastrado com sucesso!\n", novoPaciente.NomeCompleto);
}

void CadastrarAtendimento(struct ClinicaCDI *clinica) {
    clinica->numAtendimentos++;
    clinica->atendimentos = realloc(clinica->atendimentos, sizeof(struct Atendimento) * clinica->numAtendimentos);

    struct Atendimento novoAtendimento;

    printf("Selecione o paciente:\n");
    for (int i = 0; i < clinica->numPacientes; i++) {
        printf("%d. %s\n", i + 1, clinica->pacientes[i].NomeCompleto);
    }

    int escolhaPaciente;
    printf("Escolha o número correspondente ao paciente: ");
    scanf("%d", &escolhaPaciente);

    if (escolhaPaciente <= 0 || escolhaPaciente > clinica->numPacientes) {
        printf("Escolha inválida.\n");
        return;
    }

    novoAtendimento.Paciente = clinica->pacientes[escolhaPaciente - 1];

    printf("Data do Atendimento (DD/MM/AAAA): ");
    scanf("%s", novoAtendimento.Data);

    printf("Selecione o procedimento:\n");
    printf("1. Raios-X de Tórax em PA\n");
    printf("2. Ultrassonografia Obstétrica\n");
    printf("3. Ultrassonografia de Próstata\n");
    printf("4. Tomografia\n");

    int escolhaProcedimento;
    char sexo;
    printf("Escolha o número correspondente ao procedimento: ");
    scanf("%d", &escolhaProcedimento);

    switch (escolhaProcedimento) {
        case 1:
            strcpy(novoAtendimento.Procedimento, "Raios-X de Tórax em PA");
            break;
        case 2:
            if (novoAtendimento.Paciente.Sexo == 'M') {
                printf("Esse procedimento é apenas para mulheres\n");
                return;
            } else {
                strcpy(novoAtendimento.Procedimento, "Ultrassonografia Obstétrica");
            }
            break;
        case 3:
            if (novoAtendimento.Paciente.Sexo == 'F') {
                printf("Esse procedimento é apenas para homens\n");
                return;
            } else {
                strcpy(novoAtendimento.Procedimento, "Ultrassonografia de Próstata");
            }
            break;
        case 4:
            for (int i = 0; i < clinica->numAtendimentos; i++) {
                if (strcmp(clinica->atendimentos[i].Paciente.NomeCompleto, novoAtendimento.Paciente.NomeCompleto) == 0 &&
                    strcmp(clinica->atendimentos[i].Procedimento, "Ultrassonografia Obstétrica") == 0 &&
                    (strcmp(clinica->atendimentos[i].Data, novoAtendimento.Data) >= 0 ||
                     (strcmp(clinica->atendimentos[i].Data, novoAtendimento.Data) < 0 && strcmp(novoAtendimento.Data, "31/12/9999") <= 0))) {
                    printf("Tomografia não pode ser realizada, pois o paciente realizou Ultrassonografia Obstétrica nos últimos três meses.\n");
                    return;
                }
            }
            strcpy(novoAtendimento.Procedimento, "Tomografia");
            break;
        default:
            printf("Escolha inválida.\n");
            return;
    }

    printf("Duração em minutos: ");
    scanf("%d", &novoAtendimento.DuracaoMinutos);

    clinica->atendimentos[clinica->numAtendimentos - 1] = novoAtendimento;

    printf("Atendimento registrado com sucesso para o paciente %s na data %s.\n", novoAtendimento.Paciente.NomeCompleto, novoAtendimento.Data);
}

void ListarPacientes(struct ClinicaCDI *clinica) {
    for (int i = 0; i < clinica->numPacientes; i++) {
        printf("Nome: %s, Data de Nascimento: %s\n", clinica->pacientes[i].NomeCompleto, clinica->pacientes[i].DataDeNascimento);
    }
}

void ListarAtendimentosPorData(struct ClinicaCDI *clinica, char data[11]) {
    for (int i = 0; i < clinica->numAtendimentos; i++) {
        if (strcmp(clinica->atendimentos[i].Data, data) == 0) {
            printf("Paciente: %s, Data de Nascimento: %s, Procedimento: %s\n",
                   clinica->atendimentos[i].Paciente.NomeCompleto,
                   clinica->atendimentos[i].Paciente.DataDeNascimento,
                   clinica->atendimentos[i].Procedimento);
        }
    }
}

int ObterNumeroDeProcedimentos(struct ClinicaCDI *clinica, char procedimento[50], char dataInicial[11], char dataFinal[11]) {
    int quantidadeProcedimentos = 0;
    for (int i = 0; i < clinica->numAtendimentos; i++) {
        if (strcmp(clinica->atendimentos[i].Procedimento, procedimento) == 0 &&
            strcmp(clinica->atendimentos[i].Data, dataInicial) >= 0 &&
            strcmp(clinica->atendimentos[i].Data, dataFinal) <= 0) {
            quantidadeProcedimentos++;
        }
    }

    printf("Número de procedimentos '%s' realizados no período: %d\n", procedimento, quantidadeProcedimentos);
    return quantidadeProcedimentos;
}

void ObterTempoTotalDeDuracaoProcedimento(struct ClinicaCDI *clinica, char procedimento[50], char dataInicial[11], char dataFinal[11]) {
    int totalDuracao = 0;
    for (int i = 0; i < clinica->numAtendimentos; i++) {
        if (strcmp(clinica->atendimentos[i].Procedimento, procedimento) == 0 &&
            strcmp(clinica->atendimentos[i].Data, dataInicial) >= 0 &&
            strcmp(clinica->atendimentos[i].Data, dataFinal) <= 0) {
            totalDuracao += clinica->atendimentos[i].DuracaoMinutos;
        }
    }

    printf("Tempo total de duração do procedimento '%s' no período: %d minutos\n", procedimento, totalDuracao);
}

void ObterTempoTotalDeDuracaoTodosProcedimentos(struct ClinicaCDI *clinica, char dataInicial[11], char dataFinal[11]) {
    int totalDuracao = 0;
    for (int i = 0; i < clinica->numAtendimentos; i++) {
        if (strcmp(clinica->atendimentos[i].Data, dataInicial) >= 0 &&
            strcmp(clinica->atendimentos[i].Data, dataFinal) <= 0) {
            totalDuracao += clinica->atendimentos[i].DuracaoMinutos;
        }
    }

    printf("Tempo total de duração de todos os procedimentos no período: %d minutos\n", totalDuracao);
}

void ExibirMenu() {
    printf("=== MENU ===\n");
    printf("1. Cadastrar Paciente\n");
    printf("2. Cadastrar Atendimento\n");
    printf("3. Listar Pacientes\n");
    printf("4. Listar Atendimentos por Data\n");
    printf("5. Número de Procedimentos em um Período\n");
    printf("6. Tempo Total de Duração de um Procedimento em um Período\n");
    printf("7. Tempo Total de Duração de Todos Procedimentos em um Período\n");
    printf("8. Sair\n");
    printf("==============\n");
}

int main() {
    struct ClinicaCDI clinica;
    InicializarClinica(&clinica);

    int opcao;

    do {
        ExibirMenu();
        printf("Escolha uma opção: ");
        scanf("%d", &opcao);

        switch (opcao) {
            case 1:
                CadastrarPaciente(&clinica);
                break;
            case 2:
                CadastrarAtendimento(&clinica);
                break;
            case 3:
                ListarPacientes(&clinica);
                break;
            case 4: {
                char data[11];
                printf("Digite a data (DD/MM/AAAA): ");
                scanf("%s", data);
                ListarAtendimentosPorData(&clinica, data);
                break;
            }
            case 5: {
                char procedimento[50];
                char dataInicial[11];
                char dataFinal[11];
                printf("Digite o procedimento: ");
                scanf("%s", procedimento);
                printf("Digite a data inicial (DD/MM/AAAA): ");
                scanf("%s", dataInicial);
                printf("Digite a data final (DD/MM/AAAA): ");
                scanf("%s", dataFinal);
                ObterNumeroDeProcedimentos(&clinica, procedimento, dataInicial, dataFinal);
                break;
            }
            case 6: {
                char procedimento[50];
                char dataInicial[11];
                char dataFinal[11];
                printf("Digite o procedimento: ");
                scanf("%s", procedimento);
                printf("Digite a data inicial (DD/MM/AAAA): ");
                scanf("%s", dataInicial);
                printf("Digite a data final (DD/MM/AAAA): ");
                scanf("%s", dataFinal);
                ObterTempoTotalDeDuracaoProcedimento(&clinica, procedimento, dataInicial, dataFinal);
                break;
            }
            case 7: {
                char dataInicial[11];
                char dataFinal[11];
                printf("Digite a data inicial (DD/MM/AAAA): ");
                scanf("%s", dataInicial);
                printf("Digite a data final (DD/MM/AAAA): ");
                scanf("%s", dataFinal);
                ObterTempoTotalDeDuracaoTodosProcedimentos(&clinica, dataInicial, dataFinal);
                break;
            }
            case 8:
                printf("Saindo do programa...\n");
                break;
            default:
                printf("Opção inválida. Tente novamente.\n");
        }

    } while (opcao != 8);

    free(clinica.pacientes);
    free(clinica.atendimentos);

    return 0;
}