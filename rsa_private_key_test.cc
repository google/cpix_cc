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

#include <memory>

#include "cpix_util.h"
#include "gtest/gtest.h"
#include "rsa_public_key.h"

namespace cpix {
namespace {

constexpr char kGoodPublicKey[] =
    "-----BEGIN PUBLIC KEY-----\n"
    "MIGfMA0GCSqGSIb3DQEBAQUAA4GNADCBiQKBgQCp2w+8HUdECo8V5yuKYrWJmUbL\n"
    "tD6nSyVifN543axXvNSFzQfWNOGVkMsCo6W4hpl5eHv1p9Hqdcf/ZYQDWCK726u6\n"
    "hsZA81AblAOOXKaUaxvFC+ZKRJf+MtUGnv0v7CrGoblm1mMC/OQI1JfSsYi68Epn\n"
    "aOLepTZw+GLTnusQgwIDAQAB\n"
    "-----END PUBLIC KEY-----\n";

constexpr char kGoodPrivateKey[] =
    "-----BEGIN RSA PRIVATE KEY-----\n"
    "MIICXAIBAAKBgQCp2w+8HUdECo8V5yuKYrWJmUbLtD6nSyVifN543axXvNSFzQfW\n"
    "NOGVkMsCo6W4hpl5eHv1p9Hqdcf/ZYQDWCK726u6hsZA81AblAOOXKaUaxvFC+ZK\n"
    "RJf+MtUGnv0v7CrGoblm1mMC/OQI1JfSsYi68EpnaOLepTZw+GLTnusQgwIDAQAB\n"
    "AoGBAKDuq3PikblH/9YS11AgwjwC++7ZcltzeZJdGTSPY1El2n6Dip9ML0hUjeSM\n"
    "ROIWtac/nsNcJCnvOnUjK/c3NIAaGJcfRPiH/S0Ga6ROiDfFj2UXAmk/v4wRRUzr\n"
    "5lsA0jgEt5qcq2Xr/JPQVGB4wUgL/yQK0dDhW0EdrJ707e3BAkEA1aIHbmcVfCP8\n"
    "Y/uWuK0lvWxrIWfR5MlHhI8tD9lvkot2kyXiV+jB6/gktwk1QaFsy7dCXn7w03+k\n"
    "xrjEGGN+kQJBAMuKf55lDtU9K2Js3YSStTZAXP+Hz7XpoLxmbWFyGvBx806WjgAD\n"
    "624irwS+0tBxkERbRcisfb2cXmAx8earT9MCQDZuVCpjBWxd1t66qYpgQ29iAmG+\n"
    "jBIY3qn9uOOC6RSTiCCx1FvFqDMxRFmGdRVFxeyZwsVE3qNksF0Zko0MPKECQCEe\n"
    "oDV97DP2iCCz5je0R5hUUM2jo8DOC0GcyR+aGZgWcqjPBrwp5x08t43mHxeb4wW8\n"
    "dFZ6+trnntO4TMxkA9ECQB+yCPgO1zisJWYuD46KISoesYhwHe5C1BQElQgi9bio\n"
    "U39fFo88w1pok23a2CZBEXguSvCvexeB68OggdDXvy0=\n"
    "-----END RSA PRIVATE KEY-----\n";

constexpr char kStringToEncrypt[] =
    "I3LkAjCWpXuOtjYnB45VgcyFM9CO2sAy7tsaVW6x+"
    "5nR0wcOhBrPyU0Cgq0OVbkP2Fn5u3rArpc0bJw=";

TEST(CPIXUtilTest, RSADecryptPEM) {
  std::unique_ptr<RSAPrivateKey> key =
      RSAPrivateKey::CreateFromPEM(kGoodPrivateKey);
  ASSERT_TRUE(key);
  std::unique_ptr<RSAPublicKey> pubkey =
      RSAPublicKey::CreateFromPEM(kGoodPublicKey);
  ASSERT_TRUE(pubkey);
  EXPECT_EQ(BytesToBase64String(key->Decrypt(
                pubkey->Encrypt(Base64StringToBytes(kStringToEncrypt)))),
            kStringToEncrypt);
}

TEST(CPIXUtilTest, RSADecryptDER) {
  std::unique_ptr<RSAPrivateKey> key = RSAPrivateKey::CreateFromDER(
      Base64StringToBytes(StripPEMHeadersAndNewlines(kGoodPrivateKey)));
  ASSERT_TRUE(key);
  std::unique_ptr<RSAPublicKey> pubkey =
      RSAPublicKey::CreateFromPEM(kGoodPublicKey);
  ASSERT_TRUE(pubkey);
  EXPECT_EQ(BytesToBase64String(key->Decrypt(
                pubkey->Encrypt(Base64StringToBytes(kStringToEncrypt)))),
            kStringToEncrypt);
}

}  // namespace
}  // namespace cpix
