 uint16 CachingPermutedEntropyProvider::GetPermutedValue(
    uint32 randomization_seed) const {
  DCHECK(thread_checker_.CalledOnValidThread());

  uint16 value = 0;
  if (!FindValue(randomization_seed, &value)) {
    value = PermutedEntropyProvider::GetPermutedValue(randomization_seed);
    AddToCache(randomization_seed, value);
  }
  return value;
 }
