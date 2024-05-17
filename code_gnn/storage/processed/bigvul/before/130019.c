bool VariationsSeedStore::LoadSeed(variations::VariationsSeed* seed) {
  invalid_base64_signature_.clear();

  std::string seed_data;
  if (!ReadSeedData(&seed_data))
    return false;

  const std::string base64_seed_signature =
      local_state_->GetString(prefs::kVariationsSeedSignature);
  const VerifySignatureResult result =
      VerifySeedSignature(seed_data, base64_seed_signature);
  if (result != VARIATIONS_SEED_SIGNATURE_ENUM_SIZE) {
    UMA_HISTOGRAM_ENUMERATION("Variations.LoadSeedSignature", result,
                              VARIATIONS_SEED_SIGNATURE_ENUM_SIZE);
    if (result != VARIATIONS_SEED_SIGNATURE_VALID) {
      ClearPrefs();
      RecordVariationSeedEmptyHistogram(VARIATIONS_SEED_INVALID_SIGNATURE);
      invalid_base64_signature_ = base64_seed_signature;
      return false;
    }
  }

  if (!seed->ParseFromString(seed_data)) {
    ClearPrefs();
    RecordVariationSeedEmptyHistogram(VARIATIONS_SEED_CORRUPT_PROTOBUF);
    return false;
  }

  if (seed->has_country_code() &&
      local_state_->GetString(prefs::kVariationsCountry).empty()) {
    local_state_->SetString(prefs::kVariationsCountry, seed->country_code());
  }
  variations_serial_number_ = seed->serial_number();
  seed_has_country_code_ = seed->has_country_code();
  RecordVariationSeedEmptyHistogram(VARIATIONS_SEED_NOT_EMPTY);
  return true;
}
