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

#include "usage_rule.h"

#include <memory>
#include <string>
#include <utility>

#include "absl/memory/memory.h"
#include "cpix_util.h"
#include "glog/logging.h"
#include "xml_node.h"

namespace cpix {
UsageRule::~UsageRule() = default;

bool UsageRule::AddLabelFilter(const std::string& label) {
  label_filters_.push_back(label);
  return true;
}

bool UsageRule::AddVideoFilter(const VideoFilter& filter) {
  if ((filter.min_pixels > filter.max_pixels && filter.max_pixels != -1) ||
      (filter.min_fps > filter.max_fps && filter.max_fps != -1)) {
    LOG(WARNING) << "Invalid video filter parameters. Filter not added.";
    return false;
  }
  video_filters_.push_back(filter);
  return true;
}

bool UsageRule::AddAudioFilter(const AudioFilter& filter) {
  if (filter.min_channels > filter.max_channels && filter.max_channels != -1) {
    LOG(WARNING) << "Invalid audio filter parameters. Filter not added.";
    return false;
  }
  audio_filters_.push_back(filter);
  return true;
}

bool UsageRule::AddBitrateFilter(const BitrateFilter& filter) {
  if (filter.min_bitrate > filter.max_bitrate && filter.max_bitrate != -1) {
    LOG(WARNING) << "Invalid bitrate filter parameters. Filter not added.";
    return false;
  }
  bitrate_filters_.push_back(filter);
  return true;
}

bool UsageRule::AddKeyPeriodFilter(const std::string& id) {
  key_period_filter_ids_.push_back(id);
  return true;
}

std::unique_ptr<XMLNode> UsageRule::GetNode() {
  if (kid_.empty()) {
    return nullptr;
  }

  std::unique_ptr<XMLNode> root =
      absl::make_unique<XMLNode>("", "ContentKeyUsageRule");

  if (!id().empty()) {
    root->AddAttribute("id", id());
  }

  root->AddAttribute("kid", BytesToGUID(kid_));

  if (!intended_track_type_.empty()) {
    root->AddAttribute("intendedTrackType", intended_track_type_);
  }

  for (auto const& filter : key_period_filter_ids_) {
    std::unique_ptr<XMLNode> key_period_id =
        absl::make_unique<XMLNode>("", "KeyPeriodFilter");
    key_period_id->AddAttribute("periodId", filter);
    root->AddChild(std::move(key_period_id));
  }

  for (auto const& filter : label_filters_) {
    std::unique_ptr<XMLNode> label =
        absl::make_unique<XMLNode>("", "LabelFilter");
    label->AddAttribute("label", filter);
    root->AddChild(std::move(label));
  }

  for (auto const& filter : video_filters_) {
    std::unique_ptr<XMLNode> video_filter =
        absl::make_unique<XMLNode>("", "VideoFilter");
    if (filter.min_pixels != -1)
      video_filter->AddAttribute("minPixels",
                                 std::to_string(filter.min_pixels));
    if (filter.max_pixels != -1)
      video_filter->AddAttribute("maxPixels",
                                 std::to_string(filter.max_pixels));
    if (filter.hdr) {
      video_filter->AddAttribute("hdr", "true");
    }
    if (filter.wcg) {
      video_filter->AddAttribute("wcg", "true");
    }
    if (filter.min_fps != -1)
      video_filter->AddAttribute("minFps", std::to_string(filter.min_fps));
    if (filter.max_fps != -1)
      video_filter->AddAttribute("maxFps", std::to_string(filter.max_fps));
    root->AddChild(std::move(video_filter));
  }

  for (auto const& filter : audio_filters_) {
    std::unique_ptr<XMLNode> audio_filter =
        absl::make_unique<XMLNode>("", "AudioFilter");
    if (filter.min_channels != -1)
      audio_filter->AddAttribute("minChannels",
                                 std::to_string(filter.min_channels));
    if (filter.max_channels != -1)
      audio_filter->AddAttribute("maxChannels",
                                 std::to_string(filter.max_channels));
    root->AddChild(std::move(audio_filter));
  }

  for (auto const& filter : bitrate_filters_) {
    std::unique_ptr<XMLNode> bitrate_filter =
        absl::make_unique<XMLNode>("", "BitrateFilter");
    if (filter.min_bitrate != -1)
      bitrate_filter->AddAttribute("minBitrate",
                                   std::to_string(filter.min_bitrate));
    if (filter.max_bitrate != -1)
      bitrate_filter->AddAttribute("maxBitrate",
                                   std::to_string(filter.max_bitrate));
    root->AddChild(std::move(bitrate_filter));
  }

  return root;
}

bool UsageRule::Deserialize(std::unique_ptr<XMLNode> node) {
  std::string attribute;
  if (!(attribute = node->GetAttribute("id")).empty()) {
    set_id(attribute);
  }

  kid_ = GUIDStringToBytes(node->GetAttribute("kid"));

  std::unique_ptr<XMLNode> child;

  if (!(attribute = node->GetAttribute("id")).empty()) {
    set_id(attribute);
  }

  if (!(attribute = node->GetAttribute("intendedTrackType")).empty()) {
    intended_track_type_ = attribute;
  }

  while ((child = node->GetFirstChildByName("KeyPeriodFilter"))) {
    AddKeyPeriodFilter(child->GetAttribute("periodId"));
  }

  while ((child = node->GetFirstChildByName("LabelFilter"))) {
    AddLabelFilter(child->GetAttribute("label"));
  }

  while ((child = node->GetFirstChildByName("VideoFilter"))) {
    VideoFilter filter;
    if (!(attribute = child->GetAttribute("minPixels")).empty()) {
      filter.min_pixels = std::stoi(attribute);
    }

    if (!(attribute = child->GetAttribute("maxPixels")).empty()) {
      filter.max_pixels = std::stoi(attribute);
    }

    if ((attribute = child->GetAttribute("hdr")) == "true") {
      filter.hdr = true;
    }

    if ((attribute = child->GetAttribute("wcg")) == "true") {
      filter.wcg = true;
    }

    if (!(attribute = child->GetAttribute("minFps")).empty()) {
      filter.min_fps = std::stoi(attribute);
    }

    if (!(attribute = child->GetAttribute("maxFps")).empty()) {
      filter.max_fps = std::stoi(attribute);
    }
    AddVideoFilter(filter);
  }

  while ((child = node->GetFirstChildByName("AudioFilter"))) {
    AudioFilter filter;
    if (!(attribute = child->GetAttribute("minChannels")).empty()) {
      filter.min_channels = std::stoi(attribute);
    }

    if (!(attribute = child->GetAttribute("maxChannels")).empty()) {
      filter.max_channels = std::stoi(attribute);
    }
    AddAudioFilter(filter);
  }

  while ((child = node->GetFirstChildByName("BitrateFilter"))) {
    BitrateFilter filter;
    if (!(attribute = child->GetAttribute("minBitrate")).empty()) {
      filter.min_bitrate = std::stoi(attribute);
    }

    if (!(attribute = child->GetAttribute("maxBitrate")).empty()) {
      filter.max_bitrate = std::stoi(attribute);
    }
    AddBitrateFilter(filter);
  }

  return true;
}

}  // namespace cpix
