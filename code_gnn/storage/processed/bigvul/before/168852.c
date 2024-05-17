bool WindowsCreateFunction::ShouldOpenIncognitoWindow(
    const windows::Create::Params::CreateData* create_data,
    std::vector<GURL>* urls,
    std::string* error) {
  Profile* profile = Profile::FromBrowserContext(browser_context());
  const IncognitoModePrefs::Availability incognito_availability =
      IncognitoModePrefs::GetAvailability(profile->GetPrefs());
  bool incognito = false;
  if (create_data && create_data->incognito) {
    incognito = *create_data->incognito;
    if (incognito && incognito_availability == IncognitoModePrefs::DISABLED) {
      *error = keys::kIncognitoModeIsDisabled;
      return false;
    }
    if (!incognito && incognito_availability == IncognitoModePrefs::FORCED) {
      *error = keys::kIncognitoModeIsForced;
      return false;
    }
  } else if (incognito_availability == IncognitoModePrefs::FORCED) {
    incognito = true;
  }

  if (incognito && !profile->IsGuestSession()) {
    std::string first_url_erased;
    for (size_t i = 0; i < urls->size();) {
      if (IsURLAllowedInIncognito((*urls)[i], profile)) {
        i++;
      } else {
        if (first_url_erased.empty())
          first_url_erased = (*urls)[i].spec();
        urls->erase(urls->begin() + i);
      }
    }
    if (urls->empty() && !first_url_erased.empty()) {
      *error = ErrorUtils::FormatErrorMessage(
          keys::kURLsNotAllowedInIncognitoError, first_url_erased);
      return false;
    }
  }
  return incognito;
}
