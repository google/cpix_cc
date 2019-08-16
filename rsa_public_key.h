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

#ifndef CPIX_CC_RSA_PUBLIC_KEY_H_
#define CPIX_CC_RSA_PUBLIC_KEY_H_

#include <stdint.h>

#include <memory>
#include <string>
#include <vector>

#include "openssl/base.h"
#include "openssl/bio.h"
#include "unique_ssl_ptr.h"

namespace cpix {
class RSAPublicKey {
 public:
  ~RSAPublicKey();

  // TODO(noahmdavis): add Create() that takes in DER formatted public key
  static std::unique_ptr<RSAPublicKey> CreateFromPEM(
      const std::string& pem_public_key);

  static std::unique_ptr<RSAPublicKey> CreateFromDER(
      const std::vector<uint8_t>& der_public_key);

  // Returns the raw RSA encrypyed data of the supplied message.
  std::vector<uint8_t> Encrypt(const std::vector<uint8_t>& message);

  const RSA* rsa_key() { return rsa.get(); }

  bool MatchesKey(const RSA* key);

 private:
  RSAPublicKey() = default;

  bool SetPublicKeyPEM(const std::string& pem_public_key);
  bool SetPublicKeyDER(const std::vector<uint8_t>& der_public_key);

  UniqueSslPtr<RSA> rsa;
};
}  // namespace cpix
#endif  // CPIX_CC_RSA_PUBLIC_KEY_H_
