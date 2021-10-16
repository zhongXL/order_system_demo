load("@rules_cc//cc:defs.bzl", "cc_proto_library")
load("@rules_proto//proto:defs.bzl", "proto_library")
load("@com_github_grpc_grpc//bazel:cc_grpc_library.bzl", "cc_grpc_library")
package(default_visibility = ["//visibility:public"])

proto_library(
    name = "order_proto",
    srcs = ["order.proto"],
)

cc_proto_library(
    name = "order_cc_proto",
    deps = [":order_proto"],
)

cc_grpc_library(
    name = "order_cc_grpc",
    srcs = [":order_proto"],
    grpc_only = True,
    deps = [":order_cc_proto"],
)