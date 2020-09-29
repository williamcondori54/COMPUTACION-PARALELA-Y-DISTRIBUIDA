#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mpi.h>

//region a analizar
const double a = 0;
const double b = 10;

using namespace std;

void Get_input(int argc, char* argv[], int my_rank, double* n_p) {
  if (my_rank == 0) {
    if (argc != 2) {
      fprintf(stderr, "mpirun --mca btl_base_warn_component_unused 0 -np <N> %s <numero de trapezoides> \n", argv[0]);
      fflush(stderr);
      *n_p = -1;
    }
    else {
      *n_p = atoi(argv[1]);
    }
  }
  // Transmite el valor de n a cada proceso
  MPI_Bcast(n_p, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);

  // n negativo termina el programa
  if (*n_p <= 0) {
    MPI_Finalize();
    exit(-1);
  }
}

double funcion(double x) { 
    double x1;
    double x2;
    x1 = ((x-4.0)*(x-4.0)*(x-4.0));
    x2 = 2.0*x;
    return ((0.2*x1)-x2)+12.0;;
}

double Trapecio(double left_endpt, double right_endpt, int trap_count, double base_len) {
  double val_estimado, x;
  int i;

  val_estimado = (funcion(left_endpt) + funcion(right_endpt)) / 2.0;
  for (i = 1; i <= trap_count - 1; i++) {
    x = left_endpt + i * base_len;
    val_estimado += funcion(x);
  }
  val_estimado = val_estimado * base_len;

  return val_estimado;
}


int main(int argc, char** argv) {
  int core_actual, cores_total, local_n;
  double n, h, local_a, local_b;
  double local_int, total_int;

  MPI_Init(NULL, NULL);
  MPI_Comm_rank(MPI_COMM_WORLD, &core_actual);
  MPI_Comm_size(MPI_COMM_WORLD, &cores_total);

  /*Imprime en consola los cores y la cantidad de cores*/
  cout<<"core numero "<<core_actual<<" de "<<cores_total<<endl;

  Get_input(argc, argv, core_actual, &n); /*Leer la entrada del usuario*/

  /* h y local_n son iguales para todos los procesos */
  h = (b - a) / n;          /* longitud de cada trapecio */
  local_n = n / cores_total;  /* cantidad de trapecios por proceso */

  /* longitud total para cada core = local_n * h. */
  local_a = a + core_actual * local_n * h;
  local_b = local_a + local_n * h;

  MPI_Barrier(MPI_COMM_WORLD);

  local_int = Trapecio(local_a, local_b, local_n, h);

  MPI_Reduce(&local_int, &total_int, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);

  if (core_actual == 0) {
    cout<<"con "<<n<<" trapezoides, el valor de la integral entre "<<a<<" y "<<b<< " = "<< total_int<<endl;
  }

 MPI_Finalize();

  return 0;
}
