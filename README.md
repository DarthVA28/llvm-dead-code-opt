# LLVM Dead Code Elimination Pass
This repository contains an implementation of a dead code elimination pass `DCEPass` for LLVM IR. 

Specifically, this pass targets the following kinds of optimizations: 
1. Removes trivially dead instructions - instructions which are unused and do not have side-effects
2. Simplifies control flow by doing one of the following:
   - Removes "forwarder" basic blocks which contain only unconditional jumps to other blocks 
   - Removes conditional branches in basic blocks which jump to the same block in both cases
3. Removes dead stack `allocas`. We call an alloca dead if it is never read from and does not escape a function body.

## Instructions for Testing
To build and run this pass, ensure that `llvm-18` and `llvm-18-dev` are installed on your system. `cmake >= 3.15` is also required. Then:

1. In the top level folder, run:
   ```
   make
   ```

3. Then run the following command to run the tests:
   ```bash
    opt -load-pass-plugin ./build/libDCEPass.so -passes="dce-pass" -S tests/<path-to-test>.ll -o <path-to-output>.ll
    ```
