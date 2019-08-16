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

#ifndef CPIX_CC_KEY_PERIOD_H_
#define CPIX_CC_KEY_PERIOD_H_

#include <memory>
#include <string>

#include "cpix_element.h"

namespace cpix {

class XMLNode;

// Represents a key period, either by an index value or through explicit
// timestamps for start and end. Can be referenced by multiple UsageRules
// through the |id| field.

class KeyPeriod : public CPIXElement {
 public:
  KeyPeriod() = default;
  ~KeyPeriod();
  void SetIndex(int index);

  // Requires time strings in xs:dateTime format
  // "CCYY-MM-DDThh:mm:ss[Z|(+|-)hh:mm".
  void SetInterval(const std::string& start, const std::string& end);

 protected:
  bool Deserialize(std::unique_ptr<XMLNode> node) override;

 private:
  friend class ContentKeyList;
  std::unique_ptr<XMLNode> GetNode() override;

  int index_ = -1;
  std::string start_;
  std::string end_;
};
}  // namespace cpix

#endif  // CPIX_CC_KEY_PERIOD_H_
