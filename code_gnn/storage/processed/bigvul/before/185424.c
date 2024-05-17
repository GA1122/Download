  void CachingPermutedEntropyProvider::RegisterPrefs(
      PrefRegistrySimple* registry) {
  registry->RegisterStringPref(prefs::kVariationsPermutedEntropyCache,
                               std::string());
//   registry->RegisterStringPref(
//       chrome_variations::prefs::kVariationsPermutedEntropyCache, std::string());
  }