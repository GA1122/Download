 void CachingPermutedEntropyProvider::AddToCache(uint32 randomization_seed,
                                                uint16 value) const {
  PermutedEntropyCache::Entry* entry;
  const int kMaxSize = 25;
  if (cache_.entry_size() >= kMaxSize) {
    for (int i = 1; i < kMaxSize; ++i)
      cache_.mutable_entry()->SwapElements(i - 1, i);
    entry = cache_.mutable_entry(kMaxSize - 1);
  } else {
    entry = cache_.add_entry();
  }

  entry->set_randomization_seed(randomization_seed);
  entry->set_value(value);

  UpdateLocalState();
}
