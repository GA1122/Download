NavigationEntry* NavigationController::CreateNavigationEntry(
    const GURL& url, const GURL& referrer, PageTransition::Type transition) {
  GURL loaded_url(url);
  BrowserURLHandler::RewriteURLIfNecessary(&loaded_url, profile_);

  NavigationEntry* entry = new NavigationEntry(NULL, -1, loaded_url, referrer,
                                               string16(), transition);
  entry->set_virtual_url(url);
  entry->set_user_typed_url(url);
  if (url.SchemeIsFile()) {
    std::wstring languages = profile()->GetPrefs()->GetString(
        prefs::kAcceptLanguages);
    entry->set_title(WideToUTF16Hack(
        file_util::GetFilenameFromPath(net::FormatUrl(url, languages))));
  }
  return entry;
}
