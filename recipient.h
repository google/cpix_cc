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

#ifndef CPIX_CC_RECIPIENT_H_
#define CPIX_CC_RECIPIENT_H_

#include <stdint.h>

#include <memory>
#include <string>
#include <vector>

#include "cpix_element.h"

namespace cpix {
class XMLNode;
class RSAPublicKey;

// A core element of the CPIX document. Contains the X509 certificate of a
// "receiving entity".

class Recipient : public CPIXElement {
 public:
  Recipient() = default;
  ~Recipient();

  void set_delivery_key(const std::vector<uint8_t>& key) {
    delivery_key_ = key;
  }

 protected:
  bool Deserialize(std::unique_ptr<XMLNode> node) override;

  // Takes in a clear document key and saves it in encrypted form using the RSA
  // public key from the recipient's |delivery_key_|.
  bool SetDocumentKey(const std::vector<uint8_t>& key);
  void set_encrypted_document_key(const std::vector<uint8_t>& key) {
    encrypted_document_key_ = key;
  }
  const std::vector<uint8_t>& encrypted_document_key() const {
    return encrypted_document_key_;
  }

 private:
  friend class RecipientList;
  friend class CPIXMessage;
  std::unique_ptr<XMLNode> GetNode() override;
  std::unique_ptr<RSAPublicKey> CreateRSAPublicKey();
  std::vector<uint8_t> DecryptDocumentKeyWith(
      const std::vector<uint8_t>& private_key);

  std::vector<uint8_t> delivery_key_;
  std::vector<uint8_t> encrypted_document_key_;
};
}  // namespace cpix

#endif  // CPIX_CC_RECIPIENT_H_
