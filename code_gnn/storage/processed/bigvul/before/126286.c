  SwichToMetroUIHandler()
      : ALLOW_THIS_IN_INITIALIZER_LIST(default_browser_worker_(
            new ShellIntegration::DefaultBrowserWorker(this))),
        first_check_(true) {
    default_browser_worker_->StartCheckIsDefault();
  }
