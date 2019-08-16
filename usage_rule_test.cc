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

#include <utility>

#include "absl/memory/memory.h"
#include "cpix_util.h"
#include "gtest/gtest.h"
#include "testable_cpix_element.h"
#include "xml_node.h"

namespace cpix {
namespace {

using ::testing::Test;

constexpr char kGoodKID[] = "bd5adf51-cf04-410f-aac3-ec63a69e929e";

constexpr char kGoodXML[] =
    "<ContentKeyUsageRule "
    "kid=\"bd5adf51-cf04-410f-aac3-ec63a69e929e\" "
    "intendedTrackType=\"UHD\"><KeyPeriodFilter periodId=\"id\"/><LabelFilter "
    "label=\"label1\"/><LabelFilter label=\"label2\"/><VideoFilter "
    "minPixels=\"1\" maxPixels=\"10\" hdr=\"true\" minFps=\"100\" "
    "maxFps=\"1000\"/><AudioFilter minChannels=\"10\" "
    "maxChannels=\"100\"/><BitrateFilter minBitrate=\"100\" "
    "maxBitrate=\"1000\"/></ContentKeyUsageRule>";

constexpr char kGoodXMLVideoSparse[] =
    "<ContentKeyUsageRule kid=\"bd5adf51-cf04-410f-aac3-ec63a69e929e\">"
    "<VideoFilter minPixels=\"10\" maxFps=\"1000\"/></ContentKeyUsageRule>";

constexpr char kGoodXMLAudioBitrateSparse[] =
    "<ContentKeyUsageRule "
    "kid=\"bd5adf51-cf04-410f-aac3-ec63a69e929e\">"
    "<AudioFilter maxChannels=\"1000\"/><BitrateFilter minBitrate=\"100\"/>"
    "</ContentKeyUsageRule>";

TEST(UsageRuleTest, SerializeUsageRuleAllFieldsAndFilterTypes) {
  TestableCPIXElement<UsageRule> rule;

  VideoFilter video_filter;
  video_filter.min_pixels = 1;
  video_filter.max_pixels = 10;
  video_filter.hdr = 1;
  video_filter.wcg = 0;
  video_filter.min_fps = 100;
  video_filter.max_fps = 1000;

  AudioFilter audio_filter;
  audio_filter.min_channels = 10;
  audio_filter.max_channels = 100;

  BitrateFilter bitrate_filter;
  bitrate_filter.min_bitrate = 100;
  bitrate_filter.max_bitrate = 1000;

  rule.set_key_id(GUIDStringToBytes(kGoodKID));
  rule.SetTrackType("UHD");
  EXPECT_TRUE(rule.AddKeyPeriodFilter("id"));
  EXPECT_TRUE(rule.AddLabelFilter("label1"));
  EXPECT_TRUE(rule.AddLabelFilter("label2"));
  EXPECT_TRUE(rule.AddVideoFilter(video_filter));
  EXPECT_TRUE(rule.AddAudioFilter(audio_filter));
  EXPECT_TRUE(rule.AddBitrateFilter(bitrate_filter));
  EXPECT_EQ(rule.Serialize(), kGoodXML);
}

TEST(UsageRuleTest, SerializeUsageRuleVideoSparse) {
  TestableCPIXElement<UsageRule> rule;
  VideoFilter video_filter;
  video_filter.min_pixels = 10;
  video_filter.max_fps = 1000;
  rule.set_key_id(GUIDStringToBytes(kGoodKID));
  EXPECT_TRUE(rule.AddVideoFilter(video_filter));
  EXPECT_EQ(rule.Serialize(), kGoodXMLVideoSparse);
}

TEST(UsageRuleTest, SerializeUsageRuleAudioBitrateSparse) {
  TestableCPIXElement<UsageRule> rule;
  AudioFilter audio_filter;
  audio_filter.max_channels = 1000;
  BitrateFilter bitrate_filter;
  bitrate_filter.min_bitrate = 100;
  rule.set_key_id(GUIDStringToBytes(kGoodKID));
  EXPECT_TRUE(rule.AddAudioFilter(audio_filter));
  EXPECT_TRUE(rule.AddBitrateFilter(bitrate_filter));
  EXPECT_EQ(rule.Serialize(), kGoodXMLAudioBitrateSparse);
}

TEST(UsageRuleTest, DeserializeUsageRuleFull) {
  TestableCPIXElement<UsageRule> rule;
  std::unique_ptr<XMLNode> node = absl::make_unique<XMLNode>(kGoodXML);
  EXPECT_TRUE(rule.Deserialize(std::move(node)));
  EXPECT_EQ(rule.Serialize(), kGoodXML);
}

}  // namespace
}  // namespace cpix
