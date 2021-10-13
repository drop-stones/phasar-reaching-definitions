
#include <algorithm>
#include <iostream>
#include <utility>

#include "ReachingDefinitions.hpp"

namespace psr {

ReachingDefinitions::ReachingDefinitions(const ProjectIRDB *IRDB,
                                         const LLVMTypeHierarchy *TH,
                                         const LLVMBasedCFG *CF,
                                         const LLVMPointsToInfo *PT,
                                         std::set<std::string> EntryPoints)
  : IntraMonoProblem<ReachingDefinitionsAnalysisDomain>(
      IRDB, TH, CF, PT, std::move(EntryPoints)) {}

ReachingDefinitions::mono_container_t
ReachingDefinitions::merge(
  const ReachingDefinitions::mono_container_t &lhs,
  const ReachingDefinitions::mono_container_t &rhs)
{
  return lhs.setUnion(rhs);
}

bool
ReachingDefinitions::equal_to(
  const ReachingDefinitions::mono_container_t &lhs,
  const ReachingDefinitions::mono_container_t &rhs)
{
  return lhs == rhs;
}

ReachingDefinitions::mono_container_t
ReachingDefinitions::normalFlow(
  ReachingDefinitions::n_t inst,
  const ReachingDefinitions::mono_container_t &in)
{
  ReachingDefinitions::mono_container_t out {in};
  if (const auto *store = llvm::dyn_cast<llvm::StoreInst>(inst)) {
    // kill
    for (const auto def : out) {
      if (store->getPointerOperand() == def->getPointerOperand()) {
        std::cout << "Kill\t" << llvmIRToString(def) << std::endl;
        out.erase(def);
      }
    }
    // gen
    std::cout << "Gen\t" << llvmIRToString(store) << std::endl;
    out.insert(store);
  }
  return out;
}

std::unordered_map<ReachingDefinitions::n_t, ReachingDefinitions::mono_container_t>
ReachingDefinitions::initialSeeds()
{
  return {};
}

void ReachingDefinitions::printNode(
  std::ostream &os, ReachingDefinitions::n_t inst) const {
  os << llvmIRToString(inst);
}

void ReachingDefinitions::printDataFlowFact(
  std::ostream &os, ReachingDefinitions::d_t fact) const {
  os << llvmIRToString(fact);
}

void ReachingDefinitions::printFunction(
  std::ostream &os, ReachingDefinitions::f_t func) const {
  os << func->getName().str();
}

} // namespace psr
