load("//tensorflow/tsl/platform:rules_cc.bzl", "cc_library")
load("//tensorflow:tensorflow.bzl", "tf_cc_test")
load(
    "//tensorflow/core/platform:build_config.bzl",
    "tf_proto_library",
)

package(
    default_visibility = ["//tensorflow:internal"],
    licenses = ["notice"],
)

package_group(
    name = "friends",
    includes = [
        "//tensorflow:internal",
        "//tensorflow/compiler/xla:friends",
    ],
    packages = [
        "//third_party/australis/...",
        "//third_party/py/jax/...",
    ],
)

cc_library(
    name = "worker_thread",
    srcs = ["worker_thread.cc"],
    hdrs = ["worker_thread.h"],
    deps = [
        "//tensorflow/core:lib",
        "//tensorflow/tsl/platform:logging",
        "@com_google_absl//absl/synchronization",
    ],
)

cc_library(
    name = "event_pool",
    srcs = ["event_pool.cc"],
    hdrs = ["event_pool.h"],
    deps = [
        "//tensorflow/compiler/xla:status_macros",
        "//tensorflow/compiler/xla:statusor",
        "//tensorflow/compiler/xla:types",
        "//tensorflow/compiler/xla/stream_executor",
        "@com_google_absl//absl/memory",
        "@com_google_absl//absl/synchronization",
    ],
)

cc_library(
    name = "semaphore",
    srcs = ["semaphore.cc"],
    hdrs = ["semaphore.h"],
    deps = [
        "//tensorflow/compiler/xla:types",
        "//tensorflow/tsl/platform:logging",
        "@com_google_absl//absl/base:core_headers",
        "@com_google_absl//absl/synchronization",
    ],
)

tf_cc_test(
    name = "semaphore_test",
    srcs = ["semaphore_test.cc"],
    deps = [
        ":semaphore",
        "//tensorflow/compiler/xla:test",
        "//tensorflow/core:lib",
        "//tensorflow/tsl/platform:logging",
        "//tensorflow/tsl/platform:test_main",
        "@com_google_absl//absl/synchronization",
    ],
)

cc_library(
    name = "tracked_device_buffer",
    srcs = ["tracked_device_buffer.cc"],
    hdrs = ["tracked_device_buffer.h"],
    visibility = [
        "//learning/pathways/data_parallel:__pkg__",
        "//tensorflow:internal",
    ],
    deps = [
        ":event_pool",
        ":local_device_state",
        "//tensorflow/compiler/xla:shape_util",
        "//tensorflow/compiler/xla:types",
        "//tensorflow/compiler/xla/service:shaped_buffer",
        "//tensorflow/compiler/xla/service:transfer_manager",
        "//tensorflow/compiler/xla/stream_executor:device_memory",
        "//tensorflow/compiler/xla/stream_executor:device_memory_allocator",
        "//tensorflow/compiler/xla/stream_executor:event",
        "//tensorflow/core:lib",
        "//tensorflow/tsl/platform:logging",
        "@com_google_absl//absl/container:flat_hash_set",
        "@com_google_absl//absl/synchronization",
    ],
)

tf_cc_test(
    name = "tracked_device_buffer_test",
    srcs = ["tracked_device_buffer_test.cc"],
    deps = [
        ":tracked_device_buffer",
        "//tensorflow/compiler/xla:literal_util",
        "//tensorflow/compiler/xla:shape_util",
        "//tensorflow/compiler/xla:status_macros",
        "//tensorflow/compiler/xla:test",
        "//tensorflow/compiler/xla/client:client_library",
        "//tensorflow/compiler/xla/service:cpu_plugin",
        "//tensorflow/compiler/xla/stream_executor:device_memory",
        "//tensorflow/compiler/xla/stream_executor:device_memory_allocator",
        "//tensorflow/tsl/platform:test_main",
    ],
)

tf_cc_test(
    name = "cpu_device_test",
    srcs = ["cpu_device_test.cc"],
    deps = [
        ":cpu_device",
        ":pjrt_client",
        "//tensorflow/compiler/xla:status_macros",
        "//tensorflow/compiler/xla:test",
        "//tensorflow/compiler/xla/client:executable_build_options",
        "//tensorflow/compiler/xla/client:xla_builder",
        "//tensorflow/compiler/xla/tests:literal_test_util",
        "//tensorflow/tsl/platform:random",
        "//tensorflow/tsl/platform:test_main",
        "@com_google_absl//absl/types:span",
    ],
)

cc_library(
    name = "local_device_state",
    srcs = ["local_device_state.cc"],
    hdrs = ["local_device_state.h"],
    deps = [
        ":event_pool",
        ":semaphore",
        ":worker_thread",
        "//tensorflow/compiler/xla:status",
        "//tensorflow/compiler/xla:util",
        "//tensorflow/compiler/xla/client:local_client",
        "//tensorflow/compiler/xla/stream_executor",
        "//tensorflow/compiler/xla/stream_executor:event",
        "//tensorflow/core/profiler/lib:traceme",
        "//tensorflow/core/protobuf:error_codes_proto_impl_cc",
        "@com_google_absl//absl/memory",
        "@com_google_absl//absl/synchronization",
    ],
)

cc_library(
    name = "pjrt_client",
    srcs = ["pjrt_client.cc"],
    hdrs = ["pjrt_client.h"],
    visibility = ["//tensorflow/compiler/xla:friends"],
    deps = [
        ":pjrt_executable",
        ":pjrt_future",
        "//tensorflow/compiler/xla:literal",
        "//tensorflow/compiler/xla:shape_util",
        "//tensorflow/compiler/xla:status",
        "//tensorflow/compiler/xla:statusor",
        "//tensorflow/compiler/xla:util",
        "//tensorflow/compiler/xla:xla_data_proto_cc",
        "//tensorflow/compiler/xla/client:executable_build_options",
        "//tensorflow/compiler/xla/client:xla_computation",
        "//tensorflow/compiler/xla/service:hlo",
        "//tensorflow/compiler/xla/service:hlo_cost_analysis",
        "//tensorflow/core:lib",
        "//tensorflow/tsl/platform:logging",
        "@com_google_absl//absl/base",
        "@com_google_absl//absl/base:core_headers",
        "@com_google_absl//absl/container:flat_hash_map",
        "@com_google_absl//absl/container:inlined_vector",
        "@com_google_absl//absl/strings",
        "@com_google_absl//absl/synchronization",
        "@com_google_absl//absl/types:span",
        "@llvm-project//mlir:IR",
    ],
)

cc_library(
    name = "pjrt_client_test_common",
    testonly = 1,
    srcs = ["pjrt_client_test.cc"],
    hdrs = ["pjrt_client_test.h"],
    deps = [
        ":pjrt_client",
        "//tensorflow/compiler/xla:test",
        "//tensorflow/compiler/xla/client:xla_builder",
        "//tensorflow/compiler/xla/tests:literal_test_util",
        "@com_google_absl//absl/synchronization",
    ],
    alwayslink = 1,
)

tf_cc_test(
    name = "pjrt_client_test_cpu",
    srcs = ["pjrt_client_test_cpu.cc"],
    deps = [
        ":pjrt_client_test_common",
        ":tfrt_cpu_pjrt_client",
        "//tensorflow/tsl/platform:test_main",
    ],
)

cc_library(
    name = "pjrt_executable",
    srcs = ["pjrt_executable.cc"],
    hdrs = ["pjrt_executable.h"],
    visibility = [":friends"],
    deps = [
        "//tensorflow/compiler/xla:statusor",
        "//tensorflow/compiler/xla:util",
        "//tensorflow/compiler/xla/client:executable_build_options",
        "//tensorflow/compiler/xla/service:hlo",
        "//tensorflow/compiler/xla/service:hlo_proto_cc",
        "//tensorflow/tsl/platform:statusor",
        "@com_google_absl//absl/strings",
    ],
)

cc_library(
    name = "pjrt_compiler",
    srcs = ["pjrt_compiler.cc"],
    hdrs = ["pjrt_compiler.h"],
    visibility = [":friends"],
    deps = [
        ":pjrt_client",
        ":pjrt_executable",
        "@com_google_absl//absl/container:flat_hash_map",
        "@com_google_absl//absl/strings",
        "@com_google_absl//absl/synchronization",
        "@llvm-project//mlir:IR",
    ],
)

tf_cc_test(
    name = "pjrt_compiler_test",
    srcs = ["pjrt_compiler_test.cc"],
    deps = [
        ":pjrt_client",
        ":pjrt_compiler",
        "//tensorflow/compiler/xla/client:xla_computation",
        "@com_google_googletest//:gtest_main",
    ],
)

cc_library(
    name = "utils",
    srcs = ["utils.cc"],
    hdrs = ["utils.h"],
    visibility = ["//tensorflow/compiler/xla:friends"],
    deps = [
        "//tensorflow/compiler/xla:shape_util",
        "//tensorflow/compiler/xla:statusor",
        "//tensorflow/compiler/xla:xla_data_proto_cc",
        "//tensorflow/compiler/xla/client:executable_build_options",
        "//tensorflow/compiler/xla/client:xla_computation",
        "//tensorflow/compiler/xla/service:computation_placer",
        "//tensorflow/compiler/xla/service:hlo",
        "//tensorflow/compiler/xla/service:hlo_proto_cc",
        "@com_google_absl//absl/container:flat_hash_set",
    ],
)

cc_library(
    name = "metrics",
    srcs = ["metrics.cc"],
    hdrs = ["metrics.h"],
    deps = [
        "//tensorflow/tsl/lib/monitoring:counter",
        "@com_google_absl//absl/strings",
    ],
)

cc_library(
    name = "pjrt_stream_executor_client",
    srcs = ["pjrt_stream_executor_client.cc"],
    hdrs = ["pjrt_stream_executor_client.h"],
    visibility = ["//tensorflow/compiler/xla:friends"],
    deps = [
        ":event_pool",
        ":local_device_state",
        ":metrics",
        ":mlir_to_hlo",
        ":pjrt_client",
        ":pjrt_future",
        ":tracked_device_buffer",
        ":transpose",
        ":utils",
        "//tensorflow/compiler/xla:cpu_function_runtime",
        "//tensorflow/compiler/xla:executable_run_options",
        "//tensorflow/compiler/xla:literal",
        "//tensorflow/compiler/xla:literal_util",
        "//tensorflow/compiler/xla:shape_util",
        "//tensorflow/compiler/xla:status",
        "//tensorflow/compiler/xla:statusor",
        "//tensorflow/compiler/xla:util",
        "//tensorflow/compiler/xla:xla_data_proto_cc",
        "//tensorflow/compiler/xla/client:executable_build_options",
        "//tensorflow/compiler/xla/client:local_client",
        "//tensorflow/compiler/xla/client:xla_computation",
        "//tensorflow/compiler/xla/pjrt/distributed:protocol_proto_cc",
        "//tensorflow/compiler/xla/service:computation_layout",
        "//tensorflow/compiler/xla/service:computation_placer",
        "//tensorflow/compiler/xla/service:executable",
        "//tensorflow/compiler/xla/service:hlo",
        "//tensorflow/compiler/xla/service:hlo_cost_analysis",
        "//tensorflow/compiler/xla/service:maybe_owning_device_memory",
        "//tensorflow/compiler/xla/service:shaped_buffer",
        "//tensorflow/compiler/xla/service:transfer_manager",
        "//tensorflow/compiler/xla/service/gpu:gpu_executable_run_options",
        "//tensorflow/compiler/xla/stream_executor",
        "//tensorflow/compiler/xla/stream_executor:event",
        "//tensorflow/compiler/xla/stream_executor/host:host_platform_id",
        "//tensorflow/compiler/xla/stream_executor/lib",
        "//tensorflow/core:lib",
        "//tensorflow/core/profiler/lib:connected_traceme",
        "//tensorflow/core/profiler/lib:traceme",
        "//tensorflow/core/profiler/lib:traceme_encode",
        "//tensorflow/tsl/framework:allocator",
        "//tensorflow/tsl/platform:status",
        "@com_google_absl//absl/algorithm:container",
        "@com_google_absl//absl/base",
        "@com_google_absl//absl/container:flat_hash_map",
        "@com_google_absl//absl/container:flat_hash_set",
        "@com_google_absl//absl/container:inlined_vector",
        "@com_google_absl//absl/strings",
        "@com_google_absl//absl/strings:str_format",
        "@com_google_absl//absl/synchronization",
        "@com_google_absl//absl/time",
        "@com_google_absl//absl/types:span",
    ],
)

tf_cc_test(
    name = "pjrt_stream_executor_client_test",
    srcs = ["pjrt_stream_executor_client_test.cc"],
    deps = [
        ":pjrt_client",
        ":pjrt_stream_executor_client",
        "//tensorflow/compiler/xla:shape_util",
        "//tensorflow/compiler/xla:test",
        "//tensorflow/compiler/xla:xla_data_proto_cc",
        "//tensorflow/compiler/xla/client:client_library",
        "//tensorflow/compiler/xla/client:xla_builder",
        "//tensorflow/compiler/xla/service:cpu_plugin",
        "//tensorflow/compiler/xla/service:platform_util",
        "@com_google_absl//absl/functional:any_invocable",
        "@com_google_googletest//:gtest_main",
    ],
)

cc_library(
    name = "tpu_client",
    srcs = ["tpu_client.cc"],
    hdrs = ["tpu_client.h"],
    visibility = ["//tensorflow/compiler/xla:friends"],
    deps = [
        ":local_device_state",
        ":pjrt_stream_executor_client",
        ":tracked_device_buffer",
        ":utils",
        "//tensorflow/compiler/xla:shape_util",
        "//tensorflow/compiler/xla:status",
        "//tensorflow/compiler/xla:statusor",
        "//tensorflow/compiler/xla:util",
        "//tensorflow/compiler/xla/client:client_library",
        "//tensorflow/compiler/xla/service:computation_placer",
        "//tensorflow/compiler/xla/service:shaped_buffer",
        "//tensorflow/compiler/xla/service:tpu_computation_placer",
        "//tensorflow/compiler/xla/stream_executor",
        "//tensorflow/compiler/xla/stream_executor:device_memory",
        "//tensorflow/compiler/xla/stream_executor/lib",
        "//tensorflow/compiler/xla/stream_executor/tpu:tpu_executable",
        "//tensorflow/compiler/xla/stream_executor/tpu:tpu_executable_interface",
        "//tensorflow/compiler/xla/stream_executor/tpu:tpu_executor",
        "//tensorflow/compiler/xla/stream_executor/tpu:tpu_executor_interface",
        "//tensorflow/compiler/xla/stream_executor/tpu:tpu_platform_interface",
        "//tensorflow/compiler/xla/stream_executor/tpu:tpu_topology_external",
        "//tensorflow/compiler/xla/stream_executor/tpu:tpu_transfer_manager",
        "//tensorflow/core:lib",
        "//tensorflow/core/tpu:tpu_initializer_helper",
        "//tensorflow/core/tpu:tpu_on_demand_compiler",
        "//tensorflow/tsl/platform:logging",
        "@com_google_absl//absl/container:inlined_vector",
        "@com_google_absl//absl/memory",
        "@com_google_absl//absl/status",
    ],
)

cc_library(
    name = "interpreter_device",
    srcs = ["interpreter_device.cc"],
    hdrs = ["interpreter_device.h"],
    deps = [
        ":pjrt_stream_executor_client",
        "//tensorflow/compiler/xla:statusor",
        "//tensorflow/compiler/xla/client:client_library",
        "//tensorflow/compiler/xla/service:interpreter_plugin",
        "//tensorflow/compiler/xla/service:platform_util",
        "@com_google_absl//absl/strings",
    ],
)

cc_library(
    name = "cpu_device",
    srcs = ["cpu_device.cc"],
    hdrs = ["cpu_device.h"],
    compatible_with = [],
    visibility = [":friends"],
    deps = [
        ":pjrt_stream_executor_client",
        "//tensorflow/compiler/xla:statusor",
        "//tensorflow/compiler/xla/client:client_library",
        "//tensorflow/compiler/xla/service:cpu_plugin",
        "//tensorflow/compiler/xla/service:platform_util",
        "@com_google_absl//absl/strings",
    ],
)

cc_library(
    name = "pjrt_plugin_device_client",
    deps = [
        ":pjrt_plugin_device_client_headers",
        "//tensorflow/compiler/plugin",
    ],
)

cc_library(
    name = "pjrt_plugin_device_client_headers",
    hdrs = ["pjrt_plugin_device_client.h"],
    deps = [
        "//tensorflow/compiler/xla:statusor",
    ],
)

cc_library(
    name = "mlir_to_hlo",
    srcs = ["mlir_to_hlo.cc"],
    hdrs = ["mlir_to_hlo.h"],
    visibility = [":friends"],
    deps = [
        "//tensorflow/compiler/mlir/tensorflow:error_util",
        "//tensorflow/compiler/mlir/xla:mlir_hlo_to_hlo",
        "//tensorflow/compiler/xla:status",
        "//tensorflow/compiler/xla/client:xla_computation",
        "//tensorflow/compiler/xla/mlir_hlo",
        "//tensorflow/compiler/xla/mlir_hlo:chlo_legalize_to_hlo_pass",
        "//tensorflow/compiler/xla/mlir_hlo:sink_constants_to_control_flow",
        "//tensorflow/compiler/xla/mlir_hlo:sparse_chlo_legalize_to_linalg",
        "//tensorflow/compiler/xla/mlir_hlo:stablehlo_legalize_to_hlo_pass",
        "@com_google_absl//absl/strings",
        "@llvm-project//mlir:FuncDialect",
        "@llvm-project//mlir:IR",
        "@llvm-project//mlir:Parser",
        "@llvm-project//mlir:Pass",
        "@llvm-project//mlir:Transforms",
        "@stablehlo//:chlo_ops",
    ],
)

cc_library(
    name = "pjrt_future",
    hdrs = ["pjrt_future.h"],
    visibility = [":friends"],
    deps = [
        "@com_google_absl//absl/functional:any_invocable",
        "@com_google_absl//absl/types:span",
        "@tf_runtime//:hostcontext",
        "@tf_runtime//:support",
    ],
)

cc_library(
    name = "tracked_tfrt_cpu_device_buffer",
    srcs = ["tracked_tfrt_cpu_device_buffer.cc"],
    hdrs = ["tracked_tfrt_cpu_device_buffer.h"],
    deps = [
        "//tensorflow/compiler/xla:cpu_function_runtime",
        "//tensorflow/compiler/xla:shape_util",
        "//tensorflow/compiler/xla:util",
        "//tensorflow/tsl/platform:platform_port",
        "@com_google_absl//absl/base",
        "@com_google_absl//absl/container:inlined_vector",
        "@com_google_absl//absl/synchronization",
        "@com_google_absl//absl/types:span",
        "@tf_runtime//:hostcontext",
    ],
)

tf_cc_test(
    name = "tracked_tfrt_cpu_device_buffer_test",
    srcs = ["tracked_tfrt_cpu_device_buffer_test.cc"],
    deps = [
        ":tracked_tfrt_cpu_device_buffer",
        "@com_google_googletest//:gtest_main",
        "@tf_runtime//:hostcontext",
    ],
)

cc_library(
    name = "tfrt_cpu_pjrt_client",
    srcs = ["tfrt_cpu_pjrt_client.cc"],
    hdrs = ["tfrt_cpu_pjrt_client.h"],
    compatible_with = [],
    visibility = ["//tensorflow/compiler/xla:friends"],
    deps = [
        ":mlir_to_hlo",
        ":pjrt_client",
        ":pjrt_executable",
        ":pjrt_future",
        ":semaphore",
        ":tracked_tfrt_cpu_device_buffer",
        ":transpose",
        ":utils",
        ":worker_thread",
        "//tensorflow/compiler/xla:literal",
        "//tensorflow/compiler/xla:shape_util",
        "//tensorflow/compiler/xla:statusor",
        "//tensorflow/compiler/xla:util",
        "//tensorflow/compiler/xla:xla_data_proto_cc",
        "//tensorflow/compiler/xla/client:executable_build_options",
        "//tensorflow/compiler/xla/client:xla_computation",
        "//tensorflow/compiler/xla/service:buffer_assignment",
        "//tensorflow/compiler/xla/service:computation_placer_hdr",
        "//tensorflow/compiler/xla/service:dump",
        "//tensorflow/compiler/xla/service:executable",
        "//tensorflow/compiler/xla/service:hlo_cost_analysis",
        "//tensorflow/compiler/xla/service:hlo_module_util",
        "//tensorflow/compiler/xla/service:hlo_proto_cc",
        "//tensorflow/compiler/xla/service/cpu:cpu_compiler",
        "//tensorflow/compiler/xla/service/cpu:cpu_executable",
        "//tensorflow/compiler/xla/service/cpu:cpu_xfeed",
        "//tensorflow/core/profiler/lib:connected_traceme",
        "//tensorflow/core/profiler/lib:traceme",
        "//tensorflow/tsl/platform:denormal",
        "//tensorflow/tsl/platform:setround",
        "//third_party/eigen3",  # TODO(zhangqiaorjc): Remove if use TFRT threadpool.
        "@com_google_absl//absl/base:core_headers",
        "@com_google_absl//absl/container:flat_hash_map",
        "@com_google_absl//absl/strings",
        "@com_google_absl//absl/synchronization",
        "@com_google_absl//absl/types:span",
        "@tf_runtime//:hostcontext",
        "@tf_runtime//:support",
    ],
)

tf_cc_test(
    name = "tfrt_cpu_pjrt_client_test",
    srcs = ["tfrt_cpu_pjrt_client_test.cc"],
    deps = [
        ":tfrt_cpu_pjrt_client",
        "//tensorflow/compiler/xla/service:custom_call_status_public_headers",
        "//tensorflow/compiler/xla/service:custom_call_target_registry",
        "//tensorflow/compiler/xla/service:hlo_parser",
        "@com_google_googletest//:gtest_main",
    ],
)

cc_library(
    name = "lru_cache",
    hdrs = ["lru_cache.h"],
    deps = [
        "//tensorflow/tsl/platform:logging",
        "@com_google_absl//absl/container:node_hash_map",
    ],
)

tf_cc_test(
    name = "lru_cache_test",
    srcs = ["lru_cache_test.cc"],
    deps = [
        ":lru_cache",
        "//tensorflow/compiler/xla:test",
        "//tensorflow/core:lib",
        "//tensorflow/tsl/platform:logging",
        "//tensorflow/tsl/platform:test_main",
    ],
)

cc_library(
    name = "transpose",
    srcs = [
        "transpose.cc",
        "transpose_kernels.h",
    ],
    hdrs = ["transpose.h"],
    visibility = [":friends"],
    deps = [
        ":lru_cache",
        "//tensorflow/compiler/xla:permutation_util",
        "//tensorflow/compiler/xla:status",
        "//tensorflow/compiler/xla:statusor",
        "//tensorflow/compiler/xla:util",
        "//tensorflow/core/profiler/lib:traceme",
        "//tensorflow/tsl/platform:logging",
        "//third_party/eigen3",
        "@com_google_absl//absl/algorithm:container",
        "@com_google_absl//absl/container:inlined_vector",
        "@com_google_absl//absl/strings",
        "@com_google_absl//absl/strings:str_format",
        "@com_google_absl//absl/synchronization",
        "@com_google_absl//absl/types:span",
        "@com_google_absl//absl/types:variant",
    ],
)

tf_cc_test(
    name = "transpose_test",
    srcs = ["transpose_test.cc"],
    deps = [
        ":transpose",
        "//tensorflow/compiler/xla:array",
        "//tensorflow/compiler/xla:permutation_util",
        "//tensorflow/compiler/xla:shape_util",
        "//tensorflow/compiler/xla:test",
        "//tensorflow/compiler/xla:util",
        "//tensorflow/core/protobuf:error_codes_proto_impl_cc",
        "//tensorflow/tsl/platform:env",
        "//tensorflow/tsl/platform:test_benchmark",
        "//tensorflow/tsl/platform:test_main",
        "//third_party/eigen3",
        "@com_google_absl//absl/container:inlined_vector",
        "@com_google_absl//absl/numeric:int128",
    ],
)

cc_library(
    name = "pjrt_c_api_client",
    srcs = ["pjrt_c_api_client.cc"],
    hdrs = ["pjrt_c_api_client.h"],
    deps = [
        ":pjrt_client",
        ":pjrt_future",
        "//tensorflow/compiler/xla:shape_util",
        "//tensorflow/compiler/xla:util",
        "//tensorflow/compiler/xla:xla_data_proto_cc",
        "//tensorflow/compiler/xla/pjrt/c:pjrt_c_api_hdrs",
        "//tensorflow/compiler/xla/pjrt/c:pjrt_c_api_helpers",
        "//tensorflow/compiler/xla/pjrt/c:pjrt_c_api_wrapper_impl",  # TODO(skyewm): remove
        "//tensorflow/compiler/xla/stream_executor/tpu:c_api_conversions",  # TODO(b/238999986): Remove this.
        "//tensorflow/core/tpu:pjrt_api",
        "//tensorflow/core/tpu:tpu_initializer_helper",
        "//tensorflow/tsl/platform:status",
        "@llvm-project//mlir:BytecodeWriter",
    ],
)

cc_library(
    name = "host_callback",
    srcs = ["host_callback.cc"],
    hdrs = ["host_callback.h"],
    visibility = [":friends"],
    deps = [":pjrt_client"],
)

tf_cc_test(
    name = "host_callback_test",
    srcs = ["host_callback_test.cc"],
    deps = [
        ":host_callback",
        "//tensorflow/compiler/xla/tests:literal_test_util",
        "//tensorflow/tsl/lib/core:status_test_util",
        "//tensorflow/tsl/platform:test",
        "@com_google_googletest//:gtest_main",
    ],
)

tf_proto_library(
    name = "compile_options_proto",
    srcs = ["compile_options.proto"],
    cc_api_version = 2,
    protodeps = [
        "//tensorflow/compiler/xla:xla_data_proto",
        "//tensorflow/compiler/xla:xla_proto",
    ],
    visibility = ["//visibility:public"],
)
