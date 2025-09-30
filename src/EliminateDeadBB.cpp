#include "DCEPass.hpp"

namespace DCEPass_LLVM 
{

/// @brief Eliminate instructions which are trivially dead from a function body
/// @param F Input function
/// @return `true` if any instruction of the function is removed 
bool DCEPass::EliminateTriviallyDeadInstructions(llvm::Function &F) {
    llvm::SmallVector<llvm::Instruction*, 16> deadInstructions;
    for (auto &BB : F) 
        for (auto &I : BB) 
            if (isInstructionTriviallyDead(&I)) deadInstructions.push_back(&I);
        
    bool changed = !deadInstructions.empty();
    while (!deadInstructions.empty()) {
        auto *I = deadInstructions.pop_back_val();
        llvm::RecursivelyDeleteTriviallyDeadInstructions(I);
    }
    return changed;
}

/// @brief Eliminate basic blocks which only contain unconditional jumps
/// @param F 
/// @return `true` if any block is removed
bool DCEPass::EliminateRedundantBasicBlocks(llvm::Function &F) {
    bool Changed = false;

    for (auto It = F.begin(), E = F.end(); It != E; ) {
        llvm::BasicBlock *BB = &*It++;
        if (BB == &F.getEntryBlock()) continue; 
        if (BB->hasAddressTaken()) continue;

        auto *Br = llvm::dyn_cast<llvm::BranchInst>(BB->getTerminator());
        if (!Br || !Br->isUnconditional())
            continue;

        bool Safe = true;
        for (auto &I : *BB) {
            if (&I == Br) break;
            if (!llvm::isa<llvm::PHINode>(&I) &&
                !llvm::isa<llvm::DbgInfoIntrinsic>(&I)) {
                Safe = false;
                break;
            }
        }
        if (!Safe) continue;

        if (llvm::TryToSimplifyUncondBranchFromEmptyBlock(BB)) {
            Changed = true;
        }
    }

    return Changed;
}

/// @brief Eliminate trivially dead blocks and unnecessary conditional branches
/// @param F Input function 
/// @return `true` if any block or branch is removed or simplified
bool DCEPass::EliminateRedundantBranches(llvm::Function &F) {
    bool changed = false;
    for (auto &BB : F) {
        auto *br = llvm::dyn_cast<llvm::BranchInst>(BB.getTerminator());
        if (!br) continue;
        if (!br->isConditional()) continue;
        auto *T0 = br->getSuccessor(0);
        auto *T1 = br->getSuccessor(1);
        if (T0 == T1) {
            auto *Target = T0;
            llvm::IRBuilder<> Builder(br);
            Builder.CreateBr(Target);
            br->eraseFromParent();
            changed = true;
        }
    }
    return changed;
}

/// @brief Simplifies control flow inside a function by changing conditional branches to 
///         unconditional branches wherever possible, removing redundant blocks and unreachable blocks
/// @param F 
/// @return `true` if any change is made to the function
bool DCEPass::SimplifyControlFlow(llvm::Function &F) {
    bool changed = false;
    llvm::errs() << "Simplifying control flow...\n";
    changed |= EliminateRedundantBranches(F);
    llvm::errs() << "After eliminating redundant branches:\n";
    changed |= EliminateRedundantBasicBlocks(F);
    llvm::errs() << "After eliminating redundant basic blocks:\n";
    changed |= llvm::removeUnreachableBlocks(F);
    llvm::errs() << "After removing unreachable blocks:\n";
    return changed;
}

}