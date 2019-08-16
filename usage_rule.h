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

#ifndef CPIX_CC_USAGE_RULE_H_
#define CPIX_CC_USAGE_RULE_H_

#include <stdint.h>

#include <limits>
#include <memory>
#include <string>
#include <vector>

#include "cpix_element.h"

namespace cpix {

class XMLNode;

struct VideoFilter {
  int min_pixels = -1;
  int max_pixels = -1;
  bool hdr = false;
  bool wcg = false;
  int min_fps = -1;
  int max_fps = -1;
};

struct AudioFilter {
  int min_channels = -1;
  int max_channels = -1;
};

struct BitrateFilter {
  int min_bitrate = -1;
  int max_bitrate = -1;
};

// A core element of the CPIX document. Contains information mapping a
// ContentKey to a media context through a variety of filters. Each UsageRule
// maps directly to a single ContentKey through |kid_|.

class UsageRule : public CPIXElement {
 public:
  UsageRule() = default;
  ~UsageRule();
  const std::vector<uint8_t>& kid() const { return kid_; }
  void set_key_id(const std::vector<uint8_t>& kid) { kid_ = kid; }

  void SetTrackType(const std::string& trackType) {
    intended_track_type_ = trackType;
  }
  bool AddLabelFilter(const std::string& label);
  bool AddVideoFilter(const VideoFilter& filter);
  bool AddAudioFilter(const AudioFilter& filter);
  bool AddBitrateFilter(const BitrateFilter& filter);

  // Adds a new KeyPeriodFilter element to this UsageRule.
  // It is the supplied |id| should match a pre-existing KeyPeriod in
  // KeyPeriodList.
  bool AddKeyPeriodFilter(const std::string& id);

 protected:
  bool Deserialize(std::unique_ptr<XMLNode> node) override;

 private:
  friend class UsageRuleList;
  std::unique_ptr<XMLNode> GetNode() override;

  std::vector<uint8_t> kid_;
  std::string intended_track_type_;
  std::vector<std::string> label_filters_;
  std::vector<VideoFilter> video_filters_;
  std::vector<AudioFilter> audio_filters_;
  std::vector<BitrateFilter> bitrate_filters_;
  std::vector<std::string> key_period_filter_ids_;
};
}  // namespace cpix

#endif  // CPIX_CC_USAGE_RULE_H_
