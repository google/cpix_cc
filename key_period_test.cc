// Copyright 2019 Google LLC
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//  https://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include "key_period.h"

#include <memory>
#include <utility>

#include "absl/memory/memory.h"
#include "gtest/gtest.h"
#include "testable_cpix_element.h"
#include "xml_node.h"

namespace cpix {
namespace {

using ::testing::Test;

constexpr char kGoodXMLIndex[] = "<ContentKeyPeriod index=\"3\"/>";

constexpr char kGoodXMLInterval[] =
    "<ContentKeyPeriod start=\"1970-01-01T12:00:00\" "
    "end=\"1970-01-01T12:30:00\"/>";

TEST(KeyPeriodTest, SerializeKeyPeriodIndex) {
  TestableCPIXElement<KeyPeriod> key_period;

  key_period.SetIndex(3);

  EXPECT_EQ(key_period.Serialize(), kGoodXMLIndex);
}

TEST(KeyPeriodTest, SerializeKeyPeriodInterval) {
  TestableCPIXElement<KeyPeriod> key_period;

  key_period.SetInterval("1970-01-01T12:00:00", "1970-01-01T12:30:00");
  EXPECT_EQ(key_period.Serialize(), kGoodXMLInterval);
}

TEST(ContentKeyTest, DeserializeKeyPeriodIndex) {
  TestableCPIXElement<KeyPeriod> key_period;
  std::unique_ptr<XMLNode> node = absl::make_unique<XMLNode>(kGoodXMLIndex);
  key_period.Deserialize(std::move(node));
  EXPECT_EQ(key_period.Serialize(), kGoodXMLIndex);
}

TEST(ContentKeyTest, DeserializeKeyPeriodInterval) {
  TestableCPIXElement<KeyPeriod> key_period;
  std::unique_ptr<XMLNode> node = absl::make_unique<XMLNode>(kGoodXMLInterval);
  key_period.Deserialize(std::move(node));
  EXPECT_EQ(key_period.Serialize(), kGoodXMLInterval);
}

}  // namespace
}  // namespace cpix
