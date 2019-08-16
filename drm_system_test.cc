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

#include "drm_system.h"

#include <utility>

#include "absl/memory/memory.h"
#include "cpix_util.h"
#include "gtest/gtest.h"
#include "testable_cpix_element.h"
#include "xml_node.h"

namespace cpix {
namespace {

using ::testing::Test;

constexpr char kGoodKID[] = "bd5adf51cf04410faac3ec63a69e929e";
constexpr char kGoodSystemId[] = "edef8ba979d64acea3c827dcd51d21ed";
constexpr char kGoodContentProtectionData[] =
    "PHBzc2ggeG1sbnM9InVybjptcGVnOmNlbmM6MjAxMyI+"
    "QUFBQU9IQnpjMmdBQUFBQTdlK0xxWG5XU3M2anlDZmMxUjBoN1FBQUFCZ1NFTFRER0l2dDNVVT"
    "ltOEljdktkV1lqbEk0OXlWbXdZPTwvcHNzaD4=";

constexpr char kGoodHLSSignalingMaster[] =
    "I0VYVC1YLVNFU1NJT04tS0VZOk1FVEhPRD1TQU1QTEUtQUVTLUNUUixVUkk9ImRhdGE6dGV4dC"
    "9wbGFpbjtiYXNlNjQsQUFBQU9IQnpjMmdBQUFBQTdlK0xxWG5XU3M2anlDZmMxUjBoN1FBQUFC"
    "Z1NFTFRER0l2dDNVVTltOEljdktkV1lqbEk0OXlWbXdZPSIsS0VZSUQ9MHhCNEMzMTg4QkVERE"
    "Q0NTNEOUJDMjFDQkNBNzU2NjIzOSxLRVlGT1JNQVQ9InVybjp1dWlkOmVkZWY4YmE5LTc5ZDYt"
    "NGFjZS1hM2M4LTI3ZGNkNTFkMjFlZCIsS0VZRk9STUFUVkVSU0lPTlM9IjEi";

constexpr char kGoodHLSSignalingMedia[] =
    "I0VYVC1YLUtFWTpNRVRIT0Q9U0FNUExFLUFFUy1DVFIsVVJJPSJkYXRhOnRleHQvcGxhaW47Ym"
    "FzZTY0LEFBQUFPSEJ6YzJnQUFBQUE3ZStMcVhuV1NzNmp5Q2ZjMVIwaDdRQUFBQmdTRUxUREdJ"
    "dnQzVVU5bThJY3ZLZFdZamxJNDl5Vm13WT0iLEtFWUlEPTB4QjRDMzE4OEJFRERENDUzRDlCQz"
    "IxQ0JDQTc1NjYyMzksS0VZRk9STUFUPSJ1cm46dXVpZDplZGVmOGJhOS03OWQ2LTRhY2UtYTNj"
    "OC0yN2RjZDUxZDIxZWQiLEtFWUZPUk1BVFZFUlNJT05TPSIxIg==";

constexpr char kGoodSmoothStreamingData[] =
    "xAEAAAEAAQC6ATwAVwBSAE0ASABFAEEARABFAFIAIAB4AG0AbABuAHMAPQAiAGgAdAB0AHAAOg"
    "AvAC8AcwBjAGgAZQBtAGEAcwAuAG0AaQBjAHIAbwBzAG8AZgB0AC4AYwBvAG0ALwBEAFIATQAv"
    "ADIAMAAwADcALwAwADMALwBQAGwAYQB5AFIAZQBhAGQAeQBIAGUAYQBkAGUAcgAiACAAdgBlAH"
    "IAcwBpAG8AbgA9ACIANAAuADAALgAwAC4AMAAiAD4APABEAEEAVABBAD4APABQAFIATwBUAEUA"
    "QwBUAEkATgBGAE8APgA8AEsARQBZAEwARQBOAD4AMQA2ADwALwBLAEUAWQBMAEUATgA+"
    "ADwAQQBMAEcASQBEAD4AQQBFAFMAQwBUAFIAPAAvAEEATABHAEkARAA+"
    "ADwALwBQAFIATwBUAEUAQwBUAEkATgBGAE8APgA8AEsASQBEAD4AQwBJADEAcQBTAFAAYwBwAD"
    "kAVQBLAGEAbQBxAEcAcgBtAHcAYQBGAHIAUQA9AD0APAAvAEsASQBEAD4APAAvAEQAQQBUAEEA"
    "PgA8AC8AVwBSAE0ASABFAEEARABFAFIAPgA=";

constexpr char kGoodXML[] =
    "<DRMSystem kid=\"bd5adf51-cf04-410f-aac3-ec63a69e929e\" "
    "systemId=\"edef8ba9-79d6-4ace-a3c8-27dcd51d21ed\"><ContentProtectionData>"
    "PHBzc2ggeG1sbnM9InVybjptcGVnOmNlbmM6MjAxMyI+"
    "QUFBQU9IQnpjMmdBQUFBQTdlK0xxWG5XU3M2anlDZmMxUjBoN1FBQUFCZ1NFTFRER0l2dDNVVT"
    "ltOEljdktkV1lqbEk0OXlWbXdZPTwvcHNzaD4=</"
    "ContentProtectionData><HLSSignalingData "
    "playlist=\"master\">"
    "I0VYVC1YLVNFU1NJT04tS0VZOk1FVEhPRD1TQU1QTEUtQUVTLUNUUixVUkk9ImRhdGE6dGV4dC"
    "9wbGFpbjtiYXNlNjQsQUFBQU9IQnpjMmdBQUFBQTdlK0xxWG5XU3M2anlDZmMxUjBoN1FBQUFC"
    "Z1NFTFRER0l2dDNVVTltOEljdktkV1lqbEk0OXlWbXdZPSIsS0VZSUQ9MHhCNEMzMTg4QkVERE"
    "Q0NTNEOUJDMjFDQkNBNzU2NjIzOSxLRVlGT1JNQVQ9InVybjp1dWlkOmVkZWY4YmE5LTc5ZDYt"
    "NGFjZS1hM2M4LTI3ZGNkNTFkMjFlZCIsS0VZRk9STUFUVkVSU0lPTlM9IjEi</"
    "HLSSignalingData><HLSSignalingData "
    "playlist=\"media\">"
    "I0VYVC1YLUtFWTpNRVRIT0Q9U0FNUExFLUFFUy1DVFIsVVJJPSJkYXRhOnRleHQvcGxhaW47Ym"
    "FzZTY0LEFBQUFPSEJ6YzJnQUFBQUE3ZStMcVhuV1NzNmp5Q2ZjMVIwaDdRQUFBQmdTRUxUREdJ"
    "dnQzVVU5bThJY3ZLZFdZamxJNDl5Vm13WT0iLEtFWUlEPTB4QjRDMzE4OEJFRERENDUzRDlCQz"
    "IxQ0JDQTc1NjYyMzksS0VZRk9STUFUPSJ1cm46dXVpZDplZGVmOGJhOS03OWQ2LTRhY2UtYTNj"
    "OC0yN2RjZDUxZDIxZWQiLEtFWUZPUk1BVFZFUlNJT05TPSIxIg==</"
    "HLSSignalingData><SmoothStreamingProtectionHeaderData>"
    "xAEAAAEAAQC6ATwAVwBSAE0ASABFAEEARABFAFIAIAB4AG0AbABuAHMAPQAiAGgAdAB0AHAAOg"
    "AvAC8AcwBjAGgAZQBtAGEAcwAuAG0AaQBjAHIAbwBzAG8AZgB0AC4AYwBvAG0ALwBEAFIATQAv"
    "ADIAMAAwADcALwAwADMALwBQAGwAYQB5AFIAZQBhAGQAeQBIAGUAYQBkAGUAcgAiACAAdgBlAH"
    "IAcwBpAG8AbgA9ACIANAAuADAALgAwAC4AMAAiAD4APABEAEEAVABBAD4APABQAFIATwBUAEUA"
    "QwBUAEkATgBGAE8APgA8AEsARQBZAEwARQBOAD4AMQA2ADwALwBLAEUAWQBMAEUATgA+"
    "ADwAQQBMAEcASQBEAD4AQQBFAFMAQwBUAFIAPAAvAEEATABHAEkARAA+"
    "ADwALwBQAFIATwBUAEUAQwBUAEkATgBGAE8APgA8AEsASQBEAD4AQwBJADEAcQBTAFAAYwBwAD"
    "kAVQBLAGEAbQBxAEcAcgBtAHcAYQBGAHIAUQA9AD0APAAvAEsASQBEAD4APAAvAEQAQQBUAEEA"
    "PgA8AC8AVwBSAE0ASABFAEEARABFAFIAPgA=</"
    "SmoothStreamingProtectionHeaderData></DRMSystem>";

TEST(DRMSystemTest, SerializeDRMSystem) {
  TestableCPIXElement<DRMSystem> drm;

  drm.set_key_id(HexStringToBytes(kGoodKID));
  drm.set_system_id(HexStringToBytes(kGoodSystemId));
  drm.set_content_protection_data(
      Base64StringToBytes(kGoodContentProtectionData));
  drm.set_hls_signaling_master(Base64StringToBytes(kGoodHLSSignalingMaster));
  drm.set_hls_signaling_media(Base64StringToBytes(kGoodHLSSignalingMedia));
  drm.set_smooth_streaming_data(Base64StringToBytes(kGoodSmoothStreamingData));

  EXPECT_EQ(drm.Serialize(), kGoodXML);
}

TEST(DRMSystemTest, DeserializeDRMSystem) {
  TestableCPIXElement<DRMSystem> drm;
  std::unique_ptr<XMLNode> node = absl::make_unique<XMLNode>(kGoodXML);
  EXPECT_TRUE(drm.Deserialize(std::move(node)));
  EXPECT_EQ(drm.content_protection_data(),
            Base64StringToBytes(kGoodContentProtectionData));
  EXPECT_EQ(drm.hls_signaling_master(),
            Base64StringToBytes(kGoodHLSSignalingMaster));
  EXPECT_EQ(drm.hls_signaling_media(),
            Base64StringToBytes(kGoodHLSSignalingMedia));
  EXPECT_EQ(drm.smooth_streaming_data(),
            Base64StringToBytes(kGoodSmoothStreamingData));
  EXPECT_TRUE(drm.hds_signaling_data().empty());
}

}  // namespace
}  // namespace cpix
