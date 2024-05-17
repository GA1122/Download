void SetPreferencesFromJson(Profile* profile, const std::string& json) {
  base::DictionaryValue* dict = nullptr;
  std::unique_ptr<base::Value> parsed = base::JSONReader::Read(json);
  if (!parsed || !parsed->GetAsDictionary(&dict))
    return;
  DictionaryPrefUpdate update(profile->GetPrefs(), prefs::kDevToolsPreferences);
  for (base::DictionaryValue::Iterator it(*dict); !it.IsAtEnd(); it.Advance()) {
    if (!it.value().IsType(base::Value::Type::STRING))
      continue;
    update.Get()->SetWithoutPathExpansion(
        it.key(), it.value().CreateDeepCopy());
  }
}
