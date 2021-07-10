#ifndef LER_ARQUIVO_H
#define LER_ARQUIVO_H

#include <fstream>
#include <list>
#include <string>
#include <iostream>

std::list<std::string> ler_arquivo(std::string nome_arquivo) {

    std::list<std::string> palavras;

    std::ifstream arquivo(nome_arquivo);
    if( arquivo.is_open() ) {
        std::string palavra;
        while( arquivo >> palavra ) {
            palavra.erase( palavra.find_last_not_of(" \n\r\t,.")+1 ); // trim = remove os espaços, vírgulas e pontos
            palavras.push_back(palavra);
        }
        arquivo.close();
    } else {
        std::cerr << "Nao foi possivel abrir o arquivo: " << nome_arquivo << std::endl;
    }
    return palavras;
}

#endif // LER_ARQUIVO_H
