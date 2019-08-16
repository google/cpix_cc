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

#include "content_key.h"
#include "cpix_message.h"
#include "key_period.h"
#include "usage_rule.h"

int main() {
  const std::vector<uint8_t> key_id = {
      0xe3, 0x47, 0x74, 0xd9, 0xd7, 0x75, 0xeb, 0x56, 0xb7, 0xe3, 0xbf, 0x3b,
      0x6b, 0x5e, 0x79, 0xe7, 0xbd, 0xb7, 0xdb, 0x97, 0x78, 0xed, 0xbf, 0x34};

  const std::vector<uint8_t> content_key_value = {
      0x80, 0xfc, 0x6d, 0xd0, 0xf3, 0x30, 0xac, 0x73,
      0x38, 0x4d, 0xd8, 0xf0, 0x75, 0x09, 0xa1, 0x85};

  cpix::CPIXMessage message;

  std::unique_ptr<cpix::KeyPeriod> key_period(new cpix::KeyPeriod);
  key_period->SetIndex(1001);

  key_period->set_id("key_period_1");

  message.AddKeyPeriod(std::move(key_period));

  std::unique_ptr<cpix::ContentKey> key(new cpix::ContentKey);
  key->set_key_id(key_id);
  key->SetKeyValue(content_key_value);

  std::unique_ptr<cpix::UsageRule> rule(new cpix::UsageRule);

  rule->AddKeyPeriodFilter("key_period_1");

  rule->set_key_id(key->key_id());

  message.AddContentKey(std::move(key));
  message.AddUsageRule(std::move(rule));

  printf("Usage Rules with Key Period:\n\n%s\n\n", message.ToString().c_str());
}
