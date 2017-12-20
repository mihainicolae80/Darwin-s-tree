num_cores	= [2 4 6 8 10 16]';

% read times
time_omp 	= load('./time_omp');
time_pthreads	= load('./time_pthreads');
time_serial	= load('./time_serial');

speedup_openmp = time_serial ./ time_omp;
efficiency_openmp = speedup_openmp ./ num_cores

speedup_pthreads = time_serial ./ time_pthreads;
efficiency_pthreads = speedup_pthreads ./ num_cores



% OPENMP
figure;
hold on;
plot(num_cores, speedup_openmp, '-or');
plot(num_cores, speedup_pthreads, '-ob');
title("Speedup");
xlabel("Number of threads");
ylabel("Speedup");
legend("Openmp", "Pthreads");


% EFFICIENCY
figure;
hold on;
plot(num_cores, efficiency_openmp, '-or');
plot(num_cores, efficiency_pthreads, '-ob');
title("Efficiency");
xlabel("Number of threads");
ylabel("Efficiency");
legend("Openmp", "Pthreads");

