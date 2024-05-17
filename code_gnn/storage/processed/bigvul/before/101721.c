TabContents* Browser::OpenApplicationWindow(
    Profile* profile,
    const Extension* extension,
    extension_misc::LaunchContainer container,
    const GURL& url_input,
    Browser** app_browser) {
  GURL url;
  if (!url_input.is_empty()) {
    if (extension)
      DCHECK(extension->web_extent().MatchesURL(url_input));
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

  Type type = extension && (container == extension_misc::LAUNCH_PANEL) ?
      TYPE_PANEL : TYPE_POPUP;

  gfx::Rect window_bounds;
  if (extension) {
    window_bounds.set_width(extension->launch_width());
    window_bounds.set_height(extension->launch_height());
  }

  Browser* browser = Browser::CreateForApp(type, app_name, window_bounds,
                                           profile);

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
