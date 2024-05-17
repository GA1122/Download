CachingPermutedEntropyProvider::CachingPermutedEntropyProvider(
    PrefService* local_state,
    uint16 low_entropy_source,
    size_t low_entropy_source_max)
    : PermutedEntropyProvider(low_entropy_source, low_entropy_source_max),
      local_state_(local_state) {
  ReadFromLocalState();
}
