load("@rules_cc//cc:defs.bzl", "cc_library")
package(default_visibility = ["//visibility:public"])

cc_library(
    name = "order_data",
    srcs = ["order_data.cpp"],
    hdrs = ["order_data.h"],
)



cc_library(
    name = "order_database",
    srcs = ["order_database.cpp"],
    hdrs = ["order_database.h"],
    deps = [
        ":order_data",
        "//mysql:mysql",
    ],
)