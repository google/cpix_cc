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

#include "usage_rule_list.h"

#include <memory>
#include <utility>

#include "absl/memory/memory.h"
#include "gmock/gmock.h"
#include "gtest/gtest.h"
#include "testable_cpix_element.h"
#include "usage_rule.h"
#include "xml_node.h"

namespace cpix {
namespace {

using ::testing::ByMove;
using ::testing::Return;
using ::testing::Test;

class MockUsageRule : public UsageRule {
 public:
  MOCK_METHOD(std::unique_ptr<XMLNode>, GetNode, (), (override));
};

constexpr char kGoodXML[] =
    "<ContentKeyUsageRuleList><MockUsageRule1/><MockUsageRule2/></"
    "ContentKeyUsageRuleList>";

TEST(UsageRuleListTest, SerializeUsageRuleList) {
  TestableCPIXElement<UsageRuleList> usage_rule_list;
  std::unique_ptr<MockUsageRule> rule1 = absl::make_unique<MockUsageRule>();
  std::unique_ptr<MockUsageRule> rule2 = absl::make_unique<MockUsageRule>();

  EXPECT_CALL(*rule1, GetNode())
      .WillOnce(
          Return(ByMove(absl::make_unique<XMLNode>("", "MockUsageRule1"))));
  EXPECT_CALL(*rule2, GetNode())
      .WillOnce(
          Return(ByMove(absl::make_unique<XMLNode>("", "MockUsageRule2"))));

  usage_rule_list.AddUsageRule(std::move(rule1));
  usage_rule_list.AddUsageRule(std::move(rule2));

  EXPECT_EQ(usage_rule_list.Serialize(), kGoodXML);
}

}  // namespace
}  // namespace cpix
