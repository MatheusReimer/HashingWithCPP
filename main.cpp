#include <iostream>
#include <list>
#include <cmath>

#include "hash_enderecamento_aberto.h"
#include "hash_encadeada.h"
#include "ler_arquivo.h"

using namespace std;

TabelaHash_EnderecamentoAberto executa_hash_fechada(list<string> palavras_texto,
                                                    int tamanho_hash,
                                                    FuncaoHash funcao_hash,
                                                    int& num_colisoes) {

    //////// Tabela Hash com endereçamento aberto - hash fechada
    TabelaHash_EnderecamentoAberto t_hash_fechada;
    inicializar_tabela_hash_fechada(t_hash_fechada, // Tabela
                                    tamanho_hash,   // Tamanho
                                    funcao_hash     // Funcao de Hash a ser usada
                                    );
    for(string palavra : palavras_texto) {
        num_colisoes += inserir_tabela_hash_fechada(t_hash_fechada,palavra);
    }

    cout << "\n-------- Tabela Fechada apos insercoes --------" << endl;
    imprimir_tabela_hash_fechada(t_hash_fechada);
    cout << "Ocorreram '" << num_colisoes << "' colisoes ao inserir o texto na hash fechada!" << endl;
    cout << "Numero celulas ocupadas na hash fechada: " << num_celulas_ocupadas_hash_fechada(t_hash_fechada)
         << " de " << tamanho_hash << " <-> " << perc_ocupacao_hash_fechada(t_hash_fechada)
         << "% de ocupacao" << endl;
    return t_hash_fechada;
}

TabelaHash_Encadeada executa_hash_aberta(list<string> palavras_texto,
                                         int tamanho_hash,
                                         FuncaoHash funcao_hash,
                                         int& num_colisoes) {
    //////// Tabela hash com encadeamento - hash aberta
    TabelaHash_Encadeada t_hash_encadeada;
    inicializar_hash_aberta(t_hash_encadeada,   // Tabela
                            tamanho_hash,       // Tamanho
                            funcao_hash         // Função de hashing a ser usada
                            );
    for(string palavra : palavras_texto) {
        num_colisoes += inserir_hash_aberta(t_hash_encadeada,palavra);
    }

    cout << "\n-------- Hash aberta apos insercoes --------" << endl;
    imprimir_hash_aberta(t_hash_encadeada);
    cout << "Ocorreram '" << num_colisoes << "' colisoes ao inserir o texto na hash aberta!" << endl;
    cout << "Numero celulas ocupadas na hash aberta: " << num_celulas_ocupadas_hash_aberta(t_hash_encadeada)
         << " de " << tamanho_hash << " <-> " << perc_ocupacao_hash_aberta(t_hash_encadeada)
         << "% de ocupacao" << endl;
    return t_hash_encadeada;
}

int main() {

    list<string> palavras_texto = ler_arquivo("teste");

    cout << "Numero de palavras total no texto: " << palavras_texto.size() << endl;
//    int tamanho_hash = ceil(palavras_texto.size() * 1.25); // Tabela com 25% de espaços a mais do que o número de palavras
    int tamanho_hash = ceil(palavras_texto.size() * 1.5);    // Tabela com 50% de espaços a mais do que o número de palavras
    cout << "O tamanho das hashes sera' de '" << tamanho_hash << "' posicoes!" << endl;

    // Define a função de hashing a ser usada
    FuncaoHash* funcao_hash = funcao_hash_soma_caracteres;
//  FuncaoHash* funcao_hash = funcao_hash_multiplica_caracteres;

    int n_colisoes_aberta = 0, n_colisoes_fechada = 0;
    TabelaHash_EnderecamentoAberto t_hash_fechada =
            executa_hash_fechada(palavras_texto,tamanho_hash,funcao_hash,n_colisoes_fechada);
    TabelaHash_Encadeada t_hash_aberta =
            executa_hash_aberta(palavras_texto,tamanho_hash,funcao_hash,n_colisoes_aberta);

    float perc_ocup_aberta = perc_ocupacao_hash_aberta(t_hash_aberta);
    float perc_ocup_fechada = perc_ocupacao_hash_fechada(t_hash_fechada);
    int ocupacao_aberta = num_celulas_ocupadas_hash_aberta(t_hash_aberta);
    int ocupacao_fechada = num_celulas_ocupadas_hash_fechada(t_hash_fechada);
    //// Fechamento de ambas as tabelas.
    cout << "\n\n----------- Estatisticas -----------\n"
            "\n!!!!  Qtd de Colisoes !!!!"
         << "\n\tHash Fechada (Enderecamento Aberto - Open Addressing): " << n_colisoes_fechada
         << "\n\tHash Aberta (Encadeada - Linked): " << n_colisoes_aberta
         << endl;
    cout << "O percentual de colisoes da tabela hash encadeada corresponde a: "
         << ((float)n_colisoes_aberta / (float)n_colisoes_fechada * 100)
         << "% da tabela de enderecamento aberto!" << endl;
    cout << "\n!!!! Ocupacao !!!!"
         << "\n\tHash Fechada: " << ocupacao_fechada << " de " << tamanho_hash
         << " <-> " << perc_ocup_fechada << "%"
         << "\n\tHash Aberta: " << ocupacao_aberta << " de " << tamanho_hash
         << " <-> " << perc_ocup_aberta << "%"
         << endl;
    cout << "    O total de palavras repetidas foi: "
         << (palavras_texto.size() - ocupacao_fechada)
         << endl;
    float relacao_ocupacao_fechada = ((float)ocupacao_fechada / (float) ocupacao_aberta);
    cout << "A ocupacao da tabela hash de enderecamento aberto e': "
         << ((relacao_ocupacao_fechada > 1) ? (relacao_ocupacao_fechada-1)*100 : 0)
         << "% maior do que a encadeada!" << endl;


/*  // Teste simplificado de operações de hash
    TabelaHash_EnderecamentoAberto t_hash_soma;
    inicializar_tabela_hash_fechada(t_hash_soma,10,funcao_hash_soma_caracteres);

    n_colisoes += inserir_tabela_hash_fechada(t_hash_soma,"oi"); // [6]
    n_colisoes += inserir_tabela_hash_fechada(t_hash_soma,"io"); // [6]
    n_colisoes += inserir_tabela_hash_fechada(t_hash_soma,"nj"); // [6]
    n_colisoes += inserir_tabela_hash_fechada(t_hash_soma,"oi"); // [6] - 2x
    n_colisoes += inserir_tabela_hash_fechada(t_hash_soma,"oj"); // [7]
    n_colisoes += inserir_tabela_hash_fechada(t_hash_soma,"ok"); // [8]

    std::cout << "Hash antes!" << std::endl;
    imprime_tabela_hash_fechada(t_hash_soma);

    apagar_tabela_hash_fechada(t_hash_soma,"oi"); // Teste 1
    apagar_tabela_hash_fechada(t_hash_soma,"io"); // Teste 2
    apagar_tabela_hash_fechada(t_hash_soma,"nj"); // Teste 3

    std::cout << "Hash depois!" << std::endl;
    imprime_tabela_hash_fechada(t_hash_soma);
*/
/*
    TabelaHash_EnderecamentoAberto tabela;
    inicializar_tabela_hash_fechada(tabela,10,funcao_hash_soma_caracteres);

    inserir_tabela_hash_fechada(tabela,
        "pneumoultramicroscopicossilicovulcanoconiotico");
    imprimir_tabela_hash_fechada(tabela);
*/
    return 0;
}
