bool VariationsSeedStore::StoreSeedDataNoDelta(
    const std::string& seed_data,
    const std::string& base64_seed_signature,
    const std::string& country_code,
    const base::Time& date_fetched,
    variations::VariationsSeed* parsed_seed) {
  if (seed_data.empty()) {
    RecordSeedStoreHistogram(VARIATIONS_SEED_STORE_FAILED_EMPTY);
    return false;
  }

  variations::VariationsSeed seed;
  if (!seed.ParseFromString(seed_data)) {
    RecordSeedStoreHistogram(VARIATIONS_SEED_STORE_FAILED_PARSE);
    return false;
  }

  const VerifySignatureResult result =
      VerifySeedSignature(seed_data, base64_seed_signature);
  if (result != VARIATIONS_SEED_SIGNATURE_ENUM_SIZE) {
    UMA_HISTOGRAM_ENUMERATION("Variations.StoreSeedSignature", result,
                              VARIATIONS_SEED_SIGNATURE_ENUM_SIZE);
    if (result != VARIATIONS_SEED_SIGNATURE_VALID) {
      RecordSeedStoreHistogram(VARIATIONS_SEED_STORE_FAILED_SIGNATURE);
      return false;
    }
  }

  std::string compressed_seed_data;
  if (!metrics::GzipCompress(seed_data, &compressed_seed_data)) {
    RecordSeedStoreHistogram(VARIATIONS_SEED_STORE_FAILED_GZIP);
    return false;
  }

  std::string base64_seed_data;
  base::Base64Encode(compressed_seed_data, &base64_seed_data);

  local_state_->ClearPref(prefs::kVariationsSeed);

  if (!country_code.empty())
    local_state_->SetString(prefs::kVariationsCountry, country_code);
  else if (seed.has_country_code())
    local_state_->SetString(prefs::kVariationsCountry, seed.country_code());

  local_state_->SetString(prefs::kVariationsCompressedSeed, base64_seed_data);
  UpdateSeedDateAndLogDayChange(date_fetched);
  local_state_->SetString(prefs::kVariationsSeedSignature,
                          base64_seed_signature);
  variations_serial_number_ = seed.serial_number();
  if (parsed_seed)
    seed.Swap(parsed_seed);

  RecordSeedStoreHistogram(VARIATIONS_SEED_STORE_SUCCESS);
  return true;
}
