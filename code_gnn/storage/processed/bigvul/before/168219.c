std::pair<Browser*, int> GetBrowserAndTabForDisposition(
    const NavigateParams& params) {
  Profile* profile = params.initiating_profile;

#if BUILDFLAG(ENABLE_EXTENSIONS)
  if (base::FeatureList::IsEnabled(features::kDesktopPWAWindowing) &&
      params.open_pwa_window_if_possible) {
    const extensions::Extension* app = extensions::util::GetInstalledPwaForUrl(
        profile, params.url, extensions::LAUNCH_CONTAINER_WINDOW);
    if (app) {
      std::string app_name =
          web_app::GenerateApplicationNameFromExtensionId(app->id());
      return {
          new Browser(Browser::CreateParams::CreateForApp(
              app_name,
              true,   
              params.window_bounds, profile, params.user_gesture)),
          -1};
    }
  }
#endif

  switch (params.disposition) {
    case WindowOpenDisposition::SWITCH_TO_TAB:
#if !defined(OS_ANDROID)
      for (auto browser_it = BrowserList::GetInstance()->begin_last_active();
           browser_it != BrowserList::GetInstance()->end_last_active();
           ++browser_it) {
        Browser* browser = *browser_it;
        if (browser->profile()->IsSameProfile(profile) &&
            browser->profile()->GetProfileType() == profile->GetProfileType()) {
          int index = GetIndexOfExistingTab(browser, params);
          if (index >= 0)
            return {browser, index};
        }
      }
#endif
      FALLTHROUGH;
    case WindowOpenDisposition::CURRENT_TAB:
      if (params.browser)
        return {params.browser, -1};
      return {GetOrCreateBrowser(profile, params.user_gesture), -1};
    case WindowOpenDisposition::SINGLETON_TAB: {
      int index = GetIndexOfExistingTab(params.browser, params);
      if (index >= 0)
        return {params.browser, index};
    }
      FALLTHROUGH;
    case WindowOpenDisposition::NEW_FOREGROUND_TAB:
    case WindowOpenDisposition::NEW_BACKGROUND_TAB:
      if (params.browser && WindowCanOpenTabs(params.browser))
        return {params.browser, -1};

      return {GetOrCreateBrowser(profile, params.user_gesture), -1};
    case WindowOpenDisposition::NEW_POPUP: {
      std::string app_name;
#if BUILDFLAG(ENABLE_EXTENSIONS)
      if (!params.extension_app_id.empty()) {
        app_name = web_app::GenerateApplicationNameFromExtensionId(
            params.extension_app_id);
      } else if (params.browser && !params.browser->app_name().empty()) {
        app_name = params.browser->app_name();
      } else if (params.source_contents) {
        extensions::TabHelper* extensions_tab_helper =
            extensions::TabHelper::FromWebContents(params.source_contents);
        if (extensions_tab_helper && extensions_tab_helper->is_app()) {
          app_name = web_app::GenerateApplicationNameFromExtensionId(
              extensions_tab_helper->extension_app()->id());
        }
      }
#endif
      if (app_name.empty()) {
        Browser::CreateParams browser_params(Browser::TYPE_POPUP, profile,
                                             params.user_gesture);
        browser_params.trusted_source = params.trusted_source;
        browser_params.initial_bounds = params.window_bounds;
        return {new Browser(browser_params), -1};
      }
      return {new Browser(Browser::CreateParams::CreateForApp(
                  app_name, params.trusted_source, params.window_bounds,
                  profile, params.user_gesture)),
              -1};
    }
    case WindowOpenDisposition::NEW_WINDOW:
      return {new Browser(Browser::CreateParams(profile, params.user_gesture)),
              -1};
    case WindowOpenDisposition::OFF_THE_RECORD:
      return {GetOrCreateBrowser(profile->GetOffTheRecordProfile(),
                                 params.user_gesture),
              -1};
    case WindowOpenDisposition::SAVE_TO_DISK:
    case WindowOpenDisposition::IGNORE_ACTION:
      return {nullptr, -1};
    default:
      NOTREACHED();
  }
  return {nullptr, -1};
}
