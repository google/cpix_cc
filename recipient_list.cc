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

#include "recipient_list.h"

#include <utility>

#include "absl/memory/memory.h"
#include "cpix_element.h"
#include "cpix_util.h"

namespace cpix {

RecipientList::~RecipientList() = default;

bool RecipientList::AddRecipient(std::unique_ptr<Recipient> recipient) {
  if (recipient->delivery_key_.empty()) {
    return false;
  }

  AddElement(std::move(recipient));
  return true;
}

std::unique_ptr<CPIXElement> RecipientList::CreateElement() {
  return absl::make_unique<Recipient>();
}
}  // namespace cpix
