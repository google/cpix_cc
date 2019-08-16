#
# Copyright 2019 Google LLC
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#  https://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#

package(
    default_visibility = [
        "//visibility:private",
    ],
)

licenses(["notice"])  # Apache 2.0

exports_files(["LICENSE"])

HIDDEN_COPTS = ["-fvisibility=hidden"]

PUBLIC_COPTS = ["-fvisibility=default"]

cc_binary(
    name = "libcpix_sdk.so",
    copts = HIDDEN_COPTS,
    linkshared = 1,
    deps = [
        ":content_key",
        ":content_key_list",
        ":cpix_message",
        ":drm_system",
        ":drm_system_list",
        ":key_period",
        ":key_period_list",
        ":recipient",
        ":recipient_list",
        ":usage_rule",
        ":usage_rule_list",
    ],
)

cc_library(
    name = "cpix_element",
    srcs = ["cpix_element.cc"],
    hdrs = ["cpix_element.h"],
    copts = PUBLIC_COPTS,
    deps = [":xml_node"],
)

cc_library(
    name = "cpix_element_list",
    srcs = ["cpix_element_list.cc"],
    hdrs = ["cpix_element_list.h"],
    copts = PUBLIC_COPTS,
    deps = [
        ":cpix_element",
        ":xml_node",
        "@com_google_absl//absl/memory",
    ],
)

cc_test(
    name = "cpix_element_list_test",
    size = "small",
    srcs = ["cpix_element_list_test.cc"],
    deps = [
        ":cpix_element",
        ":cpix_element_list",
        ":testable_cpix_element",
        ":testable_cpix_element_list",
        ":xml_node",
        "@com_google_absl//absl/memory",
        "@googletest_repo//:gtest_main",
    ],
)

cc_library(
    name = "recipient",
    srcs = ["recipient.cc"],
    hdrs = ["recipient.h"],
    copts = PUBLIC_COPTS,
    deps = [
        ":cpix_element",
        ":cpix_util",
        ":rsa_private_key",
        ":rsa_public_key",
        ":x509_certificate",
        ":xml_node",
        "@boringssl_repo//:crypto",
        "@com_google_absl//absl/memory",
    ],
)

cc_test(
    name = "recipient_test",
    size = "small",
    srcs = ["recipient_test.cc"],
    deps = [
        ":cpix_util",
        ":recipient",
        ":testable_cpix_element",
        ":xml_node",
        "@com_google_absl//absl/memory",
        "@googletest_repo//:gtest_main",
    ],
)

cc_library(
    name = "recipient_list",
    srcs = ["recipient_list.cc"],
    hdrs = ["recipient_list.h"],
    copts = PUBLIC_COPTS,
    deps = [
        "recipient",
        ":cpix_element",
        ":cpix_element_list",
        ":cpix_util",
        "@com_google_absl//absl/memory",
    ],
)

cc_library(
    name = "xml_node",
    srcs = ["xml_node.cc"],
    hdrs = [
        "xml_node.h",
    ],
    deps = [
        ":unique_xml_ptr",
        "@com_google_absl//absl/memory",
        "@libxml",
    ],
)

cc_test(
    name = "xml_node_test",
    size = "small",
    srcs = ["xml_node_test.cc"],
    deps = [
        ":xml_node",
        "@com_google_absl//absl/memory",
        "@googletest_repo//:gtest_main",
    ],
)

cc_library(
    name = "xml_util",
    srcs = ["xml_util.cc"],
    hdrs = [
        "xml_util.h",
    ],
    data = [
        "schemas/cpix.xsd",
        "schemas/pskc.xsd",
        "schemas/xenc-schema.xsd",
        "schemas/xmldsig-core-schema.xsd",
    ],
    deps = [
        ":unique_xml_ptr",
        "@libxml",
    ],
)

cc_test(
    name = "xml_util_test",
    size = "small",
    srcs = [":xml_util_test.cc"],
    deps = [
        ":xml_util",
        "@googletest_repo//:gtest_main",
    ],
)

cc_library(
    name = "cpix_util",
    srcs = ["cpix_util.cc"],
    hdrs = ["cpix_util.h"],
    deps = [
        "@boringssl_repo//:crypto",
        "@com_google_absl//absl/strings",
    ],
)

cc_test(
    name = "cpix_util_test",
    size = "small",
    srcs = [":cpix_util_test.cc"],
    deps = [
        ":cpix_util",
        "@googletest_repo//:gtest_main",
    ],
)

cc_library(
    name = "aes_cryptor",
    srcs = ["aes_cryptor.cc"],
    hdrs = ["aes_cryptor.h"],
    deps = [
        ":cpix_util",
        ":unique_ssl_ptr",
        "@boringssl_repo//:crypto",
        "@com_google_absl//absl/memory",
        "@com_google_glog//:glog",
    ],
)

cc_test(
    name = "aes_cryptor_test",
    size = "small",
    srcs = ["aes_cryptor_test.cc"],
    deps = [
        ":aes_cryptor",
        ":cpix_util",
        "@googletest_repo//:gtest_main",
    ],
)

cc_library(
    name = "unique_ssl_ptr",
    hdrs = ["unique_ssl_ptr.h"],
    deps = ["@boringssl_repo//:crypto"],
)

cc_library(
    name = "unique_xml_ptr",
    hdrs = ["unique_xml_ptr.h"],
    deps = [
        "@libxml",
    ],
)

cc_library(
    name = "rsa_public_key",
    srcs = ["rsa_public_key.cc"],
    hdrs = ["rsa_public_key.h"],
    deps = [
        ":cpix_util",
        ":unique_ssl_ptr",
        "@boringssl_repo//:crypto",
        "@com_google_absl//absl/memory",
    ],
)

cc_test(
    name = "rsa_public_key_test",
    size = "small",
    srcs = ["rsa_public_key_test.cc"],
    deps = [
        ":cpix_util",
        ":rsa_private_key",
        ":rsa_public_key",
        "@googletest_repo//:gtest_main",
    ],
)

cc_library(
    name = "rsa_private_key",
    srcs = ["rsa_private_key.cc"],
    hdrs = ["rsa_private_key.h"],
    deps = [
        ":cpix_util",
        ":unique_ssl_ptr",
        "@boringssl_repo//:crypto",
        "@com_google_absl//absl/memory",
    ],
)

cc_test(
    name = "rsa_private_key_test",
    size = "small",
    srcs = ["rsa_private_key_test.cc"],
    deps = [
        ":cpix_util",
        ":rsa_private_key",
        ":rsa_public_key",
        "@googletest_repo//:gtest_main",
    ],
)

cc_library(
    name = "cpix_message",
    srcs = ["cpix_message.cc"],
    hdrs = ["cpix_message.h"],
    copts = PUBLIC_COPTS,
    deps = [
        ":aes_cryptor",
        ":content_key",
        ":content_key_list",
        ":cpix_element",
        ":cpix_util",
        ":drm_system",
        ":drm_system_list",
        ":key_period",
        ":key_period_list",
        ":recipient",
        ":recipient_list",
        ":rsa_private_key",
        ":rsa_public_key",
        ":usage_rule",
        ":usage_rule_list",
        ":xml_node",
        ":xml_util",
        "@com_google_absl//absl/memory",
        "@com_google_glog//:glog",
    ],
)

cc_test(
    name = "cpix_message_test",
    size = "small",
    srcs = ["cpix_message_test.cc"],
    deps = [
        ":content_key",
        ":cpix_element",
        ":cpix_message",
        ":cpix_util",
        ":recipient",
        ":xml_node",
        ":xml_util",
        "@com_google_absl//absl/memory",
        "@googletest_repo//:gtest_main",
    ],
)

cc_library(
    name = "content_key",
    srcs = ["content_key.cc"],
    hdrs = ["content_key.h"],
    copts = PUBLIC_COPTS,
    deps = [
        ":cpix_element",
        ":cpix_util",
        ":xml_node",
        "@com_google_absl//absl/memory",
    ],
)

cc_test(
    name = "content_key_test",
    size = "small",
    srcs = ["content_key_test.cc"],
    deps = [
        ":content_key",
        ":cpix_util",
        ":testable_cpix_element",
        ":xml_node",
        "@com_google_absl//absl/memory",
        "@googletest_repo//:gtest_main",
    ],
)

cc_library(
    name = "content_key_list",
    srcs = ["content_key_list.cc"],
    hdrs = ["content_key_list.h"],
    copts = PUBLIC_COPTS,
    deps = [
        ":aes_cryptor",
        ":content_key",
        ":cpix_element",
        ":cpix_element_list",
        ":cpix_util",
        ":xml_node",
        "@com_google_absl//absl/memory",
    ],
)

cc_test(
    name = "content_key_list_test",
    size = "small",
    srcs = ["content_key_list_test.cc"],
    deps = [
        ":content_key",
        ":content_key_list",
        ":cpix_util",
        ":testable_cpix_element",
        ":xml_node",
        "@com_google_absl//absl/memory",
        "@googletest_repo//:gtest_main",
    ],
)

cc_library(
    name = "usage_rule",
    srcs = ["usage_rule.cc"],
    hdrs = ["usage_rule.h"],
    copts = PUBLIC_COPTS,
    deps = [
        ":cpix_element",
        ":cpix_util",
        ":xml_node",
        "@com_google_absl//absl/memory",
        "@com_google_glog//:glog",
    ],
)

cc_test(
    name = "usage_rule_test",
    srcs = ["usage_rule_test.cc"],
    deps = [
        ":cpix_util",
        ":testable_cpix_element",
        ":usage_rule",
        ":xml_node",
        "@com_google_absl//absl/memory",
        "@googletest_repo//:gtest_main",
    ],
)

cc_library(
    name = "usage_rule_list",
    srcs = ["usage_rule_list.cc"],
    hdrs = ["usage_rule_list.h"],
    copts = PUBLIC_COPTS,
    deps = [
        ":cpix_element",
        ":cpix_element_list",
        ":usage_rule",
        ":xml_node",
        "@com_google_absl//absl/memory",
    ],
)

cc_test(
    name = "usage_rule_list_test",
    size = "small",
    srcs = ["usage_rule_list_test.cc"],
    deps = [
        ":testable_cpix_element",
        ":usage_rule",
        ":usage_rule_list",
        ":xml_node",
        "@com_google_absl//absl/memory",
        "@googletest_repo//:gtest_main",
    ],
)

cc_library(
    name = "drm_system",
    srcs = ["drm_system.cc"],
    hdrs = ["drm_system.h"],
    copts = PUBLIC_COPTS,
    deps = [
        ":cpix_element",
        ":cpix_util",
        ":xml_node",
        "@com_google_absl//absl/memory",
    ],
)

cc_test(
    name = "drm_system_test",
    size = "small",
    srcs = ["drm_system_test.cc"],
    deps = [
        ":cpix_util",
        ":drm_system",
        ":testable_cpix_element",
        ":xml_node",
        "@com_google_absl//absl/memory",
        "@googletest_repo//:gtest_main",
    ],
)

cc_library(
    name = "drm_system_list",
    srcs = ["drm_system_list.cc"],
    hdrs = ["drm_system_list.h"],
    copts = PUBLIC_COPTS,
    deps = [
        ":cpix_element",
        ":cpix_element_list",
        ":drm_system",
        ":xml_node",
        "@com_google_absl//absl/memory",
    ],
)

cc_test(
    name = "drm_system_list_test",
    size = "small",
    srcs = ["drm_system_list_test.cc"],
    deps = [
        ":cpix_util",
        ":drm_system",
        ":drm_system_list",
        ":testable_cpix_element",
        ":xml_node",
        "@com_google_absl//absl/memory",
        "@googletest_repo//:gtest_main",
    ],
)

cc_library(
    name = "key_period",
    srcs = ["key_period.cc"],
    hdrs = ["key_period.h"],
    copts = PUBLIC_COPTS,
    deps = [
        ":cpix_element",
        ":xml_node",
        "@com_google_absl//absl/memory",
        "@com_google_absl//absl/strings",
        "@com_google_glog//:glog",
    ],
)

cc_test(
    name = "key_period_test",
    size = "small",
    srcs = ["key_period_test.cc"],
    deps = [
        ":key_period",
        ":testable_cpix_element",
        ":xml_node",
        "@com_google_absl//absl/memory",
        "@googletest_repo//:gtest_main",
    ],
)

cc_library(
    name = "key_period_list",
    srcs = ["key_period_list.cc"],
    hdrs = ["key_period_list.h"],
    copts = PUBLIC_COPTS,
    deps = [
        ":cpix_element",
        ":cpix_element_list",
        ":key_period",
        "@com_google_absl//absl/memory",
    ],
)

cc_library(
    name = "x509_certificate",
    srcs = ["x509_certificate.cc"],
    hdrs = ["x509_certificate.h"],
    deps = [
        ":unique_ssl_ptr",
        "@boringssl_repo//:crypto",
        "@com_google_absl//absl/memory",
        "@com_google_glog//:glog",
    ],
)

cc_test(
    name = "x509_certificate_test",
    size = "small",
    srcs = ["x509_certificate_test.cc"],
    deps = [
        ":cpix_util",
        ":x509_certificate",
        "@googletest_repo//:gtest_main",
    ],
)

cc_library(
    name = "testable_cpix_element",
    testonly = 1,
    hdrs = ["testable_cpix_element.h"],
)

cc_library(
    name = "testable_cpix_element_list",
    testonly = 1,
    hdrs = ["testable_cpix_element_list.h"],
)
