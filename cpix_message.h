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

#ifndef CPIX_CC_CPIX_MESSAGE_H_
#define CPIX_CC_CPIX_MESSAGE_H_

#include <stdint.h>

#include <memory>
#include <string>
#include <vector>

#include "content_key.h"
#include "content_key_list.h"
#include "cpix_element.h"
#include "drm_system.h"
#include "drm_system_list.h"
#include "key_period.h"
#include "key_period_list.h"
#include "recipient.h"
#include "recipient_list.h"
#include "usage_rule.h"
#include "usage_rule_list.h"

// CPIXMessage acts as a container for an entire CPIX document. It contains all
// other CPIX elements, and has useful functions for the user to add and modify
// CPIX data, as well as load in existing CPIX documents and write out new ones.

namespace cpix {

class XMLNode;

class CPIXMessage : public CPIXElement {
 public:
  CPIXMessage();
  ~CPIXMessage();

  // Generate a CPIX document as an XML-formatted string based on the contents
  // of this message.
  std::string ToString() { return Serialize(); }

  // Deserialize the contents of an existing CPIX document into the CPIXMessage
  // OO structure, allowing for modification/insertion/deletion.
  bool FromString(const std::string& xml);

  // Uses the provided private key to decrypt ContentKeys.
  bool DecryptWith(const std::vector<uint8_t>& private_key);

  void set_content_id(const std::string& id) { content_id_ = id; }
  void set_name(const std::string& name) { name_ = name; }
  const std::string& content_id() const { return content_id_; }
  const std::string& name() const { return name_; }

  bool AddContentKey(std::unique_ptr<ContentKey> key);

  ContentKey* FindContentKeyById(const std::vector<uint8_t>& kid) {
    return content_keys_->FindContentKey(kid);
  }

  // Add a new ContentKey to the message, and any associated DRMSystems and
  // UsageRules.
  bool AddContentKey(std::unique_ptr<ContentKey> key,
                     std::vector<std::unique_ptr<DRMSystem>> drm_systems,
                     std::vector<std::unique_ptr<UsageRule>> rules);

  bool AddDRMSystem(std::unique_ptr<DRMSystem> drm);

  bool AddUsageRule(std::unique_ptr<UsageRule> rule);

  bool AddKeyPeriod(std::unique_ptr<KeyPeriod> key_period);

  bool AddRecipient(std::unique_ptr<Recipient> recipient);

  // Pass in an xml string an an absolute path to an XML schema schema file to
  // validate the document against the schema.
  static bool ValidateXML(const std::string& xml,
                          const std::string& schema_uri);

 private:
  friend class CPIXMessageTest;

  bool Deserialize(std::unique_ptr<XMLNode> node) override;
  std::unique_ptr<XMLNode> GetNode() override;

  std::string content_id_;
  std::string name_;
  std::vector<uint8_t> document_key_;
  std::unique_ptr<RecipientList> recipients_;
  std::unique_ptr<ContentKeyList> content_keys_;
  std::unique_ptr<DRMSystemList> drm_systems_;
  std::unique_ptr<UsageRuleList> usage_rules_;
  std::unique_ptr<KeyPeriodList> key_periods_;

  void InjectRecipientListForTest(
      std::unique_ptr<RecipientList> recipient_list);
  void InjectContentKeyListForTest(std::unique_ptr<ContentKeyList> key_list);
  void InjectDRMSystemListForTest(std::unique_ptr<DRMSystemList> drm_list);
  void InjectUsageRuleListForTest(std::unique_ptr<UsageRuleList> rule_list);
  void InjectKeyPeriodListForTest(
      std::unique_ptr<KeyPeriodList> key_period_list);
};
}  // namespace cpix
#endif  // CPIX_CC_CPIX_MESSAGE_H_
