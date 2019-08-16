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

#include "cpix_message.h"

#include <cstdint>
#include <memory>
#include <utility>
#include <vector>

#include "absl/memory/memory.h"
#include "aes_cryptor.h"
#include "cpix_util.h"
#include "glog/logging.h"
#include "rsa_private_key.h"
#include "rsa_public_key.h"
#include "xml_node.h"
#include "xml_util.h"

namespace cpix {
CPIXMessage::CPIXMessage() {
  recipients_ = absl::make_unique<RecipientList>();
  content_keys_ = absl::make_unique<ContentKeyList>();
  drm_systems_ = absl::make_unique<DRMSystemList>();
  usage_rules_ = absl::make_unique<UsageRuleList>();
  key_periods_ = absl::make_unique<KeyPeriodList>();
}

CPIXMessage::~CPIXMessage() = default;

bool CPIXMessage::FromString(const std::string& xml) {
  std::unique_ptr<XMLNode> root = absl::make_unique<XMLNode>(xml);
  return Deserialize(std::move(root));
}

bool CPIXMessage::DecryptWith(const std::vector<uint8_t>& private_key) {
  std::unique_ptr<RSAPrivateKey> private_key_ptr =
      RSAPrivateKey::CreateFromDER(private_key);
  if (!private_key_ptr) {
    LOG(ERROR) << "Invalid private key";
    return false;
  }

  bool match = false;
  for (const auto& recipient : recipients_->elements_) {
    Recipient* recipient_ptr = static_cast<Recipient*>(recipient.get());
    if (recipient_ptr->CreateRSAPublicKey()->MatchesKey(
            private_key_ptr->rsa_key())) {
      std::vector<uint8_t> document_key =
          recipient_ptr->DecryptDocumentKeyWith(private_key);
      if (document_key.empty()) {
        LOG(ERROR) << "Failure to decrypt document key";
        return false;
      }
      document_key_ = document_key;
      match = true;
      break;
    }
  }
  if (!match) {
    LOG(ERROR) << "Provided RSA private key does not match any recipients of "
                  "the document";
    return false;
  }
  if (!content_keys_->DecryptContentKeys(document_key_)) {
    LOG(ERROR) << "Failure to decrypt content keys";
    return false;
  }
  return true;
}

std::unique_ptr<XMLNode> CPIXMessage::GetNode() {
  std::unique_ptr<XMLNode> root = absl::make_unique<XMLNode>("", "CPIX");
  root->AddAttribute("xmlns:xsi", "http://www.w3.org/2001/XMLSchema-instance");
  root->AddAttribute("xmlns:xsd", "http://www.w3.org/2001/XMLSchema");
  root->AddAttribute("xmlns", "urn:dashif:org:cpix");
  root->AddAttribute("xmlns:ds", "http://www.w3.org/2000/09/xmldsig#");
  root->AddAttribute("xmlns:enc", "http://www.w3.org/2001/04/xmlenc#");
  root->AddAttribute("xmlns:pskc", "urn:ietf:params:xml:ns:keyprov:pskc");
  if (!content_id_.empty()) {
    root->AddAttribute("contentId", content_id_);
  }

  if (!recipients_->elements_.empty() && document_key_.empty()) {
    document_key_ = GetRandomBytes(32);
  }

  for (auto& element : recipients_->elements_) {
    Recipient* recipient = static_cast<Recipient*>(element.get());
    if (recipient->encrypted_document_key().empty()) {
      recipient->SetDocumentKey(document_key_);
    }
  }

  if (!document_key_.empty()) {
    for (auto& element : content_keys_->elements_) {
      ContentKey* key = static_cast<ContentKey*>(element.get());
      if (!key->is_encrypted()) {
        std::unique_ptr<AESCryptor> aes = AESCryptor::Create(document_key_);
        if (!key->explicit_iv().empty()) {
          aes->SetIV(key->explicit_iv());
        }
        std::vector<uint8_t> encrypted_key = aes->CBCEncrypt(key->key_value());
        if (encrypted_key.empty()) {
          LOG(ERROR) << "Key encryption failed";
          return nullptr;
        }
        key->SetEncryptedKeyValue(encrypted_key);
      }
    }
  }

  root->AddChild(recipients_->GetNode());

  root->AddChild(content_keys_->GetNode());

  root->AddChild(drm_systems_->GetNode());

  root->AddChild(key_periods_->GetNode());

  root->AddChild(usage_rules_->GetNode());

  return root;
}

bool CPIXMessage::Deserialize(std::unique_ptr<XMLNode> node) {
  if (!node) {
    return false;
  }
  std::string attribute;
  if (!(attribute = node->GetAttribute("id")).empty()) {
    set_id(attribute);
  }

  if (!(attribute = node->GetAttribute("contentId")).empty()) {
    set_content_id(attribute);
  }

  if (!(attribute = node->GetAttribute("name")).empty()) {
    set_name(attribute);
  }

  recipients_->Deserialize(node->GetFirstChildByName("DeliveryDataList"));

  content_keys_->Deserialize(node->GetFirstChildByName("ContentKeyList"));

  drm_systems_->Deserialize(node->GetFirstChildByName("DRMSystemList"));

  key_periods_->Deserialize(node->GetFirstChildByName("ContentKeyPeriodList"));

  usage_rules_->Deserialize(
      node->GetFirstChildByName("ContentKeyUsageRuleList"));

  return true;
}

bool CPIXMessage::AddContentKey(std::unique_ptr<ContentKey> key) {
  return content_keys_->AddContentKey(std::move(key));
}

bool CPIXMessage::AddContentKey(
    std::unique_ptr<ContentKey> key,
    std::vector<std::unique_ptr<DRMSystem>> drm_systems,
    std::vector<std::unique_ptr<UsageRule>> rules) {
  const std::vector<uint8_t>& kid = key->kid();
  if (!content_keys_->AddContentKey(std::move(key))) {
    return false;
  }

  for (auto& drm_system : drm_systems) {
    drm_system->set_key_id(kid);
    if (!drm_systems_->AddDRMSystem(std::move(drm_system))) {
      return false;
    }
  }

  for (auto& rule : rules) {
    rule->set_key_id(kid);
    if (!usage_rules_->AddUsageRule(std::move(rule))) {
      return false;
    }
  }
  return true;
}

bool CPIXMessage::AddDRMSystem(std::unique_ptr<DRMSystem> drm) {
  if (!content_keys_->FindContentKey(drm->kid())) {
    return false;
  }
  return drm_systems_->AddDRMSystem(std::move(drm));
}

bool CPIXMessage::AddUsageRule(std::unique_ptr<UsageRule> rule) {
  if (!content_keys_->FindContentKey(rule->kid())) {
    return false;
  }
  return usage_rules_->AddUsageRule(std::move(rule));
}

bool CPIXMessage::AddKeyPeriod(std::unique_ptr<KeyPeriod> key_period) {
  return key_periods_->AddKeyPeriod(std::move(key_period));
}

bool CPIXMessage::AddRecipient(std::unique_ptr<Recipient> recipient) {
  return recipients_->AddRecipient(std::move(recipient));
}

bool CPIXMessage::ValidateXML(const std::string& xml,
                              const std::string& schema_uri) {
  return cpix::ValidateXML(xml, schema_uri);
}

void CPIXMessage::InjectRecipientListForTest(
    std::unique_ptr<RecipientList> recipient_list) {
  recipients_ = std::move(recipient_list);
}

void CPIXMessage::InjectContentKeyListForTest(
    std::unique_ptr<ContentKeyList> key_list) {
  content_keys_ = std::move(key_list);
}

void CPIXMessage::InjectDRMSystemListForTest(
    std::unique_ptr<DRMSystemList> drm_list) {
  drm_systems_ = std::move(drm_list);
}

void CPIXMessage::InjectUsageRuleListForTest(
    std::unique_ptr<UsageRuleList> rule_list) {
  usage_rules_ = std::move(rule_list);
}

void CPIXMessage::InjectKeyPeriodListForTest(
    std::unique_ptr<KeyPeriodList> key_period_list) {
  key_periods_ = std::move(key_period_list);
}

}  // namespace cpix
