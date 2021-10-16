load("@rules_cc//cc:defs.bzl", "cc_binary", "cc_library")

cc_library(
    name = "order_server",
    srcs = ["order_server.cpp"],
    hdrs = ["order_server.h"],
    deps = [
        "//order_proto:order_cc_grpc",
        "//order_data:order_database",
        "@com_github_grpc_grpc//:grpc++",
        "@com_github_grpc_grpc//:grpc++_reflection",
    ],
)

cc_binary(
    name = "main_server",
    srcs = ["main_server.cpp"],
    deps = [
        ":order_server",
        "//order_proto:order_cc_grpc",
        "//lib:common_fun",
        "@com_github_grpc_grpc//:grpc++",
        "@com_github_grpc_grpc//:grpc++_reflection",
        
    ],
)