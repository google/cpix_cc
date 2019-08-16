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

#include "rsa_private_key.h"

#include <stdio.h>

#include <cstdint>
#include <memory>
#include <vector>

#include "absl/memory/memory.h"
#include "cpix_util.h"
#include "openssl/base.h"
#include "openssl/bio.h"
#include "openssl/evp.h"
#include "openssl/pem.h"
#include "openssl/rsa.h"
#include "openssl/x509.h"

namespace cpix {

RSAPrivateKey::~RSAPrivateKey() = default;

std::unique_ptr<RSAPrivateKey> RSAPrivateKey::CreateFromPEM(
    const std::string& pem_private_key) {
  if (pem_private_key.empty()) {
    return nullptr;
  }

  std::unique_ptr<RSAPrivateKey> key = absl::WrapUnique(new RSAPrivateKey);

  if (!key->SetPrivateKeyPEM(pem_private_key)) {
    return nullptr;
  }

  return key;
}

std::unique_ptr<RSAPrivateKey> RSAPrivateKey::CreateFromDER(
    const std::vector<uint8_t>& private_key_bytes) {
  if (private_key_bytes.empty()) {
    return nullptr;
  }

  std::unique_ptr<RSAPrivateKey> key = absl::WrapUnique(new RSAPrivateKey);

  if (!key->SetPrivateKeyDER(private_key_bytes)) {
    return nullptr;
  }

  return key;
}

bool RSAPrivateKey::SetPrivateKeyPEM(const std::string& pem_private_key) {
  UniqueSslPtr<BIO> bio(BIO_new_mem_buf((void*)pem_private_key.c_str(), -1));

  if (!bio) {
    return false;
  }

  UniqueSslPtr<EVP_PKEY> key(
      PEM_read_bio_PrivateKey(bio.get(), nullptr, nullptr, nullptr));

  rsa = UniqueSslPtr<RSA>(EVP_PKEY_get1_RSA(key.get()));

  if (!rsa) {
    return false;
  }

  return true;
}

bool RSAPrivateKey::SetPrivateKeyDER(
    const std::vector<uint8_t>& der_private_key) {
  UniqueSslPtr<BIO> bio(
      BIO_new_mem_buf(reinterpret_cast<const void*>(der_private_key.data()),
                      der_private_key.size()));

  if (!bio) {
    return false;
  }

  UniqueSslPtr<EVP_PKEY> key(d2i_PrivateKey_bio(bio.get(), nullptr));

  rsa = UniqueSslPtr<RSA>(EVP_PKEY_get1_RSA(key.get()));

  if (!rsa) {
    return false;
  }

  return true;
}

std::vector<uint8_t> RSAPrivateKey::Decrypt(
    const std::vector<uint8_t>& message) {
  if (!rsa) {
    return std::vector<uint8_t>();
  }

  std::vector<uint8_t> result(RSA_size(rsa.get()));

  int result_length =
      RSA_private_decrypt(RSA_size(rsa.get()), message.data(), result.data(),
                          rsa.get(), RSA_PKCS1_OAEP_PADDING);

  if (result_length == -1) {
    return std::vector<uint8_t>();
  }
  result.resize(result_length);
  return result;
}
}  // namespace cpix
