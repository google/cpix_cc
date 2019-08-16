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

#ifndef CPIX_CC_DRM_SYSTEM_H_
#define CPIX_CC_DRM_SYSTEM_H_

#include <cstdint>
#include <memory>
#include <string>
#include <vector>

#include "cpix_element.h"

namespace cpix {

class XMLNode;

// A core element of the CPIX document. Contains information directly related to
// a DRM system. One DRMSystem object maps to one ContentKey through |kid_|.

class DRMSystem : public CPIXElement {
 public:
  DRMSystem() = default;
  ~DRMSystem();

  const std::vector<uint8_t>& kid() const { return kid_; }
  const std::vector<uint8_t>& system_id() const { return system_id_; }
  const std::vector<uint8_t>& content_protection_data() const {
    return content_protection_data_;
  }
  const std::vector<uint8_t>& pssh() const { return pssh_; }
  const std::vector<uint8_t>& hls_signaling_master() const {
    return hls_signaling_master_;
  }
  const std::vector<uint8_t>& hls_signaling_media() const {
    return hls_signaling_media_;
  }
  const std::vector<uint8_t>& hds_signaling_data() const {
    return hds_signaling_data_;
  }
  const std::vector<uint8_t>& smooth_streaming_data() const {
    return smooth_streaming_data_;
  }
  const std::vector<uint8_t>& uri_ext_x_key() const { return uri_ext_x_key_; }

  void set_key_id(const std::vector<uint8_t>& kid) { kid_ = kid; }

  void set_system_id(const std::vector<uint8_t>& system_id) {
    system_id_ = system_id;
  }
  void set_content_protection_data(const std::vector<uint8_t>& data) {
    content_protection_data_ = data;
  }
  void set_pssh(const std::vector<uint8_t>& pssh) { pssh_ = pssh; }
  void set_hls_signaling_master(const std::vector<uint8_t>& hls) {
    hls_signaling_master_ = hls;
  }
  void set_hls_signaling_media(const std::vector<uint8_t>& hls) {
    hls_signaling_media_ = hls;
  }
  void set_smooth_streaming_data(const std::vector<uint8_t>& data) {
    smooth_streaming_data_ = data;
  }
  void set_uri_ext_x_key(const std::vector<uint8_t>& key) {
    uri_ext_x_key_ = key;
  }

  void set_hds_ignaling_data(const std::vector<uint8_t>& data) {
    hds_signaling_data_ = data;
  }

 protected:
  bool Deserialize(std::unique_ptr<XMLNode> node) override;

 private:
  friend class DRMSystemList;
  std::unique_ptr<XMLNode> GetNode() override;

  std::vector<uint8_t> kid_;
  std::vector<uint8_t> system_id_;
  std::vector<uint8_t> content_protection_data_;
  std::vector<uint8_t> pssh_;
  std::vector<uint8_t> hls_signaling_master_;
  std::vector<uint8_t> hls_signaling_media_;
  std::vector<uint8_t> smooth_streaming_data_;
  std::vector<uint8_t> uri_ext_x_key_;
  std::vector<uint8_t> hds_signaling_data_;
};  // namespace cpix
}  // namespace cpix

#endif  // CPIX_CC_DRM_SYSTEM_H_
