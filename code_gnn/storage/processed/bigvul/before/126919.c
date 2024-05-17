void BrowserTabStripController::OnPreferenceChanged(
    PrefServiceBase* service,
    const std::string& pref_name) {
  if (pref_name == prefs::kTabStripLayoutType) {
    UpdateLayoutType();
  }
}
