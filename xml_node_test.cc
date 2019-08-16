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

#include <memory>
#include <utility>
#include <vector>

#include "absl/memory/memory.h"
#include "gtest/gtest.h"

namespace cpix {
namespace {

constexpr char kXMLString2Children[] =
    "<parent attr=\"true\"><ns:child1 xmlns:ns=\"http://foo.com\" "
    "attr=\"value\">text</ns:child1><ns:child1 "
    "xmlns:ns=\"http://foo.com\"/></parent>";

constexpr char kXMLChild1String[] =
    "<ns:child1 xmlns:ns=\"http://foo.com\" "
    "attr=\"value\">text</ns:child1>";

constexpr char kXMLChild2String[] =
    "<ns:child1 "
    "xmlns:ns=\"http://foo.com\"/>";

constexpr char kXMLStringNoChildren[] = "<parent attr=\"true\"/>";

constexpr char kXMLStringContentNode[] = "<root>thecontent</root>";

constexpr char kXMLString4Generations[] =
    "<gen1><gen2><gen3><gen4/></gen3></gen2></gen1>";

constexpr char kContentNodeContent[] = "thecontent";
TEST(XMLNodeTest, AddChildSuccess) {
  std::unique_ptr<XMLNode> root = absl::make_unique<XMLNode>("", "parent");
  std::unique_ptr<XMLNode> child = absl::make_unique<XMLNode>("", "child");
  EXPECT_TRUE(root->AddChild(std::move(child)));
}

TEST(XMLNodeTest, AsString) {
  std::unique_ptr<XMLNode> root = absl::make_unique<XMLNode>("", "parent");
  std::unique_ptr<XMLNode> child1 = absl::make_unique<XMLNode>("ns", "child1");
  std::unique_ptr<XMLNode> child2 = absl::make_unique<XMLNode>("ns", "child1");
  root->AddAttribute("attr", "true");
  child1->AddAttribute("xmlns:ns", "http://foo.com");
  child1->AddAttribute("attr", "value");
  child1->SetContent("text");
  child2->AddAttribute("xmlns:ns", "http://foo.com");
  EXPECT_TRUE(root->AddChild(std::move(child1)));
  EXPECT_TRUE(root->AddChild(std::move(child2)));
  EXPECT_EQ(root->AsString(), kXMLString2Children);
}

TEST(XMLNodeTest, FromString) { XMLNode root(kXMLString2Children); }

TEST(XMLNodeTest, GetAttribute) {
  XMLNode root(kXMLString2Children);
  root.AddAttribute("foo", "bar");
  EXPECT_EQ(root.GetAttribute("attr"), "true");
  EXPECT_EQ(root.GetAttribute("foo"), "bar");
}

TEST(XMLNodeTest, GetFirstChildSuccess) {
  XMLNode root(kXMLString2Children);
  std::unique_ptr<XMLNode> child(root.GetFirstChild());
  EXPECT_EQ(child->AsString(), kXMLChild1String);
}

TEST(XMLNodeTest, GetFirstChildFail) {
  XMLNode root(kXMLStringNoChildren);
  EXPECT_FALSE(root.GetFirstChild());
}

TEST(XMLNodeTest, GetChildByNameSuccess) {
  XMLNode root(kXMLString2Children);
  std::unique_ptr<XMLNode> child(root.GetFirstChildByName("child1"));
  EXPECT_EQ(child->AsString(), kXMLChild1String);
}

TEST(XMLNodeTest, GetChildByNameFailure) {
  XMLNode root(kXMLString2Children);
  EXPECT_FALSE(root.GetFirstChildByName("childnoexist"));
}

TEST(XMLNodeTest, GetChildrenFull) {
  XMLNode root(kXMLString2Children);
  std::vector<std::unique_ptr<XMLNode>> children =
      root.GetChildrenByName("child1");
  ASSERT_EQ(children.size(), 2);
  EXPECT_EQ(children[0]->AsString(), kXMLChild1String);
  EXPECT_EQ(children[1]->AsString(), kXMLChild2String);
}

TEST(XMLNodeTest, GetChildrenEmpty) {
  XMLNode root(kXMLStringNoChildren);
  EXPECT_TRUE(root.GetChildrenByName("child1").empty());
}

TEST(XMLNodeTest, GetContent) {
  XMLNode root(kXMLStringContentNode);
  EXPECT_EQ(root.GetContent(), kContentNodeContent);
}

TEST(XMLNodeTest, GetDescendantNode) {
  XMLNode root(kXMLString4Generations);
  std::unique_ptr<XMLNode> child(
      root.GetDescendantNode({"gen2", "gen3", "gen4"}));

  ASSERT_TRUE(child);
  EXPECT_EQ(child->GetName(), "gen4");
}
}  // namespace
}  // namespace cpix
