num_cores	= [2 4 6 8 10 16]';

% read times
time_omp 	= load('./time_omp');
time_mpi 	= load('./time_mpi');
time_pthreads	= load('./time_pthreads');
time_serial	= load('./time_serial');

speedup_openmp = time_serial ./ time_omp;
efficiency_openmp = speedup_openmp ./ num_cores;

speedup_pthreads = time_serial ./ time_pthreads;
efficiency_pthreads = speedup_pthreads ./ num_cores;

speedup_mpi = time_serial ./ time_mpi;
efficiency_mpi = speedup_mpi ./ num_cores;


% OPENMP
figure;
hold on;
plot(num_cores(1:5), speedup_openmp(1:5), '-or');
plot(num_cores(1:5), speedup_pthreads(1:5), '-ob');
plot(num_cores(1:5), speedup_mpi(1:5), '-ob');
title("Speedup");
xlabel("Number of threads");
ylabel("Speedup");
legend("Openmp", "Pthreads", "Mpi");


% EFFICIENCY
figure;
hold on;
plot(num_cores(1:5), efficiency_openmp(1:5), '-or');
plot(num_cores(1:5), efficiency_pthreads(1:5), '-ob');
plot(num_cores(1:5), efficiency_mpi(1:5), '-ob');
title("Efficiency");
xlabel("Number of threads");
ylabel("Efficiency");
legend("Openmp", "Pthreads", "Mpi");
