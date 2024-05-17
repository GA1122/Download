bool BookmarksFunction::EditBookmarksEnabled() {
  PrefService* prefs = PrefServiceFromBrowserContext(profile_);
  if (prefs->GetBoolean(prefs::kEditBookmarksEnabled))
    return true;
  error_ = keys::kEditBookmarksDisabled;
  return false;
}
