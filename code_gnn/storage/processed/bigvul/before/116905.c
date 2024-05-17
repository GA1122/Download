  Browser* CreateRestoredBrowser(Browser::Type type,
                                 gfx::Rect bounds,
                                 ui::WindowShowState show_state) {
    Browser* browser = new Browser(type, profile_);
    browser->set_override_bounds(bounds);
    browser->set_show_state(show_state);
    browser->set_is_session_restore(true);
    browser->InitBrowserWindow();
    return browser;
  }
