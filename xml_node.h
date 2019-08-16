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

// Class and methods to wrap and simplify XML operations. Based on standard
// libxml.

#ifndef CPIX_CC_XML_NODE_H_
#define CPIX_CC_XML_NODE_H_

#include <memory>
#include <string>
#include <vector>

#include "libxml/tree.h"
#include "unique_xml_ptr.h"

namespace cpix {

class XMLNode {
 public:
  // Constructor with single string argument taken in XML, parses it, and
  // populates the XMLNode accordingly.
  explicit XMLNode(const std::string& xml);

  // Constructor with two string arguments creates a new XMLNode with root
  // element of corresponding namespace "ns" and element name "name".
  XMLNode(const std::string& ns, const std::string& name);

  // Create a new XMLNode with the supplied node.
  XMLNode(UniqueXmlPtr<xmlNode> node);

  ~XMLNode();

  XMLNode(XMLNode&&) = default;
  XMLNode& operator=(XMLNode&&) = default;

  XMLNode(const XMLNode&) = delete;
  XMLNode& operator=(const XMLNode&) = delete;

  // Add an existing node as a child of this one.
  bool AddChild(std::unique_ptr<XMLNode> child);

  // Add Text Content to this node.
  void SetContent(const std::string& content);

  // Add an Attribute to this node.
  void AddAttribute(const std::string& name, const std::string& value);

  // This node acts as the root of an XML chunk which is written out to a
  // string.
  std::string AsString();

  // Returns value of specified attribute in |node_|, empty string if attribute
  // |attribute_name| does not exist. Note that this function does not
  // differentiate between attributes of value "" and non-existent attributes.
  std::string GetAttribute(const std::string& attribute_name);

  // Returns the text content of |node_|.
  std::string GetContent();

  std::string GetName() {
    return reinterpret_cast<const char*>(node_.get()->name);
  }

  // Return the first child of node_, or nullptr. Removes returned node from
  // existing tree context.
  std::unique_ptr<XMLNode> GetFirstChild();

  // Return the direct child of node_ with element name "name", or nullptr. If
  // multiple children with the same name exist, return the first one. Removes
  // returned node from existing tree context.
  std::unique_ptr<XMLNode> GetFirstChildByName(const std::string& name);

  // Returns a vector of all direct children of a specified name of node_.
  // Removes returned nodes from existing tree context.
  std::vector<std::unique_ptr<XMLNode>> GetChildrenByName(
      const std::string& element_name);

  // Return the first node following a direct descendant line specified by each
  // name in |descendant_tree|. Null if not found. Every node in
  // |descendant_tree| is freed of its existing context.
  std::unique_ptr<XMLNode> GetDescendantNode(
      std::vector<std::string> descendant_tree);

 private:
  UniqueXmlPtr<xmlNode> node_;
};
}  // namespace cpix
#endif  // CPIX_CC_XML_NODE_H_
