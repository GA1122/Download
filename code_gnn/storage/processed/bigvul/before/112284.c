ShellWindow* ShellWindow::Create(Profile* profile,
                                 const extensions::Extension* extension,
                                 const GURL& url,
                                 const ShellWindow::CreateParams& params) {
  ShellWindow* window =
      ShellWindow::CreateImpl(profile, extension, url, params);
  ShellWindowRegistry::Get(profile)->AddShellWindow(window);
  return window;
}
