#include "quantum.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

//регистр
QuantumState create_quantum_state(int n) {
    QuantumState state;
    state.n = n;
    state.size = (size_t)1 << n;
    state.head = NULL; 
    return state;
}


void set_amplitude(QuantumState* state, size_t index, double _Complex val) {
    if (cabs(val) < EPSILON) {
        // Если амплитуда упала ниже порога, удаляем узел из памяти
        Node** curr = &(state->head);
        while (*curr) {
            if ((*curr)->index == index) {
                Node* to_free = *curr;
                *curr = (*curr)->next;
                free(to_free);
                return;
            }
            curr = &((*curr)->next);
        }
        return;
    }

    // Если узел уже существует, просто обновляем его значение
    Node** curr = &(state->head);
    while (*curr) {
        if ((*curr)->index == index) {
            (*curr)->amplitude = val;
            return;
        }
        curr = &((*curr)->next);
    }

    // Если узла нет, создаем новый в динамической памяти 
    Node* new_node = (Node*)malloc(sizeof(Node));
    if (!new_node) {
        perror("Ошибка выделения памяти для узла");
        exit(EXIT_FAILURE);
    }
    new_node->index = index;
    new_node->amplitude = val;
    new_node->next = state->head;
    state->head = new_node; //Новый элемент становится первым.
}


double _Complex get_amplitude(const QuantumState* state, size_t index) {
    Node* curr = state->head;
    while (curr) {
        if (curr->index == index) {
            return curr->amplitude;
        }
        curr = curr->next;
    }
    return 0.0 + 0.0 * I;
}


int count_nonzero_states(const QuantumState* state) {
    int count = 0;
    Node* curr = state->head;
    while (curr) {
        count++;
        curr = curr->next;
    }
    return count;
}


void print_state(const QuantumState* state) {
    Node* curr = state->head;
    while (curr) {
        printf("|");
        for (int bit = state->n - 1; bit >= 0; bit--) {
            printf("%d", (int)((curr->index >> bit) & 1));
        }
        printf("> : %.4f + %.4fi (P=%.4f)\n", 
               creal(curr->amplitude), cimag(curr->amplitude), pow(cabs(curr->amplitude), 2));
        curr = curr->next;
    }
}

// Полное освобождение динамической памяти
void free_quantum_state(QuantumState* state) {
    Node* curr = state->head;
    while (curr) {
        Node* next = curr->next;
        free(curr);
        curr = next;
    }
    state->head = NULL;
}


void apply_hadamard_all(QuantumState* state) {
    double complex equal_amp = 1.0 / sqrt((double)state->size);
    for (size_t i = 0; i < state->size; i++) {
        set_amplitude(state, i, equal_amp + 0.0 * I);
    }
}


void apply_oracle(QuantumState* state, size_t target) {
    double complex amp = get_amplitude(state, target);
    set_amplitude(state, target, -amp);
}


void apply_diffusion(QuantumState* state) {
    double complex sum = 0;
    Node* curr = state->head;
    while (curr) {
        sum += curr->amplitude;
        curr = curr->next;
    }
    double complex mean = sum / (double)state->size;


    double complex* tmp = (double complex*)calloc(state->size, sizeof(double complex));
    for (size_t i = 0; i < state->size; i++) {
        double complex old_amp = get_amplitude(state, i);
        tmp[i] = 2.0 * mean - old_amp;
    }
    for (size_t i = 0; i < state->size; i++) {
        set_amplitude(state, i, tmp[i]);
    }
    free(tmp);
}
