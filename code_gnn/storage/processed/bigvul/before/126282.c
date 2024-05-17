void BrowserCommandController::OnPreferenceChanged(
    PrefServiceBase* service,
    const std::string& pref_name) {
  if (pref_name == prefs::kPrintingEnabled) {
    UpdatePrintingState();
  } else if (pref_name == prefs::kIncognitoModeAvailability) {
    UpdateCommandsForIncognitoAvailability();
  } else if (pref_name == prefs::kDevToolsDisabled) {
    UpdateCommandsForDevTools();
  } else if (pref_name == prefs::kEditBookmarksEnabled) {
    UpdateCommandsForBookmarkEditing();
  } else if (pref_name == prefs::kShowBookmarkBar) {
    UpdateCommandsForBookmarkBar();
  } else if (pref_name == prefs::kAllowFileSelectionDialogs) {
    UpdateSaveAsState();
    UpdateOpenFileState();
  } else if (pref_name == prefs::kInManagedMode) {
    UpdateCommandsForMultipleProfiles();
  } else {
    NOTREACHED();
  }
}
