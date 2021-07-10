#ifndef FUNCOES_HASH_H
#define FUNCOES_HASH_H

#include <string>

// Typedef (Definição de tipo) da "interface" de função hash
typedef int FuncaoHash(std::string palavra,int tamanho);

/////////////////// Funções de Hashing ///////////////////

int funcao_hash_soma_caracteres(std::string palavra, int tamanho_hash) {
   int soma = 0;
   for(unsigned int i = 0; i < palavra.length(); i++) {
       soma += palavra.at(i);
   }
#ifdef DEBUG_HASH_FUNCTION
   std::cout << "Funcao Hash - Soma de caracteres - Palavra: " << palavra
             << " - Hash de Tamanho: " << tamanho_hash << " - somatorio: "
             << soma << " - indice: " << (soma%tamanho_hash) << std::endl;
#endif
   return (soma % tamanho_hash);
}

int funcao_hash_multiplica_caracteres(std::string palavra, int tamanho_hash) {
   long long unsigned int multiplicador = 1;
   for(unsigned int i = 0; i < palavra.length(); i++) {
       multiplicador *= palavra.at(i);
   }
#ifdef DEBUG_HASH_FUNCTION
   std::cout << "Funcao Hash - Multiplicacao de caracteres - Palavra: " << palavra
             << " - Hash de Tamanho: " << tamanho_hash << " - produtorio: "
             << multiplicador << " - indice: " << (multiplicador%tamanho_hash) << std::endl;
#endif
   return (multiplicador % tamanho_hash);
}



#endif // FUNCOES_HASH_H
