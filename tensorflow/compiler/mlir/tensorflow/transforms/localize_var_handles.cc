/* Copyright 2020 The TensorFlow Authors. All Rights Reserved.

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
==============================================================================*/

#include <memory>

#include "llvm/ADT/DenseSet.h"
#include "llvm/ADT/STLExtras.h"
#include "llvm/Support/Casting.h"
#include "mlir/Dialect/Func/IR/FuncOps.h"  // from @llvm-project
#include "mlir/IR/Attributes.h"  // from @llvm-project
#include "mlir/IR/Block.h"  // from @llvm-project
#include "mlir/IR/Builders.h"  // from @llvm-project
#include "mlir/IR/BuiltinOps.h"  // from @llvm-project
#include "mlir/IR/Operation.h"  // from @llvm-project
#include "mlir/IR/Value.h"  // from @llvm-project
#include "mlir/Pass/Pass.h"  // from @llvm-project
#include "mlir/Pass/PassRegistry.h"  // from @llvm-project
#include "tensorflow/compiler/mlir/tensorflow/analysis/resource_dataflow.h"
#include "tensorflow/compiler/mlir/tensorflow/ir/tf_ops.h"
#include "tensorflow/compiler/mlir/tensorflow/ir/tf_saved_model.h"
#include "tensorflow/compiler/mlir/tensorflow/transforms/passes.h"
#include "tensorflow/compiler/mlir/tensorflow/transforms/passes_detail.h"

namespace mlir {
namespace TF {

namespace {

// Analyzes the inputs to ClusterFuncOps in the module, and annotates their
// invoked functions whether each input has the same data across replicas.
struct LocalizeVarHandlesPass
    : public LocalizeVarHandlesPassBase<LocalizeVarHandlesPass> {
  void runOnOperation() override;
};

void MaybeCreateVarHandleForOp(Operation* op, DataFlowSolver& solver) {
  Value resource;
  if (auto read = llvm::dyn_cast<TF::ReadVariableOp>(op)) {
    resource = read.resource();
  } else if (auto write = llvm::dyn_cast<TF::AssignVariableOp>(op)) {
    resource = write.resource();
  }

  if (llvm::dyn_cast_or_null<TF::VarHandleOp>(resource.getDefiningOp())) {
    return;  // We're already directly after a VarHandleOp.
  }

  const TF::ResourceDataflowAnalysis::StateT* state =
      solver.lookupState<TF::ResourceDataflowAnalysis::StateT>(resource);
  if (!state) {
    // Can't actually happen. Even for gaps in the dataflow, we'll receive
    // the initial state.
    return;
  }
  auto ops = state->getValue().ops;
  if (ops.size() != 1) {
    return;
  }
  Operation* source = *ops.begin();
  llvm::StringRef container;
  llvm::StringRef shared_name;
  if (auto global = llvm::dyn_cast<tf_saved_model::GlobalTensorOp>(source)) {
    container = "";
    shared_name = global.getSymName();
  } else if (auto handle = llvm::dyn_cast<TF::VarHandleOp>(source)) {
    container = handle.container();
    shared_name = handle.shared_name();
  } else {
    // Can't happen, as long as this file and resource_dataflow.cc are in sync.
    return;
  }
  OpBuilder builder(op);
  auto vh = builder.create<TF::VarHandleOp>(op->getLoc(), resource.getType(),
                                            container, shared_name);
  op->setOperand(0, vh.resource());
}

void LocalizeVarHandlesPass::runOnOperation() {
  auto module = getOperation();

  DataFlowSolver solver;
  solver.load<dataflow::DeadCodeAnalysis>();
  solver.load<TF::ResourceDataflowAnalysis>();
  if (failed(solver.initializeAndRun(module))) return signalPassFailure();

  OpBuilder globalBuilder(module.getBodyRegion());

  module.walk(
      [&](TF::ReadVariableOp op) { MaybeCreateVarHandleForOp(op, solver); });
  module.walk(
      [&](TF::AssignVariableOp op) { MaybeCreateVarHandleForOp(op, solver); });
}

}  // namespace

std::unique_ptr<OperationPass<ModuleOp>> CreateLocalizeVarHandlesPass() {
  return std::make_unique<LocalizeVarHandlesPass>();
}

}  // namespace TF
}  // namespace mlir
