load("@rules_cc//cc:defs.bzl", "cc_binary", "cc_library")
package(default_visibility = ["//visibility:public"])

cc_binary(
    name = "main_client",
    srcs = ["main_client.cpp"],
    deps = [
        "//order_http:httplib",
        "//lib:common_fun",
        "//json:jsoncpp",
        
    ],
)