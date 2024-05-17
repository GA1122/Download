bool MasterPreferences::InitializeFromString(const std::string& json_data) {
  if (!json_data.empty())
    master_dictionary_.reset(ParseDistributionPreferences(json_data));

  bool data_is_valid = true;
  if (!master_dictionary_.get()) {
    master_dictionary_.reset(new base::DictionaryValue());
    data_is_valid = false;
  } else {
    master_dictionary_->GetDictionary(
        installer::master_preferences::kDistroDict, &distribution_);
  }

  InitializeProductFlags();
  EnforceLegacyPreferences();
  return data_is_valid;
}
