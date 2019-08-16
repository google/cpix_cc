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

#ifndef CPIX_CC_CPIX_ELEMENT_LIST_H_
#define CPIX_CC_CPIX_ELEMENT_LIST_H_

#include <cstdint>
#include <memory>
#include <string>
#include <vector>

#include "cpix_element.h"

namespace cpix {
class XMLNode;

class CPIXElementList : public CPIXElement {
 public:
  CPIXElementList(const std::string& element_list_name);
  ~CPIXElementList();

 protected:
  bool Deserialize(std::unique_ptr<XMLNode> node);
  std::unique_ptr<XMLNode> GetNode() override;
  void AddElement(std::unique_ptr<CPIXElement> element);
  virtual std::unique_ptr<CPIXElement> CreateElement() = 0;

  std::string element_list_name_;
  std::vector<std::unique_ptr<CPIXElement>> elements_;
};
}  // namespace cpix

#endif  // CPIX_CC_CPIX_ELEMENT_LIST_H_
