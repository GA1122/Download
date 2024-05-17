void ExtensionTabUtil::CreateTab(WebContents* web_contents,
                                 const std::string& extension_id,
                                 WindowOpenDisposition disposition,
                                 const gfx::Rect& initial_pos,
                                 bool user_gesture) {
  Profile* profile =
      Profile::FromBrowserContext(web_contents->GetBrowserContext());
  chrome::HostDesktopType active_desktop = chrome::GetActiveDesktop();
  Browser* browser = chrome::FindTabbedBrowser(profile, false, active_desktop);
  const bool browser_created = !browser;
  if (!browser)
    browser = new Browser(Browser::CreateParams(profile, active_desktop));
  chrome::NavigateParams params(browser, web_contents);

  if (disposition == NEW_POPUP)
    params.extension_app_id = extension_id;

  params.disposition = disposition;
  params.window_bounds = initial_pos;
  params.window_action = chrome::NavigateParams::SHOW_WINDOW;
  params.user_gesture = user_gesture;
  chrome::Navigate(&params);

  if (browser_created && (browser != params.browser))
    browser->window()->Close();
}
