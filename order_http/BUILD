load("@rules_cc//cc:defs.bzl", "cc_binary", "cc_library")
package(default_visibility = ["//visibility:public"])

cc_library(
    name = "order_client",
    srcs = ["order_client.cpp"],
    hdrs = ["order_client.h"],
    deps = [
        "//order_proto:order_cc_grpc",
        "//order_data:order_data",
        "@com_github_grpc_grpc//:grpc++",

    ],
)

cc_library(
    name="httplib",
    srcs=["httplib.cpp"],
    hdrs=["httplib.h"],
)

cc_library(
    name = "http_service",
    srcs = ["http_service.cpp"],
    hdrs = ["httplib.h","http_service.h"],
    deps = [
        ":httplib",
        ":order_client",
        "//lib:common_fun",
        "//order_proto:order_cc_grpc",
        "@com_github_grpc_grpc//:grpc++",
        "@com_github_grpc_grpc//:grpc++_reflection",
        "//json:jsoncpp",
    ],
)

cc_binary(
    name = "main_http",
    srcs = ["main_http.cpp"],
    deps = [
        ":http_service",
    ],
)