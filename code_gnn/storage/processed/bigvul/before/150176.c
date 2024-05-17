std::vector<GURL> StartupBrowserCreator::GetURLsFromCommandLine(
    const base::CommandLine& command_line,
    const base::FilePath& cur_dir,
    Profile* profile) {
  DCHECK(profile);

  std::vector<GURL> urls;

  const base::CommandLine::StringVector& params = command_line.GetArgs();
  for (size_t i = 0; i < params.size(); ++i) {
    base::FilePath param = base::FilePath(params[i]);
    if ((param.value().size() > 2) && (param.value()[0] == '?') &&
        (param.value()[1] == ' ')) {
      GURL url(GetDefaultSearchURLForSearchTerms(
          TemplateURLServiceFactory::GetForProfile(profile),
          param.LossyDisplayName().substr(2)));
      if (url.is_valid()) {
        urls.push_back(url);
        continue;
      }
    }


    GURL url = GURL(param.MaybeAsASCII());

    if (!url.is_valid() || url.SchemeIsFile()) {
      base::ThreadRestrictions::ScopedAllowIO allow_io;
      url = url_formatter::FixupRelativeFile(cur_dir, param);
    }
    if (!url.is_valid())
      continue;

    const GURL settings_url = GURL(chrome::kChromeUISettingsURL);
    bool url_points_to_an_approved_settings_page = false;
#if defined(OS_CHROMEOS)
    url_points_to_an_approved_settings_page =
        url.GetOrigin() == settings_url.GetOrigin();
#else
    const GURL reset_settings_url =
        settings_url.Resolve(chrome::kResetProfileSettingsSubPage);
    url_points_to_an_approved_settings_page = url == reset_settings_url;
#if defined(OS_WIN)
    const GURL reset_settings_url_with_cct_hash = reset_settings_url.Resolve(
        std::string("#") +
        settings::ResetSettingsHandler::kCctResetSettingsHash);
    url_points_to_an_approved_settings_page =
        url_points_to_an_approved_settings_page ||
        url == reset_settings_url_with_cct_hash;
#endif   
#endif   

    ChildProcessSecurityPolicy* policy =
        ChildProcessSecurityPolicy::GetInstance();
    if (policy->IsWebSafeScheme(url.scheme()) ||
        url.SchemeIs(url::kFileScheme) ||
        url_points_to_an_approved_settings_page ||
        (url.spec().compare(url::kAboutBlankURL) == 0)) {
      urls.push_back(url);
    }
  }
  return urls;
}
