bool VariationsSeedStore::StoreSeedData(
    const std::string& data,
    const std::string& base64_seed_signature,
    const std::string& country_code,
    const base::Time& date_fetched,
    bool is_delta_compressed,
    variations::VariationsSeed* parsed_seed) {
  if (!is_delta_compressed) {
    const bool result =
        StoreSeedDataNoDelta(data, base64_seed_signature, country_code,
                             date_fetched, parsed_seed);
    if (result) {
      UMA_HISTOGRAM_COUNTS_1000("Variations.StoreSeed.Size",
                                data.length() / 1024);
    }
    return result;
  }

  DCHECK(invalid_base64_signature_.empty());
  RecordSeedStoreHistogram(VARIATIONS_SEED_STORE_DELTA_COUNT);

  std::string existing_seed_data;
  std::string updated_seed_data;
  if (!ReadSeedData(&existing_seed_data)) {
    RecordSeedStoreHistogram(VARIATIONS_SEED_STORE_FAILED_DELTA_READ_SEED);
    return false;
  }
  if (!ApplyDeltaPatch(existing_seed_data, data, &updated_seed_data)) {
    RecordSeedStoreHistogram(VARIATIONS_SEED_STORE_FAILED_DELTA_APPLY);
    return false;
  }

  const bool result =
      StoreSeedDataNoDelta(updated_seed_data, base64_seed_signature,
                           country_code, date_fetched, parsed_seed);
  if (result) {
    int size_reduction = updated_seed_data.length() - data.length();
    UMA_HISTOGRAM_PERCENTAGE("Variations.StoreSeed.DeltaSize.ReductionPercent",
                             100 * size_reduction / updated_seed_data.length());
    UMA_HISTOGRAM_COUNTS_1000("Variations.StoreSeed.DeltaSize",
                              data.length() / 1024);
  } else {
    RecordSeedStoreHistogram(VARIATIONS_SEED_STORE_FAILED_DELTA_STORE);
  }
  return result;
}
