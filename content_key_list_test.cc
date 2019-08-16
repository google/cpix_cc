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

#include "content_key_list.h"

#include <memory>
#include <utility>

#include "absl/memory/memory.h"
#include "content_key.h"
#include "cpix_util.h"
#include "gmock/gmock.h"
#include "gtest/gtest.h"
#include "testable_cpix_element.h"
#include "xml_node.h"

namespace cpix {
namespace {

using ::testing::ByMove;
using ::testing::Return;
using ::testing::Test;

class MockContentKey : public ContentKey {
 public:
  MOCK_METHOD(std::unique_ptr<XMLNode>, GetNode, (), (override));
};

constexpr char kGoodXML[] =
    "<ContentKeyList><MockContentKey1/><MockContentKey2/></"
    "ContentKeyList>";

constexpr char kGoodRawKID[] = "bd5adf51cf04410faac3ec63a69e929e";

constexpr char kGoodKeyValue[] = "3iv9lYwafpe0uEmxDc6PSw==";

TEST(ContentKeyListTest, SerializeContentKeyList) {
  TestableCPIXElement<ContentKeyList> key_list;
  std::unique_ptr<MockContentKey> key1 = absl::make_unique<MockContentKey>();
  std::unique_ptr<MockContentKey> key2 = absl::make_unique<MockContentKey>();

  key1->set_key_id(HexStringToBytes(kGoodRawKID));
  key1->SetKeyValue(Base64StringToBytes(kGoodKeyValue));
  key2->set_key_id(HexStringToBytes(kGoodRawKID));
  key2->SetKeyValue(Base64StringToBytes(kGoodKeyValue));

  EXPECT_CALL(*key1, GetNode())
      .WillOnce(
          Return(ByMove(absl::make_unique<XMLNode>("", "MockContentKey1"))));
  EXPECT_CALL(*key2, GetNode())
      .WillOnce(
          Return(ByMove(absl::make_unique<XMLNode>("", "MockContentKey2"))));

  key_list.AddContentKey(std::move(key1));
  key_list.AddContentKey(std::move(key2));

  EXPECT_EQ(key_list.Serialize(), kGoodXML);
}

}  // namespace
}  // namespace cpix
