/* Copyright 2022 The TensorFlow Authors. All Rights Reserved.

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

#include "tensorflow/compiler/xla/pjrt/host_callback.h"

#include <cstring>
#include <utility>

#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include "tensorflow/compiler/xla/tests/literal_test_util.h"
#include "tensorflow/tsl/lib/core/status_test_util.h"

namespace xla {
namespace {

class TestPjRtHostMemoryForDeviceManager
    : public PjRtHostMemoryForDeviceManager {
 public:
  ~TestPjRtHostMemoryForDeviceManager() override = default;

  StatusOr<PjRtChunk> ToDeviceLayout(const void* src_data, size_t src_size,
                                     const Shape& host_shape,
                                     const Shape& device_shape) override {
    auto chunk = PjRtChunk::AllocateDefault(src_size);
    std::memcpy(chunk.data(), src_data, src_size);
    return chunk;
  }

  Status ToHostLayout(const void* src_data, size_t src_size,
                      const Shape& src_shape, void* dst_data, size_t dst_size,
                      const Shape& dst_shape) override {
    CHECK_EQ(src_size, dst_size);
    std::memcpy(dst_data, src_data, src_size);
    return OkStatus();
  }
};

TEST(HostCallbackTest, Basic) {
  HostCallback host_callback;

  Shape shape = ShapeUtil::MakeShape(F32, {2, 2});
  size_t byte_size = ShapeUtil::ByteSizeOf(shape);

  host_callback.operands = {HostCallbackArgInfo{/*channel_id=*/1, shape}};
  host_callback.results = {HostCallbackArgInfo{/*channel_id=*/2, shape}};
  host_callback.callback = [byte_size](void** outputs, void** inputs) {
    std::memcpy(outputs[0], inputs[0], byte_size);
    return OkStatus();
  };

  HostCallbackStates states;

  auto& send_callbacks = states.send_callbacks.emplace_back();
  auto& recv_callbacks = states.recv_callbacks.emplace_back();

  TestPjRtHostMemoryForDeviceManager test_host_memory_for_device_manager;

  auto context = CreateHostCallbackStateAndAppendSendRecvCallbacks(
      std::move(host_callback), &test_host_memory_for_device_manager,
      send_callbacks, recv_callbacks);

  PjRtTransferMetadata metadata;
  metadata.device_shape = shape;

  auto literal = LiteralUtil::CreateR2({{1.0f, 2.0f}, {3.0f, 4.0f}});
  auto chunk = PjRtChunk::AllocateDefault(/*size=*/byte_size);
  ASSERT_EQ(chunk.size(), literal.size_bytes());
  std::memcpy(chunk.data(), literal.untyped_data(), literal.size_bytes());

  TF_ASSERT_OK(context->OnSend(/*arg_num=*/0, metadata, std::move(chunk)));

  CopyToDeviceStream stream(/*total_bytes=*/byte_size, /*granule_bytes=*/8);
  context->Receive(/*res_num=*/0, metadata, stream);

  auto received_chunk = stream.ConsumeNextChunk().value();
  ASSERT_FALSE(stream.ConsumeNextChunk());

  BorrowingLiteral borrowing_literal(
      reinterpret_cast<const char*>(received_chunk.data()), shape);

  EXPECT_TRUE(LiteralTestUtil::Equal(literal, borrowing_literal));
}

}  // namespace
}  // namespace xla
