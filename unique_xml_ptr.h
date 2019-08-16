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

#ifndef CPIX_CC_UNIQUE_XML_PTR_H_
#define CPIX_CC_UNIQUE_XML_PTR_H_

#include <memory>

#include "libxml/tree.h"
#include "libxml/xmlschemastypes.h"

namespace cpix {

struct XmlDeleter {
  inline void operator()(xmlNodePtr ptr) const { xmlFreeNode(ptr); }
  inline void operator()(xmlBufferPtr ptr) const { xmlBufferFree(ptr); }
  inline void operator()(xmlDocPtr ptr) const { xmlFreeDoc(ptr); }
  inline void operator()(xmlSchemaPtr ptr) const { xmlSchemaFree(ptr); }
  inline void operator()(xmlSchemaParserCtxtPtr ptr) const {
    xmlSchemaFreeParserCtxt(ptr);
  }
  inline void operator()(xmlSchemaValidCtxtPtr ptr) const {
    xmlSchemaFreeValidCtxt(ptr);
  }
  inline void operator()(xmlChar* ptr) const { xmlFree(ptr); }
};

template <typename XmlType>
using UniqueXmlPtr = std::unique_ptr<XmlType, XmlDeleter>;

}  // namespace cpix
#endif  // CPIX_CC_UNIQUE_XML_PTR_H_
