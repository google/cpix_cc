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
#include <vector>

#include "absl/memory/memory.h"
#include "aes_cryptor.h"
#include "content_key.h"
#include "cpix_element.h"
#include "cpix_util.h"
#include "xml_node.h"

namespace cpix {

ContentKeyList::~ContentKeyList() = default;

bool ContentKeyList::AddContentKey(std::unique_ptr<ContentKey> key) {
  if (key->kid().empty() || key->key_value().empty()) {
    return false;
  }

  AddElement(std::move(key));
  return true;
}

ContentKey* ContentKeyList::FindContentKey(const std::vector<uint8_t>& kid) {
  if (kid.empty()) {
    return nullptr;
  }

  for (const auto& element : elements_) {
    if (static_cast<ContentKey*>(element.get())->kid() == kid) {
      return static_cast<ContentKey*>(element.get());
    }
  }
  return nullptr;
}

std::unique_ptr<CPIXElement> ContentKeyList::CreateElement() {
  return absl::make_unique<ContentKey>();
}

bool ContentKeyList::DecryptContentKeys(
    const std::vector<uint8_t>& decrypt_key) {
  if (decrypt_key.empty()) {
    return false;
  }

  for (const auto& key : elements_) {
    ContentKey* key_ptr = static_cast<ContentKey*>(key.get());
    std::unique_ptr<AESCryptor> aes = AESCryptor::Create(decrypt_key);
    if (!key_ptr->explicit_iv().empty()) {
      if (!aes->SetIV(key_ptr->explicit_iv())) {
        return false;
      }
    }
    std::vector<uint8_t> plaintext = aes->CBCDecrypt(key_ptr->key_value());

    if (plaintext.empty()) {
      return false;
    }
    key_ptr->SetKeyValue(plaintext);
  }
  return true;
}
}  // namespace cpix
