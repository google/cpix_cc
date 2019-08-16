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

#include <memory>

#include "libxml/xmlschemastypes.h"
#include "unique_xml_ptr.h"

namespace cpix {

bool ValidateXML(const std::string& xml, const std::string& schema_uri) {
  UniqueXmlPtr<xmlSchemaParserCtxt> parser_context(
      xmlSchemaNewParserCtxt(schema_uri.c_str()));

  UniqueXmlPtr<xmlSchema> schema(xmlSchemaParse(parser_context.get()));

  UniqueXmlPtr<xmlDoc> doc(xmlParseMemory(xml.c_str(), xml.size()));
  if (!doc) {
    return false;
  }
  UniqueXmlPtr<xmlSchemaValidCtxt> validation_context(
      xmlSchemaNewValidCtxt(schema.get()));
  return xmlSchemaValidateDoc(validation_context.get(), doc.get()) == 0;
}

std::string GetCpixSchema() { return "schemas/cpix.xsd"; }

}  // namespace cpix
