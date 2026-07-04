#include <stdio.h>
#include <math.h>
#include <mpi.h>

double f(double x)
{
    return x * x;          // Example: x²
}

double Trap(double left_endpt,
            double right_endpt,
            int trap_count,
            double base_len)
{
    double estimate;
    double x;
    int i;

    estimate = (f(left_endpt) + f(right_endpt)) / 2.0;

    for(i = 1; i < trap_count; i++)
    {
        x = left_endpt + i * base_len;
        estimate += f(x);
    }

    estimate = estimate * base_len;

    return estimate;
}

int main(int argc, char* argv[])
{
    int my_rank;
    int comm_sz;

    double a = 0.0;
    double b = 10.0;

    int n = 1000000;

    double h;

    double local_a;
    double local_b;

    int local_n;

    double local_int;
    double total_int;

    MPI_Init(&argc, &argv);

    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &comm_sz);

    h = (b - a) / n;

    local_n = n / comm_sz;

    local_a = a + my_rank * local_n * h;
    local_b = local_a + local_n * h;

    local_int = Trap(local_a, local_b, local_n, h);

    MPI_Reduce(&local_int,
               &total_int,
               1,
               MPI_DOUBLE,
               MPI_SUM,
               0,
               MPI_COMM_WORLD);

    if(my_rank == 0)
    {
        printf("Approximate Integral = %.10f\n", total_int);
    }

    MPI_Finalize();

    return 0;
}