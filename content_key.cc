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

#include <memory>
#include <utility>
#include <vector>

#include "absl/memory/memory.h"
#include "cpix_util.h"
#include "xml_node.h"

namespace cpix {
ContentKey::~ContentKey() = default;

std::unique_ptr<XMLNode> ContentKey::GetNode() {
  if (key_value_.empty() || kid_.empty()) {
    return nullptr;
  }

  std::unique_ptr<XMLNode> root = absl::make_unique<XMLNode>("", "ContentKey");
  if (!id().empty()) {
    root->AddAttribute("id", id());
  }

  root->AddAttribute("kid", BytesToGUID(kid_));

  std::unique_ptr<XMLNode> value;
  if (is_encrypted_) {
    value = absl::make_unique<XMLNode>("pskc", "EncryptedValue");

    std::unique_ptr<XMLNode> encryption_method =
        absl::make_unique<XMLNode>("enc", "EncryptionMethod");
    encryption_method->AddAttribute(
        "Algorithm", "http://www.w3.org/2001/04/xmlenc#aes256-cbc");

    std::unique_ptr<XMLNode> cipher_data =
        absl::make_unique<XMLNode>("enc", "CipherData");

    std::unique_ptr<XMLNode> cipher_value =
        absl::make_unique<XMLNode>("enc", "CipherValue");

    cipher_value->SetContent(BytesToBase64String(key_value_));

    cipher_data->AddChild(std::move(cipher_value));
    value->AddChild(std::move(encryption_method));
    value->AddChild(std::move(cipher_data));
    if (!explicit_iv_.empty()) {
      root->AddAttribute("explicitIV", BytesToBase64String(explicit_iv_));
    }
  } else {
    value = absl::make_unique<XMLNode>("pskc", "PlainValue");
    value->SetContent(BytesToBase64String(key_value_));
  }

  std::unique_ptr<XMLNode> secret =
      absl::make_unique<XMLNode>("pskc", "Secret");
  secret->AddChild(std::move(value));

  std::unique_ptr<XMLNode> data = absl::make_unique<XMLNode>("", "Data");
  data->AddChild(std::move(secret));

  root->AddChild(std::move(data));

  return root;
}

void ContentKey::SetEncryptedKeyValue(const std::vector<uint8_t>& value) {
  is_encrypted_ = true;
  key_value_ = value;
}

void ContentKey::SetKeyValue(const std::vector<uint8_t>& value) {
  is_encrypted_ = false;
  key_value_ = value;
}

bool ContentKey::Deserialize(std::unique_ptr<XMLNode> node) {
  std::string attribute;
  if (!(attribute = node->GetAttribute("id")).empty()) {
    set_id(attribute);
  }

  kid_ = GUIDStringToBytes(node->GetAttribute("kid"));

  if (!(attribute = node->GetAttribute("explicitIV")).empty()) {
    explicit_iv_ = Base64StringToBytes(attribute);
  }

  std::unique_ptr<XMLNode> child = node->GetDescendantNode({"Data", "Secret"});

  if (!child) {
    return false;
  }

  std::unique_ptr<XMLNode> data;

  if ((data = child->GetFirstChildByName("PlainValue"))) {
    key_value_ = Base64StringToBytes(data->GetContent());
    is_encrypted_ = false;
    return true;
  }

  data =
      child->GetDescendantNode({"EncryptedValue", "CipherData", "CipherValue"});

  if (!data) {
    return false;
  }

  key_value_ = Base64StringToBytes(data->GetContent());
  is_encrypted_ = true;

  return true;
}

}  // namespace cpix
