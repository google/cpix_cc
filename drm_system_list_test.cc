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

#include "drm_system_list.h"

#include <memory>
#include <utility>

#include "absl/memory/memory.h"
#include "cpix_util.h"
#include "drm_system.h"
#include "gmock/gmock.h"
#include "gtest/gtest.h"
#include "testable_cpix_element.h"
#include "xml_node.h"

namespace cpix {
namespace {

using ::testing::ByMove;
using ::testing::Return;
using ::testing::Test;

class MockDRMSystem : public DRMSystem {
 public:
  MOCK_METHOD(std::unique_ptr<XMLNode>, GetNode, (), (override));
};

constexpr char kGoodXML[] =
    "<DRMSystemList><MockDRMSystem1/><MockDRMSystem2/></"
    "DRMSystemList>";

constexpr char kGoodGUID[] = "bd5adf51cf04410faac3ec63a69e929e";

TEST(DRMSystemListTest, SerializeDRMSystemList) {
  TestableCPIXElement<DRMSystemList> drm_list;
  std::unique_ptr<MockDRMSystem> drm1 = absl::make_unique<MockDRMSystem>();
  std::unique_ptr<MockDRMSystem> drm2 = absl::make_unique<MockDRMSystem>();

  drm1->set_system_id(HexStringToBytes(kGoodGUID));
  drm1->set_key_id(HexStringToBytes(kGoodGUID));
  drm2->set_system_id(HexStringToBytes(kGoodGUID));
  drm2->set_key_id(HexStringToBytes(kGoodGUID));

  EXPECT_CALL(*drm1, GetNode())
      .WillOnce(
          Return(ByMove(absl::make_unique<XMLNode>("", "MockDRMSystem1"))));
  EXPECT_CALL(*drm2, GetNode())
      .WillOnce(
          Return(ByMove(absl::make_unique<XMLNode>("", "MockDRMSystem2"))));

  drm_list.AddDRMSystem(std::move(drm1));
  drm_list.AddDRMSystem(std::move(drm2));

  EXPECT_EQ(drm_list.Serialize(), kGoodXML);
}

}  // namespace
}  // namespace cpix
