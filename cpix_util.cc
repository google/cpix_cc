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

#include "cpix_util.h"

#include <stddef.h>

#include <algorithm>
#include <vector>

#include "absl/strings/escaping.h"
#include "absl/strings/str_cat.h"
#include "absl/strings/string_view.h"
#include "openssl/rand.h"

namespace cpix {

std::vector<uint8_t> HexStringToBytes(const std::string& str) {
  std::string byte_string = absl::HexStringToBytes(str);
  return std::vector<uint8_t>(byte_string.begin(), byte_string.end());
}

std::vector<uint8_t> Base64StringToBytes(const std::string& str) {
  std::string data_str;
  absl::Base64Unescape(str, &data_str);
  return std::vector<uint8_t>(data_str.begin(), data_str.end());
}

std::vector<uint8_t> GUIDStringToBytes(const std::string& str) {
  std::string stripped = str;
  stripped.erase(std::remove(stripped.begin(), stripped.end(), '-'),
                 stripped.end());
  return HexStringToBytes(stripped);
}

std::string BytesToBase64String(const std::vector<uint8_t>& data) {
  return absl::Base64Escape(absl::string_view(
      reinterpret_cast<const char*>(data.data()), data.size()));
}

std::string BytesToGUID(const std::vector<uint8_t>& data) {
  std::string str;
  for (size_t i = 0; i < data.size(); i++) {
    absl::StrAppend(&str, absl::Hex(data[i], absl::kZeroPad2));
    if (i == 3 || i == 5 || i == 7 || i == 9) {
      absl::StrAppend(&str, "-");
    }
  }
  return str;
}

std::string BytesToHexString(const std::vector<uint8_t>& data) {
  std::string str(data.begin(), data.end());
  return absl::BytesToHexString(str);
}

std::vector<uint8_t> GetRandomBytes(int num_bytes) {
  std::vector<uint8_t> result(num_bytes);
  RAND_bytes(result.data(), result.size());
  return result;
}

std::string AddCertHeadersAndNewlines(const std::string& key) {
  std::string newlines = key;
  for (size_t i = 64; i < newlines.size(); i += 65) {
    newlines.insert(i, "\n");
  }
  return absl::StrCat(kCertHeader, newlines, kCertFooter);
}

std::string AddPubKeyHeadersAndNewlines(const std::string& key) {
  std::string newlines = key;
  for (size_t i = 64; i < key.size(); i += 65) {
    newlines.insert(i, "\n");
  }
  return absl::StrCat(kPubKeyHeader, newlines, kPubKeyFooter);
}

std::string AddPrivateKeyHeadersAndNewlines(const std::string& key) {
  std::string newlines = key;
  for (int i = 64; i < key.size(); i += 65) {
    newlines.insert(i, "\n");
  }
  return absl::StrCat(kPrivateKeyHeader, newlines, kPrivateKeyFooter);
}

std::string StripPEMHeadersAndNewlines(const std::string& cert) {
  std::string stripped = cert;
  stripped.erase(0, stripped.find('\n') + 1);
  stripped.erase(stripped.rfind('\n', stripped.size() - 2), std::string::npos);
  stripped.erase(std::remove(stripped.begin(), stripped.end(), '\n'),
                 stripped.end());
  return stripped;
}

}  // namespace cpix
