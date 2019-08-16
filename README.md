# CPIX C++ Library

This library implements the __[CPIX 2.2 Specification](https://dashif.org/docs/CPIX2.2/Cpix.html)__ for the exchange of encryption keys and DRM information.

This library can be used to create, read, and modify CPIX documents.

This is not an officially supported Google product.

## Setting up for development

1. We use [bazel](https://bazel.build/) to manage the third_party dependencies and to build the library.

2. Language support
  - STL is required. (e.g. `libstdc++`, `libc++`, etc)
  - C++11 is required.

3. Third party dependencies (downloaded by bazel automatically during compilation) are used when building the library.
  - [absl](https://github.com/abseil/abseil-cpp/tree/master/absl)
  - [boringssl](https://github.com/google/boringssl.git)
  - [libxml](https://github.com/GNOME/libxml2)
  - [googletest](https://github.com/google/googletest)
  - [glog](https://github.com/google/glog)
  - [gflags](https://github.com/gflags/gflags/)

4. Get the source and build
```bash
git clone https://github.com/google/cpix_cc.git
cd cpix_cc
bazel build :all
bazel test :all
```

## Quickstart

The `CPIXMessage` class represents an entire document, so whether you want to create a new document from scratch or read and modify an existing one, start with a `CPIXMessage` object.

Please refer to [`examples`](examples) for examples on how to use the library.

## Common Use Cases

### Serializing a constructed CPIX document

```c++
CPIXMessage message;
...
// Add elements to the message.
...

// Simply call the ToString() function to obtain a std::string of the XML
// generated from the message object.
std::string xml = message.ToString();
```

### Add clear ContentKeys

```c++
CPIXMessage message;

std::unique_ptr<ContentKey> key(new ContentKey);
key->set_key_id(kid_raw_bytes);
key->SetKeyValue(key_value_raw_bytes);

message.AddContentKey(std::move(key));
```

### Add encrypted ContentKeys

```c++
CPIXMessage message;

std::unique_ptr<ContentKey> key(new ContentKey);
key->set_key_id(kid_raw_bytes);
key->SetKeyValue(key_value_raw_bytes);

message.AddContentKey(std::move(key));

std::unique_ptr<Recipient> rcpt(new Recipient);
rcpt->set_delivery_key(der_formatted_X509_certificate);
message.AddRecipient(std::move(rcpt));
```

### Loading an existing CPIX document

```c++
CPIXMessage message;

// After this function call, all content from the supplied document will be
// deserialized into |message|.
message.FromString(string_form_of_cpix_document);
```

### Loading an encrypted CPIX document

```c++
CPIXMessage message;

message.FromString(string_form_of_cpix_document);

// If the supplied private key matches one of the certificates in the newly
// deserialized document, the ContentKeys will be decrypted.
message.DecryptWith(der_encoded_private_key);
```

### Add keys with usage rules with different policies

```c++
CPIXMessage message;

// First ContentKey and UsageRule
std::unique_ptr<ContentKey> sd_key(new ContentKey);
sd_key->set_key_id(sd_kid_raw_bytes);
sd_key->SetKeyValue(sd_key_value_raw_bytes);

message.AddContentKey(std::move(sd_key));

std::unique_ptr<UsageRule> sd_rule(new UsageRule);

// Create structs (defined in usage_rule.h) for the filters you want.
VideoFilter sd_video_filter;
sd_video_filter.min_pixels = 0;
sd_video_filter.max_pixels = 768 * 576;

// Add filters to the rule. Any valid set of rules, however dense or sparse,
// can be added to a single UsageRule.
sd_rule->AddVideoFilter(sd_video_filter);

// Since each UsageRule maps to a single ContentKey, the |kid_| field MUST be
// populated before adding the UsageRule to a CPIXMessage. See "Adding DRMSystem
// and UsageRule without explicit Key ID" for exception.
sd_rule->set_key_id(sd_key->key_id());

message.AddUsageRule(std::move(sd_rule));

// Second ContentKey and UsageRule
std::unique_ptr<ContentKey> hd_key(new ContentKey);
hd_key->set_key_id(hd_kid_raw_bytes);
hd_key->SetKeyValue(hd_key_value_raw_bytes);

message.AddContentKey(std::move(hd_key));

std::unique_ptr<UsageRule> hd_rule(new UsageRule);

VideoFilter hd_video_filter;
hd_video_filter.min_pixels = 768 * 576 + 1;
hd_video_filter.max_pixels = 1920 * 1080;

hd_rule->AddVideoFilter(hd_video_filter);

hd_rule->set_key_id(hd_key->key_id());

message.AddUsageRule(std::move(hd_rule));
```

### Add keys with usage rules with different labels

```c++
CPIXMessage message;

// First ContentKey and UsageRule
std::unique_ptr<ContentKey> sd_key(new ContentKey);
sd_key->set_key_id(sd_kid_raw_bytes);
sd_key->SetKeyValue(sd_key_value_raw_bytes);

message.AddContentKey(std::move(sd_key));

std::unique_ptr<UsageRule> sd_rule(new UsageRule);

sd_rule.AddLabelFilter("SD");

// Since each UsageRule maps to a single ContentKey, the |kid_| field MUST be
// populated before adding the UsageRule to a CPIXMessage. See "Adding DRMSystem
// and UsageRule without explicit Key ID" for exception.
sd_rule->set_key_id(sd_key->key_id());

message.AddUsageRule(std::move(sd_rule));

// Second ContentKey and UsageRule
std::unique_ptr<ContentKey> hd_key(new ContentKey);
hd_key->set_key_id(hd_kid_raw_bytes);
hd_key->SetKeyValue(hd_key_value_raw_bytes);

message.AddContentKey(std::move(hd_key));

std::unique_ptr<UsageRule> hd_rule(new UsageRule);

hd_rule->AddLabelFilter("HD");

hd_rule->set_key_id(hd_key->key_id());

message.AddUsageRule(std::move(hd_rule));
```

### Creating UsageRules with KeyPeriod

```c++
// Create a new KeyPeriodFilter and set the period by index or explicit
// time interval.
std::unique_ptr<KeyPeriod> key_period(new KeyPeriod);
key_period->SetIndex(1001);

// Set the KeyPeriod's |id| (to be referenced by a UsageRule).
key_period->set_id("key_period_1");

// Add it to some existing message.
message.AddKeyPeriod(std::move(key_period));

// Create a new UsageRule
std::unique_ptr<UsageRule> rule(new UsageRule);
//Set the Key ID
rule->set_key_id(kid_raw_bytes_of_existing_content_key);

// Add a new KeyPeriodFilter by referencing the |id| of an existing filter.
// Multiple UsageRules may reference the same KeyPeriod.
rule->AddKeyPeriodFilter("key_period_1");

// Add the UsageRule to the same existing message.
message.AddUsageRule(std::move(rule));
```

### Creating DRMSystems

```c++
std::unique_ptr<DRMSystem> drm(new DRMSystem);
// System ID is a required field of DRMSystem.
drm->set_system_id(system_id_raw_bytes);

// Any or all of these data fields may be set for DRMSystem. See DRMSystem.h for
// a complete list.
drm->set_content_protection_data(content_protection_data_raw_bytes);
drm->set_hls_signaling_master(hls_signaling_master_raw_bytes);
drm->set_hls_signaling_media(hls_signaling_media_raw_bytes);

// Since each DRMSystem maps to a single ContentKey, the |kid_| field MUST be
// populated before adding the UsageRule to a CPIXMessage. See "Adding DRMSystem
// and UsageRule without explicit Key ID" for exception.
drm->set_key_id(kid_raw_bytes_of_existing_content_key);

// Add the DRMSystem to some existing message.
message.AddDRMSystem(std::move(drm));
```

### Adding DRMSystem and UsageRule without explicit Key ID

```c++
CPIXMessage message;

std::unique_ptr<ContentKey> key(new ContentKey);
key->set_key_id(kid_raw_bytes);
key->SetKeyValue(key_value_raw_bytes);

std::unique_ptr<UsageRule> rule(new UsageRule);
...
// Add any filters.
...
// Notice we do not explicitly set the KeyID.

std::vector<std::unique_ptr<UsageRule>> rules;
rules.push_back(std::move(rule));

std::unique_ptr<DRMSystem> drm(new DRMSystem);
drm->set_system_id(system_id_raw_bytes);
...
// Add any DRM signaling information
...
// Notice we do not explicitly set the KeyID.

std::vector<std::unique_ptr<DRMSystem>> drms;
drms.push_back(std::move(drm));

// All DRMSystems and UsageRules in the two supplied vectors have their Key ID
// fields auto-populated to match the Key ID of the supplied ContentKey.
message.AddContentKey(std::move(key), std::move(drms), std::move(rules));
```

## Contributing

If you have improvements or fixes, we would love to have your contributions. See https://github.com/google/cpix_cc/blob/master/CONTRIBUTING.md for details.
