void ExternalProtocolHandler::SetBlockState(const std::string& scheme,
                                            BlockState state,
                                            Profile* profile) {
  DCHECK_NE(state, BLOCK);

  PrefService* profile_prefs = profile->GetPrefs();
  if (profile_prefs) {   
    DictionaryPrefUpdate update_excluded_schemas_profile(
        profile_prefs, prefs::kExcludedSchemes);
    if (!update_excluded_schemas_profile->empty()) {
      if (state == DONT_BLOCK)
        update_excluded_schemas_profile->SetBoolean(scheme, false);
      else
        update_excluded_schemas_profile->Remove(scheme, nullptr);
    }
  }
}
