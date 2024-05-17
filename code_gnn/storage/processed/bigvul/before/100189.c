  virtual void Run() {
    if (ShellIntegration::IsDefaultBrowser())
      return;

    ChromeThread::PostTask(
        ChromeThread::UI, FROM_HERE, new NotifyNotDefaultBrowserTask());
  }
