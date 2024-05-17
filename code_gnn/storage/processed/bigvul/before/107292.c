Browser* Browser::CreateForApp(const std::string& app_name,
                               const gfx::Size& window_size,
                               Profile* profile,
                               bool is_panel) {
  Browser::Type type = TYPE_APP;

  if (is_panel) {
    type = TYPE_APP_POPUP;
  }

  Browser* browser = new Browser(type, profile);
  browser->app_name_ = app_name;

  if (!window_size.IsEmpty()) {
    gfx::Rect initial_pos(window_size);
    browser->set_override_bounds(initial_pos);
  }

  browser->CreateBrowserWindow();

  return browser;
}
