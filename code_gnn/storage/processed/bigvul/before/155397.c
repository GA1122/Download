bool IsAutoplayAllowedByPolicy(content::WebContents* contents,
                               PrefService* prefs) {
  DCHECK(prefs);

  if (prefs->GetBoolean(prefs::kAutoplayAllowed) &&
      prefs->IsManagedPreference(prefs::kAutoplayAllowed)) {
    return true;
  }

  if (!contents)
    return false;

  const base::ListValue* autoplay_whitelist =
      prefs->GetList(prefs::kAutoplayWhitelist);
  return autoplay_whitelist &&
         prefs->IsManagedPreference(prefs::kAutoplayWhitelist) &&
         IsURLWhitelisted(contents->GetURL(), autoplay_whitelist->GetList());
}
