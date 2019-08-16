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

#include "cpix_element_list.h"

#include <utility>

#include "absl/memory/memory.h"
#include "xml_node.h"

namespace cpix {
CPIXElementList::CPIXElementList(const std::string& element_list_name) {
  element_list_name_ = element_list_name;
}

CPIXElementList::~CPIXElementList() = default;

bool CPIXElementList::Deserialize(std::unique_ptr<XMLNode> node) {
  if (!node) {
    return true;
  }

  std::string attribute;
  if (!(attribute = node->GetAttribute("id")).empty()) {
    set_id(attribute);
  }

  std::unique_ptr<XMLNode> child_node;
  // TODO(noahmdavis): check to make sure child_node is correct element type
  while ((child_node = node->GetFirstChild())) {
    std::unique_ptr<CPIXElement> element = CreateElement();
    if (!element->Deserialize(std::move(child_node))) {
      return false;
    }
    AddElement(std::move(element));
  }
  return true;
}

std::unique_ptr<XMLNode> CPIXElementList::GetNode() {
  if (elements_.empty()) {
    return nullptr;
  }

  std::unique_ptr<XMLNode> root =
      absl::make_unique<XMLNode>("", element_list_name_);

  if (!id().empty()) {
    root->AddAttribute("id", id());
  }

  for (const auto& element : elements_) {
    std::unique_ptr<XMLNode> element_node = element->GetNode();
    if (!element_node) {
      return nullptr;
    }
    root->AddChild(std::move(element_node));
  }

  return root;
}

void CPIXElementList::AddElement(std::unique_ptr<CPIXElement> element) {
  elements_.push_back(std::move(element));
}

}  // namespace cpix
