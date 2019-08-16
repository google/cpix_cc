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

#include "rsa_public_key.h"

#include <cstdint>
#include <memory>
#include <vector>

#include "absl/memory/memory.h"
#include "cpix_util.h"
#include "openssl/base.h"
#include "openssl/bio.h"
#include "openssl/pem.h"
#include "openssl/rsa.h"

namespace cpix {

RSAPublicKey::~RSAPublicKey() = default;

std::unique_ptr<RSAPublicKey> RSAPublicKey::CreateFromPEM(
    const std::string& pem_public_key) {
  if (pem_public_key.empty()) {
    return nullptr;
  }

  std::unique_ptr<RSAPublicKey> key = absl::WrapUnique(new RSAPublicKey);

  if (!key->SetPublicKeyPEM(pem_public_key)) {
    return nullptr;
  }

  return key;
}

std::unique_ptr<RSAPublicKey> RSAPublicKey::CreateFromDER(
    const std::vector<uint8_t>& der_public_key) {
  if (der_public_key.empty()) {
    return nullptr;
  }

  std::unique_ptr<RSAPublicKey> key = absl::WrapUnique(new RSAPublicKey);

  if (!key->SetPublicKeyDER(der_public_key)) {
    return nullptr;
  }

  return key;
}

bool RSAPublicKey::SetPublicKeyPEM(const std::string& pem_public_key) {
  UniqueSslPtr<BIO> bio(BIO_new_mem_buf((void*)pem_public_key.c_str(), -1));

  if (!bio) {
    return false;
  }

  UniqueSslPtr<EVP_PKEY> key(
      PEM_read_bio_PUBKEY(bio.get(), nullptr, nullptr, nullptr));

  rsa = UniqueSslPtr<RSA>(EVP_PKEY_get1_RSA(key.get()));

  if (!rsa) {
    return false;
  }

  return true;
}

bool RSAPublicKey::SetPublicKeyDER(const std::vector<uint8_t>& der_public_key) {
  UniqueSslPtr<BIO> bio(
      BIO_new_mem_buf(reinterpret_cast<const void*>(der_public_key.data()),
                      der_public_key.size()));

  if (!bio) {
    return false;
  }

  UniqueSslPtr<EVP_PKEY> key(d2i_PUBKEY_bio(bio.get(), nullptr));

  rsa = UniqueSslPtr<RSA>(EVP_PKEY_get1_RSA(key.get()));

  if (!rsa) {
    return false;
  }

  return true;
}

std::vector<uint8_t> RSAPublicKey::Encrypt(
    const std::vector<uint8_t>& message) {
  if (!rsa) {
    return std::vector<uint8_t>();
  }

  std::vector<uint8_t> result(RSA_size(rsa.get()));
  size_t result_length =
      RSA_public_encrypt(message.size(), message.data(), result.data(),
                         rsa.get(), RSA_PKCS1_OAEP_PADDING);

  if (result_length != result.size()) {
    return std::vector<uint8_t>();
  }

  return result;
}

bool RSAPublicKey::MatchesKey(const RSA* key) {
  if (!key) return false;
  return BN_cmp(key->n, rsa.get()->n) == 0;
}
}  // namespace cpix
