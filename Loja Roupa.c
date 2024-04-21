/* Loja de Roupas - Projeto de Algorítmos - Código Fonte em C
Integrantes de Grupo:
============================================
- Diego Tasso da Cunha Ferreira: 1230202973
============================================
- Maria Luiza França Mendes: 1230112799
============================================
- Maurício Muquim Mendonça: 1230108958
============================================
*/

// Includes
#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include <string.h> 

// Estrutura da Data
typedef struct {
    int dia;
    int mes;
    int ano;
} Data;

// Estrutura do Cliente
typedef struct {
    char nome[100];
    char codCliente[30];
} Cliente;

// Estrutura das vendas obtidos com base nas entadas do usuário e cálculos
typedef struct {
    int codigo;
    char item[100];
    char marca[100];
    int quantidade;
    float precoUnitario;
    float precoFinal;
    Data dataVenda;
    Cliente cliente;
} Venda;

// Declaração das das funções e procedimentos presentes no código 
float calcularPrecoFinal(Venda venda);
int compararVendas(const void *a, const void *b);
void imprimirRelatoriosPorData(Venda vendas[], int numVendas, float faturamentoBruto, int numClientes);
void realizarVenda(Venda vendas[], int *numVendas, float *faturamentoBruto, int *numClientes, FILE *arquivo);
void listarVendasPorData(Venda vendas[], int numVendas, FILE *arquivo);
void salvarDados(Venda vendas[], int numVendas, FILE *arquivo);
void carregarDados(Venda vendas[], int *numVendas, FILE *arquivo);
void encontrarItemMaisVendido(Venda vendas[], int numVendas);
void encontrarItemMenosVendido(Venda vendas[], int numVendas);

// Função principal
int main() {

    // Corrigindo erros de Acentuação gráfica
    setlocale(LC_ALL, "Portuguese");
    int opcao;

    // Variáveis de para vendas
    Venda vendas[100];
    float faturamentoBruto = 0;
    int numVendas = 0;
    int numClientes = 0;

     // Abertura/criação do arquivo .dat com verificação de erros e condicional para função com apontador
    FILE *arquivo = fopen("loja_roupa.dat", "r+b");
    if (arquivo == NULL) {
        arquivo = fopen("loja_roupa.dat", "wb");
        if (arquivo == NULL) {
            printf("Erro ao abrir o arquivo para escrita.\n");
            return 1;
        }
    } else {
        carregarDados(vendas, &numVendas, arquivo);
    }

    // Menu Principal do Programa
    do {
        printf("\n|========== Loja de Roupas - Menu =======|\n");
        printf("| 1. Realizar uma venda                  |\n");
        printf("| 2. Listar vendas por data              |\n");
        printf("| 3. Imprimir Relatorios por data        |\n");
        printf("| 4. Sair do Programa                    |\n");
        printf("|========================================|\n");
        printf("Escolha uma opcao: ");
        scanf("%d", &opcao);

        // Switch Case com todas as opções do Menu
        switch (opcao) {
            case 1:
                realizarVenda(vendas, &numVendas, &faturamentoBruto, &numClientes, arquivo);
                break;
            case 2:
                listarVendasPorData(vendas, numVendas, arquivo);
                break;
            case 3:
                imprimirRelatoriosPorData(vendas, numVendas, faturamentoBruto, numClientes);
                break;
            case 4:
                salvarDados(vendas, numVendas, arquivo);
                printf("Saindo do programa...\n");
                break;
            default:
                printf("Opcao invalida. Por favor, escolha uma opcao valida.\n");
        }
    } while (opcao != 4);

    fclose(arquivo);
    printf("Programa encerrado.\n");

    return 0;
}

//Função de cálculo do Preço Final
float calcularPrecoFinal(Venda venda) {
    return venda.quantidade * venda.precoUnitario;
}

//Função para comparação visando a realização da ordenação em ordem decrescente das vendas
int compararVendas(const void *a, const void *b) {
    const Venda *vendaA = (const Venda *)a;
    const Venda *vendaB = (const Venda *)b;
    return (vendaB->precoFinal - vendaA->precoFinal);
}

// Função de impressão de relatórios por data especificamente citada pelo usuário
void imprimirRelatoriosPorData(Venda vendas[], int numVendas, float faturamentoBruto, int numClientes) {
    Data dataConsulta;
    printf("Digite a data para consultar o relatório (dd/mm/yyyy): ");
    scanf("%d/%d/%d", &dataConsulta.dia, &dataConsulta.mes, &dataConsulta.ano);

     // Relatório com loop para todas as vendas presentes no arquivo de dados de um dia específico que também é citado, além da procura e escrita do item mais e menos vendido
    printf("\n========== Relatorios para a data %02d/%02d/%04d ==========\n", dataConsulta.dia, dataConsulta.mes, dataConsulta.ano);
    printf("Numero de vendas realizadas: ");
    encontrarItemMaisVendido(vendas, numVendas);
    encontrarItemMenosVendido(vendas, numVendas);

    int vendasData = 0;
    float faturamentoData = 0;
    int clientesData = 0;

     // Loop
    for (int i = 0; i < numVendas; i++) {
        if (vendas[i].dataVenda.dia == dataConsulta.dia &&
            vendas[i].dataVenda.mes == dataConsulta.mes &&
            vendas[i].dataVenda.ano == dataConsulta.ano) {
            vendasData++;
            faturamentoData += vendas[i].precoFinal;
            clientesData++;
        }
    }

    // Escrevendo na tela o faturamente bruto total da empresa, junto ao número de vendas e clientes que realizaram uma compra
    printf("Faturamento bruto total: R$ %.2f\n", faturamentoData);
    printf("Numero de clientes atendidos: %d\n", clientesData);

    // Condicional do Loop
    if (vendasData > 0) {
        printf("Vendas realizadas:\n");
        for (int i = 0; i < numVendas; i++) {
            if (vendas[i].dataVenda.dia == dataConsulta.dia &&
                vendas[i].dataVenda.mes == dataConsulta.mes &&
                vendas[i].dataVenda.ano == dataConsulta.ano) {
                printf("Codigo: %d, Item: %s, Marca: %s, Quantidade: %d, Preco Unitario: R$ %.2f, Preco Final: R$ %.2f, Cliente: %s, Código do Cliente: %s\n",
                       vendas[i].codigo, vendas[i].item, vendas[i].marca, vendas[i].quantidade,
                       vendas[i].precoUnitario, vendas[i].precoFinal, vendas[i].cliente.nome, vendas[i].cliente.codCliente);
            }
        }
    } else {
        printf("Nenhuma venda encontrada para a data especificada.\n");
    }
}

// Função para realizar uma venda
void realizarVenda(Venda vendas[], int *numVendas, float *faturamentoBruto, int *numClientes, FILE *arquivo) {
    printf("Digite os detalhes da venda e do cliente: \n");

    while (1) {
        Venda venda;

        // Entrada que possibilita a finalização do programa, junto da obtenção de outras entradas que se relacionam com a roupa específica a ser comprada.
        printf("\nEntre com o codigo do item (ou 0 para finalizar): ");
        scanf("%d", &venda.codigo);
        if (venda.codigo == 0) {
            break;
        }
        printf("Item: ");
        scanf("%s", venda.item);
        getchar();
        printf("Marca: ");
        scanf("%s", venda.marca);
        getchar();
        printf("Entre com a quantidade de itens: ");
        scanf("%d", &venda.quantidade);
        printf("Entre com o preço unitário do item: ");
        scanf("%f", &venda.precoUnitario);
        getchar();  

        float precoFinalItem;

        // Condicional da possibilidade de desconto de 10 porcento caso sejam comprados 3 itens ou mais de um mesmo produto específico
        if (venda.quantidade >= 3) {
            precoFinalItem = calcularPrecoFinal(venda) * 0.9;
        } else {
            precoFinalItem = calcularPrecoFinal(venda);
        }

        // Escrita na tela do valor do item adquirido + desconto caso tenha
        if (venda.quantidade >= 3) {
            venda.precoFinal = calcularPrecoFinal(venda) * 0.9;
            printf("Preço Pago pelo(s) Item(s) com desconto de 10 Por cento: R$ %.2f\n", precoFinalItem);
            
        } else {
            venda.precoFinal = calcularPrecoFinal(venda);
            printf("Preço Pago pelo(s) Item(s): R$ %.2f\n", precoFinalItem);
        }

        // Nome e código do cliente para validar a existencia de um
        printf("Digite o nome do cliente: ");
        scanf("%s", venda.cliente.nome);
        getchar();
        printf("Digite o Código do cliente: ");
        scanf("%s", venda.cliente.codCliente);

        // Data da venda para consulta de dados posterior 
        printf("Digite a data da venda (dd/mm/yyyy): ");
        scanf("%d/%d/%d", &venda.dataVenda.dia, &venda.dataVenda.mes, &venda.dataVenda.ano);
        
        // Valores obtidos
        *faturamentoBruto += venda.precoFinal;
        (*numClientes)++;

        vendas[(*numVendas)++] = venda;
    }

    // Ordenação baseado na função de comparação
    qsort(vendas, *numVendas, sizeof(Venda), compararVendas);

    // Busca pelo item mais e menos vendido
    encontrarItemMaisVendido(vendas, *numVendas);
    encontrarItemMenosVendido(vendas, *numVendas);

    // Relatório de Vendas Geral
    printf("\n========== Relatorio de Vendas Geral ==========\n");
    for (int i = 0; i < *numVendas; i++) {
        printf("Codigo: %d, Item: %s, Marca: %s, Quantidade: %d, Preco Unitario: R$ %.2f, Preco Final: R$ %.2f, Data: %02d/%02d/%04d, Cliente: %s, Código do Cliente: %s\n",
            vendas[i].codigo, vendas[i].item, vendas[i].marca, vendas[i].quantidade,
            vendas[i].precoUnitario, vendas[i].precoFinal, vendas[i].dataVenda.dia, vendas[i].dataVenda.mes, vendas[i].dataVenda.ano, vendas[i].cliente.nome, vendas[i].cliente.codCliente);
    }

     // Salva os dados atualizados no arquivo
    salvarDados(vendas, *numVendas, arquivo);
}

// Consulta de vendas baseado na data
void listarVendasPorData(Venda vendas[], int numVendas, FILE *arquivo) {
    Data dataConsulta;

    // Consultando pela data as vendas do dia
    printf("Digite a data para consultar as vendas (dd/mm/yyyy): ");
    scanf("%d/%d/%d", &dataConsulta.dia, &dataConsulta.mes, &dataConsulta.ano);

    // Loop para demonstração de todas as vendas realizadas no dia listado, modelo (Código - Item - Marca, Quantidade - Preço Unitário - Preço Final)
    printf("\nVendas realizadas na data %02d/%02d/%04d:\n", dataConsulta.dia, dataConsulta.mes, dataConsulta.ano);
    printf("=================================================\n");

    int encontradas = 0;
    for (int i = 0; i < numVendas; i++) {
        if (vendas[i].dataVenda.dia == dataConsulta.dia &&
            vendas[i].dataVenda.mes == dataConsulta.mes &&
            vendas[i].dataVenda.ano == dataConsulta.ano) {
            printf("Codigo: %d, Item: %s, Marca: %s, Quantidade: %d, Preco Unitario: R$ %.2f, Preco Final: R$ %.2f\n",
                   vendas[i].codigo, vendas[i].item, vendas[i].marca, vendas[i].quantidade,
                   vendas[i].precoUnitario, vendas[i].precoFinal);
            encontradas++;
        }
    }

    // Tratamento de erros para não existência de vendas.
    if (encontradas == 0) {
        printf("Nenhuma venda encontrada para a data especificada.\n");
    }
}

// Função para salvar os dados no arquivo
void salvarDados(Venda vendas[], int numVendas, FILE *arquivo) {
    fseek(arquivo, 0, SEEK_SET);
    fwrite(&numVendas, sizeof(int), 1, arquivo);
    fwrite(vendas, sizeof(Venda), numVendas, arquivo);
}

// Função para carregar os dados do arquivo
void carregarDados(Venda vendas[], int *numVendas, FILE *arquivo) {
    fseek(arquivo, 0, SEEK_SET);
    fread(numVendas, sizeof(int), 1, arquivo);
    fread(vendas, sizeof(Venda), *numVendas, arquivo);
}

// Função para encontrar o item mais vendido
void encontrarItemMaisVendido(Venda vendas[], int numVendas) {
    int maxVendas = 0;
    char itemMaisVendido[100];

    for (int i = 0; i < numVendas; i++) {
        if (vendas[i].quantidade > maxVendas) {
            maxVendas = vendas[i].quantidade;
            strcpy(itemMaisVendido, vendas[i].item);
        }
    }

    printf("\nO item mais vendido é: %s com %d unidades vendidas.\n", itemMaisVendido, maxVendas);
}

// Função para encontrar o item menos vendido
void encontrarItemMenosVendido(Venda vendas[], int numVendas) {
    int minVendas = vendas[0].quantidade;
    char itemMenosVendido[100];

    for (int i = 1; i < numVendas; i++) {
        if (vendas[i].quantidade < minVendas) {
            minVendas = vendas[i].quantidade;
            strcpy(itemMenosVendido, vendas[i].item);
        }
    }

    printf("\nO item menos vendido é: %s com %d unidades vendidas.\n", itemMenosVendido, minVendas);
}
