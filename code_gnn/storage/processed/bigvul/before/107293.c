Browser* Browser::CreateForDevTools(Profile* profile) {
  Browser* browser = new Browser(TYPE_DEVTOOLS, profile);
  browser->app_name_ = DevToolsWindow::kDevToolsApp;
  browser->CreateBrowserWindow();
  return browser;
}
