void KeyboardOverlayHandler::GetLabelMap(const ListValue* args) {
  DCHECK(profile_);
  PrefService* pref_service = profile_->GetPrefs();
  typedef std::map<ModifierKey, ModifierKey> ModifierMap;
  ModifierMap modifier_map;
  modifier_map[chromeos::input_method::kSearchKey] = static_cast<ModifierKey>(
      pref_service->GetInteger(prefs::kLanguageXkbRemapSearchKeyTo));
  modifier_map[chromeos::input_method::kLeftControlKey] =
      static_cast<ModifierKey>(
          pref_service->GetInteger(prefs::kLanguageXkbRemapControlKeyTo));
  modifier_map[chromeos::input_method::kLeftAltKey] = static_cast<ModifierKey>(
      pref_service->GetInteger(prefs::kLanguageXkbRemapAltKeyTo));

  DictionaryValue dict;
  for (ModifierMap::const_iterator i = modifier_map.begin();
       i != modifier_map.end(); ++i) {
    dict.SetString(ModifierKeyToLabel(i->first), ModifierKeyToLabel(i->second));
  }

  web_ui_->CallJavascriptFunction("initIdentifierMap", dict);
}
