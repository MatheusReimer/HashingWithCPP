#ifndef HASH_ENCADEADA_H
#define HASH_ENCADEADA_H

#include "funcoes_hash.h"

// Célula específica para contar o número de ocorrências de
// palavras em um texto para tabela hash aberta (ou encadeada)
struct TCelulaEnc {
    std::string palavra;
    int frequencia;
    TCelulaEnc* proxima;
};

struct TabelaHash_Encadeada {
    TCelulaEnc** tabela;
    int tamanho;
    FuncaoHash* h;
};

bool operator==(const TCelulaEnc& a, const TCelulaEnc& b) {
    if(a.frequencia == b.frequencia &&
            a.palavra == b.palavra) {
        return true;
    }
    return false;
}
bool operator!=(const TCelulaEnc& a, const TCelulaEnc& b) {
    return !(a == b);
}

/**
 * @brief inicializar_hash_aberta Inicializa a tabela hash encadeada com
 * um tamanho personalizado e função de hashing específica. Todas as posições
 * da tabela são inicializadas como "Nulas".
 * @param t Tabela a ser inicializada
 * @param tamanho Tamanho da tabela a ser inicializada
 * @param f_h Função hash a ser utilizada pela tabela
 */
void inicializar_hash_aberta(TabelaHash_Encadeada& t, int tamanho,FuncaoHash* f_h) {

    t.tabela = new TCelulaEnc*[tamanho];
    t.tamanho = tamanho;
    t.h = f_h;
    for(int i = 0; i < tamanho; i++) {
        t.tabela[i] = NULL;
    }
}

/**
 * @brief obter_celula_hash_aberta Obtém uma célula caso já esteja presente na tabela.
 * @param t Tabela em que será realizada a busca
 * @param indice_hash Índice na tabela "já" mapeado pela função hash
 * @param indice_encadeamento Posição no encadeamento da célula buscada
 * @return A Célula buscada caso exista, NULL caso não exista na tabela (algum parâmetro inválido).
 */
TCelulaEnc* obter_celula_hash_aberta(const TabelaHash_Encadeada t, int indice_hash, int indice_encadeamento) {
    if(indice_hash < 0 || indice_hash >= t.tamanho) {
        return NULL;
    }
    TCelulaEnc* nav = t.tabela[indice_hash];
    int i = 0;
    while(i < indice_encadeamento) {
        if(nav == NULL) {
            return NULL;
        }
        i++;
        nav = nav->proxima;
    }
    return nav;
}

/**
 * @brief pesquisa_indice_hash_aberta Identifica o índice no encadeamento de determinada
 * posição da tabela hash
 * @param t Tabela a ser buscada
 * @param chave Chave/palavra a ser buscada
 * @param indice_hash (Opcional) Se passado, evita a chamada da função de hash
 * @return O índice do elemento com a chave buscada no encadeamento de determinada posição (indice_hash).
 * -1 Caso não encontre a chave buscada.
 */
int pesquisa_indice_hash_aberta(const TabelaHash_Encadeada t, std::string chave, int indice_hash=-1) {

    if(indice_hash == -1) {
        indice_hash = t.h(chave, t.tamanho);
    }

    if(t.tabela[indice_hash] != NULL) {
        TCelulaEnc* nav = t.tabela[indice_hash];
        int i = 0;
        while(nav != NULL) {
            if(nav->palavra == chave) {
                return i;
            }
            i++;
            nav = nav->proxima;
        }
    }
    return -1;
}

TCelulaEnc* pesquisa_hash_aberta(const TabelaHash_Encadeada t,std::string chave) {
    int indice = t.h(chave, t.tamanho);
    int indice_enc = pesquisa_indice_hash_aberta(t,chave,indice);
    if(indice_enc < 0) {
        return NULL;
    }
    return obter_celula_hash_aberta(t,indice,indice_enc);
}

TCelulaEnc* nova_celula_enc(std::string palavra) {
    TCelulaEnc* nova = new TCelulaEnc;
    nova->frequencia = 1;
    nova->palavra = palavra;
    nova->proxima = NULL;
    return nova;
}

int inserir_hash_aberta(TabelaHash_Encadeada& t, std::string palavra){
    int indice = t.h(palavra,t.tamanho);
    TCelulaEnc* nav = t.tabela[indice];
    while(nav != NULL) {
        if(nav->palavra == palavra) {
            nav->frequencia++;
#ifdef DEBUG_INSERTION
            std::cout << "Palavra '" << palavra << "' ja existe - num de repeticoes: "
                      << nav->frequencia << std::endl;

#endif
            return 0; // Não houve colisão - já existe
        }
        if(nav->proxima == NULL) {
            nav->proxima = nova_celula_enc(palavra);
#ifdef DEBUG_INSERTION
            std::cout << "Inserindo no final da posicao: " << indice << std::endl;
#endif
            return 1; // Insere no fim do encadeamento, ou seja, houve colisão!
        }
        nav = nav->proxima;
    }

#ifdef DEBUG_INSERTION
    std::cout << "Inserindo no inicio da posicao: " << indice << std::endl;
#endif
    // Insere no início - não houve colisão, pois é a primeira célula na posição
    t.tabela[indice] = nova_celula_enc(palavra);
    return 0;

/*  // Estratégia alternativa!
// Buscando e incrementando. Se não encontrar, insere no início do encadeamento!
    TCelulaEnc* busca = pesquisa_hash_aberta(t,chave);
    if(busca != NULL){ // Encontrou
        busca->frequencia++;
#define DEBUG_INSERTION
        std::cout << "Palavra '" << palavra << "' ja existe - num de repeticoes: "
                  << busca->frequencia << " - conferindo celula: " << busca->palavra
                  << std::endl;
#endif
        return 0;
    }
    // Ao não encontrar a chave, insere no início da posição calculada pela função
    // hash
    int indice = t.h(palavra,t.tamanho);
    TCelulaEnc* novo = nova_celula_enc(palavra);
    novo->proxima = t.tabela[indice];
    t.tabela[indice] = novo;
    if(novo->proxima == NULL) {
        return 0; // Não houve colisão
    } else {
        return 1; // Houve colisão, já existe(m) Célula(s) na mesma posição.
    }
*/
}

std::ostream& operator << (std::ostream& os, TCelulaEnc* cel) {
    if(cel == NULL) {
        os << "Celula NULA";
    } else {
        for(int i = 0; cel != NULL; i++,cel=cel->proxima) {
            os << "\n\t| Item {" << i << "}: " << cel->palavra << " - freq: " << cel->frequencia;
        }
    }
    return os;
}

void imprimir_hash_aberta(const TabelaHash_Encadeada t) {
    std::cout << "\nImprimindo Tabela Hash - Linked!" << std::endl;
    for(int i = 0; i < t.tamanho; i++) {
        std::cout << "Hash Linked[" << i << "]: " << t.tabela[i] << std::endl;
    }
}

int num_celulas_ocupadas_hash_aberta(const TabelaHash_Encadeada t_hash) {
    int num_ocupadas = 0;
    for(int i = 0; i < t_hash.tamanho; i++) {
        if(t_hash.tabela[i] != NULL) {
            num_ocupadas++;
        }
    }
    return num_ocupadas;
}

float perc_ocupacao_hash_aberta(const TabelaHash_Encadeada t_hash) {
    return (float) num_celulas_ocupadas_hash_aberta(t_hash) / (float) t_hash.tamanho;
}


#endif // HASH_ENCADEADA_H
