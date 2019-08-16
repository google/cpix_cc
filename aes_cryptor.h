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

#ifndef CPIX_CC_AES_CRYPTOR_H_
#define CPIX_CC_AES_CRYPTOR_H_

#include <stdint.h>

#include <memory>
#include <string>
#include <vector>

namespace cpix {
class AESCryptor {
 public:
  ~AESCryptor();

  static std::unique_ptr<AESCryptor> Create(const std::vector<uint8_t>& key);

  bool SetIV(const std::vector<uint8_t>& iv);

  std::vector<uint8_t> CBCEncrypt(const std::vector<uint8_t>& message);

  std::vector<uint8_t> CBCDecrypt(const std::vector<uint8_t>& message);

 private:
  AESCryptor() = default;

  std::vector<uint8_t> key_;
  std::vector<uint8_t> iv_;
};
}  // namespace cpix
#endif  // CPIX_CC_AES_CRYPTOR_H_
