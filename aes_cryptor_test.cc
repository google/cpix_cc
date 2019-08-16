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

#include <memory>

#include "cpix_util.h"
#include "gtest/gtest.h"

namespace cpix {
namespace {

// Test vectors from:
// https://csrc.nist.gov/CSRC/media/Projects/Cryptographic-Standards-and-Guidelines/documents/examples/AES_CBC.pdf

constexpr char kPlaintext[] =
    "6bc1bee22e409f96e93d7e117393172aae2d8a571e03ac9c9eb76fac45af8e5130c81c46a3"
    "5ce411e5fbc1191a0a52eff69f2445df4f9b17ad2b417be66c3710";

constexpr char kPlaintextWithIV[] =
    "6bc0bce12a459991e134741a7f9e1925d86421fb9f1a1eda505ee1375746972c604ed7ddf3"
    "2efdff7020d0238b7c2a5d8521f2fd3c8eef2cdc3da7e5c44ea206";

constexpr char kCiphertext[] =
    "f58c4c04d6e5f1ba779eabfb5f7bfbd69cfc4e967edb808d679f777bc6702c7d39f23369a9"
    "d9bacfa530e26304231461b2eb05e2c39be9fcda6c19078c6a9d1b3f461796d6b0d6b2e0c2"
    "a72b4d80e644";

constexpr char kCiphertextWithIV[] =
    "f58c4c04d6e5f1ba779eabfb5f7bfbd69677d0f393846028457243f4039ae01264b95db0cd"
    "3d51113d75f582180ec27ca933be0f5f6c2bd819806ab78d01d64e18a0026f46b0f450f48e"
    "6040a9c60426";

constexpr char kGoodAesKey[] =
    "603DEB1015CA71BE2B73AEF0857D77811F352C073B6108D72D9810A30914DFF4";

constexpr char kGoodIV[] = "000102030405060708090A0B0C0D0E0F";

TEST(AESUtilTest, AESEncrypt) {
  std::unique_ptr<AESCryptor> aes =
      AESCryptor::Create(HexStringToBytes(kGoodAesKey));
  aes->SetIV(HexStringToBytes(kGoodIV));
  EXPECT_EQ(BytesToHexString(aes->CBCEncrypt(HexStringToBytes(kPlaintext))),
            kCiphertext);
}

TEST(AESUtilTest, AESDecrypt) {
  std::unique_ptr<AESCryptor> aes =
      AESCryptor::Create(HexStringToBytes(kGoodAesKey));
  aes->SetIV(HexStringToBytes(kGoodIV));
  EXPECT_EQ(BytesToHexString(aes->CBCDecrypt(HexStringToBytes(kCiphertext))),
            kPlaintext);
}

TEST(AESUtilTest, AESEncryptEmptyIV) {
  std::unique_ptr<AESCryptor> aes =
      AESCryptor::Create(HexStringToBytes(kGoodAesKey));
  EXPECT_EQ(
      BytesToHexString(aes->CBCEncrypt(HexStringToBytes(kPlaintextWithIV))),
      kCiphertextWithIV);
}

TEST(AESUtilTest, AESDecryptEmptyIV) {
  std::unique_ptr<AESCryptor> aes =
      AESCryptor::Create(HexStringToBytes(kGoodAesKey));
  EXPECT_EQ(
      BytesToHexString(aes->CBCDecrypt(HexStringToBytes(kCiphertextWithIV))),
      kPlaintextWithIV);
}
}  // namespace
}  // namespace cpix
