// Implemente em C a estimativa estocástica de π. Paralelize com #pragma omp parallel for e explique o resultado incorreto. Corrija a condição de corrida utilizando o #pragma omp critical e reestruturando com #pragma omp parallel seguido de #pragma omp for e aplicando as cláusulas private, firstprivate, lastprivate e shared. Teste diferentes combinações e explique como cada cláusula afeta o comportamento do programa. Comente também como a cláusula default(none) pode ajudar a tornar o escopo mais claro em programas complexos.

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <omp.h>

const int N = 9999999; /**< Number of Monte Carlo samples */

/**
 * @brief Sequential version of π estimation
 * @details Demonstrates the baseline sequential implementation
 */
void test_sequential()
{
    clock_t start = clock();
    int count = 0;
    
    for (int i = 0; i < N; i++)
    {
        double x = (double)rand() / RAND_MAX;
        double y = (double)rand() / RAND_MAX;
        if (x * x + y * y <= 1.0)
            count++;
    }

    double pi = 4.0 * count / N;
    double elapsed = (double)(clock() - start) / CLOCKS_PER_SEC;
    printf("✅ Case 1 - Sequential:      π ≈ %-15.15f | Time: %.3fs\n", pi, elapsed);
}

/**
 * @brief Naive parallel version with race condition
 * @details Shows incorrect results due to race condition on 'count'
 */
void test_with_pragma_omp_parallel_for()
{
    clock_t start = clock();
    int count = 0;
    
    #pragma omp parallel for
    for (int i = 0; i < N; i++)
    {
        double x = (double)rand() / RAND_MAX;
        double y = (double)rand() / RAND_MAX;
        if (x * x + y * y <= 1.0)
            count++;
    }

    double pi = 4.0 * count / N;
    double elapsed = (double)(clock() - start) / CLOCKS_PER_SEC;
    printf("❌ Case 2 - Parallel for:    π ≈ %-15.15f | Time: %.3fs (race condition)\n", pi, elapsed);
}

/**
 * @brief Corrected version with critical section
 * @details Fixes race condition but with performance penalty
 */
void test_with_pragma_omp_parallel_and_parallel_for()
{
    clock_t start = clock();
    int count = 0;
    
    #pragma omp parallel
    {
        #pragma omp for
        for (int i = 0; i < N; i++)
        {
            double x = (double)rand() / RAND_MAX;
            double y = (double)rand() / RAND_MAX;
            #pragma omp critical
            if (x * x + y * y <= 1.0)
                count++;
        }
    }

    double pi = 4.0 * count / N;
    double elapsed = (double)(clock() - start) / CLOCKS_PER_SEC;
    printf("✅ Case 3 - Critical:        π ≈ %-15.15f | Time: %.3fs (idleness)\n", pi, elapsed);
}

/**
 * @brief Version using private variable
 * @details Uses thread-local variable to reduce synchronization
 */
void test_with_pragma_omp_parallel_and_parallel_for__private()
{
    clock_t start = clock();
    int count = 0;
    int local_count = 0;

    #pragma omp parallel private(local_count)
    {
        local_count = 0;
        #pragma omp for
        for (int i = 0; i < N; i++)
        {
            double x = (double)rand() / RAND_MAX;
            double y = (double)rand() / RAND_MAX;
            if (x * x + y * y <= 1.0)
                local_count++;
        }

        #pragma omp critical
        count += local_count;
    }

    double pi = 4.0 * count / N;
    double elapsed = (double)(clock() - start) / CLOCKS_PER_SEC;
    printf("✅ Case 4 - Private:         π ≈ %-15.15f | Time: %.3fs\n", pi, elapsed);
}

/**
 * @brief Version using firstprivate
 * @details Demonstrates thread-local initialization of seed
 */
void test_with_pragma_omp_parallel_and_parallel_for__firstprivate()
{
    clock_t start = clock();
    int count = 0;
    unsigned int seed = time(NULL);

    #pragma omp parallel firstprivate(seed)
    {
        int local_count = 0;
        seed += omp_get_thread_num();

        #pragma omp for
        for (int i = 0; i < N; i++)
        {
            double x = (double)rand_r(&seed) / RAND_MAX;
            double y = (double)rand_r(&seed) / RAND_MAX;
            if (x * x + y * y <= 1.0)
                local_count++;
        }

        #pragma omp critical
        count += local_count;
    }

    double pi = 4.0 * count / N;
    double elapsed = (double)(clock() - start) / CLOCKS_PER_SEC;
    printf("✅ Case 5 - Firstprivate:    π ≈ %-15.15f | Time: %.3fs\n", pi, elapsed);
}

/**
 * @brief Version using lastprivate
 * @details Demonstrates preservation of last iteration value
 */
void test_with_pragma_omp_parallel_and_parallel_for__lastprivate()
{
    clock_t start = clock();
    int count = 0;
    int last_i = -1;

    #pragma omp parallel
    {
        int local_count = 0;

        #pragma omp for lastprivate(last_i)
        for (int i = 0; i < N; i++)
        {
            double x = (double)rand() / RAND_MAX;
            double y = (double)rand() / RAND_MAX;
            if (x * x + y * y <= 1.0)
                local_count++;
            last_i = i;
        }

        #pragma omp critical
        count += local_count;
    }

    double pi = 4.0 * count / N;
    double elapsed = (double)(clock() - start) / CLOCKS_PER_SEC;
    printf("✅ Case 6 - Lastprivate:     π ≈ %-15.15f | Time: %.3fs | Last i: %d\n", pi, elapsed, last_i);
}

/**
 * @brief Version using default(none)
 * @details Forces explicit scope declaration for all variables
 */
void test_with_pragma_omp_parallel_and_parallel_for__default_none()
{
    clock_t start = clock();
    int count = 0;

    #pragma omp parallel default(none) shared(count, N)
    {
        int local_count = 0;
        unsigned int seed = time(NULL) ^ omp_get_thread_num();

        #pragma omp for
        for (int i = 0; i < N; i++)
        {
            double x = (double)rand_r(&seed) / RAND_MAX;
            double y = (double)rand_r(&seed) / RAND_MAX;
            if (x * x + y * y <= 1.0)
                local_count++;
        }

        #pragma omp critical
        count += local_count;
    }

    double pi = 4.0 * count / N;
    double elapsed = (double)(clock() - start) / CLOCKS_PER_SEC;
    printf("✅ Case 7 - Default(none):   π ≈ %-15.15f | Time: %.3fs\n", pi, elapsed);
}

/**
 * @brief Main function
 * @details Runs all test cases and prints results
 */
int main()
{
    printf("==> Reference π ≈ 3.141592653589793\n");
    printf("==== OpenMP π Estimation Tests ====\n\n");

    test_sequential();
    test_with_pragma_omp_parallel_for();
    test_with_pragma_omp_parallel_and_parallel_for();
    test_with_pragma_omp_parallel_and_parallel_for__private();
    test_with_pragma_omp_parallel_and_parallel_for__firstprivate();
    test_with_pragma_omp_parallel_and_parallel_for__lastprivate();
    test_with_pragma_omp_parallel_and_parallel_for__default_none();

    printf("\n(Use OMP_NUM_THREADS to change thread count)\n");
    return 0;
}