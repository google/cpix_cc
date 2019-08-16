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
#include "drm_system.h"

int main() {
  const std::vector<uint8_t> key_id = {
      0xe3, 0x47, 0x74, 0xd9, 0xd7, 0x75, 0xeb, 0x56, 0xb7, 0xe3, 0xbf, 0x3b,
      0x6b, 0x5e, 0x79, 0xe7, 0xbd, 0xb7, 0xdb, 0x97, 0x78, 0xed, 0xbf, 0x34};

  const std::vector<uint8_t> content_key_value = {
      0x80, 0xfc, 0x6d, 0xd0, 0xf3, 0x30, 0xac, 0x73,
      0x38, 0x4d, 0xd8, 0xf0, 0x75, 0x09, 0xa1, 0x85};

  const std::vector<uint8_t> drm_system_id = {
      0xd1, 0xad, 0xf4, 0x79, 0xae, 0x1f, 0xe7, 0x7f, 0x5d, 0xe1, 0xbd, 0x36,
      0xf7, 0x86, 0xf6, 0xd9, 0xbd, 0xdf, 0x6d, 0xad, 0xb9, 0xef, 0xa7, 0x77};

  const std::vector<uint8_t> example_drm_system_data = {
      0xc7, 0xf8, 0x1a, 0xa1, 0x2f, 0xdf, 0x0e, 0x2f,
      0x01, 0xa8, 0x63, 0x48, 0x86, 0x48, 0xb1, 0xc1};

  cpix::CPIXMessage message;

  std::unique_ptr<cpix::ContentKey> key1(new cpix::ContentKey);
  key1->set_key_id(key_id);
  key1->SetKeyValue(content_key_value);

  std::unique_ptr<cpix::DRMSystem> drm(new cpix::DRMSystem);
  drm->set_system_id(drm_system_id);

  drm->set_content_protection_data(example_drm_system_data);
  drm->set_hls_signaling_master(example_drm_system_data);
  drm->set_hls_signaling_media(example_drm_system_data);

  drm->set_key_id(key1->key_id());

  message.AddContentKey(std::move(key1));
  message.AddDRMSystem(std::move(drm));

  printf("Content Key not encrypted:\n\n%s\n\n", message.ToString().c_str());
}
