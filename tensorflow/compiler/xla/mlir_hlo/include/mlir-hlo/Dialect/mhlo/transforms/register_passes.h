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

#ifndef MLIR_HLO_DIALECT_MHLO_TRANSFORMS_REGISTER_PASSES_H_
#define MLIR_HLO_DIALECT_MHLO_TRANSFORMS_REGISTER_PASSES_H_

#include "mlir-hlo/Dialect/mhlo/transforms/passes.h"
#include "mlir/Pass/Pass.h"

namespace mlir {

namespace func {
class FuncOp;
}  // namespace func

namespace mhlo {

// TODO(csigg): this file is not following the passes.h patterns used elsewhere.

std::unique_ptr<OperationPass<func::FuncOp>>
createTestInferShapedTypeMethodsPass();
std::unique_ptr<Pass> createTestMaterializeBroadcastsPass();
std::unique_ptr<Pass> createTestUnfuseBatchNormPass();

#define GEN_PASS_REGISTRATION
#include "mlir-hlo/Dialect/mhlo/transforms/mhlo_passes.h.inc"

inline void registerAllMhloPasses() { registerMHLOPasses(); }

}  // namespace mhlo
}  // namespace mlir

#endif  // MLIR_HLO_DIALECT_MHLO_TRANSFORMS_REGISTER_PASSES_H_
