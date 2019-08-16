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

#include "xml_util.h"

#include "gtest/gtest.h"

namespace cpix {
namespace {

constexpr char kGoodCPIX[] =
    "<?xml version=\"1.0\" encoding=\"utf-8\"?><CPIX "
    "xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\" "
    "xmlns:xsd=\"http://www.w3.org/2001/XMLSchema\" "
    "xmlns=\"urn:dashif:org:cpix\" "
    "xmlns:ds=\"http://www.w3.org/2000/09/xmldsig#\" "
    "xmlns:enc=\"http://www.w3.org/2001/04/xmlenc#\" "
    "xmlns:pskc=\"urn:ietf:params:xml:ns:keyprov:pskc\"><ContentKeyList><"
    "ContentKey "
    "kid=\"40d02dd1-61a3-4787-a155-572325d47b80\"><Data><pskc:Secret><pskc:"
    "PlainValue>gPxt0PMwrHM4TdjwdQmhhQ==</pskc:PlainValue></pskc:Secret></"
    "Data></ContentKey><ContentKey "
    "kid=\"0a30ea4f-539d-4b02-94b2-2b3fba2576d3\"><Data><pskc:Secret><pskc:"
    "PlainValue>x/gaoS/fDi8BqGNIhkixwQ==</pskc:PlainValue></pskc:Secret></"
    "Data></ContentKey><ContentKey "
    "kid=\"9f7908fa-5d5c-4097-ba53-50edc2235fbc\"><Data><pskc:Secret><pskc:"
    "PlainValue>3iv9lYwafpe0uEmxDc6PSw==</pskc:PlainValue></pskc:Secret></"
    "Data></ContentKey><ContentKey "
    "kid=\"fac2cbf5-889c-412b-a385-04a29d409bdc\"><Data><pskc:Secret><pskc:"
    "PlainValue>1OZVZZoYFSU2X/7qT3sHwg==</pskc:PlainValue></pskc:Secret></"
    "Data></ContentKey></ContentKeyList></CPIX>";

constexpr char kNotCPIX[] =
    "<?xml version=\"1.0\" encoding=\"utf-8\"?><CPIX "
    "xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\" "
    "xmlns:xsd=\"http://www.w3.org/2001/XMLSchema\" "
    "xmlns=\"urn:dashif:org:cpix\" "
    "xmlns:ds=\"http://www.w3.org/2000/09/xmldsig#\" "
    "xmlns:enc=\"http://www.w3.org/2001/04/xmlenc#\" "
    "xmlns:pskc=\"urn:ietf:params:xml:ns:keyprov:pskc\"><NotCPIXElement "
    "/></CPIX>";

TEST(XMLNodeTest, AcceptGoodCPIX) {
  EXPECT_TRUE(ValidateXML(kGoodCPIX, GetCpixSchema()));
}

TEST(XMLNodeTest, RejectNotCPIX) {
  EXPECT_FALSE(ValidateXML(kNotCPIX, GetCpixSchema()));
}

}  // namespace
}  // namespace cpix
