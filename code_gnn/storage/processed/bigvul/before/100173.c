std::vector<GURL> BrowserInit::LaunchWithProfile::GetURLsFromCommandLine(
    Profile* profile) {
  std::vector<GURL> urls;
  std::vector<std::wstring> params = command_line_.GetLooseValues();
  for (size_t i = 0; i < params.size(); ++i) {
    std::wstring& value = params[i];
    if (value.find(L"? ") == 0) {
      const TemplateURL* default_provider =
          profile->GetTemplateURLModel()->GetDefaultSearchProvider();
      if (!default_provider || !default_provider->url()) {
        urls.push_back(
            GURL(WideToUTF8(URLFixerUpper::FixupRelativeFile(cur_dir_,
                                                             value))));
        continue;
      }
      const TemplateURLRef* search_url = default_provider->url();
      DCHECK(search_url->SupportsReplacement());
      urls.push_back(GURL(WideToUTF8(search_url->ReplaceSearchTerms(
          *default_provider, value.substr(2),
          TemplateURLRef::NO_SUGGESTIONS_AVAILABLE, std::wstring()))));
    } else {
      GURL url = GURL(WideToUTF8(
          URLFixerUpper::FixupRelativeFile(cur_dir_, value)));
      if (url.is_valid())
        urls.push_back(url);
    }
  }
  return urls;
}
