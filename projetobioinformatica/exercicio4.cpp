#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <map>
#include <omp.h>
#include <mpi.h>

using namespace std;

map<string, char> criarTabelaAminoacidos() {
    return {
        {"CCA", 'P'}, {"CCG", 'P'}, {"CCU", 'P'}, {"CCC", 'P'}, 
        {"UCU", 'S'}, {"UCA", 'S'}, {"UCG", 'S'}, {"UCC", 'S'}, 
        {"CAG", "G"}, {"CAA", "G"},
        {"ACA", 'T'}, {"ACC", 'T'}, {"ACU", 'T'}, {"ACG", 'T'}, 
        {"AUG", 'M'}, 
        {"UGA", 'C'}, 
        {"UGC", 'I'}, {"UGU", 'I'},
        {"GUG", 'V'}, {"GUA", 'V'}, {"GUC", 'V'}, {"GUU", 'V'}

    };
}