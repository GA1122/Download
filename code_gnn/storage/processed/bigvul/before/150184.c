void StartupBrowserCreator::OpenStartupPages(Browser* browser,
                                             bool process_startup) {
  base::CommandLine command_line(base::CommandLine::NO_PROGRAM);
  chrome::startup::IsFirstRun is_first_run =
      first_run::IsChromeFirstRun() ? chrome::startup::IS_FIRST_RUN
                                    : chrome::startup::IS_NOT_FIRST_RUN;
  StartupBrowserCreatorImpl startup_browser_creator_impl(
      base::FilePath(), command_line, is_first_run);
  SessionStartupPref session_startup_pref =
      StartupBrowserCreator::GetSessionStartupPref(command_line,
                                                   browser->profile());
  startup_browser_creator_impl.OpenURLsInBrowser(browser, process_startup,
                                                 session_startup_pref.urls);
}
