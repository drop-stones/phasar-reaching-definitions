
#include "ReachingDefinitions.hpp"

#include <boost/filesystem/operations.hpp>

#include "phasar/DB/ProjectIRDB.h"
#include "phasar/PhasarLLVM/ControlFlow/LLVMBasedICFG.h"
#include "phasar/PhasarLLVM/TypeHierarchy/LLVMTypeHierarchy.h"
#include "phasar/PhasarLLVM/Pointer/LLVMPointsToSet.h"
#include "phasar/PhasarLLVM/DataFlowSolver/Mono/Solver/IntraMonoSolver.h"
#include "phasar/Utils/LLVMShorthands.h"
#include "phasar/Utils/Logger.h"

using namespace psr;

int main(int argc, char **argv)
{
  if (argc < 2 || !boost::filesystem::exists(argv[1]) ||
      boost::filesystem::is_directory(argv[1])) {
    std::cerr << "ReachingDefinitions Analysis\n\n"
                 "Usage: " << argv[0] << " <LLVM IR file>" << std::endl;
    exit(1);
  }

  initializeLogger(false);
  ProjectIRDB DB({argv[1]});
  if (const auto *func = DB.getFunctionDefinition("main")) {
    LLVMTypeHierarchy TH{DB};
    LLVMPointsToSet PT{DB};
    LLVMBasedICFG CF(DB, CallGraphAnalysisType::OTF, {"main"}, &TH, &PT);
    ReachingDefinitions RD(&DB, &TH, &CF, &PT, {"main"});
    IntraMonoSolver<ReachingDefinitionsAnalysisDomain> MonoSolver(RD);

    MonoSolver.solve();

    //std::cout << "LLVMTypeHierarchy::print" << std::endl;
    //TH.print(std::cout);
    //std::cout << "\nLLVMPointsToSet::print" << std::endl;
    //PT.print(std::cout);
    MonoSolver.dumpResults();
  } else {
    std::cerr << "Error: file does not contain a 'main' function!" << std::endl;
  }

  return 0;
}
