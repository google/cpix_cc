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

#include "xml_node.h"

#include <stddef.h>

#include <cstdio>
#include <memory>
#include <utility>
#include <vector>

#include "absl/memory/memory.h"
#include "libxml/parser.h"
#include "libxml/tree.h"
#include "libxml/xmlschemastypes.h"
#include "unique_xml_ptr.h"

namespace cpix {

XMLNode::XMLNode(const std::string& xml) {
  UniqueXmlPtr<xmlDoc> doc(xmlParseMemory(xml.c_str(), xml.size()));
  node_ =
      UniqueXmlPtr<xmlNode>(xmlCopyNode(xmlDocGetRootElement(doc.get()), 1));
}

XMLNode::XMLNode(const std::string& ns, const std::string& name) {
  node_ = UniqueXmlPtr<xmlNode>(xmlNewNode(NULL, BAD_CAST name.c_str()));
  if (!ns.empty()) {
    xmlSetNs(node_.get(), xmlNewNs(node_.get(), NULL, BAD_CAST ns.c_str()));
  }
}

XMLNode::XMLNode(UniqueXmlPtr<xmlNode> node) { node_ = std::move(node); }

XMLNode::~XMLNode() = default;

bool XMLNode::AddChild(std::unique_ptr<XMLNode> child) {
  if (!child) {
    return false;
  }

  // |child| is now owned by this method, so we release here to avoid
  // destruction.
  if (!xmlAddChild(node_.get(), child->node_.get())) {
    return false;
  }
  child->node_.release();
  return true;
}

void XMLNode::AddAttribute(const std::string& name, const std::string& value) {
  xmlNewProp(node_.get(), BAD_CAST name.c_str(), BAD_CAST value.c_str());
}

void XMLNode::SetContent(const std::string& content) {
  xmlNodeSetContent(node_.get(), BAD_CAST content.c_str());
}

std::string XMLNode::AsString() {
  UniqueXmlPtr<xmlBuffer> buf(xmlBufferCreate());
  xmlNodeDump(buf.get(), nullptr, node_.get(), 0, 0);
  std::string xml = reinterpret_cast<const char*>(xmlBufferContent(buf.get()));
  return xml;
}

std::string XMLNode::GetAttribute(const std::string& attribute_name) {
  UniqueXmlPtr<xmlChar> attribute_value(
      xmlGetProp(node_.get(), BAD_CAST attribute_name.c_str()));
  if (!attribute_value) {
    return "";
  }
  return reinterpret_cast<const char*>(attribute_value.get());
}

std::string XMLNode::GetContent() {
  return reinterpret_cast<const char*>(
      UniqueXmlPtr<xmlChar>(xmlNodeGetContent(node_.get())).get());
}

std::unique_ptr<XMLNode> XMLNode::GetFirstChild() {
  UniqueXmlPtr<xmlNode> curr(node_.get()->xmlChildrenNode);
  if (!curr) return nullptr;

  xmlUnlinkNode(curr.get());
  return absl::make_unique<XMLNode>(std::move(curr));
}

std::unique_ptr<XMLNode> XMLNode::GetFirstChildByName(const std::string& name) {
  xmlNodePtr curr = node_.get()->xmlChildrenNode;
  while (curr) {
    if ((!xmlStrcmp(curr->name, BAD_CAST name.c_str()))) {
      UniqueXmlPtr<xmlNode> node(curr);
      xmlUnlinkNode(node.get());
      return absl::make_unique<XMLNode>(std::move(node));
    }
    curr = curr->next;
  }
  return nullptr;
}

std::vector<std::unique_ptr<XMLNode>> XMLNode::GetChildrenByName(
    const std::string& element_name) {
  xmlNodePtr curr = node_.get()->xmlChildrenNode;
  std::vector<std::unique_ptr<XMLNode>> nodes;
  while (curr) {
    if ((!xmlStrcmp(curr->name, BAD_CAST element_name.c_str()))) {
      xmlNodePtr temp = curr;
      curr = curr->next;
      UniqueXmlPtr<xmlNode> node(temp);
      xmlUnlinkNode(node.get());
      nodes.push_back(absl::make_unique<XMLNode>(std::move(node)));
    } else {
      curr = curr->next;
    }
  }
  return nodes;
}

std::unique_ptr<XMLNode> XMLNode::GetDescendantNode(
    std::vector<std::string> descendant_tree) {
  if (descendant_tree.empty()) {
    return nullptr;
  }

  std::unique_ptr<XMLNode> node = GetFirstChildByName(descendant_tree[0]);

  if (!node) {
    return nullptr;
  }

  for (size_t i = 1; i < descendant_tree.size(); i++) {
    node = node->GetFirstChildByName(descendant_tree[i]);
    if (!node) {
      return nullptr;
    }
  }
  return node;
}

// TODO(noahmdavis): Add xml canonicalization function.

}  // namespace cpix
