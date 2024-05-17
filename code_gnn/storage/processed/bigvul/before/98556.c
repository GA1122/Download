static Browser* GetBrowserInProfileWithId(Profile* profile,
                                          const int window_id,
                                          bool include_incognito,
                                          std::string* error_message) {
  Profile* incognito_profile =
      include_incognito ? profile->GetOffTheRecordProfile() : NULL;
  for (BrowserList::const_iterator browser = BrowserList::begin();
       browser != BrowserList::end(); ++browser) {
    if (((*browser)->profile() == profile ||
         (*browser)->profile() == incognito_profile) &&
        ExtensionTabUtil::GetWindowId(*browser) == window_id)
      return *browser;
  }

  if (error_message)
    *error_message= ExtensionErrorUtils::FormatErrorMessage(
        keys::kWindowNotFoundError, IntToString(window_id));

  return NULL;
}
