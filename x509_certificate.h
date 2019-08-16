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

#ifndef CPIX_CC_X509_CERTIFICATE_H_
#define CPIX_CC_X509_CERTIFICATE_H_

#include <memory>
#include <string>
#include <vector>

#include "openssl/base.h"
#include "openssl/bio.h"
#include "unique_ssl_ptr.h"

namespace cpix {
class X509Certificate {
 public:
  ~X509Certificate();

  static std::unique_ptr<X509Certificate> CreateFromPEM(
      const std::string& pem_certificate);

  static std::unique_ptr<X509Certificate> CreateFromDER(
      const std::vector<uint8_t>& cert_bytes);

  std::string GetPubKey();

 private:
  X509Certificate() = default;

  bool SetCertificatePEM(const std::string& pem_certificate);
  bool SetCertificateDER(const std::vector<uint8_t>& der_certificate);

  UniqueSslPtr<X509> cert_;
};
}  // namespace cpix
#endif  // CPIX_CC_X509_CERTIFICATE_H_
