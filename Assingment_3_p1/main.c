#include <time.h>
#include "matrix.h"

#define MAX_THREAD 1000

int main()
{
    matrix_t *p_matrix_a = NULL;
    matrix_t *p_matrix_b = NULL;
    clock_t start = 0;
    clock_t end = 0;
    double cpu_time_used = 0;
    int num_threads = 0;

    // Initialize_matrix
    printf("Input matrix A\n");
    p_matrix_a = initialize_matrix();
    printf("Input matrix B\n");
    p_matrix_b = initialize_matrix();

    // Print matrix
    printf("Matrix A:\n");
    print_matrix(p_matrix_a);
    printf("Matrix B:\n");
    print_matrix(p_matrix_b);

    // Check if matrix can multiply
    if (!check_matrix_can_multiply(p_matrix_a->m_column, p_matrix_b->m_row))
    {
        printf("Matrix can't multiply\n");
        free_matrix(2, p_matrix_a, p_matrix_b);
        return 0;
    }

    // Get input number of thread
    printf("Number of thread: ");
    get_input_int(&num_threads, 1, MAX_THREAD);

    // Calculate and print product of 2 matrix
    start = clock();
    matrix_t *p_matrix_product = multiply_matrix(p_matrix_a, p_matrix_b);
    end = clock();
    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
    printf("Matrix A * Matrix B not using multy thread:\n");
    print_matrix(p_matrix_product);
    printf("Function take %f s\n", cpu_time_used);

    // Calculate and print product of 2 matrix using multy thread
    printf("Matrix A * Matrix B using multy thread:\n");
    start = clock();
    matrix_t *p_matrix_product_multy_thread = multiply_matrix_multi_thread(p_matrix_a, p_matrix_b, num_threads);
    end = clock();
    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
    print_matrix(p_matrix_product_multy_thread);
    printf("Function take %f s\n", cpu_time_used);

    // Free allocated memory
    free_matrix(4, p_matrix_a, p_matrix_b, p_matrix_product, p_matrix_product_multy_thread);

    return 0;
}
