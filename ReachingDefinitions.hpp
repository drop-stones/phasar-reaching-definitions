#ifndef REACHING_DEFINITIONS_HPP_
#define REACHING_DEFINITIONS_HPP_

#include <set>
#include <string>
#include <unordered_map>
#include "llvm/IR/Instructions.h"

#include "phasar/PhasarLLVM/ControlFlow/LLVMBasedCFG.h"
#include "phasar/PhasarLLVM/ControlFlow/LLVMBasedICFG.h"
#include "phasar/PhasarLLVM/DataFlowSolver/Mono/IntraMonoProblem.h"
#include "phasar/PhasarLLVM/Domain/AnalysisDomain.h"
#include "phasar/PhasarLLVM/Pointer/LLVMPointsToInfo.h"
#include "phasar/PhasarLLVM/TypeHierarchy/LLVMTypeHierarchy.h"
#include "phasar/Utils/BitVectorSet.h"
#include "phasar/Utils/LLVMShorthands.h"

namespace psr {

struct ReachingDefinitionsDomain : public LLVMAnalysisDomainDefault {
  // Data-flow fact
  using d_t = const llvm::StoreInst *;
  using mono_container_t = BitVectorSet<d_t>;
};

class ReachingDefinitions : public IntraMonoProblem<ReachingDefinitionsDomain> {
public:
  using n_t = ReachingDefinitionsDomain::n_t;
  using d_t = ReachingDefinitionsDomain::d_t;
  using f_t = ReachingDefinitionsDomain::f_t;
  using t_t = ReachingDefinitionsDomain::t_t;
  using v_t = ReachingDefinitionsDomain::v_t;
  using i_t = ReachingDefinitionsDomain::i_t;
  using mono_container_t = ReachingDefinitionsDomain::mono_container_t;

  ReachingDefinitions(const ProjectIRDB *IRDB,
                      const LLVMTypeHierarchy *TH,
                      const LLVMBasedCFG *CFG,
                      const LLVMPointsToInfo *PT,
                      std::set<std::string> EntryPoints);

  ~ReachingDefinitions() override = default;

  mono_container_t
  merge(const mono_container_t &lhs,
        const mono_container_t &rhs) override;

  bool
  equal_to(const mono_container_t &lhs,
           const mono_container_t &rhs) override;

  mono_container_t
  normalFlow(n_t inst, const mono_container_t &in) override;

  std::unordered_map<n_t, mono_container_t>
  initialSeeds() override;

  void printNode(std::ostream &os, n_t inst) const override;

  void printDataFlowFact(std::ostream &os, d_t fact) const override;

  void printFunction(std::ostream &os, f_t func) const override;
};

} // namespace psr

#endif
