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

#ifndef CPIX_CC_RSA_PRIVATE_KEY_H_
#define CPIX_CC_RSA_PRIVATE_KEY_H_

#include <cstdint>
#include <memory>
#include <string>
#include <vector>

#include "openssl/base.h"
#include "openssl/bio.h"
#include "unique_ssl_ptr.h"

namespace cpix {
class RSAPrivateKey {
 public:
  ~RSAPrivateKey();

  // TODO(noahmdavis): add Create() that takes in DER formatted public key
  static std::unique_ptr<RSAPrivateKey> CreateFromPEM(
      const std::string& pem_private_key);

  static std::unique_ptr<RSAPrivateKey> CreateFromDER(
      const std::vector<uint8_t>& private_key_bytes);

  // Returns the raw RSA unencrypted data of the supplied message.
  std::vector<uint8_t> Decrypt(const std::vector<uint8_t>& message);

  const RSA* rsa_key() { return rsa.get(); }

 private:
  RSAPrivateKey() = default;

  bool SetPrivateKeyPEM(const std::string& pem_private_key);
  bool SetPrivateKeyDER(const std::vector<uint8_t>& der_private_key);

  UniqueSslPtr<RSA> rsa;
};
}  // namespace cpix
#endif  // CPIX_CC_RSA_PRIVATE_KEY_H_
