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

#ifndef CPIX_CC_CONTENT_KEY_H_
#define CPIX_CC_CONTENT_KEY_H_

#include <cstdint>
#include <memory>
#include <string>
#include <vector>

#include "cpix_element.h"

namespace cpix {
class XMLNode;

// A core element of the CPIX document. Contains information directly related to
// an encryption key.

class ContentKey : public CPIXElement {
 public:
  ContentKey() = default;
  ~ContentKey();
  const std::vector<uint8_t>& kid() const { return kid_; }
  const std::vector<uint8_t>& key_value() const { return key_value_; }
  bool is_encrypted() const { return is_encrypted_; }
  const std::vector<uint8_t>& explicit_iv() const { return explicit_iv_; }
  void set_key_id(const std::vector<uint8_t>& key_id) { kid_ = key_id; }
  const std::vector<uint8_t>& key_id() { return kid_; }
  void set_explicit_iv(const std::vector<uint8_t>& iv) { explicit_iv_ = iv; }

  // Requires the clear key value. Will be encrypted later on document
  // serialization if at least one Recipient is present.
  void SetKeyValue(const std::vector<uint8_t>& value);

 protected:
  bool Deserialize(std::unique_ptr<XMLNode> node) override;
  void SetEncryptedKeyValue(const std::vector<uint8_t>& value);

 private:
  friend class ContentKeyList;
  friend class CPIXMessage;
  std::unique_ptr<XMLNode> GetNode() override;

  std::vector<uint8_t> kid_;
  std::vector<uint8_t> key_value_;
  std::vector<uint8_t> explicit_iv_;
  bool is_encrypted_ = false;
};
}  // namespace cpix

#endif  // CPIX_CC_CONTENT_KEY_H_
