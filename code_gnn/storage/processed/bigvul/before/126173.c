Browser::CreateParams Browser::CreateParams::CreateForDevTools(
    Profile* profile) {
  CreateParams params(TYPE_POPUP, profile);
  params.app_name = DevToolsWindow::kDevToolsApp;
  return params;
}
