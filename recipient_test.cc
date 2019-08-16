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

#include "recipient.h"

#include <memory>
#include <utility>

#include "absl/memory/memory.h"
#include "cpix_util.h"
#include "gtest/gtest.h"
#include "testable_cpix_element.h"
#include "xml_node.h"

namespace cpix {
namespace {

using ::testing::Test;

class TestableRecipient : public Recipient {
 public:
  TestableRecipient() = default;
  ~TestableRecipient() = default;
  using Recipient::Deserialize;
  using Recipient::encrypted_document_key;
  using Recipient::Serialize;
  using Recipient::set_encrypted_document_key;
  using Recipient::SetDocumentKey;
};

constexpr char kGoodPubKey[] =
    "-----BEGIN PUBLIC KEY-----\n"
    "MIGfMA0GCSqGSIb3DQEBAQUAA4GNADCBiQKBgQCp2w+8HUdECo8V5yuKYrWJmUbL\n"
    "tD6nSyVifN543axXvNSFzQfWNOGVkMsCo6W4hpl5eHv1p9Hqdcf/ZYQDWCK726u6\n"
    "hsZA81AblAOOXKaUaxvFC+ZKRJf+MtUGnv0v7CrGoblm1mMC/OQI1JfSsYi68Epn\n"
    "aOLepTZw+GLTnusQgwIDAQAB\n"
    "-----END PUBLIC KEY-----\n";

constexpr char kGoodXMLNS[] =
    "<DeliveryData xmlns:ds=\"http://www.w3.org/2000/09/xmldsig#\" "
    "xmlns:enc=\"http://www.w3.org/2001/04/xmlenc#\" "
    "xmlns:pskc=\"urn:ietf:params:xml:ns:keyprov:pskc\"><DeliveryKey><ds:"
    "X509Data><ds:X509Certificate>MIIFEjCCA/"
    "qgAwIBAgIJAJ49e4qDHLbhMA0GCSqGSIb3DQEBBQUAMIG2MQswCQYDVQQGEwJVUzETMBEGA1UE"
    "CBMKQ2FsaWZvcm5pYTEWMBQGA1UEBxMNTW91bnRhaW4gVmlldzEPMA0GA1UEChMGR29vZ2xlMR"
    "owGAYDVQQLExFVbml0IFRlc3RpbmcgT25seTErMCkGA1UEAxMiR29vZ2xlIFJvb3QgQ0EgKFVu"
    "aXQgVGVzdGluZyBPbmx5KTEgMB4GCSqGSIb3DQEJARYRaWZldHRlQGdvb2dsZS5jb20wHhcNMT"
    "EwOTE5MDUzNDQxWhcNMjYwOTE1MDUzNDQxWjCBtjELMAkGA1UEBhMCVVMxEzARBgNVBAgTCkNh"
    "bGlmb3JuaWExFjAUBgNVBAcTDU1vdW50YWluIFZpZXcxDzANBgNVBAoTBkdvb2dsZTEaMBgGA1"
    "UECxMRVW5pdCBUZXN0aW5nIE9ubHkxKzApBgNVBAMTIkdvb2dsZSBSb290IENBIChVbml0IFRl"
    "c3RpbmcgT25seSkxIDAeBgkqhkiG9w0BCQEWEWlmZXR0ZUBnb29nbGUuY29tMIIBIjANBgkqhk"
    "iG9w0BAQEFAAOCAQ8AMIIBCgKCAQEAsgv6IPVAwk4l35fXNPrs/"
    "qGyswzvXfvxyVko8IlkIwxjK1Hk485GWsRPRsRafHFUFkneiZxq7Zix/"
    "+aR6NvjUuOh6APHLNkId4er0x7qqEL+s3Fv/"
    "+HfKBy3WgvAFeC5QSmRRPr5Dqm5MGRe9s66EzlzFx7OAGtEeG8n0iNJusnVUq70n5knhdgR7eP"
    "JAmpxOEGZLh1J0FV1ImL/wtFnr8VVCYEpeCk2m53/"
    "Q5CtAZmGYsCokLTNOLP422NYsj3M8dtE9TPvQIQx02nuKD44Gc1FQrJt/"
    "hW4Y6U7O7u+"
    "dIIDZ3R5Ox21fZ0v7rDxLi42zmeq9Co1Q51qeY67Umgs2wIDAQABo4IBHzCCARswHQYDVR0OBB"
    "YEFPsPcYPvtMldvIpebzq6MZJIWw37MIHrBgNVHSMEgeMwgeCAFPsPcYPvtMldvIpebzq6MZJI"
    "Ww37oYG8pIG5MIG2MQswCQYDVQQGEwJVUzETMBEGA1UECBMKQ2FsaWZvcm5pYTEWMBQGA1UEBx"
    "MNTW91bnRhaW4gVmlldzEPMA0GA1UEChMGR29vZ2xlMRowGAYDVQQLExFVbml0IFRlc3Rpbmcg"
    "T25seTErMCkGA1UEAxMiR29vZ2xlIFJvb3QgQ0EgKFVuaXQgVGVzdGluZyBPbmx5KTEgMB4GCS"
    "qGSIb3DQEJARYRaWZldHRlQGdvb2dsZS5jb22CCQCePXuKgxy24TAMBgNVHRMEBTADAQH/"
    "MA0GCSqGSIb3DQEBBQUAA4IBAQBDCSWYx1uWgt+gpGzT9RCc1tAdrkeEOcl66jAHU3Z+NUyNX+"
    "O57e8+NHUsXsNHzJ5NBkDc8WY/yzSGD7x/"
    "a0Sl5zWzbL6eD4bR9dcFOxUzFlfNHegrv+qbGXILs7MpUThGaNjRMPYUV+"
    "R51ed2QyOF04Dl1IskcLnEu5DbYBKbTj/PHTyRO8A9IXivgYaD/"
    "WJgyd+0DN5gLiyXGp2yXgJyRQkt9XAmrZkyr/8/"
    "Ms0ljJYPuE7JazS+"
    "txYb5qwCpGioE07mUvlhLQX8Rp73J1yJCNU0YxwphYG8t3nM6qt8GqfGx32B7HMxwNdfqVkr56"
    "swON/GLYFXySWHvBL10c68</ds:X509Certificate></ds:X509Data></"
    "DeliveryKey><DocumentKey "
    "Algorithm=\"http://www.w3.org/2001/04/"
    "xmlenc#aes256-cbc\"><Data><pskc:Secret><pskc:EncryptedValue><enc:"
    "EncryptionMethod "
    "Algorithm=\"http://www.w3.org/2001/04/xmlenc#rsa-oaep-mgf1p\"/"
    "><enc:CipherData><enc:CipherValue>encrypteddocumentkey<"
    "/enc:CipherValue></enc:CipherData></pskc:EncryptedValue></pskc:Secret></"
    "Data></DocumentKey></DeliveryData>";

constexpr char kGoodXML[] =
    "<DeliveryData><DeliveryKey><ds:X509Data><ds:X509Certificate>MIIFEjCCA/"
    "qgAwIBAgIJAJ49e4qDHLbhMA0GCSqGSIb3DQEBBQUAMIG2MQswCQYDVQQGEwJVUzETMBEGA1UE"
    "CBMKQ2FsaWZvcm5pYTEWMBQGA1UEBxMNTW91bnRhaW4gVmlldzEPMA0GA1UEChMGR29vZ2xlMR"
    "owGAYDVQQLExFVbml0IFRlc3RpbmcgT25seTErMCkGA1UEAxMiR29vZ2xlIFJvb3QgQ0EgKFVu"
    "aXQgVGVzdGluZyBPbmx5KTEgMB4GCSqGSIb3DQEJARYRaWZldHRlQGdvb2dsZS5jb20wHhcNMT"
    "EwOTE5MDUzNDQxWhcNMjYwOTE1MDUzNDQxWjCBtjELMAkGA1UEBhMCVVMxEzARBgNVBAgTCkNh"
    "bGlmb3JuaWExFjAUBgNVBAcTDU1vdW50YWluIFZpZXcxDzANBgNVBAoTBkdvb2dsZTEaMBgGA1"
    "UECxMRVW5pdCBUZXN0aW5nIE9ubHkxKzApBgNVBAMTIkdvb2dsZSBSb290IENBIChVbml0IFRl"
    "c3RpbmcgT25seSkxIDAeBgkqhkiG9w0BCQEWEWlmZXR0ZUBnb29nbGUuY29tMIIBIjANBgkqhk"
    "iG9w0BAQEFAAOCAQ8AMIIBCgKCAQEAsgv6IPVAwk4l35fXNPrs/"
    "qGyswzvXfvxyVko8IlkIwxjK1Hk485GWsRPRsRafHFUFkneiZxq7Zix/"
    "+aR6NvjUuOh6APHLNkId4er0x7qqEL+s3Fv/"
    "+HfKBy3WgvAFeC5QSmRRPr5Dqm5MGRe9s66EzlzFx7OAGtEeG8n0iNJusnVUq70n5knhdgR7eP"
    "JAmpxOEGZLh1J0FV1ImL/wtFnr8VVCYEpeCk2m53/"
    "Q5CtAZmGYsCokLTNOLP422NYsj3M8dtE9TPvQIQx02nuKD44Gc1FQrJt/"
    "hW4Y6U7O7u+"
    "dIIDZ3R5Ox21fZ0v7rDxLi42zmeq9Co1Q51qeY67Umgs2wIDAQABo4IBHzCCARswHQYDVR0OBB"
    "YEFPsPcYPvtMldvIpebzq6MZJIWw37MIHrBgNVHSMEgeMwgeCAFPsPcYPvtMldvIpebzq6MZJI"
    "Ww37oYG8pIG5MIG2MQswCQYDVQQGEwJVUzETMBEGA1UECBMKQ2FsaWZvcm5pYTEWMBQGA1UEBx"
    "MNTW91bnRhaW4gVmlldzEPMA0GA1UEChMGR29vZ2xlMRowGAYDVQQLExFVbml0IFRlc3Rpbmcg"
    "T25seTErMCkGA1UEAxMiR29vZ2xlIFJvb3QgQ0EgKFVuaXQgVGVzdGluZyBPbmx5KTEgMB4GCS"
    "qGSIb3DQEJARYRaWZldHRlQGdvb2dsZS5jb22CCQCePXuKgxy24TAMBgNVHRMEBTADAQH/"
    "MA0GCSqGSIb3DQEBBQUAA4IBAQBDCSWYx1uWgt+gpGzT9RCc1tAdrkeEOcl66jAHU3Z+NUyNX+"
    "O57e8+NHUsXsNHzJ5NBkDc8WY/yzSGD7x/"
    "a0Sl5zWzbL6eD4bR9dcFOxUzFlfNHegrv+qbGXILs7MpUThGaNjRMPYUV+"
    "R51ed2QyOF04Dl1IskcLnEu5DbYBKbTj/PHTyRO8A9IXivgYaD/"
    "WJgyd+0DN5gLiyXGp2yXgJyRQkt9XAmrZkyr/8/"
    "Ms0ljJYPuE7JazS+"
    "txYb5qwCpGioE07mUvlhLQX8Rp73J1yJCNU0YxwphYG8t3nM6qt8GqfGx32B7HMxwNdfqVkr56"
    "swON/GLYFXySWHvBL10c68</ds:X509Certificate></ds:X509Data></"
    "DeliveryKey><DocumentKey "
    "Algorithm=\"http://www.w3.org/2001/04/"
    "xmlenc#aes256-cbc\"><Data><pskc:Secret><pskc:EncryptedValue><enc:"
    "EncryptionMethod "
    "Algorithm=\"http://www.w3.org/2001/04/xmlenc#rsa-oaep-mgf1p\"/"
    "><enc:CipherData><enc:CipherValue>encrypteddocumentkey<"
    "/enc:CipherValue></enc:CipherData></pskc:EncryptedValue></pskc:Secret></"
    "Data></DocumentKey></DeliveryData>";

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

TEST(RecipientTest, SerializeRecipient) {
  TestableRecipient recipient;

  recipient.set_delivery_key(
      Base64StringToBytes(StripPEMHeadersAndNewlines(kGoodCert)));
  recipient.set_encrypted_document_key(
      Base64StringToBytes("encrypteddocumentkey"));
  EXPECT_EQ(recipient.Serialize(), kGoodXML);
}

TEST(RecipientTest, DeserializeRecipient) {
  TestableCPIXElement<Recipient> recipient;
  std::unique_ptr<XMLNode> node = absl::make_unique<XMLNode>(kGoodXMLNS);
  EXPECT_TRUE(recipient.Deserialize(std::move(node)));
  EXPECT_EQ(recipient.Serialize(), kGoodXML);
}

TEST(RecipientTest, SetEncryptedDocumentKey) {
  TestableRecipient recipient;
  recipient.set_delivery_key(
      Base64StringToBytes(StripPEMHeadersAndNewlines(kGoodCert)));
  recipient.SetDocumentKey(
      Base64StringToBytes(StripPEMHeadersAndNewlines(kGoodPubKey)));
  EXPECT_FALSE(recipient.encrypted_document_key().empty());
}
}  // namespace
}  // namespace cpix
