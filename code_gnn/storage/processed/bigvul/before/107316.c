TabContents* Browser::OpenApplicationWindow(
    Profile* profile,
    const Extension* extension,
    extension_misc::LaunchContainer container,
    const GURL& url_input,
    Browser** app_browser) {
  GURL url;
  if (!url_input.is_empty()) {
    if (extension)
      DCHECK(extension->web_extent().ContainsURL(url_input));
    url = url_input;
  } else {
    DCHECK(extension);
    url = extension->GetFullLaunchURL();
  }

  std::string app_name;
  if (extension)
    app_name =
        web_app::GenerateApplicationNameFromExtensionId(extension->id());
  else
    app_name = web_app::GenerateApplicationNameFromURL(url);

  RegisterAppPrefs(app_name, profile);

  bool as_panel = extension && (container == extension_misc::LAUNCH_PANEL);

  gfx::Size window_size;
  if (extension)
    window_size.SetSize(extension->launch_width(),
                        extension->launch_height());

  Browser* browser = Browser::CreateForApp(app_name, window_size, profile,
                                           as_panel);
  if (app_browser)
    *app_browser = browser;

  TabContentsWrapper* wrapper =
      browser->AddSelectedTabWithURL(url, PageTransition::START_PAGE);
  TabContents* contents = wrapper->tab_contents();
  contents->GetMutableRendererPrefs()->can_accept_load_drops = false;
  contents->render_view_host()->SyncRendererPrefs();
  browser->window()->Show();

  contents->view()->SetInitialFocus();
  return contents;
}
