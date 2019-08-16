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

#include <cstdint>
#include <iterator>
#include <vector>

#include "gtest/gtest.h"

namespace cpix {
namespace {

constexpr char kGoodBase64[] = "3iv9lYwafpe0uEmxDc6PSw==";
constexpr uint8_t kGoodBase64Bytes[] = {0xde, 0x2b, 0xfd, 0x95, 0x8c, 0x1a,
                                        0x7e, 0x97, 0xb4, 0xb8, 0x49, 0xb1,
                                        0x0d, 0xce, 0x8f, 0x4b};
constexpr char kGoodHexString[] = "bd5adf51cf04410faac3ec63a69e929e";
constexpr uint8_t kGoodHexBytes[] = {0xbd, 0x5a, 0xdf, 0x51, 0xcf, 0x04,
                                     0x41, 0x0f, 0xaa, 0xc3, 0xec, 0x63,
                                     0xa6, 0x9e, 0x92, 0x9e};
constexpr char kGoodGUIDString[] = "bd5adf51-cf04-410f-aac3-ec63a69e929e";

constexpr char kGoodPubKey[] =
    "-----BEGIN PUBLIC KEY-----\n"
    "MIGfMA0GCSqGSIb3DQEBAQUAA4GNADCBiQKBgQCp2w+8HUdECo8V5yuKYrWJmUbL\n"
    "tD6nSyVifN543axXvNSFzQfWNOGVkMsCo6W4hpl5eHv1p9Hqdcf/ZYQDWCK726u6\n"
    "hsZA81AblAOOXKaUaxvFC+ZKRJf+MtUGnv0v7CrGoblm1mMC/OQI1JfSsYi68Epn\n"
    "aOLepTZw+GLTnusQgwIDAQAB\n"
    "-----END PUBLIC KEY-----\n";

constexpr char kGoodPubKeyNoHeader[] =
    "MIGfMA0GCSqGSIb3DQEBAQUAA4GNADCBiQKBgQCp2w+"
    "8HUdECo8V5yuKYrWJmUbLtD6nSyVifN543axXvNSFzQfWNOGVkMsCo6W4hpl5eHv1p9Hqdcf/"
    "ZYQDWCK726u6hsZA81AblAOOXKaUaxvFC+ZKRJf+MtUGnv0v7CrGoblm1mMC/"
    "OQI1JfSsYi68EpnaOLepTZw+GLTnusQgwIDAQAB";

constexpr char kGoodCert[] =
    "-----BEGIN CERTIFICATE-----\n"
    "MIIFEjCCA/qgAwIBAgIJAJ49e4qDHLbhMA0GCSqGSIb3DQEBBQUAMIG2MQswCQYD\n"
    "VQQGEwJVUzETMBEGA1UECBMKQ2FsaWZvcm5pYTEWMBQGA1UEBxMNTW91bnRhaW4g\n"
    "VmlldzEPMA0GA1UEChMGR29vZ2xlMRowGAYDVQQLExFVbml0IFRlc3RpbmcgT25s\n"
    "eTErMCkGA1UEAxMiR29vZ2xlIFJvb3QgQ0EgKFVuaXQgVGVzdGluZyBPbmx5KTEg\n"
    "MB4GCSqGSIb3DQEJARYRaWZldHRlQGdvb2dsZS5jb20wHhcNMTEwOTE5MDUzNDQx\n"
    "WhcNMjYwOTE1MDUzNDQxWjCBtjELMAkGA1UEBhMCVVMxEzARBgNVBAgTCkNhbGlm\n"
    "b3JuaWExFjAUBgNVBAcTDU1vdW50YWluIFZpZXcxDzANBgNVBAoTBkdvb2dsZTEa\n"
    "MBgGA1UECxMRVW5pdCBUZXN0aW5nIE9ubHkxKzApBgNVBAMTIkdvb2dsZSBSb290\n"
    "IENBIChVbml0IFRlc3RpbmcgT25seSkxIDAeBgkqhkiG9w0BCQEWEWlmZXR0ZUBn\n"
    "b29nbGUuY29tMIIBIjANBgkqhkiG9w0BAQEFAAOCAQ8AMIIBCgKCAQEAsgv6IPVA\n"
    "wk4l35fXNPrs/qGyswzvXfvxyVko8IlkIwxjK1Hk485GWsRPRsRafHFUFkneiZxq\n"
    "7Zix/+aR6NvjUuOh6APHLNkId4er0x7qqEL+s3Fv/+HfKBy3WgvAFeC5QSmRRPr5\n"
    "Dqm5MGRe9s66EzlzFx7OAGtEeG8n0iNJusnVUq70n5knhdgR7ePJAmpxOEGZLh1J\n"
    "0FV1ImL/wtFnr8VVCYEpeCk2m53/Q5CtAZmGYsCokLTNOLP422NYsj3M8dtE9TPv\n"
    "QIQx02nuKD44Gc1FQrJt/hW4Y6U7O7u+dIIDZ3R5Ox21fZ0v7rDxLi42zmeq9Co1\n"
    "Q51qeY67Umgs2wIDAQABo4IBHzCCARswHQYDVR0OBBYEFPsPcYPvtMldvIpebzq6\n"
    "MZJIWw37MIHrBgNVHSMEgeMwgeCAFPsPcYPvtMldvIpebzq6MZJIWw37oYG8pIG5\n"
    "MIG2MQswCQYDVQQGEwJVUzETMBEGA1UECBMKQ2FsaWZvcm5pYTEWMBQGA1UEBxMN\n"
    "TW91bnRhaW4gVmlldzEPMA0GA1UEChMGR29vZ2xlMRowGAYDVQQLExFVbml0IFRl\n"
    "c3RpbmcgT25seTErMCkGA1UEAxMiR29vZ2xlIFJvb3QgQ0EgKFVuaXQgVGVzdGlu\n"
    "ZyBPbmx5KTEgMB4GCSqGSIb3DQEJARYRaWZldHRlQGdvb2dsZS5jb22CCQCePXuK\n"
    "gxy24TAMBgNVHRMEBTADAQH/MA0GCSqGSIb3DQEBBQUAA4IBAQBDCSWYx1uWgt+g\n"
    "pGzT9RCc1tAdrkeEOcl66jAHU3Z+NUyNX+O57e8+NHUsXsNHzJ5NBkDc8WY/yzSG\n"
    "D7x/a0Sl5zWzbL6eD4bR9dcFOxUzFlfNHegrv+qbGXILs7MpUThGaNjRMPYUV+R5\n"
    "1ed2QyOF04Dl1IskcLnEu5DbYBKbTj/PHTyRO8A9IXivgYaD/WJgyd+0DN5gLiyX\n"
    "Gp2yXgJyRQkt9XAmrZkyr/8/Ms0ljJYPuE7JazS+txYb5qwCpGioE07mUvlhLQX8\n"
    "Rp73J1yJCNU0YxwphYG8t3nM6qt8GqfGx32B7HMxwNdfqVkr56swON/GLYFXySWH\n"
    "vBL10c68\n"
    "-----END CERTIFICATE-----\n";

constexpr char kGoodCertNoHeaders[] =
    "MIIFEjCCA/qgAwIBAgIJAJ49e4qDHLbhMA0GCSqGSIb3DQEBBQUAMIG2MQswCQYD"
    "VQQGEwJVUzETMBEGA1UECBMKQ2FsaWZvcm5pYTEWMBQGA1UEBxMNTW91bnRhaW4g"
    "VmlldzEPMA0GA1UEChMGR29vZ2xlMRowGAYDVQQLExFVbml0IFRlc3RpbmcgT25s"
    "eTErMCkGA1UEAxMiR29vZ2xlIFJvb3QgQ0EgKFVuaXQgVGVzdGluZyBPbmx5KTEg"
    "MB4GCSqGSIb3DQEJARYRaWZldHRlQGdvb2dsZS5jb20wHhcNMTEwOTE5MDUzNDQx"
    "WhcNMjYwOTE1MDUzNDQxWjCBtjELMAkGA1UEBhMCVVMxEzARBgNVBAgTCkNhbGlm"
    "b3JuaWExFjAUBgNVBAcTDU1vdW50YWluIFZpZXcxDzANBgNVBAoTBkdvb2dsZTEa"
    "MBgGA1UECxMRVW5pdCBUZXN0aW5nIE9ubHkxKzApBgNVBAMTIkdvb2dsZSBSb290"
    "IENBIChVbml0IFRlc3RpbmcgT25seSkxIDAeBgkqhkiG9w0BCQEWEWlmZXR0ZUBn"
    "b29nbGUuY29tMIIBIjANBgkqhkiG9w0BAQEFAAOCAQ8AMIIBCgKCAQEAsgv6IPVA"
    "wk4l35fXNPrs/qGyswzvXfvxyVko8IlkIwxjK1Hk485GWsRPRsRafHFUFkneiZxq"
    "7Zix/+aR6NvjUuOh6APHLNkId4er0x7qqEL+s3Fv/+HfKBy3WgvAFeC5QSmRRPr5"
    "Dqm5MGRe9s66EzlzFx7OAGtEeG8n0iNJusnVUq70n5knhdgR7ePJAmpxOEGZLh1J"
    "0FV1ImL/wtFnr8VVCYEpeCk2m53/Q5CtAZmGYsCokLTNOLP422NYsj3M8dtE9TPv"
    "QIQx02nuKD44Gc1FQrJt/hW4Y6U7O7u+dIIDZ3R5Ox21fZ0v7rDxLi42zmeq9Co1"
    "Q51qeY67Umgs2wIDAQABo4IBHzCCARswHQYDVR0OBBYEFPsPcYPvtMldvIpebzq6"
    "MZJIWw37MIHrBgNVHSMEgeMwgeCAFPsPcYPvtMldvIpebzq6MZJIWw37oYG8pIG5"
    "MIG2MQswCQYDVQQGEwJVUzETMBEGA1UECBMKQ2FsaWZvcm5pYTEWMBQGA1UEBxMN"
    "TW91bnRhaW4gVmlldzEPMA0GA1UEChMGR29vZ2xlMRowGAYDVQQLExFVbml0IFRl"
    "c3RpbmcgT25seTErMCkGA1UEAxMiR29vZ2xlIFJvb3QgQ0EgKFVuaXQgVGVzdGlu"
    "ZyBPbmx5KTEgMB4GCSqGSIb3DQEJARYRaWZldHRlQGdvb2dsZS5jb22CCQCePXuK"
    "gxy24TAMBgNVHRMEBTADAQH/MA0GCSqGSIb3DQEBBQUAA4IBAQBDCSWYx1uWgt+g"
    "pGzT9RCc1tAdrkeEOcl66jAHU3Z+NUyNX+O57e8+NHUsXsNHzJ5NBkDc8WY/yzSG"
    "D7x/a0Sl5zWzbL6eD4bR9dcFOxUzFlfNHegrv+qbGXILs7MpUThGaNjRMPYUV+R5"
    "1ed2QyOF04Dl1IskcLnEu5DbYBKbTj/PHTyRO8A9IXivgYaD/WJgyd+0DN5gLiyX"
    "Gp2yXgJyRQkt9XAmrZkyr/8/Ms0ljJYPuE7JazS+txYb5qwCpGioE07mUvlhLQX8"
    "Rp73J1yJCNU0YxwphYG8t3nM6qt8GqfGx32B7HMxwNdfqVkr56swON/GLYFXySWH"
    "vBL10c68";

TEST(CPIXUtilTest, Base64StringToBytes) {
  EXPECT_EQ(Base64StringToBytes(kGoodBase64),
            std::vector<uint8_t>(std::begin(kGoodBase64Bytes),
                                 std::end(kGoodBase64Bytes)));
}

TEST(CPIXUtilTest, BytesToBase64String) {
  EXPECT_EQ(BytesToBase64String(std::vector<uint8_t>(
                std::begin(kGoodBase64Bytes), std::end(kGoodBase64Bytes))),
            kGoodBase64);
}

TEST(CPIXUtilTest, HexStringToBytes) {
  EXPECT_EQ(HexStringToBytes(kGoodHexString),
            std::vector<uint8_t>(
                kGoodHexBytes,
                kGoodHexBytes + sizeof(kGoodHexBytes) / sizeof(uint8_t)));
}

TEST(CPIXUtilTest, BytesToGUID) {
  EXPECT_EQ(BytesToGUID(std::vector<uint8_t>(
                kGoodHexBytes,
                kGoodHexBytes + sizeof(kGoodHexBytes) / sizeof(uint8_t))),
            kGoodGUIDString);
}

TEST(CPIXUtilTest, GetRandomBytes) { EXPECT_EQ(GetRandomBytes(32).size(), 32); }

TEST(CPIXUtilTest, AddPubKeyHeaders) {
  EXPECT_EQ(AddPubKeyHeadersAndNewlines(kGoodPubKeyNoHeader), kGoodPubKey);
}

TEST(CPIXUtilTest, AddCertHeaders) {
  EXPECT_EQ(AddCertHeadersAndNewlines(kGoodCertNoHeaders), kGoodCert);
}

TEST(CPIXUtilTest, StripPubKeyHeaders) {
  EXPECT_EQ(StripPEMHeadersAndNewlines(kGoodPubKey), kGoodPubKeyNoHeader);
}

TEST(CPIXUtilTest, StripCertHeaders) {
  EXPECT_EQ(StripPEMHeadersAndNewlines(kGoodCert), kGoodCertNoHeaders);
}

TEST(CPIXUtilTest, GUIDToBytes) {
  EXPECT_EQ(GUIDStringToBytes(kGoodGUIDString),
            HexStringToBytes(kGoodHexString));
}

}  // namespace
}  // namespace cpix
