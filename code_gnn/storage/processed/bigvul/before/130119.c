bool CachingPermutedEntropyProvider::FindValue(uint32 randomization_seed,
                                               uint16* value) const {
  for (int i = 0; i < cache_.entry_size(); ++i) {
    if (cache_.entry(i).randomization_seed() == randomization_seed) {
      *value = cache_.entry(i).value();
      return true;
    }
  }
  return false;
}
