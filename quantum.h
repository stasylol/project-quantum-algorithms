#ifndef QUANTUM_H
#define QUANTUM_H

#include <stdio.h>
#include <stdlib.h>
#include <complex.h>

#define EPSILON 1e-10

// Структура узла разреженного списка
struct Node {
    size_t index;                  
    double _Complex amplitude;     
    struct Node* next;
};
typedef struct Node Node;


typedef struct {
    int n;          
    size_t size;    
    Node* head;     
} QuantumState;


QuantumState create_quantum_state(int n);
void free_quantum_state(QuantumState* state);
void set_amplitude(QuantumState* state, size_t index, double _Complex val);
double _Complex get_amplitude(const QuantumState* state, size_t index);
int count_nonzero_states(const QuantumState* state);
void print_state(const QuantumState* state);

// Прототипы квантовых операций алгоритма Гровера
void apply_hadamard_all(QuantumState* state);
void apply_oracle(QuantumState* state, size_t target);
void apply_diffusion(QuantumState* state);

#endif 


