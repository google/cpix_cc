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

#include "x509_certificate.h"

#include "absl/memory/memory.h"
#include "glog/logging.h"
#include "openssl/base.h"
#include "openssl/bio.h"
#include "openssl/pem.h"
#include "openssl/x509.h"

namespace cpix {

X509Certificate::~X509Certificate() = default;

std::unique_ptr<X509Certificate> X509Certificate::CreateFromPEM(
    const std::string& pem_certificate) {
  if (pem_certificate.empty()) {
    return nullptr;
  }

  std::unique_ptr<X509Certificate> cert = absl::WrapUnique(new X509Certificate);

  if (!cert->SetCertificatePEM(pem_certificate)) {
    return nullptr;
  }

  return cert;
}

std::unique_ptr<X509Certificate> X509Certificate::CreateFromDER(
    const std::vector<uint8_t>& cert_bytes) {
  if (cert_bytes.empty()) {
    return nullptr;
  }

  std::unique_ptr<X509Certificate> cert = absl::WrapUnique(new X509Certificate);

  if (!cert->SetCertificateDER(cert_bytes)) {
    return nullptr;
  }

  return cert;
}

std::string X509Certificate::GetPubKey() {
  DCHECK(cert_);

  UniqueSslPtr<EVP_PKEY> key(X509_get_pubkey(cert_.get()));

  if (!key) {
    return "";
  }

  UniqueSslPtr<BIO> bio(BIO_new(BIO_s_mem()));
  if (PEM_write_bio_PUBKEY(bio.get(), key.get()) != 1) {
    return "";
  }
  char* ptr;
  int len = BIO_get_mem_data(bio.get(), &ptr);
  if (len == 0) {
    return "";
  }
  return std::string(ptr, len);
}

bool X509Certificate::SetCertificatePEM(const std::string& pem_certificate) {
  UniqueSslPtr<BIO> bio(BIO_new_mem_buf(
      reinterpret_cast<const void*>(pem_certificate.c_str()), -1));

  if (!bio) {
    return false;
  }

  cert_ = UniqueSslPtr<X509>(
      PEM_read_bio_X509_AUX(bio.get(), nullptr, nullptr, nullptr));

  if (!cert_) {
    return false;
  }

  return true;
}

bool X509Certificate::SetCertificateDER(
    const std::vector<uint8_t>& der_certificate) {
  UniqueSslPtr<BIO> bio(
      BIO_new_mem_buf(reinterpret_cast<const void*>(der_certificate.data()),
                      der_certificate.size()));

  if (!bio) {
    return false;
  }

  cert_ = UniqueSslPtr<X509>(d2i_X509_bio(bio.get(), nullptr));

  if (!cert_) {
    return false;
  }

  return true;
}

}  // namespace cpix
