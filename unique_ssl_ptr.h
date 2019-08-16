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

#ifndef CPIX_CC_UNIQUE_SSL_PTR_H_
#define CPIX_CC_UNIQUE_SSL_PTR_H_

#include <memory>

#include "openssl/base.h"
#include "openssl/bio.h"
#include "openssl/rsa.h"
#include "openssl/x509.h"

namespace cpix {

struct SslDeleter {
  inline void operator()(BIO* ptr) const { (BIO_free(ptr)); }
  inline void operator()(RSA* ptr) const { (RSA_free(ptr)); }
  inline void operator()(X509* ptr) const { (X509_free(ptr)); }
  inline void operator()(EVP_PKEY* ptr) const { (EVP_PKEY_free(ptr)); }
  inline void operator()(EVP_CIPHER_CTX* ptr) const {
    EVP_CIPHER_CTX_free(ptr);
  }
};  // namespace cpix

template <typename SslType>
using UniqueSslPtr = std::unique_ptr<SslType, SslDeleter>;

}  // namespace cpix

#endif  // CPIX_CC_UNIQUE_SSL_PTR_H_
