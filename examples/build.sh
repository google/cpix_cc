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

#!/bin/bash

# Working directory must be cpix_cc
if [ ${PWD##*/} != "cpix_cc" ]
then
  echo "Working directory must be 'cpix_cc'"
  exit
fi

# Build the library file (will be located in bazel-bin
bazel build :all

# Test
bazel test :all

# Build the example binaries
g++ -Lbazel-bin -lcpix_sdk -I. -o out examples/clear_content_keys.cc
LD_LIBRARY_PATH=bazel-bin ./out

g++ -Lbazel-bin -lcpix_sdk -I. -o out examples/encrypted_content_keys.cc
LD_LIBRARY_PATH=bazel-bin ./out

g++ -Lbazel-bin -lcpix_sdk -I. -o out examples/content_keys_with_usage_rules_and_drm_systems.cc
LD_LIBRARY_PATH=bazel-bin ./out

g++ -Lbazel-bin -lcpix_sdk -I. -o out examples/content_keys_with_drm_systems.cc
LD_LIBRARY_PATH=bazel-bin ./out

g++ -Lbazel-bin -lcpix_sdk -I. -o out examples/content_keys_with_usage_rules_labels.cc
LD_LIBRARY_PATH=bazel-bin ./out

g++ -Lbazel-bin -lcpix_sdk -I. -o out examples/content_keys_with_usage_rules_policies.cc
LD_LIBRARY_PATH=bazel-bin ./out

g++ -Lbazel-bin -lcpix_sdk -I. -o out examples/content_keys_with_usage_rules_key_period.cc
LD_LIBRARY_PATH=bazel-bin ./out
