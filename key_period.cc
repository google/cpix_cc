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

#include "key_period.h"

#include <memory>
#include <string>

#include "absl/memory/memory.h"
#include "absl/strings/numbers.h"
#include "glog/logging.h"
#include "xml_node.h"

namespace cpix {
KeyPeriod::~KeyPeriod() = default;

void KeyPeriod::SetIndex(int index) {
  start_ = "";
  end_ = "";
  index_ = index;
}

void KeyPeriod::SetInterval(const std::string& start, const std::string& end) {
  index_ = -1;
  start_ = start;
  end_ = end;
}

std::unique_ptr<XMLNode> KeyPeriod::GetNode() {
  if ((index_ != -1 && !(start_.empty() && end_.empty())) ||
      (index_ == -1 && (start_.empty() || end_.empty()))) {
    return nullptr;
  }

  std::unique_ptr<XMLNode> root =
      absl::make_unique<XMLNode>("", "ContentKeyPeriod");
  if (!id().empty()) {
    root->AddAttribute("id", id());
  }

  if (index_ != -1) {
    root->AddAttribute("index", std::to_string(index_));
  } else {
    // Assumes start/end string in proper xs:dateTime format which is not
    // currently checked.
    root->AddAttribute("start", start_);
    root->AddAttribute("end", end_);
  }

  return root;
}

bool KeyPeriod::Deserialize(std::unique_ptr<XMLNode> node) {
  if (!node->GetAttribute("index").empty()) {
    int index;
    if (!absl::SimpleAtoi(node->GetAttribute("index"), &index)) {
      LOG(ERROR) << "Invalid KeyPeriod. Not added to document\n";
      return false;
    }
    SetIndex(index);
    return true;
  }
  if (!node->GetAttribute("start").empty() &&
      !node->GetAttribute("end").empty()) {
    SetInterval(node->GetAttribute("start"), node->GetAttribute("end"));
    return true;
  }
  LOG(ERROR) << "Invalid KeyPeriod. Not added to document\n";
  return false;
}

}  // namespace cpix
