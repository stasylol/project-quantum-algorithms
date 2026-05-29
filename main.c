#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "quantum.h"

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

int main() {
    int n = 3;             
    size_t target_x0 = 5;  

    size_t N = (size_t)1 << n;
    if (target_x0 >= N) {
        fprintf(stderr, "Ошибка: Искомый элемент вне диапазона базиса.\n");
        return EXIT_FAILURE;
    }

    printf("=== Квантовая симуляция алгоритма Гровера (RAM версия) ===\n");
    printf("Кубитов (n): %d, Размер базы (N): %zu\n", n, N);
    printf("Искомый элемент (x0): %zu\n\n", target_x0);


    QuantumState state = create_quantum_state(n);

    // суперпозиция
    apply_hadamard_all(&state);
    printf("После инициализации H^xn (равномерная суперпозиция):\n");
    print_state(&state);
    printf("Ненулевых состояний в списке: %d\n\n", count_nonzero_states(&state));

    
    int iterations = (int)floor((M_PI / 4.0) * sqrt((double)N));
    printf("Запуск итераций Гровера. Количество шагов: %d\n", iterations);

    for (int step = 0; step < iterations; step++) {
        apply_oracle(&state, target_x0);
        apply_diffusion(&state);
    }


    printf("\n=== Результат измерения ===\n");
    print_state(&state);
    
    double _Complex final_amp = get_amplitude(&state, target_x0);
    double probability = pow(cabs(final_amp), 2);
    
    printf("\nНайденный элемент: %zu с вероятностью P(x0) = %.4f\n", target_x0, probability);
    printf("Осталось элементов в памяти: %d\n", count_nonzero_states(&state));

    
    free_quantum_state(&state);
    return EXIT_SUCCESS;
}

