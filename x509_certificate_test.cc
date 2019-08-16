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

#include <memory>

#include "cpix_util.h"
#include "gtest/gtest.h"

namespace cpix {
namespace {

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

constexpr char kGoodPubKey[] =
    "-----BEGIN PUBLIC KEY-----\n"
    "MIIBIjANBgkqhkiG9w0BAQEFAAOCAQ8AMIIBCgKCAQEAsgv6IPVAwk4l35fXNPrs\n"
    "/qGyswzvXfvxyVko8IlkIwxjK1Hk485GWsRPRsRafHFUFkneiZxq7Zix/+aR6Nvj\n"
    "UuOh6APHLNkId4er0x7qqEL+s3Fv/+HfKBy3WgvAFeC5QSmRRPr5Dqm5MGRe9s66\n"
    "EzlzFx7OAGtEeG8n0iNJusnVUq70n5knhdgR7ePJAmpxOEGZLh1J0FV1ImL/wtFn\n"
    "r8VVCYEpeCk2m53/Q5CtAZmGYsCokLTNOLP422NYsj3M8dtE9TPvQIQx02nuKD44\n"
    "Gc1FQrJt/hW4Y6U7O7u+dIIDZ3R5Ox21fZ0v7rDxLi42zmeq9Co1Q51qeY67Umgs\n"
    "2wIDAQAB\n"
    "-----END PUBLIC KEY-----\n";

TEST(X509CertificateTest, GetPublicKey) {
  std::unique_ptr<X509Certificate> cert =
      X509Certificate::CreateFromPEM(kGoodCert);
  ASSERT_TRUE(cert);
  EXPECT_EQ(cert->GetPubKey(), kGoodPubKey);
}

TEST(X509CertificateTest, DERCert) {
  std::unique_ptr<X509Certificate> cert = X509Certificate::CreateFromDER(
      Base64StringToBytes(StripPEMHeadersAndNewlines(kGoodCert)));
  ASSERT_TRUE(cert);
}

}  // namespace
}  // namespace cpix
