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

#include "rsa_public_key.h"

#include <memory>

#include "cpix_util.h"
#include "gtest/gtest.h"
#include "rsa_private_key.h"

namespace cpix {
namespace {

constexpr char kGoodPubKey[] =
    "-----BEGIN PUBLIC KEY-----\n"
    "MIIBIjANBgkqhkiG9w0BAQEFAAOCAQ8AMIIBCgKCAQEAuJjJZIYFzeJMmJ6BPi8B\n"
    "vFuA/jKLh8U1ICbqmIY2TS+FRuJxdCO9iAVazKEewVqWWkGNbnK673U8yF+pSeDu\n"
    "vBxGelF6IQL3wNOchXcSpyWBqzFcw6+tBPzgF9FE8D/77Il87wyRjYmJ1WKqNL2V\n"
    "GqoB4iOldDn1sKc+toMp3LzZHAAGdtqSxz6C6yaVCqvp1Hq0v+CFqTlUFPPPQzR8\n"
    "JxRcci4SgSZ5ShrDCJiDYXglUGgHu3c6PH6gvgB3Gb3HrkDNJrJ58CvrMjGZr/ZX\n"
    "vkb4CDVVjv2nV5tJss3RC8MIWLxHJbV1JLABUY36OXI5JY67K7ZTBB3FqFLzyGSa\n"
    "TwIDAQAB\n"
    "-----END PUBLIC KEY-----\n";

constexpr char kMatchPrivKey[] =
    "-----BEGIN PRIVATE KEY-----\n"
    "MIIEvgIBADANBgkqhkiG9w0BAQEFAASCBKgwggSkAgEAAoIBAQC4mMlkhgXN4kyY\n"
    "noE+LwG8W4D+MouHxTUgJuqYhjZNL4VG4nF0I72IBVrMoR7BWpZaQY1ucrrvdTzI\n"
    "X6lJ4O68HEZ6UXohAvfA05yFdxKnJYGrMVzDr60E/OAX0UTwP/vsiXzvDJGNiYnV\n"
    "Yqo0vZUaqgHiI6V0OfWwpz62gyncvNkcAAZ22pLHPoLrJpUKq+nUerS/4IWpOVQU\n"
    "889DNHwnFFxyLhKBJnlKGsMImINheCVQaAe7dzo8fqC+AHcZvceuQM0msnnwK+sy\n"
    "MZmv9le+RvgINVWO/adXm0myzdELwwhYvEcltXUksAFRjfo5cjkljrsrtlMEHcWo\n"
    "UvPIZJpPAgMBAAECggEBAIJvUz64hLlpwf3FYkVyKaPSdF8pj/YrntJNLU8A5rW7\n"
    "ck0YDFqoNnA6eGJHJtgxDjRQRYNZhkVVbdGX5qLRUV+SCyq7D7Trx+x0mhCkhoWc\n"
    "tvckNgv/wh3YIPAzaqboTzZgH+s50k9UdBRXVzVVEFp67kHANlTd74EXa/TC0X2q\n"
    "r7ZfaHh+oLReCJTUmEcYbJjm7hR0fpG/HFqaLI0deVoszJtyrU2RRqqgLrNw57bI\n"
    "EcbI1HrmBhte+9X8CaWBJY6OCmlcQlA/MgbFPKFQSaLfHJbj0u6sHb2U+/JqzFG0\n"
    "GojUujIgspquBYuV1X4NJskvGj0fPeN3weU5ZQGlQsECgYEA4nTor424a1MPdCGP\n"
    "AKWQWx6wr73ASvT9vBVhzQ3NRWlRExEO9feTue2EVquCMuKAgvlLAmENpKb34AxD\n"
    "tZIS3D0uphEaYuMUNmi/DyQAF2hbdK/VS7jqtbnfB07GslvDd83jVJk6vsCXDmYg\n"
    "keusTIYWvAwjuN2+sToIKpsYn0kCgYEA0K3c0DMxtwQI3Rs0OH7ueJyxly1olntf\n"
    "GGXfUdc8Us3qWDc4whg7F04LMZcAtwM/Uh8QqfZh4izWm0B/kmYmvAiRaBPEHlLO\n"
    "UsWl6Z4iMf0zAbsg8MTD0mfU1x/zDGW9u8SLOe9rDXPcRSX2ojIeFV0qAgTUgPh0\n"
    "w9L/b2d8NNcCgYBrqrfil5fh9xi21ym9Xfo3SG0q8SWWvcpRfT0FmBVLdhok3Yxm\n"
    "Z3JmCMZNyrdZaBU/sDT4iZpJ+VSKwmoCztOGwNQIAsOv1lUK1VxZX31no6cv4FWh\n"
    "7eYxxpCCUfLoY/aYZFw8IaorfTeUkfvURN+jdnEisQbe4xbB/lxZnWnkeQKBgFeb\n"
    "xop6E7McNIRvOVtGnuNwFXa8j54tqVza2AjJ7f9th6cFeAS0m4pLL5PgK2KqMFlP\n"
    "xXU1hR6paGfBik1yKWzjw/gH5Fw+s3Q4mlEcHIsUG+fsIgYy9WtOQCMYvLdFt4Qv\n"
    "HaxzAWSicaVuLL8nf03E0TALyxNiO9NCsA6bSZ/9AoGBAK+BtdkkfS5ORxgGa1t0\n"
    "r34Q1/TZZcULkZcKNxh7bX2kN/dMw7YqO091oqaNqqMjhyNvPKOqDq/7l4ua4EQt\n"
    "VsWPrLXJr1rz3tX8ku34tPNYobfwswwxREnJ83qVlh4psSEVUSuye7ePYZHGHxER\n"
    "era2Yj30oJ7K4qv2ovDQptw5\n"
    "-----END PRIVATE KEY-----\n";

constexpr char kNoMatchPrivKey[] =
    "-----BEGIN PRIVATE KEY-----\n"
    "MIIEvAIBADANBgkqhkiG9w0BAQEFAASCBKYwggSiAgEAAoIBAQDFCWrkOfOguHuB\n"
    "5/exwm6GA8rGy8IEMq/xEaB3xsn6Lc5NIfvRB183XlMEi6K1PazIvR4n0aG/L/1v\n"
    "lvkrRy1nh5TKhs84THHWgde6ligRzRcr7moU4BaHtFbOE+redd7DH/kbBW3xRT1N\n"
    "dix5DArmFbkGvy5t4PTdsUL92cu9Nei3W4heUyieuL0+/hHKLCEJ+DUbNXGNuDHT\n"
    "VIGwdYGxlCfmFDmQeLokdnCMTvGqctYaui3qXQ0f6iamoHHZryS6F05qQMHEZMzi\n"
    "YFBZwU7hQJlECseH5+Bf/Z/JhcyY4Yw1vTyYS/PJ24zLK5R2XXK1h007hMgNsz0A\n"
    "kEwtB4K5AgMBAAECggEAcW0VcqKFxCzPNJ4zscnq1A0Uy1H6s232aMEr7ieql/GM\n"
    "hEye6NrkjjwncENt/AWwFLMBzAxan3Ph4j433BkSYznihqAKZC9ljQWcCZZtJKo6\n"
    "v5h0yuWhMqe3HNTMbRnPjeY3Ab4jZzPjsVbpDRnIoGE67U5bTPemHoTvurCJH7cI\n"
    "3malRiDft3HfJPM6qi4NIkqKlr6PJszhA8uTVgc+oGTK/KHxc7eSaeUKTsTPS+8G\n"
    "bKm8PLNOvgg18i1+//nHQgiEg2N3vSzbn+z9shQyaWBkMsp82e8SninS/e5XMumD\n"
    "I5Cku+3te/tn9XEkrkGx7+xVBESWlC9JxOya1PsNjQKBgQDuj/Wml06ybSpeaj06\n"
    "8sxbbdugVjBhx8bJyNYXeFyMb3rclqXPiWmr2F+fc9O2Aq9tvoaO4KJWauebs3zY\n"
    "BuRr9EvTJGiwhEhrkQNZCX5XDNol8oB4Yvk8Z/JoKES6P2G5K7CHBQ48BUBj9ZcW\n"
    "MbbEuwOUneBDUFB3aA8wi3P6twKBgQDTcGvH5FSGo8KrdkLqMqRprt6FW4slRjlx\n"
    "iUdQpZztMwnqWthbZm/Z9q0B930xkR/YZ+0SdvlCoWSiq2fUK2SR5HlaEJWDcNdG\n"
    "7GA0ksgOrf6+DAfiv5VG/bviypOAjXxWE0XSUmL+vTXmBj7g69GAjTeZzh1kK6VT\n"
    "Q4vS9Bi+DwKBgAolHxo1hwg5cjrPo8j0+FHPCy73W1ldKIv+OXSIDs9+Eih9U5HO\n"
    "0yqWqgJa557u4Ez3dDtxPBYWt8C8UkvRhzc8DVluS/FtvrKsWQsZR0Emb8R7WpFE\n"
    "DDtQZFOfmsSc22gHfptPULqmsoVNtV4hQRd+qHlSeW4DZViemT7EP2tjAoGAVzPV\n"
    "b+KOrdzaS8cBf/+U6XVSluOTdrMIu/enLu6u0vEq1LnU45VS6i0fW2FMA+ZboO/z\n"
    "yZrB4KYZkDFrKAkheD4ngmq2jzg5wSl54BTtZ4iBOLZThkQHd/jrxXRQ80LnFcTr\n"
    "FfwekftNQGoxAqifhgo39ToZyxdpHvcLstwzXCcCgYBkdpQm426QCTLpoAHtqpKI\n"
    "K3NIWvQf5s5/I31KvU2NzzaEqmquDhxJHq28jFuE3/hAh6KTgoyinBVCNSqQGQn8\n"
    "LPLyq8BtHc+wfXAwGrz8AzYJ2C4bE2ucTz74li8xtwiWYDyutD+9M5nmEufgIdJv\n"
    "PGldfkAdOD/CA1W4EkEHsw==\n"
    "-----END PRIVATE KEY-----\n";

constexpr char kStringToEncrypt[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";

TEST(RSAPublicKeyTest, RSAEncryptPEM) {
  std::unique_ptr<RSAPublicKey> key = RSAPublicKey::CreateFromPEM(kGoodPubKey);
  ASSERT_TRUE(key);
  EXPECT_NE(key->Encrypt(Base64StringToBytes(kStringToEncrypt)).size(), 0);
}

TEST(RSAPublicKeyTest, RSAEncryptDER) {
  std::unique_ptr<RSAPublicKey> key = RSAPublicKey::CreateFromDER(
      Base64StringToBytes(StripPEMHeadersAndNewlines(kGoodPubKey)));
  ASSERT_TRUE(key);
  EXPECT_NE(key->Encrypt(Base64StringToBytes(kStringToEncrypt)).size(), 0);
}

TEST(RSAPublicKeyTest, MatchesKeySuccessTest) {
  std::unique_ptr<RSAPublicKey> pubkey =
      RSAPublicKey::CreateFromPEM(kGoodPubKey);
  std::unique_ptr<RSAPrivateKey> privkey =
      RSAPrivateKey::CreateFromPEM(kMatchPrivKey);
  ASSERT_TRUE(pubkey);
  ASSERT_TRUE(privkey);
  EXPECT_TRUE(pubkey->MatchesKey(privkey->rsa_key()));
}

TEST(RSAPublicKeyTest, MatchesKeyFailureTest) {
  std::unique_ptr<RSAPublicKey> pubkey =
      RSAPublicKey::CreateFromPEM(kGoodPubKey);
  std::unique_ptr<RSAPrivateKey> privkey =
      RSAPrivateKey::CreateFromPEM(kNoMatchPrivKey);
  ASSERT_TRUE(pubkey);
  ASSERT_TRUE(privkey);
  EXPECT_FALSE(pubkey->MatchesKey(privkey->rsa_key()));
}

}  // namespace
}  // namespace cpix
