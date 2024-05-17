void PromoResourceService::RegisterPrefs(PrefService* local_state) {
  local_state->RegisterIntegerPref(prefs::kNtpPromoVersion, 0);
  local_state->RegisterStringPref(prefs::kNtpPromoLocale, std::string());
}
