Browser* Browser::CreateForDevTools(Profile* profile) {
#if defined(OS_CHROMEOS)
  CreateParams params(TYPE_TABBED, profile);
#else
  CreateParams params(TYPE_POPUP, profile);
#endif
  params.app_name = DevToolsWindow::kDevToolsApp;
  return CreateWithParams(params);
}
