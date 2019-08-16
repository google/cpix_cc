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

#include <memory>
#include <utility>

#include "absl/memory/memory.h"
#include "cpix_element.h"
#include "gtest/gtest.h"
#include "testable_cpix_element.h"
#include "testable_cpix_element_list.h"
#include "xml_node.h"

namespace cpix {
namespace {

using ::testing::Test;

constexpr char kGoodXML[] =
    "<CPIXElementList><CPIXElementTest/><CPIXElementTest/></CPIXElementList>";

class DummyCPIXElement : public CPIXElement {
 public:
  DummyCPIXElement() = default;
  ~DummyCPIXElement() = default;

 protected:
  std::string Serialize() { return GetNode()->AsString(); }
  bool Deserialize(std::unique_ptr<XMLNode> node) { return true; }

 private:
  std::unique_ptr<XMLNode> GetNode() {
    return absl::make_unique<XMLNode>("", "CPIXElementTest");
  }
};

class DummyCPIXElementList : public CPIXElementList {
 public:
  DummyCPIXElementList() : CPIXElementList("CPIXElementList") {}
  ~DummyCPIXElementList() = default;

 private:
  std::unique_ptr<CPIXElement> CreateElement() {
    return absl::make_unique<DummyCPIXElement>();
  }
};

TEST(CPIXElementListTest, SerializeList) {
  TestableCPIXElementList<DummyCPIXElementList> element_list;
  std::unique_ptr<TestableCPIXElement<DummyCPIXElement>> element1 =
      absl::make_unique<TestableCPIXElement<DummyCPIXElement>>();
  std::unique_ptr<TestableCPIXElement<DummyCPIXElement>> element2 =
      absl::make_unique<TestableCPIXElement<DummyCPIXElement>>();
  element_list.AddElement(std::move(element1));
  element_list.AddElement(std::move(element2));

  EXPECT_EQ(element_list.Serialize(), kGoodXML);
}

TEST(CPIXElementListTest, DeserializeList) {
  TestableCPIXElementList<DummyCPIXElementList> element_list;

  std::unique_ptr<XMLNode> node = absl::make_unique<XMLNode>(kGoodXML);
  EXPECT_TRUE(element_list.Deserialize(std::move(node)));
  EXPECT_EQ(element_list.Serialize(), kGoodXML);
}
}  // namespace
}  // namespace cpix
