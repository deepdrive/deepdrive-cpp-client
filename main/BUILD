load("@rules_cc//cc:defs.bzl", "cc_binary")

cc_binary(
    name = "deepdrive_client_test",
    srcs = ["test.cpp"],
    deps = [":deepdrive_client"],
)

cc_library(
    name = "deepdrive_client",
    srcs = ["deepdrive_client.cpp"],
    hdrs = ["deepdrive_client.hpp"],
    linkopts = [
        "-lzmq",
        #        "-pthread",
    ],
)
