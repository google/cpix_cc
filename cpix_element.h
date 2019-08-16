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

#ifndef CPIX_CC_CPIX_ELEMENT_H_
#define CPIX_CC_CPIX_ELEMENT_H_

#include <memory>
#include <string>

namespace cpix {

class XMLNode;

// CPIXElement is an abstract base class that is extended by other classes which
// act as pseudo XML elements. CPIXElement provides a common ground for these
// derived classes to build upon, specifically providing support for
// holding digital signature information and a simple ID field common to all
// CPIX elements.

class CPIXElement {
 public:
  CPIXElement() = default;
  virtual ~CPIXElement();
  CPIXElement(const CPIXElement&) = delete;
  CPIXElement& operator=(const CPIXElement&) = delete;

  const std::string& id() { return id_; }
  void set_id(const std::string& id) { id_ = id; }

 protected:
  // Returns an XML-formatted string representation of the Element according to
  // the CPIX specification.
  std::string Serialize();

  // Takes in a stringified XML document and extracts information pertaining to
  // the given derivation of CPIXElement.
  virtual bool Deserialize(std::unique_ptr<XMLNode> node) = 0;

  // Creates a hierarchy of XMLNodes representing the current object.
  virtual std::unique_ptr<XMLNode> GetNode() = 0;

 private:
  friend class CPIXElementList;
  std::string id_;
};
}  // namespace cpix
#endif  // CPIX_CC_CPIX_ELEMENT_H_
