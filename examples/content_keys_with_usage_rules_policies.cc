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
#include "usage_rule.h"

int main() {
  const std::vector<uint8_t> key_id1 = {
      0xe3, 0x47, 0x74, 0xd9, 0xd7, 0x75, 0xeb, 0x56, 0xb7, 0xe3, 0xbf, 0x3b,
      0x6b, 0x5e, 0x79, 0xe7, 0xbd, 0xb7, 0xdb, 0x97, 0x78, 0xed, 0xbf, 0x34};

  const std::vector<uint8_t> key_id2 = {
      0xd1, 0xad, 0xf4, 0x79, 0xae, 0x1f, 0xe7, 0x7f, 0x5d, 0xe1, 0xbd, 0x36,
      0xf7, 0x86, 0xf6, 0xd9, 0xbd, 0xdf, 0x6d, 0xad, 0xb9, 0xef, 0xa7, 0x77};

  const std::vector<uint8_t> content_key_value1 = {
      0x80, 0xfc, 0x6d, 0xd0, 0xf3, 0x30, 0xac, 0x73,
      0x38, 0x4d, 0xd8, 0xf0, 0x75, 0x09, 0xa1, 0x85};

  const std::vector<uint8_t> content_key_value2 = {
      0xc7, 0xf8, 0x1a, 0xa1, 0x2f, 0xdf, 0x0e, 0x2f,
      0x01, 0xa8, 0x63, 0x48, 0x86, 0x48, 0xb1, 0xc1};

  cpix::CPIXMessage message;

  std::unique_ptr<cpix::ContentKey> sd_key(new cpix::ContentKey);
  sd_key->set_key_id(key_id1);
  sd_key->SetKeyValue(content_key_value1);

  std::unique_ptr<cpix::ContentKey> hd_key(new cpix::ContentKey);
  hd_key->set_key_id(key_id2);
  hd_key->SetKeyValue(content_key_value2);

  std::unique_ptr<cpix::UsageRule> sd_rule(new cpix::UsageRule);

  cpix::VideoFilter sd_video_filter;
  sd_video_filter.min_pixels = 0;
  sd_video_filter.max_pixels = 768 * 576;

  sd_rule->AddVideoFilter(sd_video_filter);

  sd_rule->set_key_id(sd_key->key_id());

  message.AddContentKey(std::move(sd_key));
  message.AddUsageRule(std::move(sd_rule));

  std::unique_ptr<cpix::UsageRule> hd_rule(new cpix::UsageRule);

  cpix::VideoFilter hd_video_filter;
  hd_video_filter.min_pixels = 768 * 576 + 1;
  hd_video_filter.max_pixels = 1920 * 1080;

  hd_rule->AddVideoFilter(hd_video_filter);

  hd_rule->set_key_id(hd_key->key_id());

  message.AddContentKey(std::move(hd_key));
  message.AddUsageRule(std::move(hd_rule));

  printf("Usage Rules with Policies:\n\n%s\n\n", message.ToString().c_str());
}
