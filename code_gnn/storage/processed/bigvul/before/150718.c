  void SetAutoplayPrefValue(bool value) {
    profile()->GetPrefs()->SetBoolean(prefs::kBlockAutoplayEnabled, value);
  }
