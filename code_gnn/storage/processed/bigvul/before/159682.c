void ExternalProtocolHandler::ClearData(Profile* profile) {
  PrefService* prefs = profile->GetPrefs();
  prefs->ClearPref(prefs::kExcludedSchemes);
}
