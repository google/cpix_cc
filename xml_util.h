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

#ifndef CPIX_CC_XML_UTIL_H_
#define CPIX_CC_XML_UTIL_H_

#include <string>

namespace cpix {

// Checks to see if the provided XML adheres to the schema in file pointed to
// by schema_url. Returns true if valid, false otherwise.
bool ValidateXML(const std::string& xml, const std::string& schema_uri);

// Temporary function that gets path of CPIX schema file while code lives in
// google3.
std::string GetCpixSchema();

}  // namespace cpix
#endif  // CPIX_CC_XML_UTIL_H_
