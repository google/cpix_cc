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

#include "aes_cryptor.h"

#include <cstdint>
#include <memory>
#include <vector>

#include "absl/memory/memory.h"
#include "cpix_util.h"
#include "glog/logging.h"
#include "openssl/base.h"
#include "unique_ssl_ptr.h"

namespace cpix {

AESCryptor::~AESCryptor() = default;

std::unique_ptr<AESCryptor> AESCryptor::Create(
    const std::vector<uint8_t>& key) {
  if (key.size() != 32) {
    LOG(ERROR) << "KEY MUST BE 32 BYTES\n";
    return nullptr;
  }
  std::unique_ptr<AESCryptor> aes = absl::WrapUnique(new AESCryptor);
  aes->key_ = key;
  aes->iv_ = std::vector<uint8_t>(16, 0);
  return aes;
}

bool AESCryptor::SetIV(const std::vector<uint8_t>& iv) {
  if (iv.size() != 16) {
    LOG(ERROR) << "IV MUST BE 16 BITS\n";
    return false;
  }
  iv_ = iv;
  return true;
}

std::vector<uint8_t> AESCryptor::CBCEncrypt(
    const std::vector<uint8_t>& message) {
  UniqueSslPtr<EVP_CIPHER_CTX> ctx(EVP_CIPHER_CTX_new());

  if (!ctx) {
    return std::vector<uint8_t>();
  }

  if (EVP_EncryptInit_ex(ctx.get(), EVP_aes_256_cbc(), nullptr, key_.data(),
                         iv_.data()) != 1) {
    return std::vector<uint8_t>();
  }

  std::vector<uint8_t> result((message.size() / 16 + 1) * 16);
  int len;
  int result_length = 0;

  if (EVP_EncryptUpdate(ctx.get(), result.data(), &len, message.data(),
                        message.size()) != 1) {
    return std::vector<uint8_t>();
  }
  result_length += len;

  if (EVP_EncryptFinal_ex(ctx.get(), result.data() + result_length, &len) !=
      1) {
    return std::vector<uint8_t>();
  }
  result_length += len;
  result.resize(result_length);
  return result;
}

std::vector<uint8_t> AESCryptor::CBCDecrypt(
    const std::vector<uint8_t>& message) {
  UniqueSslPtr<EVP_CIPHER_CTX> ctx(EVP_CIPHER_CTX_new());

  if (!ctx) {
    return std::vector<uint8_t>();
  }

  if (EVP_DecryptInit_ex(ctx.get(), EVP_aes_256_cbc(), nullptr, key_.data(),
                         iv_.data()) != 1) {
    return std::vector<uint8_t>();
  }

  std::vector<uint8_t> result(message.size());
  int len, result_length = 0;

  if (EVP_DecryptUpdate(ctx.get(), result.data(), &len, message.data(),
                        message.size()) != 1) {
    return std::vector<uint8_t>();
  }
  result_length += len;

  if (EVP_DecryptFinal_ex(ctx.get(), result.data() + result_length, &len) !=
      1) {
    return std::vector<uint8_t>();
  }
  result_length += len;
  result.resize(result_length);
  return result;
}

}  // namespace cpix
