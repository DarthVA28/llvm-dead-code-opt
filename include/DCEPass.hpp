#pragma once 

#include "llvm/ADT/SmallPtrSet.h"
#include "llvm/IR/BasicBlock.h"
#include "llvm/IR/Constants.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/InstrTypes.h"
#include "llvm/IR/Instructions.h"
#include "llvm/IR/PassManager.h"
#include "llvm/IR/PatternMatch.h"
#include "llvm/IR/Type.h"
#include "llvm/Passes/PassBuilder.h"
#include "llvm/Transforms/Utils/BasicBlockUtils.h"
#include "llvm/Transforms/Utils/Local.h"
#include "llvm/Support/Debug.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/IR/PassManager.h"
#include "llvm/Passes/PassBuilder.h"
#include "llvm/Passes/PassPlugin.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/Transforms/Utils/Local.h"   


namespace DCEPass_LLVM
{
class DCEPass : public llvm::PassInfoMixin<DCEPass> {
private: 

    typedef struct
    {
        bool escapes; 
        bool read; 
        llvm::SmallVector<llvm::Instruction*, 32> storeInsts;
        llvm::SmallVector<llvm::Instruction*, 32> otherInsts;
    } AllocaInfo;

    static bool EliminateTriviallyDeadInstructions(llvm::Function &F);
    static bool EliminateRedundantBasicBlocks(llvm::Function &F);
    static bool EliminateRedundantBranches(llvm::Function &F);
    static bool SimplifyControlFlow(llvm::Function &F);
    static void CollectAllocaInfoTransitively(llvm::Instruction *I, AllocaInfo &info);
    static bool EliminateDeadAllocas(llvm::Function &F);

public:
    llvm::PreservedAnalyses run(llvm::Function &F, llvm::FunctionAnalysisManager &); 
};
}