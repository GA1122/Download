ExternalProtocolHandler::BlockState ExternalProtocolHandler::GetBlockState(
    const std::string& scheme,
    Profile* profile) {
  DCHECK_CURRENTLY_ON(content::BrowserThread::UI);

  if (!g_accept_requests)
    return BLOCK;

  if (scheme.length() == 1) {
    return BLOCK;
  }

  for (size_t i = 0; i < arraysize(kDeniedSchemes); ++i) {
    if (kDeniedSchemes[i] == scheme)
      return BLOCK;
  }

  for (size_t i = 0; i < arraysize(kAllowedSchemes); ++i) {
    if (kAllowedSchemes[i] == scheme)
      return DONT_BLOCK;
  }

  PrefService* profile_prefs = profile->GetPrefs();
  if (profile_prefs) {   
    DictionaryPrefUpdate update_excluded_schemas_profile(
        profile_prefs, prefs::kExcludedSchemes);
    bool should_block;
    if (update_excluded_schemas_profile->GetBoolean(scheme, &should_block) &&
        !should_block) {
      return DONT_BLOCK;
    }
  }

  return UNKNOWN;
}
