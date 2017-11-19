The following are the results of the SRT program run with different numbers of threads.

time ./srt 1-test.ppm >1-test.ppm.tmp

real    0m43.768s user    0m43.761s sys 0m0.003s mv 1-test.ppm.tmp 1-test.ppm time ./srt 2-test.ppm >2-test.ppm.tmp

real    0m22.410s user    0m44.441s sys 0m0.005s mv 2-test.ppm.tmp 2-test.ppm time ./srt 4-test.ppm >4-test.ppm.tmp

real    0m11.229s user    0m44.604s sys 0m0.004s mv 4-test.ppm.tmp 4-test.ppm time ./srt 8-test.ppm >8-test.ppm.tmp

real    0m5.773s user    0m44.708s sys 0m0.004s mv 8-test.ppm.tmp 8-test.ppm

The real or execution time is clearly much faster with more threads, and is  fastest with 8 threads. Because the old
implementation of the SRT program was embarrassingly parallel, by supporting multithreading, I improved the execution
time significantly. The user time stays roughly the same, although it increases  slightly due to the cost of more
threads in our program.

Issues: I kept on running into segmentation faults because I was not incrementing the pixels properly. Additionally,
because scaled_colors was a local variable, and we needed to print out each of the pixels for each thread, I needed to
create a global variable that would store scaled_colors so I could print them in the main function. When compiling the
program, it gave me an error saying that &scene was uninitialized. Because each thread this variable had to be used
before the multithreading began, I needed to initialize it as a global variable. Additionally, because the function that
we created was void, we needed to return NULL.
