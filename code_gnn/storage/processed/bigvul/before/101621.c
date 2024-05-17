Browser* Browser::CreateWithParams(const CreateParams& params) {
  Browser* browser = new Browser(params.type, params.profile);
  browser->app_name_ = params.app_name;
  if (!params.initial_bounds.IsEmpty())
    browser->set_override_bounds(params.initial_bounds);
  browser->InitBrowserWindow();
  return browser;
}
