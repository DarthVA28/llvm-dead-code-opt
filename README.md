# llvm-dead-code-opt
A dead code elimination pass for LLVM IR

Instructions to test: 

1. First run `make` in the top level directory. 

2. Then run the following command to test: 
```bash
opt -load-pass-plugin ./build/libDCEPass.so -passes="dce-pass" -S tests/test<n>.ll -o tests/test<n>_out.ll
```

Replacing `n=1` at the moment