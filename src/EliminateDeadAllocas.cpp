#include "DCEPass.hpp"

namespace DCEPass_LLVM 
{

/// @brief Helper function to collect information about an alloca instruction transitively
/// @param instr Root instruction to analyze
/// @param info Alloca information to populate
void DCEPass::CollectAllocaInfoTransitively(llvm::Instruction *instr, AllocaInfo &info) {
    llvm::SmallVector<llvm::Value*, 32> workList;
    llvm::SmallPtrSet<llvm::Value*, 32> visited;

    workList.push_back(instr);

    while (!workList.empty()) {
        auto *V = workList.pop_back_val();
        for (llvm::User *U : V->users()) {
            llvm::Instruction *I = llvm::dyn_cast<llvm::Instruction>(U);
            if (!I) {
                info.escapes = true;
                continue;
            }

            // Case I: Loads
            if (auto *loadInst = llvm::dyn_cast<llvm::LoadInst>(I)) {
                if (loadInst->isVolatile()) {
                    info.escapes = true;
                    continue;
                }
                if (loadInst->getPointerOperand() == V) {
                    info.read = true;
                    continue;
                }
            }

            // Case II: Stores 
            if (auto *storeInst = llvm::dyn_cast<llvm::StoreInst>(I)) {
                if (storeInst->isVolatile() || storeInst->isAtomic()) {
                    info.escapes = true;
                    continue;
                }
                if (storeInst->getValueOperand() == V) {
                    info.escapes = true;
                    continue;
                }
                if (storeInst->getPointerOperand() == V) {
                    info.storeInsts.push_back(storeInst);
                    continue;
                } 
            }

            // Case III: BitCast, GetElementPtr
            // Explore transitively
            if (llvm::isa<llvm::GetElementPtrInst>(I) || 
                llvm::isa<llvm::BitCastInst>(I)) {
                if (visited.insert(I).second) {
                    workList.push_back(I);
                }
                info.otherInsts.push_back(I);
                continue;
            }

            // Case IV: PHI and Select 
            // Explore transitively
            if (llvm::isa<llvm::PHINode>(I) || 
                llvm::isa<llvm::SelectInst>(I)) {
                if (visited.insert(I).second) {
                    workList.push_back(I);
                }
                info.otherInsts.push_back(I);
                continue;
            }

            // Case V: Passing as argument to functions 
            // First check for intrinsics since we know what they do
            if (auto *intrinsicInst = llvm::dyn_cast<llvm::IntrinsicInst>(I)) {
                auto intrinsicID = intrinsicInst->getIntrinsicID();
                llvm::errs() << "Found intrinsic: " << intrinsicID << "\n";
                if (intrinsicID == llvm::Intrinsic::memcpy || intrinsicID == llvm::Intrinsic::memmove) {
                    if (intrinsicInst->getArgOperand(1) == V) {
                        info.read = true;
                        continue;
                    } else if (intrinsicInst->getArgOperand(0) == V) {
                        info.storeInsts.push_back(intrinsicInst);
                        continue;
                    }
                } else if (intrinsicID == llvm::Intrinsic::memset) {
                    if (intrinsicInst->getArgOperand(0) == V) {
                        info.storeInsts.push_back(intrinsicInst);
                        continue;
                    }
                } else {
                    // Be conservative
                    info.escapes = true;
                    continue;
                }
            }

            // All other functions
            if (auto *callInst = llvm::dyn_cast<llvm::CallBase>(I)) {
                for (unsigned int i = 0; i < callInst->arg_size(); i++) {
                    if (callInst->getArgOperand(i) == V) {
                        info.escapes = true;
                        break;
                    }
                }
                continue;
            }
            

            // For other instructions, assume escape 
            info.escapes = true;
        }
    }

}

/// @brief Eliminate allocas that do not escape and are not read from
/// @param F Input function 
/// @return `true` if the function was modified
bool DCEPass::EliminateDeadAllocas(llvm::Function &F) {
    bool changed = false; 
    
    // Create a hashmap with AllocaInfo for each alloca instruction 
    llvm::SmallDenseMap<llvm::AllocaInst*, AllocaInfo> allocaMap;

    for (auto &BB : F) {
        for (auto &I : BB) {
            if (auto *allocaInst = llvm::dyn_cast<llvm::AllocaInst>(&I)) {
                allocaMap[allocaInst] = AllocaInfo({false, false});
            }
        }
    }

    // For each alloca, we need to check if the users of the alloca are loads
    // or if it escapes the function
    for (auto &[allocaInst, info] : allocaMap) {
        CollectAllocaInfoTransitively(allocaInst, info);
        if (info.escapes || info.read) {
            continue;
        }
        // If we reach here, the alloca is dead
        // Delete the stores 
        for (auto *storeInst : info.storeInsts) {
            if (storeInst && storeInst->use_empty()) storeInst->eraseFromParent();
        }   

        // Delete other instructions - GEP, Phi, etc. 
        llvm::SmallVector<llvm::Instruction*, 32> worklist(info.otherInsts.begin(), info.otherInsts.end());
        while (!worklist.empty()) {
            auto *I = worklist.pop_back_val();
            if (!I) continue; 
            if (llvm::RecursivelyDeleteTriviallyDeadInstructions(I)) {
                changed = true; 
                continue;
            }
            if (I->use_empty()) {
                I->eraseFromParent();
                changed = true; 
                continue;
            }
        }

        // Finally, delete the alloca itself
        if (allocaInst->use_empty()) {
            allocaInst->eraseFromParent();
            changed = true; 
        }
    }

    return changed; 
}
}