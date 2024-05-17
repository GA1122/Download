void CheckDefaultBrowserCallback() {
  if (ShellIntegration::IsDefaultBrowser() ||
      !ShellIntegration::CanSetAsDefaultBrowser()) {
    return;
  }
  BrowserThread::PostTask(BrowserThread::UI, FROM_HERE,
                          base::Bind(&NotifyNotDefaultBrowserCallback));
}
