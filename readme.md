# Ring buffer playground

Simple playground for implementing ring-buffer in different manners. For learning about atomics.

Just build as cmake project
```
mkdir build
cd build
cmake ..
make
```

To run test for verifying logic run singlethread_tests
To verify if it's ok with MPMC testcases, please run multithread_tests