time_serial 	= load('./serial/time.out');

num_cores	= [2 4 6 8 16];

% OPENMP
time_omp_2 	= load('./openmp/time_cores_2.dat');
time_omp_4 	= load('./openmp/time_cores_4.dat');
time_omp_6 	= load('./openmp/time_cores_6.dat');
time_omp_8 	= load('./openmp/time_cores_8.dat');
time_omp_16 	= load('./openmp/time_cores_16.dat');
speedup_openmp = [time_omp_2 , time_omp_4, time_omp_6, time_omp_8, time_omp_16];
speedup_openmp = time_serial ./ speedup_openmp;
eficiency_openmp = speedup_openmp ./ num_cores


hold on;
figure 1;
plot(num_cores, speedup_openmp, '-or');
title("Speedup");
xlabel("Number of threads");
ylabel("Speedup");
legend("Openmp");


figure;
plot(num_cores, eficiency_openmp, '-ok');
title("Efficiency");
xlabel("Number of threads");
ylabel("Efficiency");
legend("Openmp");

