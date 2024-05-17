void RecordSeedStoreHistogram(VariationsSeedStoreResult result) {
  UMA_HISTOGRAM_ENUMERATION("Variations.SeedStoreResult", result,
                            VARIATIONS_SEED_STORE_RESULT_ENUM_SIZE);
}
