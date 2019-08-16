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

#include <memory>
#include <utility>
#include <vector>

#include "absl/memory/memory.h"
#include "cpix_util.h"
#include "xml_node.h"

namespace cpix {
DRMSystem::~DRMSystem() = default;

std::unique_ptr<XMLNode> DRMSystem::GetNode() {
  if (kid_.empty() || system_id_.empty()) {
    return nullptr;
  }

  std::unique_ptr<XMLNode> root = absl::make_unique<XMLNode>("", "DRMSystem");

  if (!id().empty()) {
    root->AddAttribute("id", id());
  }

  root->AddAttribute("kid", BytesToGUID(kid_));
  root->AddAttribute("systemId", BytesToGUID(system_id_));

  if (!pssh_.empty()) {
    std::unique_ptr<XMLNode> child = absl::make_unique<XMLNode>("", "PSSH");
    child->SetContent(BytesToBase64String(pssh_));
    root->AddChild(std::move(child));
  }

  if (!content_protection_data_.empty()) {
    std::unique_ptr<XMLNode> child =
        absl::make_unique<XMLNode>("", "ContentProtectionData");
    child->SetContent(BytesToBase64String(content_protection_data_));
    root->AddChild(std::move(child));
  }

  if (!uri_ext_x_key_.empty()) {
    std::unique_ptr<XMLNode> child =
        absl::make_unique<XMLNode>("", "URIExtXKey");
    child->SetContent(BytesToBase64String(uri_ext_x_key_));
    root->AddChild(std::move(child));
  }

  if (!hls_signaling_master_.empty()) {
    std::unique_ptr<XMLNode> child =
        absl::make_unique<XMLNode>("", "HLSSignalingData");
    child->SetContent(BytesToBase64String(hls_signaling_master_));
    child->AddAttribute("playlist", "master");
    root->AddChild(std::move(child));
  }

  if (!hls_signaling_media_.empty()) {
    std::unique_ptr<XMLNode> child =
        absl::make_unique<XMLNode>("", "HLSSignalingData");
    child->SetContent(BytesToBase64String(hls_signaling_media_));
    child->AddAttribute("playlist", "media");
    root->AddChild(std::move(child));
  }

  if (!smooth_streaming_data_.empty()) {
    std::unique_ptr<XMLNode> child =
        absl::make_unique<XMLNode>("", "SmoothStreamingProtectionHeaderData");
    child->SetContent(BytesToBase64String(smooth_streaming_data_));
    root->AddChild(std::move(child));
  }

  if (!hds_signaling_data_.empty()) {
    std::unique_ptr<XMLNode> child =
        absl::make_unique<XMLNode>("", "HDSSignalingData");
    child->SetContent(BytesToBase64String(hds_signaling_data_));
    root->AddChild(std::move(child));
  }

  return root;
}

bool DRMSystem::Deserialize(std::unique_ptr<XMLNode> node) {
  std::string attribute;
  if (!(attribute = node->GetAttribute("id")).empty()) {
    set_id(attribute);
  }

  kid_ = GUIDStringToBytes(node->GetAttribute("kid"));
  system_id_ = GUIDStringToBytes(node->GetAttribute("systemId"));

  std::unique_ptr<XMLNode> child;
  child = node->GetFirstChildByName("PSSH");
  if (child) {
    pssh_ = Base64StringToBytes(child->GetContent());
  }

  child = node->GetFirstChildByName("ContentProtectionData");
  if (child) {
    content_protection_data_ = Base64StringToBytes(child->GetContent());
  }

  child = node->GetFirstChildByName("URIExtXKey");
  if (child) {
    uri_ext_x_key_ = Base64StringToBytes(child->GetContent());
  }

  child = node->GetFirstChildByName("HLSSignalingData");

  if (child->GetAttribute("playlist") == "master") {
    hls_signaling_master_ = Base64StringToBytes(child->GetContent());
  } else {
    hls_signaling_media_ = Base64StringToBytes(child->GetContent());
  }

  child = node->GetFirstChildByName("HLSSignalingData");

  if (child->GetAttribute("playlist") == "master") {
    hls_signaling_master_ = Base64StringToBytes(child->GetContent());
  } else {
    hls_signaling_media_ = Base64StringToBytes(child->GetContent());
  }

  child = node->GetFirstChildByName("SmoothStreamingProtectionHeaderData");
  if (child) {
    smooth_streaming_data_ = Base64StringToBytes(child->GetContent());
  }

  child = node->GetFirstChildByName("HDSSignalingData");
  if (child) {
    hds_signaling_data_ = Base64StringToBytes(child->GetContent());
  }

  return true;
}

}  // namespace cpix
