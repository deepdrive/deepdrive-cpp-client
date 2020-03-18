load("@rules_cc//cc:defs.bzl", "cc_binary")

cc_binary(
    name = "forward_agent",
    srcs = ["forward_agent.cpp"],
    visibility = ["//visibility:public"],
    deps = [":deepdrive_client"],
)

cc_library(
    name = "deepdrive_client",
    srcs = ["deepdrive_client.cpp"],
    hdrs = ["deepdrive_client.hpp",
            "vendor/include/zmq.hpp",
            "vendor/include/zmq_vendored.h"],
    #    strip_include_prefix = '.',
    linkopts = [
        "-lzmq",
        "-pthread",
    ],
    visibility = ["//visibility:public"],
    deps = ["@rapidjson"],
)
