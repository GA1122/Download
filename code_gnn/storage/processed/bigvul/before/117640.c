std::vector<GURL> BrowserInit::GetURLsFromCommandLine(
    const CommandLine& command_line,
    const FilePath& cur_dir,
    Profile* profile) {
  std::vector<GURL> urls;
  const CommandLine::StringVector& params = command_line.GetArgs();

  for (size_t i = 0; i < params.size(); ++i) {
    FilePath param = FilePath(params[i]);
    if (param.value().size() > 2 &&
        param.value()[0] == '?' && param.value()[1] == ' ') {
      const TemplateURL* default_provider =
          TemplateURLServiceFactory::GetForProfile(profile)->
          GetDefaultSearchProvider();
      if (default_provider && default_provider->url()) {
        const TemplateURLRef* search_url = default_provider->url();
        DCHECK(search_url->SupportsReplacement());
        string16 search_term = param.LossyDisplayName().substr(2);
        urls.push_back(GURL(search_url->ReplaceSearchTermsUsingProfile(
            profile, *default_provider, search_term,
            TemplateURLRef::NO_SUGGESTIONS_AVAILABLE, string16())));
        continue;
      }
    }


    GURL url;
    {
      base::ThreadRestrictions::ScopedAllowIO allow_io;
      url = URLFixerUpper::FixupRelativeFile(cur_dir, param);
    }
    if (url.is_valid()) {
      ChildProcessSecurityPolicy *policy =
          ChildProcessSecurityPolicy::GetInstance();
      if (policy->IsWebSafeScheme(url.scheme()) ||
          url.SchemeIs(chrome::kFileScheme) ||
#if defined(OS_CHROMEOS)
          (url.spec().find(chrome::kChromeUISettingsURL) == 0) ||
#endif
          (url.spec().compare(chrome::kAboutBlankURL) == 0)) {
        urls.push_back(url);
      }
    }
  }
  return urls;
}
