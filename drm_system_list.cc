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

#include "drm_system_list.h"

#include <memory>
#include <string>
#include <utility>

#include "absl/memory/memory.h"
#include "cpix_element.h"
#include "drm_system.h"
#include "xml_node.h"

namespace cpix {
DRMSystemList::~DRMSystemList() = default;

bool DRMSystemList::AddDRMSystem(std::unique_ptr<DRMSystem> drm) {
  if (drm->system_id().empty() || drm->kid().empty()) {
    return false;
  }
  AddElement(std::move(drm));
  return true;
}

std::unique_ptr<CPIXElement> DRMSystemList::CreateElement() {
  return absl::make_unique<DRMSystem>();
}

}  // namespace cpix
