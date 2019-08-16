workspace(name = "cpix_cc")

load("@bazel_tools//tools/build_defs/repo:git.bzl", "git_repository")
load("@bazel_tools//tools/build_defs/repo:http.bzl", "http_archive")

git_repository(
    name = "boringssl_repo",
    commit = "edb6488b63995693c2a6155e0b4e41c27d33b6c6",  # 2019-08-13
    remote = "https://github.com/google/boringssl.git",
)

git_repository(
    name = "com_google_absl",
    remote = "https://github.com/abseil/abseil-cpp.git",
    commit = "b49b8d16b67ec6912899684b732e6367f258cfdb", # 2019-08-07
    shallow_since = "1565203022 -0400",
)

git_repository(
    name = "com_google_glog",
    remote = "https://github.com/google/glog.git",
    tag = "v0.4.0", # 2019-03-21
)

http_archive(
    name = "com_github_gflags_gflags",
    sha256 = "6e16c8bc91b1310a44f3965e616383dbda48f83e8c1eaa2370a215057b00cabe",
    strip_prefix = "gflags-77592648e3f3be87d6c7123eb81cbad75f9aef5a",
    urls = [ # 2018-01-06
        "https://mirror.bazel.build/github.com/gflags/gflags/archive/77592648e3f3be87d6c7123eb81cbad75f9aef5a.tar.gz",
        "https://github.com/gflags/gflags/archive/77592648e3f3be87d6c7123eb81cbad75f9aef5a.tar.gz",
    ],
)

git_repository(
    name = "googletest_repo",
    commit = "b6cd405286ed8635ece71c72f118e659f4ade3fb",  # 2019-01-04
    remote = "https://github.com/google/googletest.git",
)

http_archive(
    name = "libxml",
    build_file = "@//:libxml.BUILD",
    sha256 = "f63c5e7d30362ed28b38bfa1ac6313f9a80230720b7fb6c80575eeab3ff5900c",
    strip_prefix = "libxml2-2.9.7",
    urls = [ # 2017-11-02
        "https://mirror.bazel.build/xmlsoft.org/sources/libxml2-2.9.7.tar.gz",
        "http://xmlsoft.org/sources/libxml2-2.9.7.tar.gz",
    ],
)
