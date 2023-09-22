#include "matrix.h"

bool check_matrix_can_add(int matrix_a_row, int matrix_a_col, int matrix_b_row, int matrix_b_col)
{
    return (matrix_a_row == matrix_b_row && matrix_a_col == matrix_b_col);
}

bool check_matrix_can_multiply(int matrix_a_col, int matrix_b_row)
{
    return (matrix_a_col == matrix_b_row);
}

void get_matrix_size(int *row, int *collum)
{
    printf("Matrix Row: ");
    get_input_int(row, 1, MATRIX_ROW_MAX);
    printf("Matrix Col: ");
    get_input_int(collum, 1, MATRIX_COLUNM_MAX);
}

void get_matrix_data_random(matrix_t *p_matrix)
{
    float data = 0;
    int r = 0;

    // Take input matrix from the keyboard
    for (int row = 0; row < p_matrix->m_row; row++)
    {
        for (int collum = 0; collum < p_matrix->m_column; collum++)
        {
            r = rand() % 10;
            p_matrix->m_data[row][collum] = r;
        }
    }
}

void get_matrix_data(matrix_t *p_matrix)
{
    float data = 0;

    // Take input matrix from the keyboard
    for (int row = 0; row < p_matrix->m_row; row++)
    {
        for (int collum = 0; collum < p_matrix->m_column; collum++)
        {
            printf("Enter elememt %i %i of matrix: ", row, collum);
            get_input_float(&data, DBL_MIN, DBL_MAX);
            p_matrix->m_data[row][collum] = data;
        }
    }
}

matrix_t *create_matrix(int row, int collum)
{
    matrix_t *return_matrix = malloc(sizeof(matrix_t));
    return_matrix->m_row = row;
    return_matrix->m_column = collum;

    return_matrix->m_data = (float **)malloc(row * sizeof(float *));
    for (int i = 0; i < row; i++)
    {
        return_matrix->m_data[i] = (float *)malloc(collum * sizeof(float));
    }

    return return_matrix;
}

matrix_t *initialize_matrix()
{
    int row = 0;
    int collum = 0;

    // Get matrix size
    get_matrix_size(&row, &collum);

    matrix_t *return_matrix = create_matrix(row, collum);

    //get_matrix_data(return_matrix);
    get_matrix_data_random(return_matrix);

    return return_matrix;
}

matrix_t *initialize_matrix_rand(int row, int collum)
{
    // Get matrix size
    //get_matrix_size(&row, &collum);

    matrix_t *return_matrix = create_matrix(row, collum);

    //get_matrix_data(return_matrix);
    get_matrix_data_random(return_matrix);

    return return_matrix;
}

void print_matrix(matrix_t *p_matrix)
{
    for (int row = 0; row < p_matrix->m_row; row++)
    {
        for (int collum = 0; collum < p_matrix->m_column; collum++)
        {
            printf("%10.2f", p_matrix->m_data[row][collum]);
        }
        printf("\n");
    }
}

matrix_t *sum_matrix(matrix_t *p_matrix_a, matrix_t *p_matrix_b)
{
    if (!check_matrix_can_add(p_matrix_a->m_row, p_matrix_a->m_column, p_matrix_b->m_row, p_matrix_b->m_column))
    {
        return NULL;
    }

    matrix_t *p_matrix_sum = initialize_matrix(p_matrix_a->m_row, p_matrix_a->m_column);

    for (int row = 0; row < p_matrix_sum->m_row; row++)
    {
        for (int collum = 0; collum < p_matrix_sum->m_column; collum++)
        {
            p_matrix_sum->m_data[row][collum] = p_matrix_a->m_data[row][collum] + p_matrix_b->m_data[row][collum];
        }
    }

    return p_matrix_sum;
}

matrix_t *multiply_matrix(matrix_t *p_matrix_a, matrix_t *p_matrix_b)
{
    matrix_t *p_matrix_product = NULL;

    if (!check_matrix_can_multiply(p_matrix_a->m_column, p_matrix_b->m_row))
    {
        return NULL;
    }

    // Allocate memory for result matrix
    p_matrix_product = create_matrix(p_matrix_a->m_row, p_matrix_a->m_column);

    // Calculate product of 2 matrix
    for (int i = 0; i < p_matrix_a->m_row; i++)
    {
        for (int j = 0; j < p_matrix_b->m_column; j++)
        {
            p_matrix_product->m_data[i][j] = 0;
            for (int k = 0; k < p_matrix_a->m_column; k++)
            {
                p_matrix_product->m_data[i][j] += p_matrix_a->m_data[i][k] * p_matrix_b->m_data[k][j];
            }
        }
    }

    return p_matrix_product;
}

void free_matrix(int count, ...)
{
    va_list list;
    va_start(list, count);

    // Loop through list of matrix
    for (int i = 0; i < count; i++)
    {
        matrix_t *p_matrix = va_arg(list, matrix_t *);
        if (p_matrix == NULL)
        {
            return;
        }

        for (int j = 0; j < p_matrix->m_row; j++)
        {
            free(p_matrix->m_data[j]);
        }
        free(p_matrix->m_data);
        free(p_matrix);

        p_matrix = NULL;
    }
}

// Function to multiply
void *multiply(void *arg)
{
    thread_arg_t *t_arg = (thread_arg_t *)arg;
    matrix_t *matrix_a = t_arg->matrix_a;
    matrix_t *matrix_b = t_arg->matrix_b;
    matrix_t *result = t_arg->result;
    int start_row = t_arg->start_row;
    int end_row = t_arg->end_row;
    float sum = 0.0;

    for (int i = start_row; i <= end_row; i++)
    {
        for (int j = 0; j < matrix_b->m_column; j++)
        {
            sum = 0.0;
            for (int k = 0; k < matrix_a->m_column; k++)
            {
                sum += matrix_a->m_data[i][k] * matrix_b->m_data[k][j];
            }
            result->m_data[i][j] = sum;
        }
    }

    pthread_exit(NULL);
}

// Function to perform matrix multiplication using multiple threads
matrix_t *multiply_matrix_multi_thread(matrix_t *matrix_a, matrix_t *matrix_b, int num_threads)
{
    matrix_t *result = NULL;
    pthread_t *threads = NULL;
    thread_arg_t *thread_args = NULL;
    int rows_per_thread = 0;
    int start_row = 0;
    int end_row = 0;

    if (matrix_a->m_column != matrix_b->m_row)
    {
        return NULL;
    }

    // Allocate memory for result matrix
    result = create_matrix(matrix_a->m_row, matrix_b->m_column);

    // Allocate memory for thread arguments
    threads = (pthread_t *)malloc(sizeof(pthread_t) * num_threads);
    thread_args = (thread_arg_t *)malloc(sizeof(thread_arg_t) * num_threads);

    // Calculate the number of row each thread calculate
    rows_per_thread = matrix_a->m_row / num_threads;
    start_row = 0;
    end_row = rows_per_thread - 1;

    for (int i = 0; i < num_threads; i++)
    {
        // Initialize thread args
        thread_args[i].matrix_a = matrix_a;
        thread_args[i].matrix_b = matrix_b;
        thread_args[i].result = result;
        thread_args[i].start_row = start_row;
        thread_args[i].end_row = end_row;

        // Initialize thread
        pthread_create(&threads[i], NULL, multiply, (void *)&thread_args[i]);

        // Update start_row and end_row 
        start_row = end_row + 1;
        end_row += rows_per_thread;
        if (i == num_threads - 2)
        {
            end_row = matrix_a->m_row - 1;
        }
    }

    // Wait for all threads to finish
    for (int i = 0; i < num_threads; i++)
    {
        pthread_join(threads[i], NULL);
    }

    // Free memory
    free(threads);
    free(thread_args);

    threads = NULL;
    thread_args = NULL;

    return result;
}