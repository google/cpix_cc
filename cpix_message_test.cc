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

#include "cpix_message.h"

#include <memory>
#include <utility>

#include "absl/memory/memory.h"
#include "content_key.h"
#include "cpix_util.h"
#include "gmock/gmock.h"
#include "gtest/gtest.h"
#include "recipient.h"
#include "xml_node.h"
#include "xml_util.h"

namespace cpix {
namespace {

using ::testing::Test;

class MockRecipientList : public RecipientList {
 public:
  MOCK_METHOD(std::unique_ptr<XMLNode>, GetNode, (), (override));
};

class MockContentKeyList : public ContentKeyList {
 public:
  MOCK_METHOD(std::unique_ptr<XMLNode>, GetNode, (), (override));
};

class MockDRMSystemList : public DRMSystemList {
 public:
  MOCK_METHOD(std::unique_ptr<XMLNode>, GetNode, (), (override));
};

class MockUsageRuleList : public UsageRuleList {
 public:
  MOCK_METHOD(std::unique_ptr<XMLNode>, GetNode, (), (override));
};

class MockKeyPeriodList : public KeyPeriodList {
 public:
  MOCK_METHOD(std::unique_ptr<XMLNode>, GetNode, (), (override));
};

constexpr char kCpixDocument[] =
    "<CPIX xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\" "
    "xmlns:xsd=\"http://www.w3.org/2001/XMLSchema\" "
    "xmlns=\"urn:dashif:org:cpix\" "
    "xmlns:ds=\"http://www.w3.org/2000/09/xmldsig#\" "
    "xmlns:enc=\"http://www.w3.org/2001/04/xmlenc#\" "
    "xmlns:pskc=\"urn:ietf:params:xml:ns:keyprov:pskc\"/>";

constexpr char kFullCpix[] =
    "<CPIX "
    "xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\" "
    "xmlns:xsd=\"http://www.w3.org/2001/XMLSchema\" "
    "xmlns=\"urn:dashif:org:cpix\" "
    "xmlns:ds=\"http://www.w3.org/2000/09/xmldsig#\" "
    "xmlns:enc=\"http://www.w3.org/2001/04/xmlenc#\" "
    "xmlns:pskc=\"urn:ietf:params:xml:ns:keyprov:pskc\"><DeliveryDataList><"
    "DeliveryData><DeliveryKey><ds:X509Data><ds:X509Certificate>"
    "MIIFHzCCAwegAwIBAgIQOUR2UTpsGLxKca7jlNOzQDANBgkqhkiG9w0BAQ0FADAgMR4wHAYDVQ"
    "QDExVDUElYIEV4YW1wbGUgRW50aXR5IDEwHhcNMTYwODExMDcyMTMxWhcNMzkxMjMxMjM1OTU5"
    "WjAgMR4wHAYDVQQDExVDUElYIEV4YW1wbGUgRW50aXR5IDEwggIiMA0GCSqGSIb3DQEBAQUAA4"
    "ICDwAwggIKAoICAQCrNH2BTPt13OcVznobk3xTWLLKb5zeglDNmr5ovZ8KJcV1Ibd32MkxAG7m"
    "f5nvEqxT2aDI8L4PB9HZmLvOTBHvLt38VY/"
    "jkEGQzsycYyrLLUxJoFGSRNNAQJP88aZy2D3E0W+pPYJedXNZ/"
    "7+cX5uYCufTqfwmfRu2FBrZFs4s1WI1UFqDdORYhCqlSrnqWfFKbZkbUenc/"
    "YrGDJmhrSC1hDZERilDiWAK/9sE3G9/nQLuwLEnBD10/"
    "hekg1HVyAWzCF0t03Y2e6ddBOYZbLlaib5BoaM8lZpJuis3VUd74h54sylyDI+b8rHzqwq+"
    "FpbyO+"
    "OjU0lLJ1mWfkYjNJBwMViXigNF6KK8NgYjXG02NUbxXyeer9tiORWbadsWaNrVD8uR7IZ457yY"
    "EGLGxbGO0DKicda4Fa1u+T8ZIIiZNgEoSLULlftXR4gNvKp5N3AHW+QDK+z1hai8AYaZt2R7+"
    "xj0mYA2Ek2qYIPLxI1IDMHJCZbjNK/Pfh70HvZ48GoGEtbNINbl0cQCy/"
    "IyDKdoullfaSNSK1cbaJXY8PBAxFs9rRIM4iQFPPqW2Uq+nrBdZwjAEM/"
    "kXQ1q6QMwsszxPjbGjbC3oXXW/3AsU8K9tHh/dMifAathdzyb0C/"
    "rQsjXQUPnKI9MJAb9sjktXGoX6gIdAyGd++2txtzo/"
    "dpSpQIDAQABo1UwUzBRBgNVHQEESjBIgBA+"
    "TVN76ax88utZ3FhloYo3oSIwIDEeMBwGA1UEAxMVQ1BJWCBFeGFtcGxlIEVudGl0eSAxghA5RH"
    "ZROmwYvEpxruOU07NAMA0GCSqGSIb3DQEBDQUAA4ICAQBsTD35Xdprq9dQKYXObX96QZCMfd5o"
    "FDQCK9/"
    "43+w6eyhsbPNjyE2EwlCUMtJfgRwbnc5OzzGSss8jgQcdx6ZI5hfilE+"
    "aDBFTO7XgSvCM4oEQ4FWNtkgIpYaB6iAsZEarf31kR6MiRRfSqfLk/"
    "Pg+9wB9vuSSNYxQAjnNYtSM/"
    "+3DzKhJZ6ZoTxTYAmFrZF9E2XTY5eLwM8Fvnpe2lss8bjuE9UM1qj5PutiPzZQWSSFTxr99ylN"
    "WQFt2YmpIzWfAedOF1AL1g8K5z3eLn2B6xG0ZkgaFNsnKYR6L8ei0eCVKFxMB0UPXfSx0Z2Dlo"
    "PudII7L5dYFQrAB52GT3PDS+rHlfhmQ8uZ7nCi+a3h3XopuaDjv9I2KbnMZh7vY7twbVNXyz+"
    "pIs9lh49ol6w+Hjufwfj9+maMvfuRnCP/m9oyT/qnwvOBX6FDu8qEr0TXRGkdYS8NIol/"
    "V3IKgKrc5+eo/4wqCqUePUz0ZmiCbtjIOu7kun2HYMyu2crLaO9BH1usS5UGdrYCyg/"
    "0vukIylcYwAPc3HpeIt2yaCdMKJf/"
    "GhP+E0QlYTuPo7Bx4+00j7B33w36LFEbC3pxaWqltZ8Iy4tHl0rmcer/"
    "p1gGrRZTkqOQ7zqCl7xWjGK82bh5TooT7hDBTMVcgRRTYwUy4gnY4TgXERovhHQwgGKrhxw==<"
    "/ds:X509Certificate></ds:X509Data></DeliveryKey><DocumentKey "
    "Algorithm=\"http://www.w3.org/2001/04/"
    "xmlenc#aes256-cbc\"><Data><pskc:Secret><pskc:EncryptedValue><enc:"
    "EncryptionMethod "
    "Algorithm=\"http://www.w3.org/2001/04/xmlenc#rsa-oaep-mgf1p\"/"
    "><enc:CipherData><enc:CipherValue>"
    "I3LkAdeFjCWpXuOtjYnB45VgcyFM9COJz3kxpFGz3EJQjyDNEIpKShN1ERZ0Yzm0n51SAz5tIE"
    "SewG1FL+"
    "AOLMsRN85swJcMRMrk9sUMqHbaaObzj6AXMIBodastxeIqM0T1el9NoeRpr1X9qWBJu64EiXGB"
    "QZm2MqduEpWU1MBrHxWVGzqhjtZw5LKcnrj3AAJHzCaZpAy4IF3hXzpTgu5L/HIeRNM5/"
    "J8NimQYxSi6hsrAKtTaogm3QMAa0VaK/"
    "uY3K+dIuOGWR5YbrbmnYsX39RNF8UIkkmWpJ4GldfP5FDLJdf18x0k2CsTvKQB5E4Ld/"
    "eyPDKoYej4iEGlQPasBqurp2sAy7tsaVW6x+"
    "5nR0wcOhBrPynX1PAeZvQdLoJq1w81WzVm05R7Hi8Tjgb8GZ/"
    "MuBESxMxrCCpoCvHUN+"
    "MqdMFNCvJ4IwRFYsKfvIxa3SJCL6geGnwpnpdZ79mLCygIaXRSeget7ivNwEiRw6jTVdvZ+"
    "w914Inl8q/0j6XYzwSImiGi7Wk6pTx0lLBmVT8Uxlaj9JHB/gTOSJ93RsP7/7YZswjCt/"
    "UCFrGi1cv+x8NrGCHTCuGcXcjbK4z+"
    "E2rUeXCR4GUlGNaZxeFRE5g9VdiTFGeN2TMTava8yPraXd7rruQ+"
    "butsDiU0Cgq0OVbkP2Fn5u3rArpc0bJw=</enc:CipherValue></enc:CipherData></"
    "pskc:EncryptedValue></pskc:Secret></Data></DocumentKey></DeliveryData></"
    "DeliveryDataList><ContentKeyList><ContentKey "
    "kid=\"bd5adf51-cf04-410f-aac3-ec63a69e929e\"><Data><pskc:Secret><pskc:"
    "EncryptedValue><enc:EncryptionMethod "
    "Algorithm=\"http://www.w3.org/2001/04/xmlenc#aes256-cbc\""
    "/><enc:CipherData><enc:CipherValue>mGwyX8hfOluTOPFM5QOT8zhkpyDA/OnIGTJs/"
    "qQHSE0Vy8X2nkzfCz/VxfMUb6A4</enc:CipherValue></enc:CipherData></"
    "pskc:EncryptedValue></pskc:Secret></Data></ContentKey><ContentKey "
    "kid=\"d2920429-87ab-41e6-a4c5-a8c836b6312e\"><Data><pskc:Secret><pskc:"
    "EncryptedValue><enc:EncryptionMethod "
    "Algorithm=\"http://www.w3.org/2001/04/xmlenc#aes256-cbc\""
    "/><enc:CipherData><enc:CipherValue>pqcG3BI/"
    "85LYhUUcszYkPmLm2FJO3wx3DqeqfdAb8zafGrvx7Ge0xFdW0o1aAeUQ</"
    "enc:CipherValue></enc:CipherData></"
    "pskc:EncryptedValue></pskc:Secret></Data></"
    "ContentKey><ContentKey "
    "kid=\"e17ba4b8-faff-4d30-bcba-7485e3f2e884\"><Data><pskc:Secret><pskc:"
    "EncryptedValue><enc:EncryptionMethod "
    "Algorithm=\"http://www.w3.org/2001/04/xmlenc#aes256-cbc\""
    "/><enc:CipherData><enc:CipherValue>"
    "srgTGeYWvXPshuw1stNJkAuyEeJpKIiWZQIfPDtiM00cGIZQiwTbbbkRxZKPmgb2</"
    "enc:CipherValue></enc:CipherData></"
    "pskc:EncryptedValue></pskc:Secret></Data></ContentKey><ContentKey "
    "kid=\"0ae6b9ad-92d2-4ebe-882b-1d07dee70715\"><Data><pskc:Secret><pskc:"
    "EncryptedValue><enc:EncryptionMethod "
    "Algorithm=\"http://www.w3.org/2001/04/xmlenc#aes256-cbc\""
    "/><enc:CipherData><enc:CipherValue>/"
    "pFqJhrBAqXM43LDclPdxGpkoO3WLXfhb4Uccm1ssD3ttpDEOedl7c2s+0RrSlDH</"
    "enc:CipherValue></enc:CipherData></"
    "pskc:EncryptedValue></pskc:Secret></Data></"
    "ContentKey></ContentKeyList></CPIX>";

constexpr char kCpixDocumentContentId[] =
    "<CPIX xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\" "
    "xmlns:xsd=\"http://www.w3.org/2001/XMLSchema\" "
    "xmlns=\"urn:dashif:org:cpix\" "
    "xmlns:ds=\"http://www.w3.org/2000/09/xmldsig#\" "
    "xmlns:enc=\"http://www.w3.org/2001/04/xmlenc#\" "
    "xmlns:pskc=\"urn:ietf:params:xml:ns:keyprov:pskc\" "
    "contentId=\"encryptedvideo\"/>";

constexpr char kGoodPrivateKey[] =
    "-----BEGIN RSA PRIVATE KEY-----\n"
    "MIIEpQIBAAKCAQEAuNM+5+zE5c7P7ILBIscsRBrCLB7Ag3o0z7ghz5X34OmIssjt\n"
    "P/cpkRqm4k4tLTD9iAmOtpsjOJDPN4CN2ezZGqEBk80rb8rMj78HuqlEtO5Y2rjl\n"
    "/5Am+6nJ+jvHLK4bpjS1+tgaLu0b929yNh/HExrgVrDlowcnHhKq6oEH+RHRIQWM\n"
    "dZ1cdy02d1NCRynPl0eepEkvOGgHvHM7+V6cOUdeDgyV3eL9h8oPBE8/9Lll5DGR\n"
    "ERueuNfSQeGgqyCGE9IEUMaKUfrZwhCzUvCbzU9jsSeWdBsF22E+WLwUhP4a2AWf\n"
    "wNJIIWm1SWbWZcAGhJQ90KzA4uE2j16ooByikwIDAQABAoIBAQCU2HNBvjl3xY1H\n"
    "sKmvw21Pz6judTme6M0dN/HqxqDg/dI8AXqn1/oexLL/b+wMB/c2FlRblSbRSDXJ\n"
    "Qft7NLZHLsEhccr4df82CKdF2Une2FCwnNWFcmcip8Nkhb3KqtDTkoG36U902Tl7\n"
    "lMIK+p6NMUjYKBBXscv7+RayUoXEPAfqmAN1Vo3P+MLaH5kOsdG7YyfHx148c7Fh\n"
    "t5EmF23k5koGNb7pPflLsFI/lVUCJ97Qr9YS0lz2ykvlJEQOMBvajKMkIgCWQyYX\n"
    "qeb2q61b7LtUYw3a8dK1X37pplf1LhQCmM5FmVfMkdLflrzFx+xYIZCcm7UYeHID\n"
    "1NoNLhs5AoGBAPB+uOLolPu6A/F/GYeBrtsXesLGJda+FC+VMeAjRDlePt61XNI2\n"
    "LPVGRsPg+w/TWXkd4SF3JtQ2phetniEETq1wQEgyASUOMby9e/LCYwz/ZIxF2tKM\n"
    "MJ9IEs6GnIi4xaWK46fQc9If1E9lTONaKg4o2v/bFMBZ/a8iN84JyJqtAoGBAMS9\n"
    "tt8fPRsjRPOawEFDI/wuZ+49zVQLobfzD2yjy++FVwzekGBs57tZcRrRkoNRRK4l\n"
    "9NPMTSgslolYZ2fBnNN/yQ9nJfRA8wsgKRlc/qtZ2fI9yVyMZ4iJfPhkqGhC1Rd5\n"
    "+cTXvFCFyc35W5aagnTR1OU+k45CEzEReYjaTRo/AoGBAKh5rd3P32bFNjP8vQ6v\n"
    "q6XyOd6u3x0jxZJb6zfAnO+vdU2hMehVaYNd+v8H4fmBLuHpK6i9BueNbSeTSf6J\n"
    "PPDFfVLWLPUfj3pQJ7+1q5wUYgTefuAWs40x+XLS3F/vSQ6iUL9kCW8/OwVvysBO\n"
    "jr2io2CvwkbKBCynRP7C3dR1AoGBAKVSgjwGBRQr1AIcdReiMrfzyvPgzQyl5ZQs\n"
    "1DZHgdRZKOgwkgPlH+lH5IpTtxlQDjWEwbHEiMWMikcJmYMPqBf11l9yHuucNF+R\n"
    "+9dLVNlqXIF5uB5kAZAzcGdONlG+b1AV+HOXka/cbNL/ih4I4j0GIi8tIVM2ZOgj\n"
    "tgvw3T+DAoGARLV3dSNKlJC8DIBZiElIH55uhQgKgfX0O/p2eAhHMHMWSpLrjmf7\n"
    "rxRmmFFe09h5Jmq7Ydg2ibwTLvajFxxvmoba6iakJooCI0BE96ndJLhiuDYTLMk8\n"
    "pc+jUtwSabS/PO7yQUklfzfhcFBdXQ5lWL4ZtDx6puLzGxWTMQQCZZE=\n"
    "-----END RSA PRIVATE KEY-----\n";

constexpr char kGoodCertificate[] =
    "-----BEGIN CERTIFICATE-----\n"
    "MIIDbTCCAlWgAwIBAgIUE7QH13Q/DZw1zw0khFqaYzMFlWAwDQYJKoZIhvcNAQEL\n"
    "BQAwRjELMAkGA1UEBhMCdXMxEzARBgNVBAgMCndhc2hpbmd0b24xETAPBgNVBAcM\n"
    "CGJlbGxldnVlMQ8wDQYDVQQKDAZnb29nbGUwHhcNMTkwODEyMTgyMzA4WhcNMTkw\n"
    "OTExMTgyMzA4WjBGMQswCQYDVQQGEwJ1czETMBEGA1UECAwKd2FzaGluZ3RvbjER\n"
    "MA8GA1UEBwwIYmVsbGV2dWUxDzANBgNVBAoMBmdvb2dsZTCCASIwDQYJKoZIhvcN\n"
    "AQEBBQADggEPADCCAQoCggEBALjTPufsxOXOz+yCwSLHLEQawiwewIN6NM+4Ic+V\n"
    "9+DpiLLI7T/3KZEapuJOLS0w/YgJjrabIziQzzeAjdns2RqhAZPNK2/KzI+/B7qp\n"
    "RLTuWNq45f+QJvupyfo7xyyuG6Y0tfrYGi7tG/dvcjYfxxMa4Faw5aMHJx4SquqB\n"
    "B/kR0SEFjHWdXHctNndTQkcpz5dHnqRJLzhoB7xzO/lenDlHXg4Mld3i/YfKDwRP\n"
    "P/S5ZeQxkREbnrjX0kHhoKsghhPSBFDGilH62cIQs1Lwm81PY7EnlnQbBdthPli8\n"
    "FIT+GtgFn8DSSCFptUlm1mXABoSUPdCswOLhNo9eqKAcopMCAwEAAaNTMFEwHQYD\n"
    "VR0OBBYEFOkLvUZ0osJyzjFSMNpciLzCkT4dMB8GA1UdIwQYMBaAFOkLvUZ0osJy\n"
    "zjFSMNpciLzCkT4dMA8GA1UdEwEB/wQFMAMBAf8wDQYJKoZIhvcNAQELBQADggEB\n"
    "AFsBSWVIMxj/GrhvKNjqOc8jflEHz9BxXIXl89UMkqSK8Q4tGnBHOBE9iJJYejff\n"
    "ylV3vBEXUvteeVtp+TJW6Pkbxk0vRHQ3zcB+QZ3Oam8rN2Cz7I9T3huqT65EkVcL\n"
    "/x9t495gNaMjxL/M723+cr2kjMu0T3h6AeCM8/a/XPhCpq7ct8BE4SWuKvveo0c5\n"
    "cKmbju9nEtcNXjPLQQIFYVJxI1LmlVVOZyOx0PvQuzRbctfq1FsbY3dgCGhb2gWc\n"
    "9lD5Lcg3PCBiByAnzn4gUVcU16+vKMR7MDlTxv2Ju3i+M2FAlTZOLYjOvPd0zQIP\n"
    "XG6H65F0AuJUd5SNIGJGu0s=\n"
    "-----END CERTIFICATE-----\n";

constexpr char kGoodDashedKID[] = "bd5adf51-cf04-410f-aac3-ec63a69e929e";

constexpr char kGoodKeyValue[] = "3iv9lYwafpe0uEmxDc6PSw==";

}  // namespace

class CPIXMessageTest : public ::testing::Test {
 public:
  void SerializeSetups() {
    std::unique_ptr<MockRecipientList> recipient_list =
        absl::make_unique<MockRecipientList>();
    std::unique_ptr<MockContentKeyList> key_list =
        absl::make_unique<MockContentKeyList>();
    std::unique_ptr<MockDRMSystemList> drm_list =
        absl::make_unique<MockDRMSystemList>();
    std::unique_ptr<MockUsageRuleList> rule_list =
        absl::make_unique<MockUsageRuleList>();
    std::unique_ptr<MockKeyPeriodList> key_period_list =
        absl::make_unique<MockKeyPeriodList>();

    EXPECT_CALL(*recipient_list, GetNode()).Times(1);
    EXPECT_CALL(*key_list, GetNode()).Times(1);
    EXPECT_CALL(*drm_list, GetNode()).Times(1);
    EXPECT_CALL(*rule_list, GetNode()).Times(1);
    EXPECT_CALL(*key_period_list, GetNode()).Times(1);
    message.InjectRecipientListForTest(std::move(recipient_list));
    message.InjectContentKeyListForTest(std::move(key_list));
    message.InjectDRMSystemListForTest(std::move(drm_list));
    message.InjectUsageRuleListForTest(std::move(rule_list));
    message.InjectKeyPeriodListForTest(std::move(key_period_list));
  }

  CPIXMessage message;
};

TEST_F(CPIXMessageTest, SerializeMessage) {
  SerializeSetups();
  std::string xml = message.ToString();
  EXPECT_TRUE(CPIXMessage::ValidateXML(xml, GetCpixSchema()));
  EXPECT_EQ(xml, kCpixDocument);
}

TEST_F(CPIXMessageTest, SerializeMessageWithContentId) {
  SerializeSetups();
  message.set_content_id("encryptedvideo");
  std::string xml = message.ToString();
  EXPECT_TRUE(CPIXMessage::ValidateXML(xml, GetCpixSchema()));
  EXPECT_EQ(xml, kCpixDocumentContentId);
}

TEST_F(CPIXMessageTest, LoadEmptyDocument) {
  EXPECT_TRUE(message.FromString(kCpixDocument));
}

TEST_F(CPIXMessageTest, FullDeserialize) {
  EXPECT_TRUE(message.FromString(kFullCpix));
  EXPECT_EQ(message.ToString(), kFullCpix);
}

TEST_F(CPIXMessageTest, Decrypt) {
  std::unique_ptr<Recipient> recipient = absl::make_unique<Recipient>();
  recipient->set_delivery_key(
      Base64StringToBytes(StripPEMHeadersAndNewlines(kGoodCertificate)));
  message.AddRecipient(std::move(recipient));
  std::unique_ptr<ContentKey> key = absl::make_unique<ContentKey>();
  key->SetKeyValue(Base64StringToBytes(kGoodKeyValue));
  key->set_key_id(GUIDStringToBytes(kGoodDashedKID));
  message.AddContentKey(std::move(key));
  std::string xml = message.ToString();
  ASSERT_TRUE(message.FromString(xml));
  const std::vector<uint8_t>& key_value =
      message.FindContentKeyById(GUIDStringToBytes(kGoodDashedKID))
          ->key_value();
  ASSERT_FALSE(key_value.empty());
  EXPECT_NE(key_value, Base64StringToBytes(kGoodKeyValue));
  EXPECT_TRUE(message.DecryptWith(
      Base64StringToBytes(StripPEMHeadersAndNewlines(kGoodPrivateKey))));
  EXPECT_EQ(message.FindContentKeyById(GUIDStringToBytes(kGoodDashedKID))
                ->key_value(),
            Base64StringToBytes(kGoodKeyValue));
}

}  // namespace cpix
