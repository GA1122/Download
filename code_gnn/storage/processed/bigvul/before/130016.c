void VariationsSeedStore::ClearPrefs() {
  local_state_->ClearPref(prefs::kVariationsCompressedSeed);
  local_state_->ClearPref(prefs::kVariationsSeed);
  local_state_->ClearPref(prefs::kVariationsSeedDate);
  local_state_->ClearPref(prefs::kVariationsSeedSignature);
}
