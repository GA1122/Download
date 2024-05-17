void AddEmailToOneClickRejectedList(Profile* profile,
                                    const std::string& email) {
  PrefService* pref_service = profile->GetPrefs();
  ListPrefUpdate updater(pref_service,
                         prefs::kReverseAutologinRejectedEmailList);
  updater->AppendIfNotPresent(new base::StringValue(email));
}
