ShellWindow* ShellWindow::CreateImpl(Profile* profile,
                                     const extensions::Extension* extension,
                                     const GURL& url,
                                     const ShellWindow::CreateParams& params) {
  return new ShellWindowViews(profile, extension, url, params);
}
