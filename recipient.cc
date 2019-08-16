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

#include "recipient.h"

#include <cstdint>
#include <memory>
#include <utility>
#include <vector>

#include "absl/memory/memory.h"
#include "cpix_util.h"
#include "rsa_private_key.h"
#include "rsa_public_key.h"
#include "x509_certificate.h"
#include "xml_node.h"

namespace cpix {

Recipient::~Recipient() = default;

bool Recipient::SetDocumentKey(const std::vector<uint8_t>& key) {
  std::unique_ptr<RSAPublicKey> rsa = CreateRSAPublicKey();
  if (!rsa) {
    return false;
  }

  encrypted_document_key_ = rsa->Encrypt(key);
  return true;
}

bool Recipient::Deserialize(std::unique_ptr<XMLNode> node) {
  std::string attribute;
  if (!(attribute = node->GetAttribute("id")).empty()) {
    set_id(attribute);
  }

  std::unique_ptr<XMLNode> child =
      node->GetDescendantNode({"DeliveryKey", "X509Data", "X509Certificate"});
  if (!child) {
    return false;
  }

  delivery_key_ = Base64StringToBytes(child->GetContent());

  child =
      node->GetDescendantNode({"DocumentKey", "Data", "Secret",
                               "EncryptedValue", "CipherData", "CipherValue"});
  if (!child) {
    return false;
  }

  encrypted_document_key_ = Base64StringToBytes(child->GetContent());

  return true;
}

std::unique_ptr<XMLNode> Recipient::GetNode() {
  if (encrypted_document_key_.empty()) {
    return nullptr;
  }

  std::unique_ptr<XMLNode> root =
      absl::make_unique<XMLNode>("", "DeliveryData");

  if (!id().empty()) {
    root->AddAttribute("id", id());
  }

  std::unique_ptr<XMLNode> delivery_key =
      absl::make_unique<XMLNode>("", "DeliveryKey");

  std::unique_ptr<XMLNode> x509_data =
      absl::make_unique<XMLNode>("ds", "X509Data");

  std::unique_ptr<XMLNode> x509_cert =
      absl::make_unique<XMLNode>("ds", "X509Certificate");
  x509_cert->SetContent(BytesToBase64String(delivery_key_));

  x509_data->AddChild(std::move(x509_cert));
  delivery_key->AddChild(std::move(x509_data));

  std::unique_ptr<XMLNode> document_key =
      absl::make_unique<XMLNode>("", "DocumentKey");
  document_key->AddAttribute("Algorithm",
                             "http://www.w3.org/2001/04/xmlenc#aes256-cbc");

  std::unique_ptr<XMLNode> data = absl::make_unique<XMLNode>("", "Data");

  std::unique_ptr<XMLNode> secret =
      absl::make_unique<XMLNode>("pskc", "Secret");

  std::unique_ptr<XMLNode> encrypted_value =
      absl::make_unique<XMLNode>("pskc", "EncryptedValue");

  std::unique_ptr<XMLNode> encryption_method =
      absl::make_unique<XMLNode>("enc", "EncryptionMethod");
  encryption_method->AddAttribute(
      "Algorithm", "http://www.w3.org/2001/04/xmlenc#rsa-oaep-mgf1p");

  std::unique_ptr<XMLNode> cipher_data =
      absl::make_unique<XMLNode>("enc", "CipherData");

  std::unique_ptr<XMLNode> cipher_value =
      absl::make_unique<XMLNode>("enc", "CipherValue");

  cipher_value->SetContent(BytesToBase64String(encrypted_document_key_));

  cipher_data->AddChild(std::move(cipher_value));
  encrypted_value->AddChild(std::move(encryption_method));
  encrypted_value->AddChild(std::move(cipher_data));
  secret->AddChild(std::move(encrypted_value));
  data->AddChild(std::move(secret));
  document_key->AddChild(std::move(data));

  root->AddChild(std::move(delivery_key));
  root->AddChild(std::move(document_key));

  return root;
}

std::unique_ptr<RSAPublicKey> Recipient::CreateRSAPublicKey() {
  std::unique_ptr<X509Certificate> cert =
      X509Certificate::CreateFromDER(delivery_key_);
  if (!cert) {
    return nullptr;
  }

  return RSAPublicKey::CreateFromPEM(cert->GetPubKey());
}

std::vector<uint8_t> Recipient::DecryptDocumentKeyWith(
    const std::vector<uint8_t>& private_key) {
  std::unique_ptr<RSAPrivateKey> rsa =
      RSAPrivateKey::CreateFromDER(private_key);
  if (!rsa) {
    return std::vector<uint8_t>();
  }

  return rsa->Decrypt(encrypted_document_key_);
}

}  // namespace cpix
