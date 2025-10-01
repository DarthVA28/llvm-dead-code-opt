#include "DCEPass.hpp"

namespace DCEPass_LLVM 
{
llvm::PreservedAnalyses DCEPass::run(llvm::Function &F, llvm::FunctionAnalysisManager &) {
    llvm::errs() << "DCEPass Running on " << F.getName() << "\n";
    bool preserved = true;

    while (true) {
        bool changed = false;
        // Print stuff for debugging 
        changed |= EliminateTriviallyDeadInstructions(F);
        changed |= SimplifyControlFlow(F);
        changed |= EliminateDeadAllocas(F);
        if (!changed) break;
        preserved = false;
    }

    return preserved ? llvm::PreservedAnalyses::all() : llvm::PreservedAnalyses::none();
}

extern "C"
__attribute__((visibility("default")))
llvm::PassPluginLibraryInfo llvmGetPassPluginInfo() {
  return {
    LLVM_PLUGIN_API_VERSION, "DCEPass", "0.1",
    [](llvm::PassBuilder &PB) {
      PB.registerPipelineStartEPCallback(
        [](llvm::ModulePassManager &MPM, llvm::OptimizationLevel) {
          llvm::errs() << "[DCE] plugin registered (EP start)\n";
        });


      PB.registerPipelineParsingCallback(
        [](llvm::StringRef Name, llvm::ModulePassManager &MPM,
           llvm::ArrayRef<llvm::PassBuilder::PipelineElement>) {
          if (Name == "dce-pass") {
            llvm::errs() << "[DCEPass] accepted pipeline element 'dce-pass' (module)\n";
            MPM.addPass(createModuleToFunctionPassAdaptor(DCEPass()));
            return true;
          }
          return false;
        });
    }
  };
}

}