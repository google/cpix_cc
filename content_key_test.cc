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

#include "content_key.h"

#include <utility>

#include "absl/memory/memory.h"
#include "cpix_util.h"
#include "gtest/gtest.h"
#include "testable_cpix_element.h"
#include "xml_node.h"

namespace cpix {
namespace {

using ::testing::Test;

class TestableContentKey : public TestableCPIXElement<ContentKey> {
 public:
  TestableContentKey() = default;
  ~TestableContentKey() = default;
  using TestableCPIXElement<ContentKey>::Serialize;
  using TestableCPIXElement<ContentKey>::Deserialize;
  using TestableCPIXElement<ContentKey>::SetEncryptedKeyValue;
};

constexpr char kGoodRawKID[] = "bd5adf51cf04410faac3ec63a69e929e";

constexpr char kGoodDashedKID[] = "bd5adf51-cf04-410f-aac3-ec63a69e929e";

constexpr char kGoodKeyValue[] = "3iv9lYwafpe0uEmxDc6PSw==";

constexpr char kGoodEncryptedKeyValue[] =
    "ZqZ6HrtExnFkiaf9E2OLMF7nD9L3sdzEzRObm7YiST0=";

constexpr char kGoodXMLClearNS[] =
    "<ContentKey xmlns:enc=\"http://www.w3.org/2001/04/xmlenc#\" "
    "xmlns:pskc=\"urn:ietf:params:xml:ns:keyprov:pskc\" "
    "kid=\"bd5adf51-cf04-410f-aac3-ec63a69e929e\"><Data><pskc:Secret><pskc:"
    "PlainValue>3iv9lYwafpe0uEmxDc6PSw==</pskc:PlainValue></pskc:Secret></"
    "Data></ContentKey>";

constexpr char kGoodXMLEncryptedNS[] =
    "<ContentKey xmlns:enc=\"http://www.w3.org/2001/04/xmlenc#\" "
    "xmlns:pskc=\"urn:ietf:params:xml:ns:keyprov:pskc\" "
    "kid=\"bd5adf51-cf04-410f-aac3-ec63a69e929e\"><Data><pskc:Secret><pskc:"
    "EncryptedValue><enc:EncryptionMethod "
    "Algorithm=\"http://www.w3.org/2001/04/xmlenc#aes256-cbc\"/"
    "><enc:CipherData><enc:CipherValue>"
    "ZqZ6HrtExnFkiaf9E2OLMF7nD9L3sdzEzRObm7YiST0=</enc:CipherValue></"
    "enc:CipherData></pskc:EncryptedValue></pskc:Secret></Data></ContentKey>";

constexpr char kGoodXMLClear[] =
    "<ContentKey "
    "kid=\"bd5adf51-cf04-410f-aac3-ec63a69e929e\"><Data><pskc:Secret><pskc:"
    "PlainValue>3iv9lYwafpe0uEmxDc6PSw==</pskc:PlainValue></pskc:Secret></"
    "Data></ContentKey>";

constexpr char kGoodXMLEncrypted[] =
    "<ContentKey "
    "kid=\"bd5adf51-cf04-410f-aac3-ec63a69e929e\"><Data><pskc:Secret><pskc:"
    "EncryptedValue><enc:EncryptionMethod "
    "Algorithm=\"http://www.w3.org/2001/04/xmlenc#aes256-cbc\"/"
    "><enc:CipherData><enc:CipherValue>"
    "ZqZ6HrtExnFkiaf9E2OLMF7nD9L3sdzEzRObm7YiST0=</enc:CipherValue></"
    "enc:CipherData></pskc:EncryptedValue></pskc:Secret></Data></ContentKey>";

TEST(ContentKeyTest, SerializeContentKeyClear) {
  TestableCPIXElement<ContentKey> key;

  key.set_key_id(HexStringToBytes(kGoodRawKID));
  key.SetKeyValue(Base64StringToBytes(kGoodKeyValue));

  EXPECT_EQ(key.Serialize(), kGoodXMLClear);
}

TEST(ContentKeyTest, SerializeContentKeyEncrypted) {
  TestableContentKey key;

  key.set_key_id(HexStringToBytes(kGoodRawKID));
  key.SetEncryptedKeyValue(Base64StringToBytes(kGoodEncryptedKeyValue));

  EXPECT_EQ(key.Serialize(), kGoodXMLEncrypted);
}

TEST(ContentKeyTest, DeserializeContentKeyClear) {
  TestableCPIXElement<ContentKey> key;
  std::unique_ptr<XMLNode> node = absl::make_unique<XMLNode>(kGoodXMLClearNS);
  key.Deserialize(std::move(node));
  EXPECT_EQ(key.kid(), GUIDStringToBytes(kGoodDashedKID));
  EXPECT_EQ(key.key_value(), Base64StringToBytes(kGoodKeyValue));
  EXPECT_EQ(key.Serialize(), kGoodXMLClear);
}

TEST(ContentKeyTest, DeserializeContentKeyEncrypted) {
  TestableCPIXElement<ContentKey> key;
  std::unique_ptr<XMLNode> node =
      absl::make_unique<XMLNode>(kGoodXMLEncryptedNS);
  key.Deserialize(std::move(node));
  EXPECT_EQ(key.kid(), GUIDStringToBytes(kGoodDashedKID));
  EXPECT_TRUE(key.is_encrypted());
  EXPECT_EQ(key.key_value(), Base64StringToBytes(kGoodEncryptedKeyValue));
  EXPECT_EQ(key.Serialize(), kGoodXMLEncrypted);
}

}  // namespace
}  // namespace cpix
