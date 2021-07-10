#ifndef HASH_ENDERECAMENTO_ABERTO_H
#define HASH_ENDERECAMENTO_ABERTO_H

#include "funcoes_hash.h"

// Célula específica para contar o número de ocorrências de
// palavras em um texto para tabela hash fechada (ou de endereçamento aberto)
struct TCelula {
   std::string palavra;
   int frequencia;
};

// Celula "padrao" vazia!
const TCelula CELULA_NULA{"",0};

/// A tabela de Endereçamento aberto também é conhecida como Tabela Hash Fechada!
struct TabelaHash_EnderecamentoAberto {
    TCelula *tabela;
    int tamanho;
    FuncaoHash* h;
};

bool operator==(const TCelula& a, const TCelula& b) {
    if(a.frequencia == b.frequencia &&
            a.palavra == b.palavra) {
        return true;
    }
    return false;
}
bool operator!=(const TCelula& a, const TCelula& b) {
    return !(a == b);
}


/**
 * @brief inicializar_tabela_hash_fechada Função para inicialização da tabela
 * hash de endereçamento aberto de tamanho personalizável.
 * @param tabela Tabela a ser inicializada.
 * @param tamanho Tamanho da tabela.
 * @param f_h Função hashing para callback nas operações de tabela
 * É necessário passar a função de hash a ser usada pela tabela
 */
void inicializar_tabela_hash_fechada(TabelaHash_EnderecamentoAberto& tabela,
                                     int tamanho,
                                     FuncaoHash* f_h) {
    tabela.tabela = new TCelula[tamanho];
    tabela.tamanho = tamanho;
    tabela.h = f_h;
    for(int i = 0; i < tamanho; i++) {
        tabela.tabela[i] = CELULA_NULA; // Todas as posições da tabela são iniciadas com o valor da célula nula
    }
}

int pesquisa_indice_hash_fechada(const TabelaHash_EnderecamentoAberto t_hash, const std::string chave) {
    int inicial = t_hash.h(chave,t_hash.tamanho);
    int indice = inicial;
    int i = 0;
    while( t_hash.tabela[indice] != CELULA_NULA
           && (chave != t_hash.tabela[indice].palavra)
           && i < t_hash.tamanho) {
        i++;
        indice = (inicial + i) % t_hash.tamanho;
    }

    if(t_hash.tabela[indice] != CELULA_NULA &&
            t_hash.tabela[indice].palavra == chave) {
        return indice;
    } else {
        return t_hash.tamanho; // Não encontrado
    }
}

TCelula& pesquisa_hash_fechada(const TabelaHash_EnderecamentoAberto t_hash, const std::string chave) {
    int indice = pesquisa_indice_hash_fechada(t_hash,chave);
    if(indice < t_hash.tamanho) {
        return t_hash.tabela[indice];
    }
    return (TCelula&) CELULA_NULA;
}

/**
 * @brief insere_tabela_hash_fechada Insere uma palavra na tabela hash de
 * endereçamento aberto, caso ela não exista ainda.
 * Se a palavra já existir, incrementa a frequência da mesma.
 * @param t_hash Tabela hash para inserir a palavra
 * @param palavra Palavra a ser inserida
 * @return 0 se a palavra foi inserida sem nenhuma colisão ou se a mesma já
 * existe e a frequência foi incrementada; -1 se a tabela estiver cheia; ou
 * o número de colisões ocasionados na inserção.
 */
int inserir_tabela_hash_fechada(TabelaHash_EnderecamentoAberto& t_hash,
                                const std::string palavra) {

    int indice_inicial = t_hash.h(palavra,t_hash.tamanho);
    TCelula& busca = pesquisa_hash_fechada(t_hash,palavra);
    if( busca == CELULA_NULA ) { // Ainda não foi inserida
        int indice = indice_inicial;
        int i = 0;
        while( t_hash.tabela[indice] != CELULA_NULA &&
               i < t_hash.tamanho) {
            i++;
            indice = (indice_inicial + i) % t_hash.tamanho;
#ifdef DEBUG_INSERTION
            std::cout << "Colisao '" << i << "' ao inserir a palavra: " << palavra
                      << std::endl;
#endif
        }
        if(i < t_hash.tamanho) {
            TCelula nova{palavra,1};
            t_hash.tabela[indice] = nova;
#ifdef DEBUG_INSERTION
            std::cout << "Palavra '" << palavra << "' inserida com '" << i
                      << "' colisoes" << std::endl;
#endif
            return i;
        } else {
#ifdef DEBUG_INSERTION
            std::cout << "Tabela cheia! Nao e' possivel inserir a palavra: "
                      << palavra << std::endl;
#endif
            return -1;
        }
    } else { // Já inserida
        busca.frequencia++;
#ifdef DEBUG_INSERTION
        std::cout << "Palavra '" << palavra  << "' ja existe - num repeticoes: "
                  << busca.frequencia << std::endl;
#endif
        return 0;
    }
}

void apagar_tabela_hash_fechada(TabelaHash_EnderecamentoAberto& t_hash,
                                const std::string chave) {

    int indice_dado = pesquisa_indice_hash_fechada(t_hash,chave);
    if(indice_dado == t_hash.tamanho) {
#ifdef DEBUG_DELETION
        std::cout << "Nao ha elemento com a chave '" << chave
                  << "' para ser removido!" << std::endl;
#endif
        return;
    }

    t_hash.tabela[indice_dado] = CELULA_NULA;

    int i = 1;
    int pos_vazia = indice_dado;
    int indice = (indice_dado+1)%t_hash.tamanho;
    while(t_hash.tabela[indice] != CELULA_NULA
          && i < t_hash.tamanho) {
        std::string palavra_atual = t_hash.tabela[indice].palavra;
        int hash_original = t_hash.h(palavra_atual,t_hash.tamanho);

        if(hash_original != indice) {
            t_hash.tabela[pos_vazia] = t_hash.tabela[indice];
            t_hash.tabela[indice] = CELULA_NULA;
#ifdef DEBUG_DELETION
            std::cout << "[DEL] Deslocando a palavra '" << palavra_atual
                      << "' da posicao '" << indice << "' para: "
                      << pos_vazia << std::endl;
#endif
            pos_vazia = indice;
        }
        i++;
        indice = (indice_dado + i) % t_hash.tamanho;
    }
}

std::ostream& operator << (std::ostream& os, const TCelula cel) {
    if(cel == CELULA_NULA) {
        os << "Celula NULA";
    } else {
        os << cel.palavra << " - ocorrencias: " << cel.frequencia;
    }
    return os;
}

void imprimir_tabela_hash_fechada(const TabelaHash_EnderecamentoAberto t_hash) {
    std::cout << "\nImprimindo Tabela Hash - Open Addressing!" << std::endl;
    for(int i = 0; i < t_hash.tamanho; i++) {
        std::cout << "Hash - Open Addressing[" << i << "]: " << t_hash.tabela[i] << std::endl;
    }
}

int num_celulas_ocupadas_hash_fechada(const TabelaHash_EnderecamentoAberto t_hash) {
    int num_ocupadas = 0;
    for(int i = 0; i < t_hash.tamanho; i++) {
        if(t_hash.tabela[i] != CELULA_NULA) {
            num_ocupadas++;
        }
    }
    return num_ocupadas;
}

float perc_ocupacao_hash_fechada(const TabelaHash_EnderecamentoAberto t_hash) {
    return (float) num_celulas_ocupadas_hash_fechada(t_hash) / (float) t_hash.tamanho;
}

#endif // HASH_ENDERECAMENTO_ABERTO_H
